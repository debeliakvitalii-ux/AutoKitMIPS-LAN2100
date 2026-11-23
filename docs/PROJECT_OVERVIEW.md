# AutoKitMIPS — project overview

## Purpose

AutoKitMIPS is software intended to connect a compatible **AutoKit USB adapter** to an automotive head unit built around a MIPS processor and running Windows CE. Its job is to establish and maintain the real USB connection to the adapter, exchange the required protocol data, and deliver the adapter's CarPlay or Android Auto session data to the head unit's existing multimedia software.

This project is specifically about integrating the AutoKit adapter with MIPS-based head units. It is not a project for developing the head unit's touchscreen, resistive touch panel, physical buttons, steering-wheel controls or factory user interface. Those are existing head-unit functions and remain outside AutoKitMIPS scope.

## Why this project exists

Older factory head units may have capable displays, audio systems and factory controls but lack a supported way to connect a modern AutoKit projection adapter. AutoKitMIPS explores how to add that adapter connection in software while retaining the original head unit and its existing user controls.

The aim is to make a compatible AutoKit adapter communicate with the legacy MIPS/Windows CE platform; it is not a promise that every adapter or every MIPS head unit will work.

## Project scope

- Detect and identify the connected AutoKit USB adapter.
- Establish a real USB host transport through the head unit's existing OEM USB stack and device owner.
- Manage adapter attach/detach, ownership, generations and transfer lifetimes safely.
- Exchange the protocol messages required by the adapter.
- Establish the relevant CarPlay or Android Auto session, subject to the adapter model and applicable licensing/authentication requirements.
- Carry the required network traffic between the adapter and phone session.
- Deliver audio/video or other session data to the head unit's existing multimedia components, where their interfaces permit it.
- Provide connection status, diagnostics and adapter-related configuration.

The factory display, touchscreen, buttons and steering-wheel controls are not being implemented or replaced by this project. AutoKitMIPS is intended to use the head unit's existing system facilities.

## Initial hardware target

- Kia LAN2100 / LGE_AU1350_YN
- Windows CE 5.0
- MIPSII 32-bit
- Jungo USBware HOST 3.5.14.64
- Existing stock USBware/JIPOD ownership path

Other MIPS-based head units may be considered as separate ports after their CPU/ABI, Windows CE version, OEM USB stack, device ownership path and multimedia interfaces are verified. Compatibility must not be inferred from the MIPS label alone.

See [HARDWARE_COMPATIBILITY.md](HARDWARE_COMPATIBILITY.md).

## Current status

The native USB transport through the real stock USBware/JIPOD owner is not yet proven or implemented as a working target path. There is no verified CE5/MIPSII release build and no complete AutoKit adapter session running on the physical LAN2100.

The immediate dependency is a documented, supported application/service entry into the same live stock USB owner, followed by a verified native USB transfer and safe completion, cancellation, drain and detach behavior.

See the [development status](DEVELOPMENT_STATUS_2026-11-23.md), [architecture and lifetime contract](ARCHITECTURE_AND_LIFETIME_CONTRACT.md), [USB integration help request](USB_HELP.md), and [implementation roadmap](IMPLEMENTATION_ROADMAP.md).

## Who can help

We welcome contributors experienced in:
- Windows CE 5.0, Platform Builder and eVC4;
- MIPS/MIPSII and embedded C/C++;
- Jungo USBware and USB host stacks;
- automotive OEM BSP and device-owner integration;
- AutoKit adapter protocols and CarPlay/Android Auto session integration;
- USB networking/NCM and embedded network stacks;
- audio/video data paths on legacy Windows CE head units;
- testing on compatible MIPS head units and AutoKit adapters.

Useful contributions include platform-matched API evidence, reproducible adapter identification, minimal source patches, target build fixes, protocol documentation that may legally be shared, and real-hardware test evidence.

Start with [CONTRIBUTING.md](../CONTRIBUTING.md), the [implementation roadmap](IMPLEMENTATION_ROADMAP.md), and [open issues](https://github.com/debeliakvitalii-ux/AutoKitMIPS-LAN2100/issues).

## Safety and licensing

Do not upload confidential OEM SDKs, firmware images, memory dumps, keys, certificates, credentials or personal data. Public repository visibility does not grant redistribution rights. Check licensing and provenance before publishing third-party code or documentation.
