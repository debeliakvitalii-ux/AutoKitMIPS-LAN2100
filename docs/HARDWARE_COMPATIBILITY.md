# Hardware compatibility and support boundaries

## Confirmed development target

| Item | Target |
|---|---|
| Automotive head unit | Kia LAN2100 |
| Platform/board family | LGE_AU1350_YN |
| Operating system | Windows CE 5.0 |
| CPU/ABI | MIPSII, 32-bit |
| USB host stack | Jungo USBware HOST 3.5.14.64 |
| USB ownership path | Existing stock USBware/JIPOD owner |

This identifies the primary engineering target. It does not mean the complete software currently runs on the head unit.

## Intended peripheral classes

The project intends to investigate and, where supported, connect:
- compatible external USB CarPlay adapters/dongles;
- compatible Android Auto projection adapters/devices;
- USB devices exposing a documented and supported transport/network interface;
- the head unit's built-in display, audio path, touchscreen and physical controls.

Exact adapter models, firmware versions, USB descriptors, protocol modes and licensing/authorization requirements must be recorded before compatibility can be claimed. A product advertised as "wireless CarPlay/Android Auto" is not automatically compatible with this head unit or software.

## Display and input variations

The application is expected to accommodate different head-unit display and input arrangements through model-specific configuration, not assumptions:
- display resolution and orientation (for example, nominal 800×480 only when confirmed for the target);
- framebuffer/pixel format and available graphics APIs;
- capacitive or resistive touch, including calibration and coordinate mapping;
- physical front-panel buttons and steering-wheel controls;
- OEM-specific key events, GPIO or accessory interfaces, only where documented and safely accessible.

No universal pinout, button mapping, touch protocol or video interface is implied.

## Potential future ports — not yet supported

Other Kia/Hyundai head units, other LAN-series devices, other Windows CE versions, ARM or other MIPS variants, and devices using Microsoft USB stack or a different Jungo release may be considered only as separate ports.

A port requires evidence for:
1. CPU architecture, ABI, compiler and OS version;
2. board/BSP and OEM service interfaces;
3. USB host stack and device ownership model;
4. display and audio interfaces;
5. touch and button input paths;
6. real hardware validation.

Do not label a model compatible based only on vehicle make, connector shape, OS family or similar product name.

## How to report a device

Please open an issue and include only information you are permitted to share:
- manufacturer, head-unit model and hardware revision;
- OS build/version and CPU architecture, if known;
- USB host stack/vendor/version, if known;
- adapter model and firmware version;
- display resolution and touch type;
- what works, what fails, and exact reproduction steps;
- logs stripped of serial numbers, personal data and secrets.

Do not upload proprietary firmware, confidential SDKs, credentials, certificates or raw memory dumps.
