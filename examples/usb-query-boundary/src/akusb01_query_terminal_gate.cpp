#include "akusb01_query_terminal_gate.h"
#include <string.h>

struct AKUSB01_CALL {
    const AKUSB01_EXACT_OWNER_BINDING* binding;
    AKUSB01_QUERY_OBSERVATION* out;
    void* device;
};
static BOOL akusb01_adapter(void* context, void* device,
                           const unsigned char setup[8],
                           AK_USBWARE_SYNC_COMPLETION* completion)
{
    AKUSB01_CALL* call = (AKUSB01_CALL*)context;
    call->device = device;
    call->out->issued = 1;
    return call->binding->query_terminal_sync(call->binding->user,
                                              device, setup, completion);
}
static void akusb01_revoke(AK_USBWARE_SYNC_EP0_GENERATION* g,
                           DWORD generation, AKUSB01_QUERY_OBSERVATION* out)
{
    UsbwareSyncEp0Query_WithdrawOwner(g, generation); /* same generation only */
    memset(out->response, 0, sizeof(out->response));
    memset(out->query.raw, 0, sizeof(out->query.raw));
    if (out->query.result == AK_USBWARE_QUERY_OK)
        out->query.result = AK_USBWARE_QUERY_RETIRED;
}

AKUSB01_QUERY_STATUS akusb01_query_only(
    AK_USBWARE_SYNC_EP0_GENERATION* g, DWORD generation,
    const AKUSB01_EXACT_OWNER_BINDING* binding, AKUSB01_QUERY_OBSERVATION* out)
{
    AKUSB01_CALL call;
    void* device = 0;
    void* now = 0;
    AK_USBWARE_QUERY_RESULT result;
    AKUSB01_QUERY_STATUS decision = AKUSB01_STOPPED_OR_RETIRED;

    if (!out) return AKUSB01_NO_VERIFIED_PROVIDER;
    memset(out, 0, sizeof(*out));
    out->status = AKUSB01_NO_VERIFIED_PROVIDER;
    if (!g || !binding || !binding->verified_current_owner ||
        !binding->query_terminal_sync || !binding->drain_verified ||
        !binding->release_owned || !binding->owner_dispatch_enter ||
        !binding->owner_dispatch_leave) {
        out->status = AKUSB01_BACKEND_UNAVAILABLE;
        return out->status;
    }
    if (!UsbwareSyncEp0Query_PeekCurrentDevice(g, generation, &device)) {
        out->status = AKUSB01_BAD_ATTACH_GENERATION;
        return out->status;
    }
    if (binding->verified_current_owner(binding->user, g, generation) != 1) {
        akusb01_revoke(g, generation, out);
        out->status = AKUSB01_OWNER_NOT_CURRENT;
        return out->status;
    }
    /* This wrapper does not duplicate the backend's atomic one-shot reservation, QUERY setup,
     * terminal interpretation or ACK check. The five-argument Run
     * is the sole admission/QUERY state machine. */
    call.binding = binding;
    call.out = out;
    call.device = 0;
    result = UsbwareSyncEp0Query_Run(g, generation, akusb01_adapter,
                                    &call, &out->query);
    if (result == AK_USBWARE_QUERY_ALREADY_ATTEMPTED) {
        out->status = AKUSB01_ALREADY_ATTEMPTED;
        return out->status;
    }
    if (!out->issued) {
        out->status = AKUSB01_STOPPED_OR_RETIRED;
        return out->status;
    }
    /* Both source and physical generation owner must remain verified before
     * ANY pointer-bearing drain/release callback. A failing check revokes ACK
     * and admission under backend lock; it NEVER invokes unsafe vendor cleanup. */
    if (!call.device || call.device != device ||
        !UsbwareSyncEp0Query_PeekCurrentDevice(g, generation, &now) ||
        now != device ||
        binding->verified_current_owner(binding->user, g, generation) != 1) {
        akusb01_revoke(g, generation, out);
        out->status = AKUSB01_OWNER_RETAINED_UNDRAINED;
        return out->status;
    }
    if (result == AK_USBWARE_QUERY_OK)
        decision = AKUSB01_ACK_RELEASED_SOURCE_ONLY;
    else if (result == AK_USBWARE_QUERY_SHORT_REPLY)
        decision = AKUSB01_WRONG_ACTUAL_LENGTH;
    else if (result == AK_USBWARE_QUERY_BAD_ACK)
        decision = AKUSB01_BAD_RESPONSE;
    else if (result == AK_USBWARE_QUERY_NOT_TERMINAL ||
             result == AK_USBWARE_QUERY_NATIVE_FAILED ||
             result == AK_USBWARE_QUERY_CALL_FAILED)
        decision = AKUSB01_TERMINAL_NOT_OK;

    if (binding->drain_verified(binding->user, device, generation) != 1) {
        akusb01_revoke(g, generation, out);
        out->status = AKUSB01_OWNER_RETAINED_UNDRAINED;
        return out->status;
    }
    out->drained = 1;
    /* The provider's dispatcher serializes physical owner withdrawal against
     * this final recheck, release and result publication. Holding only the
     * backend stateLock across an OEM callback would risk self-deadlock; instead
     * the backend atomically claims the final-release lease under its own lock. */
    if (binding->owner_dispatch_enter(binding->user, g, generation) != 1) {
        akusb01_revoke(g, generation, out);
        out->status = AKUSB01_OWNER_RETAINED_UNDRAINED;
        return out->status;
    }
    now = 0;
    if (!UsbwareSyncEp0Query_PeekCurrentDevice(g, generation, &now) ||
        now != device ||
        binding->verified_current_owner(binding->user, g, generation) != 1 ||
        !UsbwareSyncEp0Query_BeginReleaseLease(g, generation, device)) {
        akusb01_revoke(g, generation, out);
        out->status = AKUSB01_OWNER_RETAINED_UNDRAINED;
        binding->owner_dispatch_leave(binding->user, g, generation);
        return out->status;
    }
    /* This callback must not free another owner's memory. Physical detach
     * cannot be published until dispatcher leave. the backend can latch concurrent
     * Stop/WithdrawOwner; End rejects a positive ACK if that happened. */
    {
        const int released = binding->release_owned(binding->user, device, generation);
        const BOOL committed = UsbwareSyncEp0Query_EndReleaseLease(
            g, generation, device, released == 1 ? TRUE : FALSE);
        out->released = (released == 1) ? 1U : 0U; /* truthful call outcome */
        if (!committed) {
            akusb01_revoke(g, generation, out);
            out->status = (released == 1)
                ? AKUSB01_OWNER_RETAINED_UNDRAINED
                : AKUSB01_RELEASE_FAILED_RETAINED;
        } else {
            out->status = decision;
            if (decision == AKUSB01_ACK_RELEASED_SOURCE_ONLY)
                memcpy(out->response, out->query.raw, sizeof(out->response));
        }
    }
    binding->owner_dispatch_leave(binding->user, g, generation);
    return out->status;
}

AKUSB01_QUERY_STATUS akusb01_manual_query_once(
    AK_USBWARE_SYNC_EP0_GENERATION* g, DWORD generation,
    const AKUSB01_EXACT_OWNER_BINDING* binding, AKUSB01_QUERY_OBSERVATION* out)
{
    if (!out) return AKUSB01_BACKEND_UNAVAILABLE;
    if (!binding || !binding->query_terminal_sync ||
        !binding->verified_current_owner || !binding->drain_verified ||
        !binding->release_owned || !binding->owner_dispatch_enter ||
        !binding->owner_dispatch_leave) {
        memset(out, 0, sizeof(*out));
        out->status = AKUSB01_BACKEND_UNAVAILABLE;
        return out->status;
    }
    return akusb01_query_only(g, generation, binding, out);
}
