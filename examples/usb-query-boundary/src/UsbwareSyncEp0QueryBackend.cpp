#include "UsbwareSyncEp0QueryBackend.h"
#include <string.h>
#ifndef _WIN32
#include <sched.h>
#endif

static const unsigned char kQuerySetup[8] = { 0xC0, 0x53, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00 };
static const unsigned char kExpectedAck[4] = { 0x01, 0x00, 0x00, 0x00 };
static const DWORD kOwnerCookie = 0x55574550UL; /* "UWEP" */

static LONG UsbwareSyncEp0Query_CompareExchange(volatile LONG* target, LONG exchangeValue, LONG compareValue)
{
#ifdef _WIN32
    return InterlockedCompareExchange((LONG*)target, exchangeValue, compareValue);
#else
    return __sync_val_compare_and_swap(target, compareValue, exchangeValue);
#endif
}

static void UsbwareSyncEp0Query_Unlock(volatile LONG* target)
{
#ifdef _WIN32
    InterlockedExchange((LONG*)target, 0);
#else
    __sync_lock_release(target);
#endif
}

static void UsbwareSyncEp0Query_Lock(AK_USBWARE_SYNC_EP0_GENERATION* g)
{
    while (UsbwareSyncEp0Query_CompareExchange(&g->stateLock, 1, 0) != 0) {
#ifdef _WIN32
        Sleep(0);
#else
        sched_yield();
#endif
    }
}

static void UsbwareSyncEp0Query_UnlockGeneration(AK_USBWARE_SYNC_EP0_GENERATION* g)
{
    UsbwareSyncEp0Query_Unlock(&g->stateLock);
}

BOOL UsbwareSyncEp0Query_Init(AK_USBWARE_SYNC_EP0_GENERATION* g)
{
    if (!g)
        return FALSE;

    /*
     * Init is a pre-publication operation on fresh zero-initialized storage.
     * Once initialized, the record is single-use and Init never clears it.
     */
    if (g->ownerCookie != 0 || g->stateLock != 0)
        return FALSE;

    memset(g, 0, sizeof(*g));
    g->ownerCookie = kOwnerCookie;
    return TRUE;
}

BOOL UsbwareSyncEp0Query_BeginGeneration(
    AK_USBWARE_SYNC_EP0_GENERATION* g,
    DWORD generation,
    void* jipodSoftcOpaque,
    void* usbDeviceOpaque,
    BOOL contextLifetimeBarrierProven)
{
    BOOL accepted = FALSE;

    if (!g || generation == 0 || !jipodSoftcOpaque || !usbDeviceOpaque)
        return FALSE;

    UsbwareSyncEp0Query_Lock(g);
    if (g->ownerCookie == kOwnerCookie && g->generation == 0 && !g->queryInFlight) {
        g->generation = generation;
        g->jipodSoftcOpaque = jipodSoftcOpaque;
        g->usbDeviceOpaque = usbDeviceOpaque;
        g->attachReady = TRUE;
        g->retiring = FALSE;
        g->stopRequested = FALSE;
        g->queryInFlight = FALSE;
        g->queryAck = FALSE;
        g->queryAttempted = FALSE;
        g->releaseLeaseActive = FALSE;
        g->withdrawalPending = FALSE;
        g->contextLifetimeBarrierProven = contextLifetimeBarrierProven ? TRUE : FALSE;
        accepted = TRUE;
    }
    UsbwareSyncEp0Query_UnlockGeneration(g);
    return accepted;
}

void UsbwareSyncEp0Query_MarkRetiring(AK_USBWARE_SYNC_EP0_GENERATION* g, DWORD generation)
{
    if (!g)
        return;
    UsbwareSyncEp0Query_Lock(g);
    if (g->ownerCookie == kOwnerCookie && g->generation == generation) {
        g->retiring = TRUE;
        g->attachReady = FALSE;
    }
    UsbwareSyncEp0Query_UnlockGeneration(g);
}

void UsbwareSyncEp0Query_RequestStop(AK_USBWARE_SYNC_EP0_GENERATION* g, DWORD generation)
{
    if (!g)
        return;
    UsbwareSyncEp0Query_Lock(g);
    if (g->ownerCookie == kOwnerCookie && g->generation == generation) {
        /* Synchronous backend: STOP is only latched. No fabricated AbortTransfer exists here. */
        g->stopRequested = TRUE;
        g->retiring = TRUE;
        g->attachReady = FALSE;
    }
    UsbwareSyncEp0Query_UnlockGeneration(g);
}

BOOL UsbwareSyncEp0Query_PeekCurrentDevice(
    AK_USBWARE_SYNC_EP0_GENERATION* g, DWORD generation, void** device)
{
    BOOL ok = FALSE;
    if (device) *device = 0;
    if (!g || !device || generation == 0) return FALSE;
    UsbwareSyncEp0Query_Lock(g);
    if (g->ownerCookie == kOwnerCookie && g->generation == generation &&
        g->attachReady && !g->retiring && !g->stopRequested &&
        g->contextLifetimeBarrierProven && g->usbDeviceOpaque &&
        g->jipodSoftcOpaque) {
        *device = g->usbDeviceOpaque;
        ok = TRUE;
    }
    UsbwareSyncEp0Query_UnlockGeneration(g);
    return ok;
}

BOOL UsbwareSyncEp0Query_WithdrawOwner(
    AK_USBWARE_SYNC_EP0_GENERATION* g, DWORD generation)
{
    BOOL changed = FALSE;
    if (!g || generation == 0) return FALSE;
    UsbwareSyncEp0Query_Lock(g);
    if (g->ownerCookie == kOwnerCookie && g->generation == generation) {
        g->stopRequested = TRUE;
        g->retiring = TRUE;
        g->attachReady = FALSE;
        g->queryAck = FALSE;
        if (g->releaseLeaseActive) g->withdrawalPending = TRUE;
        /* queryAttempted is irreversible. In-flight provider lifetime is
         * owned externally and cannot be cleared, aborted or freed here. */
        changed = TRUE;
    }
    UsbwareSyncEp0Query_UnlockGeneration(g);
    return changed;
}

BOOL UsbwareSyncEp0Query_BeginReleaseLease(
    AK_USBWARE_SYNC_EP0_GENERATION* g, DWORD generation, void* device)
{
    BOOL ok = FALSE;
    if (!g || !device || !generation) return FALSE;
    UsbwareSyncEp0Query_Lock(g);
    if (g->ownerCookie == kOwnerCookie && g->generation == generation &&
        g->usbDeviceOpaque == device && g->jipodSoftcOpaque &&
        g->contextLifetimeBarrierProven && g->attachReady &&
        !g->retiring && !g->stopRequested && !g->queryInFlight &&
        g->queryAttempted && !g->releaseLeaseActive && !g->withdrawalPending) {
        g->releaseLeaseActive = TRUE;
        ok = TRUE;
    }
    UsbwareSyncEp0Query_UnlockGeneration(g);
    return ok;
}

BOOL UsbwareSyncEp0Query_EndReleaseLease(
    AK_USBWARE_SYNC_EP0_GENERATION* g, DWORD generation, void* device,
    BOOL releaseSucceeded)
{
    BOOL ok = FALSE;
    if (!g || !device || !generation) return FALSE;
    UsbwareSyncEp0Query_Lock(g);
    if (g->ownerCookie == kOwnerCookie && g->generation == generation &&
        g->usbDeviceOpaque == device && g->releaseLeaseActive) {
        ok = releaseSucceeded && !g->withdrawalPending &&
             !g->stopRequested && !g->retiring && g->attachReady;
        /* Retire regardless of callback result; retain resource ownership on
         * failure and forbid another QUERY. A different record is untouched. */
        g->queryAck = FALSE;
        g->attachReady = FALSE;
        g->retiring = TRUE;
        g->stopRequested = TRUE;
        g->releaseLeaseActive = FALSE;
    }
    UsbwareSyncEp0Query_UnlockGeneration(g);
    return ok;
}

AK_USBWARE_QUERY_RESULT UsbwareSyncEp0Query_Run(
    AK_USBWARE_SYNC_EP0_GENERATION* g,
    DWORD requestedGeneration,
    AK_USBWARE_SYNC_QUERY_FN issueQuerySync,
    void* callContext,
    AK_USBWARE_QUERY_OBSERVATION* observation)
{
    AK_USBWARE_SYNC_COMPLETION completion;
    BOOL invoked;
    DWORD callGeneration;
    void* callUsbDevice;
    void* callJipodSoftc;
    DWORD count;
    AK_USBWARE_QUERY_RESULT result;

    /* No unrecorded QUERY attempt: callers must have observation storage. */
    if (!observation)
        return AK_USBWARE_QUERY_UNKNOWN_OR_BLOCKED;
    memset(observation, 0, sizeof(*observation));
    observation->result = AK_USBWARE_QUERY_UNKNOWN_OR_BLOCKED;
    if (!g || !issueQuerySync || requestedGeneration == 0)
        return observation->result;

    /* Admission is atomic with the once-per-generation attempt reservation.
     * Never hold stateLock across the possibly blocking native callback. */
    UsbwareSyncEp0Query_Lock(g);
    if (g->ownerCookie != kOwnerCookie || g->generation != requestedGeneration)
        result = AK_USBWARE_QUERY_UNKNOWN_OR_BLOCKED;
    else if (g->queryAttempted)
        result = AK_USBWARE_QUERY_ALREADY_ATTEMPTED;
    else if (!g->contextLifetimeBarrierProven)
        result = AK_USBWARE_QUERY_UNKNOWN_OR_BLOCKED;
    else if (g->stopRequested)
        result = AK_USBWARE_QUERY_STOPPED;
    else if (g->retiring || !g->attachReady)
        result = AK_USBWARE_QUERY_RETIRED;
    else if (!g->usbDeviceOpaque || !g->jipodSoftcOpaque || g->queryInFlight)
        result = AK_USBWARE_QUERY_UNKNOWN_OR_BLOCKED;
    else {
        callGeneration = g->generation;
        callUsbDevice = g->usbDeviceOpaque;
        callJipodSoftc = g->jipodSoftcOpaque;
        g->queryInFlight = TRUE;
        g->queryAttempted = TRUE; /* consumed EVEN when native issue fails */
        result = AK_USBWARE_QUERY_OK; /* admitted sentinel, not a transfer OK */
        observation->attempted = TRUE;
    }
    UsbwareSyncEp0Query_UnlockGeneration(g);

    if (result != AK_USBWARE_QUERY_OK) {
        observation->result = result;
        return result;
    }

    memset(&completion, 0, sizeof(completion));
    invoked = issueQuerySync(callContext, callUsbDevice, kQuerySetup, &completion);

    /* Capture only reported ACTUAL bytes; do not log buffer zero-padding as
     * evidence. Even a provider's TRUE return without completion is not OK. */
    observation->terminal = completion.terminal;
    observation->state = completion.state;
    observation->nativeStatus = completion.nativeStatus;
    observation->actualLength = completion.actualLength;
    count = completion.actualLength;
    if (count > sizeof(observation->raw))
        count = sizeof(observation->raw);
    if (count)
        memcpy(observation->raw, completion.reply, count);

    UsbwareSyncEp0Query_Lock(g);
    if (g->ownerCookie != kOwnerCookie ||
        g->generation != callGeneration ||
        g->usbDeviceOpaque != callUsbDevice ||
        g->jipodSoftcOpaque != callJipodSoftc) {
        /* No presumed callback retirement on generation/owner corruption. */
        result = AK_USBWARE_QUERY_UNKNOWN_OR_BLOCKED;
    } else {
        g->queryInFlight = FALSE;
        if (g->stopRequested)
            result = AK_USBWARE_QUERY_STOPPED;
        else if (g->retiring || !g->attachReady)
            result = AK_USBWARE_QUERY_RETIRED;
        else if (!invoked)
            result = AK_USBWARE_QUERY_CALL_FAILED;
        else if (!completion.terminal)
            result = AK_USBWARE_QUERY_NOT_TERMINAL;
        else if (completion.state != AK_USBWARE_TERMINAL_SUCCESS)
            result = AK_USBWARE_QUERY_NATIVE_FAILED;
        else if (completion.actualLength != sizeof(kExpectedAck))
            result = AK_USBWARE_QUERY_SHORT_REPLY;
        else if (memcmp(completion.reply, kExpectedAck, sizeof(kExpectedAck)) != 0)
            result = AK_USBWARE_QUERY_BAD_ACK;
        else {
            g->queryAck = TRUE;
            result = AK_USBWARE_QUERY_OK;
        }
    }
    UsbwareSyncEp0Query_UnlockGeneration(g);
    observation->result = result;
    return result;
}
