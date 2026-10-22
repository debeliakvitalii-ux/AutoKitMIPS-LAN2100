# USB HOST EP0 integration: help needed

## Target and missing implementation

Project target: LAN2100, Windows CE 5.0, MIPSII, LGE_AU1350_YN, Jungo USBware HOST 3.5.14.64. The intended integration must cooperate with the existing USB owner (`jipod`) and preserve existing UWD1/MGR1 operation.

We need a real, version-matched way to submit a host control transfer and receive its terminal result. The callback names in our sample are application interfaces, **not discovered vendor exports**. Neither an internal `usb_device*` identity nor a DWORD returned by a device command establishes a callable EP0 API or a transfer result. Microsoft CE `USB_HANDLE` / `USB_FUNCS` must not be assumed interchangeable with USBware internal handles.

## First transaction to enable

Control IN setup packet (8 bytes):

```text
C0 53 00 00 00 00 04 00
```

Expected response (4 bytes): `01 00 00 00`. A positive application result requires terminal native success, actual length exactly four, matching response, current attach generation, successful drain and a valid owner-protected release. Callback return alone is insufficient.

## Concrete contributions that unblock integration

1. Matching headers and a callable implementation/import library, or an existing-owner broker interface, with provenance and permission to share.
2. Actual host device-handle acquisition: attach callback/client registration, generation identity and ownership rules for this platform.
3. Exact EP0 function prototype, calling convention, structure layout and synchronous/asynchronous completion semantics.
4. Native status interpretation and actual transferred length, including cancellation and drain behavior.
5. Device/context lifetime protection across submit, completion, drain, release and detach. Show which dispatcher or reference mechanism prevents physical destruction.

A small, documented example against this exact platform is more useful than a generic USB tutorial. If a suggested API is from another BSP/version, state the difference explicitly. Please do not infer IOCTL values, MMIO addresses or vendor success constants.

## Published work and integration boundary

See [the sample](../examples/usb-query-boundary/): generation-bound one-shot QUERY, separate terminal/native/length evidence, ownership recheck after drain, and a final dispatcher-protected release transaction. Host tests reproduce withdrawal races using fake handles; no hardware driver or OEM SDK is bundled.

| Application callback | Evidence required from a real provider |
| --- | --- |
| `verified_current_owner` | Same live device and attach generation |
| `query_terminal_sync` | Real terminal completion, native status, actual bytes |
| `drain_verified` | No outstanding operation can access released resources |
| `owner_dispatch_enter/leave` | Same physical-owner dispatcher used by detach |
| `release_owned` | Release only resources owned by this generation |

**Lifetime precondition:** the caller/provider must keep generation storage, context and device valid for the entire call, including the initial pointer read, submit and drain. The final-release dispatcher in this sample does not establish that earlier lifetime protection. `contextLifetimeBarrierProven` is a caller assertion, not a mechanism or proof. Do not enable it without a real implementation.

Stop/retire flags do not cancel native transfers. The provider must define completion/drain on every path, including unsuccessful calls. Generation records are single-use and must not be reinitialized while published.

## How to help

Reply in [issue #1](https://github.com/debeliakvitalii-ux/AutoKitMIPS-LAN2100/issues/1) with platform/version, relevant prototype or a link to shareable documentation, handle origin and lifetime/completion rules. For a code correction, identify the file, failing condition and a minimal reproducer. Hardware deployment is outside this sample's validation scope.

## Current integration boundary

The application build target is Windows CE / eVC4 MIPSII. Runtime diagnostics also identify a kernel-resident USBware module and a CE6 kernel context. A matching version string alone does not establish the native module's ABI or calling context. `jipod` names the owner/client path under investigation, not a verified standalone process.

The immediate missing link is an application/service-callable boundary to the live owner, with a device lifetime guarantee established before the first pointer access. A function located inside firmware is not automatically callable from an EXE. We also need actual transferred length and cancellation/drain semantics on timeout and detach.

A compatible SDK is useful but is not the only possible contribution: a documented existing broker, a supported owner extension point, or a concrete counterexample to an integration assumption can also help. Please distinguish target evidence from TI DSP/BIOS references, Microsoft USB_FUNCS shims, BSD headers and Jungo WinDriver WDU APIs; these are not interchangeable implementations.

The complete path still includes role-switch completion, OEM function-side support, iAP2/identity/authentication, NCM and IP before an authenticated media session. One successful QUERY would close only the first control-transfer milestone.
