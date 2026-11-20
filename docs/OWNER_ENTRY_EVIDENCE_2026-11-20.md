# Additional owner-entry evidence (2026-11-20)

This note records a bounded offline review completed after the original public sample was published. It is intended to help contributors avoid repeating checks and to identify the exact missing integration evidence.

## What was checked

A pinned historical OEM ActiveSync/sibling Phaethon executable and map were inspected offline. The inspected executable is **not proven to be the production executable running on the target LAN2100**. An independent scanner reproduced the bounded direct-callsite inventory:

- 6 direct `LoadLibraryW` call sites;
- 15 direct `GetProcAddressW` call sites;
- 4 direct `DeviceIoControl` call sites.

The observed dynamic loads and resolved names were resource, message, UI and logging related. The four direct `DeviceIoControl` sites related to version, receive, stop-receive and a command value taken from a record. This inventory did not identify a documented `jipod` owner-extension loader or a proven raw USB control-transfer entry.

This is a bounded static result for that pinned sibling executable. It does **not** prove that no indirect/private mechanism exists elsewhere, and it must not be treated as a complete scan of the production LAN2100 executable.

## PC QUERY observation — separate evidence level

The project owner reported one PC-side vendor control-IN transaction to the Carlinkit device:

- Setup: `C0 53 00 00 00 00 04 00`
- Reported actual length: `4`
- Reported bytes: `01 00 00 00`

This is a user-observed PC result, not an independently captured USB wire trace. It is not a Windows CE, USBware-owner or LAN2100 head-unit result. The PC test is not a substitute for the missing target integration.

## What remains required

A contributor still needs to provide a supported, version-matched USBware HOST 3.5.14.64 / CE5 / MIPSII integration contract or implementation that establishes all of the following:

1. An application/service-callable entry into the **existing live USB owner**.
2. A valid reference to the currently attached physical device and an attach-generation/lifetime guarantee.
3. A genuine control-IN EP0 submission for the setup packet above.
4. Terminal native status, exact transferred length and received bytes.
5. Defined timeout, cancellation and detach behavior.
6. A controller-backed guarantee that the final DMA access and callbacks have ended before buffers, descriptors or device references are released.

A firmware address, sibling-platform executable, logical command status, successful submit return, synthetic host callback or PC-side QUERY result does not establish these requirements.

## Scope and safety

No proprietary SDK, firmware image, credentials, private source or guessed IOCTL is included here. No new HCD, USBware replacement, ROM patch, direct kernel-address call or hardware-side change is proposed. The published QUERY sample remains a host-testable application boundary only; it is not a native USB provider or a head-unit-ready executable.

## Evidence provenance

The independent review is recorded in the project work log as **C-COORD58-PC-PROOF-AND-NATIVE-OWNER-GATE (2026-11-04)**. Its scoped conclusion was:

- sibling executable direct-callsite inventory: accepted for the bounded offline source scope only;
- native USBware owner backend: waiting for exact callable ABI and lifetime evidence;
- PC QUERY: user-observed pass;
- LAN2100 head-unit QUERY and real USB transport: not proven.

Contributors with a matching SDK, documented owner broker, supported extension point or production-target call trace should reply in [issue #1](https://github.com/debeliakvitalii-ux/AutoKitMIPS-LAN2100/issues/1), stating the exact platform/version and redistribution permissions.
