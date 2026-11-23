# AutoKitMIPS — project overview

## Purpose

AutoKitMIPS is an open engineering project to develop a software platform that can bring modern phone-projection connectivity to selected legacy automotive multimedia head units running Windows CE.

The first target is the Kia LAN2100 / LGE_AU1350_YN head unit. The long-term goal is to connect compatible external CarPlay or Android Auto adapters and bridge their sessions to the head unit's display, audio output and driver controls.

This is a development project, not a finished product. The repository must distinguish intended features from implemented and hardware-verified features.

## Why this project exists

Many older factory head units still provide useful vehicle integration, physical controls and an installed display, but do not offer modern phone projection. Replacing the whole head unit can mean losing factory integration. AutoKitMIPS explores whether a compatible external adapter and custom software can add that functionality while retaining the original unit.

This is a technical goal, not a guarantee that every factory head unit or adapter can be supported.

## Planned software layers

- Application and user interface: startup, connection state, settings and diagnostics.
- Hardware adaptation: model-specific display, audio, touch, buttons and OEM service integration.
- USB host transport: safe communication with the physical adapter through the platform's existing USB stack.
- Protocol/session layer: supported CarPlay/iAP2 or Android Auto/AAP session handling, subject to device and licensing requirements.
- Network bridge: USB networking, IP configuration and required service discovery.
- Media bridge: audio/video transport and synchronization.
- Input bridge: touch coordinates, resistive touch, physical keys and steering-wheel controls where accessible.
- Configuration: model-specific display geometry, key mappings and diagnostics.

These are planned layers. A successful low-level USB request alone does not mean a complete phone-projection session works.

## Initial hardware target

- Kia LAN2100 / LGE_AU1350_YN
- Windows CE 5.0
- MIPSII 32-bit
- Jungo USBware HOST 3.5.14.64
- Existing stock USBware/JIPOD ownership path

Other head units and USB stacks are future port candidates only. See [HARDWARE_COMPATIBILITY.md](HARDWARE_COMPATIBILITY.md).

## Current status

The native USB transport through the real stock owner is not yet proven or implemented as a working target path. There is no verified CE5/MIPSII release build and no complete CarPlay/Android Auto hardware session. Public host-testable components cover limited application-side contracts only.

The immediate engineering dependency is a documented, supported application/service entry into the same live stock USB owner, followed by a verified native EP0 transfer and safe completion, cancellation, drain and detach behavior.

## Who can help

Contributors with experience in Windows CE, Platform Builder/eVC4, MIPS, Jungo USBware, USB host stacks, automotive BSPs, iAP2, Android Auto/AAP, USB networking, audio/video pipelines, touchscreens and embedded UI are welcome.

Useful contributions include:
- exact platform-matched SDK/API documentation with redistribution permission;
- reproducible hardware identification and compatibility reports;
- minimal, reviewable source patches;
- target build fixes and build instructions;
- display/audio/input integration knowledge;
- independent review and real-hardware test evidence.

Start with [CONTRIBUTING.md](../CONTRIBUTING.md), the [implementation roadmap](IMPLEMENTATION_ROADMAP.md), and [open issues](https://github.com/debeliakvitalii-ux/AutoKitMIPS-LAN2100/issues).

## Safety and licensing

Do not upload confidential OEM SDKs, firmware images, memory dumps, keys, certificates, credentials or personal data. Public repository visibility does not grant redistribution rights. Check the repository's licensing status before reusing or publishing third-party code.
