#ifndef AKUSB01_QUERY_TERMINAL_GATE_H
#define AKUSB01_QUERY_TERMINAL_GATE_H
#include "UsbwareSyncEp0QueryBackend.h"

/* Application-side terminal QUERY gate. No real USBware
 * ABI or provider supplied. All callbacks are external verified requirements.
 * This is NOT a controller owner acquisition or success proof. */
enum AKUSB01_QUERY_STATUS {
    AKUSB01_NO_VERIFIED_PROVIDER = 0,
    AKUSB01_BAD_ATTACH_GENERATION = 1,
    AKUSB01_OWNER_NOT_CURRENT = 2,
    AKUSB01_TERMINAL_NOT_OK = 3,
    AKUSB01_WRONG_ACTUAL_LENGTH = 4,
    AKUSB01_BAD_RESPONSE = 5,
    AKUSB01_STOPPED_OR_RETIRED = 6,
    AKUSB01_OWNER_RETAINED_UNDRAINED = 7,
    AKUSB01_RELEASE_FAILED_RETAINED = 8,
    AKUSB01_ACK_RELEASED_SOURCE_ONLY = 9,
    AKUSB01_ALREADY_ATTEMPTED = 10,
    AKUSB01_BACKEND_UNAVAILABLE = 11
};

typedef struct AKUSB01_QUERY_OBSERVATION_TAG {
    AKUSB01_QUERY_STATUS status;
    AK_USBWARE_QUERY_OBSERVATION query; /* truthful terminal/state/nativeStatus/actual/raw */
    unsigned char response[4];         /* populated only after verified drain+release */
    unsigned int issued;
    unsigned int drained;
    unsigned int released;
} AKUSB01_QUERY_OBSERVATION;

typedef struct AKUSB01_EXACT_OWNER_BINDING_TAG {
    /* All callback contracts are UNSUPPLIED. Physical generation, vendor
     * owner and provider lifetime must be exact verified by implementation. */
    int (*verified_current_owner)(void*, const AK_USBWARE_SYNC_EP0_GENERATION*, DWORD);
    /* TRUE is invocation returned; the backend validates separate actual transfer
     * completion fields, not guessed OEM nativeStatus success values. */
    BOOL (*query_terminal_sync)(void*, void*, const unsigned char[8],
                                AK_USBWARE_SYNC_COMPLETION*);
    int (*drain_verified)(void*, void*, DWORD);
    int (*release_owned)(void*, void*, DWORD);
    /* Mandatory generation-bound dispatcher pin. enter returns 1 only after
     * locking a VERIFIED same-owner lease. Detach/withdrawal MUST use the
     * same dispatcher; leave releases it after result publication. A NULL
     * dispatcher is BACKEND_UNAVAILABLE and NEVER issues USB. */
    int (*owner_dispatch_enter)(void*, const AK_USBWARE_SYNC_EP0_GENERATION*, DWORD);
    void (*owner_dispatch_leave)(void*, const AK_USBWARE_SYNC_EP0_GENERATION*, DWORD);
    void* user;
} AKUSB01_EXACT_OWNER_BINDING;

/* Explicit compatibility helper; this sample registers no UI or startup action.
 * Use requires a verified vendor provider binding.
 * An absent backend returns BACKEND_UNAVAILABLE without setting one-shot. */
AKUSB01_QUERY_STATUS akusb01_manual_query_once(
    AK_USBWARE_SYNC_EP0_GENERATION* g, DWORD physical_generation,
    const AKUSB01_EXACT_OWNER_BINDING* binding, AKUSB01_QUERY_OBSERVATION* out);

AKUSB01_QUERY_STATUS akusb01_query_only(
    AK_USBWARE_SYNC_EP0_GENERATION* g, DWORD physical_generation,
    const AKUSB01_EXACT_OWNER_BINDING* binding, AKUSB01_QUERY_OBSERVATION* out);
#endif
