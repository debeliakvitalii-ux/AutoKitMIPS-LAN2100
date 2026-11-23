# Hardware compatibility — AutoKitMIPS

## Primary target

| Item | Target |
|---|---|
| Head unit | Kia LAN2100 |
| Platform | LGE_AU1350_YN |
| Operating system | Windows CE 5.0 |
| CPU / ABI | MIPSII, 32-bit |
| USB host stack | Jungo USBware HOST 3.5.14.64 |
| USB owner | Existing stock USBware/JIPOD owner |
| Peripheral | Compatible AutoKit USB adapter |

This is the first development target. It does not mean the complete AutoKit adapter connection is already working on the physical head unit.

## Adapter compatibility

AutoKitMIPS is intended to communicate with a compatible AutoKit USB adapter through the head unit's existing USB host stack and real device owner.

Compatibility must be checked for the exact AutoKit adapter model and firmware, including:
1. USB descriptors and enumeration behavior;
2. supported adapter protocol and operating mode;
3. required authentication and session behavior;
4. USB networking/data interfaces used by that adapter;
5. compatibility of its CarPlay or Android Auto session with the head unit's existing multimedia software path.

A generic "CarPlay" or "Android Auto" product description is not sufficient evidence. Other dongles, adapters or interface products are not automatically compatible.

## What AutoKitMIPS uses on the head unit

The project is limited to the AutoKitMIPS application and its software/data connection to the AutoKit adapter:
- the existing OEM USB host stack and device owner;
- supported MIPS/Windows CE application interfaces;
- verified adapter protocol and session interfaces;
- existing networking facilities where compatible;
- existing multimedia software components for audio/video data delivery, where documented interfaces permit it.

No new head-unit display hardware or external display interface is part of the target configuration.

## What is outside this project's scope

AutoKitMIPS does not develop, replace, operate, remap, configure or integrate:
- factory touchscreen or resistive touch panel;
- factory physical buttons or steering-wheel controls;
- button mapping or button configuration;
- external displays, external display interfaces or external video-output hardware;
- aftermarket interface boxes;
- the factory head-unit user interface, menus or control handling.

The AutoKitMIPS application may have its own status, diagnostics and adapter-configuration screen. That does not alter the head unit's factory UI.

## Future MIPS head-unit ports

Other MIPS-based automotive head units may be considered as separate compatibility targets. Each port requires evidence for:
1. exact CPU architecture and ABI;
2. Windows CE version and OEM build;
3. USB host stack vendor and exact version;
4. supported application/service entry into the real USB device owner;
5. exact AutoKit adapter USB and protocol compatibility;
6. compatible networking and existing multimedia software interfaces;
7. a target build and physical hardware validation.

A shared CPU family, similar connector or factory display does not establish compatibility. Other operating systems, CPU families, USB stacks and aftermarket interface boxes are not automatically supported.

## How to report a compatible setup

Please open an issue and provide information you are allowed to share:
- head-unit manufacturer and exact model/revision;
- CPU/OS version, if known;
- USB host stack and version, if known;
- exact AutoKit adapter model and firmware;
- sanitized USB enumeration details or logs;
- steps performed and observed result.

Do not upload confidential OEM firmware, SDKs, memory dumps, credentials, certificates or personal data.
