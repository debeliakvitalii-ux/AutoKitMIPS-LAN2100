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

This is the first target for development. It does not mean the complete AutoKit adapter connection is already working on the physical head unit.

## What AutoKitMIPS connects

The subject of this project is the USB AutoKit adapter and the software path required for the MIPS head unit to communicate with it.

Compatibility must be checked for the exact adapter model and firmware, including its USB descriptors, protocol mode and required session/authentication behavior. A generic "CarPlay" or "Android Auto" product description is not sufficient evidence.

## What is outside this project's scope

The following are existing head-unit functions and are not developed, replaced, remapped or configured by AutoKitMIPS:
- factory touchscreen or resistive touch panel;
- factory physical buttons and steering-wheel controls;
- the head unit's original user interface and button handling.

AutoKitMIPS is intended to work with the existing multimedia facilities of the head unit. Its scope is the AutoKit adapter connection and the software/data path from that adapter into the existing system.

## Future MIPS head-unit ports

Other MIPS-based automotive head units may be considered, but each is a separate compatibility target. Required evidence includes:
1. exact CPU architecture and ABI;
2. Windows CE version and OEM build;
3. USB host stack vendor and exact version;
4. supported application/service entry into the real USB device owner;
5. adapter USB/protocol compatibility;
6. a target build and physical hardware validation.

A shared CPU family or similar connector does not establish compatibility. Other operating systems, CPU families or USB stacks are not automatically supported.

## How to report a compatible setup

Please open an issue and provide information you are allowed to share:
- head-unit manufacturer and exact model/revision;
- CPU/OS version, if known;
- USB host stack and version, if known;
- exact AutoKit adapter model and firmware;
- USB enumeration details or sanitized logs;
- steps performed and observed result.

Do not upload confidential OEM firmware, SDKs, memory dumps, credentials, certificates or personal data.
