# USBware / JIPOD owner findings — 2026-11-23

## Purpose

This document consolidates the public-safe conclusions from the B131/C131 and B132/C133 work. It is intended to prevent repeated broad scans and to make the remaining evidence gap explicit.

## What is established

### 1. Stock stream front-end

The Production configuration examined in the private B132 investigation registers the stock USBware stream device as `UWD1:`. The USBware image exposes a stream front-end entry. This establishes that a stock stream-device front end exists in the examined image.

It does **not** establish that an application opens `UWD1:`, that a particular IOCTL reaches JIPOD, or that the stream front-end exposes arbitrary EP0 transfers.

### 2. Existing-owner registration/dispatch evidence

The bounded B131 investigation found static registration/dispatch relationships associated with the stock USB hub/JIPOD path. C131 accepted the static findings with limitations.

Static call paths and method-table relationships are not proof of the runtime parent node for a specific attachment, a live device lease, or an application-callable entry.

### 3. Production application boundary examined

The B132 investigation did not establish a Production application callsite that opens `UWD1:` and submits the required raw USB control request. The statically identified MyShell IOCTL path was GPIO/pin-control plumbing, not USB transport.

Generic imports such as `CreateFileW`, `DeviceIoControl`, or driver-loading functions do not identify the target device handle or prove a call into the JIPOD owner.

An unrelated IPOD/UART driver must not be conflated with USBware's JIPOD owner.

## What remains unproven

All of the following are still required before a native provider can be considered ready:

1. A supported application- or service-callable entry into the **same live stock JIPOD owner**.
2. Proof that the caller obtains the currently attached physical device, not a synthetic VID/PID or stale object.
3. A real owner/device lease carrying an unambiguous attach generation and valid lifetime before the first dereference.
4. A version-matched native EP0 submission contract, including setup packet, data buffer, direction, requested length, timeout, and calling convention.
5. Genuine terminal native completion status, distinct from submit acceptance or callback return.
6. Exact actual-length reporting and received bytes.
7. Defined timeout, cancellation, detach, and error behavior.
8. Proof that DMA/controller access and all callbacks have retired before descriptors, buffers, owner references, or device state are released.
9. A release path serialized with the actual owner/detach dispatcher.

## First required transaction

```text
bmRequestType = 0xC0
bRequest      = 0x53
wValue        = 0x0000
wIndex        = 0x0000
wLength       = 0x0004

Setup bytes:  C0 53 00 00 00 00 04 00
Expected RX:  01 00 00 00
Actual length: 4
```

The values above describe the required test transaction, not a discovered native function prototype.

## Prohibited shortcuts

- Do not call an internal firmware/ROM address as if it were a public ABI.
- Do not assume Microsoft CE `USB_HANDLE` / `USB_FUNCS` are interchangeable with USBware-private types.
- Do not treat a `UWD1:` open, generic DeviceIoControl import, logical receive/send command, submit return, or callback return as proof of raw EP0 completion.
- Do not infer an IOCTL number or native success constant.
- Do not create a second HCD, synthetic owner, fake attach generation, or fake ACK.
- Do not repeat broad ROM scans or generic IOCTL searches without new primary evidence or a specifically identified unanswered question.

## Evidence needed from a contributor

A useful contribution should include the exact USBware HOST version, CE/BSP/CPU, execution context, declaration and implementation provenance, device-handle origin, generation/lifetime rules, native status and actual-length semantics, cancellation/drain behavior, and a reproducible test boundary. See [the evidence template](CONTRIBUTOR_EVIDENCE_TEMPLATE.md).
