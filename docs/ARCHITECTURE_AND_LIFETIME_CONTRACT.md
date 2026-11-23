# Architecture and lifetime contract

## Intended boundary

```text
AutoKitMIPS application
        |
        | documented IPC / OEM-supported service API
        v
same live stock USBware JIPOD owner
        |
        | generation-bound physical-device lease
        v
native USBware HOST transfer path
        |
        v
USB host controller / physical device
```

This is the target architecture, not a claim that every boundary has already been discovered or implemented.

## Ownership invariants

- There must be one authoritative owner for the physical device.
- The operation must use the same live stock owner that manages the current attachment.
- The device identity must refer to the real physical attachment.
- Every operation must be bound to an attach generation.
- A stale generation must never be allowed to submit, complete, or release resources for a newer attachment.
- A caller must hold a valid lifetime reference before reading owner/device/context pointers.
- Detach and final release must serialize through the real owner dispatcher or an equivalent proven mechanism.

## Transfer invariants

A positive result requires all of the following:

1. Submission was accepted by the native API.
2. A terminal native completion was observed.
3. Native status is a documented success value for this exact stack/version.
4. Actual transferred length is exactly the requested length.
5. Received bytes match the expected response.
6. The attach generation and owner lease are still valid.
7. DMA/controller access is finished.
8. All callbacks that can touch the operation's resources have retired.
9. Release is performed exactly once and only for resources owned by this generation.

Submission acceptance is not terminal success. A callback return is not necessarily terminal success. A stop flag is not cancellation. A software timeout is not proof that hardware stopped accessing the buffer.

## Failure behavior

Any unknown status, length mismatch, stale generation, failed drain, uncertain callback state, or ambiguous release must fail closed. Do not emit a positive QUERY result.

On timeout or detach, the provider must document whether it:
- synchronously cancels and drains;
- waits for a terminal completion;
- retains resources until a later retirement event; or
- reports that safe retirement cannot yet be established.

If safe retirement cannot be proven, retain/poison the relevant state and report failure; never free memory still reachable by DMA or callbacks.

## Host sample boundary

The public sample models application-side validation and ownership sequencing. Its provider callbacks are interfaces for a future real provider, not discovered vendor exports. A caller-supplied boolean or assertion is not a lifetime mechanism. The sample cannot establish the lifetime of a private kernel object before the first pointer access.

## ABI discipline

Before implementing a native adapter, record:
- exact module and version;
- exact platform and execution context;
- function declaration and calling convention;
- structure sizes, alignment and field offsets;
- handle/object origin and ownership rules;
- completion and status semantics;
- cancellation and drain contract;
- source/document provenance and redistribution permission.

Do not substitute an API from another Jungo product, Windows CE USB stack, BSP, or operating-system version without proving compatibility.
