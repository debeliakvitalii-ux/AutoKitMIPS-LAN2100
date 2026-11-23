# AutoKitMIPS — project overview

## Purpose

AutoKitMIPS is an application intended to connect a compatible **AutoKit USB adapter** to an automotive head unit built around a MIPS processor and running Windows CE. Its purpose is to establish the adapter's USB connection through the head unit's existing OEM USB stack/device owner, exchange adapter protocol data, support the compatible phone session, and carry session data to the head unit's existing multimedia software components.

This project is specifically about the AutoKit adapter and the software/data path between that adapter and a compatible MIPS head unit. It does not develop or modify the head unit's factory user interface or physical controls.

## Why this project exists

Some legacy MIPS/Windows CE head units do not have a verified software path for a compatible AutoKit USB adapter. AutoKitMIPS investigates that application-side integration while retaining the existing head-unit firmware, USB host stack and multimedia software interfaces.

The goal is a reliable adapter connection on explicitly validated hardware. It is not a promise that every AutoKit adapter, every MIPS head unit or every USB stack will work.

## Project scope

- Run the AutoKitMIPS application on a supported MIPS/Windows CE head unit.
- Identify and communicate with a compatible AutoKit USB adapter.
- Establish a real USB host transport through the head unit's existing OEM USB stack and device owner.
- Manage adapter attach/detach, ownership, generations and transfer lifetimes safely.
- Exchange the protocol messages required by the exact adapter model and firmware.
- Establish the relevant CarPlay or Android Auto session, subject to adapter compatibility and applicable licensing/authentication requirements.
- Carry the required network traffic between the adapter and phone session.
- Deliver audio/video or other session data to the head unit's existing multimedia software components, where their documented interfaces permit it.
- Provide adapter connection status, diagnostics and adapter-related configuration in the AutoKitMIPS application.

These are project goals, not a statement that the listed capabilities are already implemented or working on target hardware. The current development status and validation evidence are documented separately.

## Explicit exclusions

AutoKitMIPS does not develop, replace, operate, configure, remap or integrate:
- factory touchscreen or resistive touch panels;
- physical head-unit buttons or steering-wheel buttons;
- button mapping or button configuration;
- external displays, external display interfaces or external video-output hardware;
- aftermarket interface boxes;
- the factory head-unit user interface, menus or control system.

The project may have its own application screen for adapter connection status, diagnostics and adapter settings. That application screen is not a modification of the factory head-unit UI.

## Initial hardware target

- Kia LAN2100 / LGE_AU1350_YN
- Windows CE 5.0
- MIPSII 32-bit
- Jungo USBware HOST 3.5.14.64
- Existing stock USBware/JIPOD ownership path
- Compatible AutoKit USB adapter

This is the first development target. Its listing does not establish that the complete adapter connection is already working on the physical head unit.

Other MIPS-based head units may be considered as separate ports only after their CPU/ABI, Windows CE version, OEM USB stack, supported application entry into the real device owner, AutoKit adapter compatibility and existing multimedia software interfaces are verified. Compatibility must not be inferred from the MIPS label alone.

See [HARDWARE_COMPATIBILITY.md](HARDWARE_COMPATIBILITY.md).

## Current status

The native USB transport through the real stock USBware/JIPOD owner is not yet proven or implemented as a working target path. There is no verified CE5/MIPSII release build and no complete AutoKit adapter session running on the physical LAN2100.

The immediate dependency is a documented, supported application/service entry into the same live stock USB owner, followed by a verified native USB transfer and safe completion, cancellation, drain and detach behavior. No readiness claim should be based only on host tests, static ROM addresses, historical traces or a synthetic provider.

See the [development status](DEVELOPMENT_STATUS_2026-11-23.md), [architecture and lifetime contract](ARCHITECTURE_AND_LIFETIME_CONTRACT.md), [USB integration help request](USB_HELP.md), and [implementation roadmap](IMPLEMENTATION_ROADMAP.md).

## Who can help

We welcome contributors experienced in:
- Windows CE 5.0, Platform Builder and eVC4;
- MIPS/MIPSII and embedded C/C++;
- Jungo USBware and USB host stacks;
- OEM BSP and application-to-device-owner integration;
- AutoKit adapter protocols and CarPlay/Android Auto session integration;
- USB networking/NCM and embedded network stacks;
- audio/video data transport to existing multimedia software components;
- testing on compatible MIPS head units with AutoKit USB adapters.

Useful contributions include platform-matched API evidence, reproducible adapter identification, minimal source patches, target build fixes, protocol documentation that may legally be shared, and real-hardware test evidence.

Start with [CONTRIBUTING.md](../CONTRIBUTING.md), the [implementation roadmap](IMPLEMENTATION_ROADMAP.md), and [open issues](https://github.com/debeliakvitalii-ux/AutoKitMIPS-LAN2100/issues).

## Safety and licensing

Do not upload confidential OEM SDKs, firmware images, memory dumps, keys, certificates, credentials or personal data. Public repository visibility does not grant redistribution rights. Check licensing and provenance before publishing third-party code or documentation.
