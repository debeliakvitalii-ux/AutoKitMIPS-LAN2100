#ifndef AUTOKIT_USBWARE_SYNC_EP0_QUERY_BACKEND_H
#define AUTOKIT_USBWARE_SYNC_EP0_QUERY_BACKEND_H

#ifdef _WIN32
#include <windows.h>
#else
typedef unsigned long DWORD;
typedef long LONG;
typedef int BOOL;
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif
#endif

/*
 * Application-side QUERY state machine; native USBware provider not included.
 * IMPORTANT: usbDeviceOpaque is USBware internal usb_device* identity only.
 * It MUST NOT be cast/reinterpreted as CE USB_HANDLE and MUST NOT be passed
 * into the Microsoft CE USB_FUNCS adapter.
 */

enum AK_USBWARE_QUERY_RESULT {
    AK_USBWARE_QUERY_OK = 0,
    AK_USBWARE_QUERY_UNKNOWN_OR_BLOCKED = 1,
    AK_USBWARE_QUERY_RETIRED = 2,
    AK_USBWARE_QUERY_STOPPED = 3,
    AK_USBWARE_QUERY_BAD_ACK = 4,
    AK_USBWARE_QUERY_CALL_FAILED = 5,
    AK_USBWARE_QUERY_ALREADY_ATTEMPTED = 6,
    AK_USBWARE_QUERY_NOT_TERMINAL = 7,
    AK_USBWARE_QUERY_SHORT_REPLY = 8,
    AK_USBWARE_QUERY_NATIVE_FAILED = 9
};

/* Application-defined result schema; NOT Jungo / jipod / OEM ABI.
 * A real version-matched provider (not yet supplied) must establish BOTH a
 * terminal completion and the actual transfer byte count independently.
 * nativeStatus is an uninterpreted value from that provider: no value here
 * is presumed equal to any OEM USBware success code. */
enum AK_USBWARE_TERMINAL_STATE {
    AK_USBWARE_TERMINAL_UNKNOWN = 0,
    AK_USBWARE_TERMINAL_SUCCESS = 1,
    AK_USBWARE_TERMINAL_FAILED = 2,
    AK_USBWARE_TERMINAL_CANCELLED = 3
};
struct AK_USBWARE_SYNC_COMPLETION {
    BOOL terminal;
    AK_USBWARE_TERMINAL_STATE state;
    DWORD nativeStatus;
    DWORD actualLength;
    unsigned char reply[4];
};
struct AK_USBWARE_QUERY_OBSERVATION {
    AK_USBWARE_QUERY_RESULT result;
    BOOL attempted;
    BOOL terminal;
    AK_USBWARE_TERMINAL_STATE state;
    DWORD nativeStatus;
    DWORD actualLength;
    unsigned char raw[4]; /* only raw[0 .. min(actualLength,4)-1] meaningful */
};
/* BOOL only means callback API invocation returned successfully; it is NOT
 * a transfer completion signal, native success, or indication of 4 bytes. */
typedef BOOL (*AK_USBWARE_SYNC_QUERY_FN)(
    void* callContext,
    void* usbDeviceOpaque,
    const unsigned char setup[8],
    AK_USBWARE_SYNC_COMPLETION* completion);

struct AK_USBWARE_SYNC_EP0_GENERATION {
    /*
     * Owner records are single-use. Call Init only on fresh zero-initialized,
     * unpublished storage. There is intentionally no reset/reuse API.
     *
     * Concurrency contract: once published, Run/RequestStop/MarkRetiring may overlap.
     * stateLock serializes their shared state. It is NEVER held across the
     * blocking issueQuerySync call.
     */
    volatile LONG stateLock;
    DWORD ownerCookie;
    DWORD generation;
    void* usbDeviceOpaque;
    void* jipodSoftcOpaque;
    BOOL attachReady;
    BOOL retiring;
    BOOL stopRequested;
    BOOL queryInFlight;
    BOOL queryAck;
    BOOL queryAttempted; /* permanently set on first admitted call, even on failure */
    /* Final release lease: stateLock protects the claim and withdrawal intent.
     * Physical detach MUST also serialize through the owner's external
     * dispatcher; this is not a substitute for a proven OEM owner lease. */
    BOOL releaseLeaseActive;
    BOOL withdrawalPending;

    /* Lifetime precondition: must remain FALSE until detach/device lifetime protection is exact-proven. */
    BOOL contextLifetimeBarrierProven;
};

BOOL UsbwareSyncEp0Query_Init(AK_USBWARE_SYNC_EP0_GENERATION* g);
BOOL UsbwareSyncEp0Query_BeginGeneration(
    AK_USBWARE_SYNC_EP0_GENERATION* g,
    DWORD generation,
    void* jipodSoftcOpaque,
    void* usbDeviceOpaque,
    BOOL contextLifetimeBarrierProven);
void UsbwareSyncEp0Query_MarkRetiring(AK_USBWARE_SYNC_EP0_GENERATION* g, DWORD generation);
void UsbwareSyncEp0Query_RequestStop(AK_USBWARE_SYNC_EP0_GENERATION* g, DWORD generation);
/* Owner validation. Checks and invalidation share the original
 * backend stateLock. Neither API frees/cancels provider resources or reuses a
 * generation record. A different generation is never touched. */
BOOL UsbwareSyncEp0Query_PeekCurrentDevice(
    AK_USBWARE_SYNC_EP0_GENERATION* g, DWORD generation, void** device);
BOOL UsbwareSyncEp0Query_WithdrawOwner(
    AK_USBWARE_SYNC_EP0_GENERATION* g, DWORD generation);
/* Explicit final-release transaction; no SDK/vendor calls are made here.
 * Caller must hold the verified physical-owner dispatcher lock until End.
 * During a lease WithdrawOwner latches revocation, never frees a pointer.
 * End retires the single-use generation and denies a positive result on a
 * concurrent withdrawal/stop/retire request. */
BOOL UsbwareSyncEp0Query_BeginReleaseLease(
    AK_USBWARE_SYNC_EP0_GENERATION* g, DWORD generation, void* device);
BOOL UsbwareSyncEp0Query_EndReleaseLease(
    AK_USBWARE_SYNC_EP0_GENERATION* g, DWORD generation, void* device,
    BOOL releaseSucceeded);
AK_USBWARE_QUERY_RESULT UsbwareSyncEp0Query_Run(
    AK_USBWARE_SYNC_EP0_GENERATION* g,
    DWORD requestedGeneration,
    AK_USBWARE_SYNC_QUERY_FN issueQuerySync,
    void* callContext,
    AK_USBWARE_QUERY_OBSERVATION* observation);

#endif
