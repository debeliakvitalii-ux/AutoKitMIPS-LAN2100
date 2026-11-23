# AutoKitMIPS development status — 2026-11-23

## Executive status

**Project state: active research / native USB integration blocked.**

The public repository contains a host-testable QUERY boundary example. It does not yet contain a production USBware HOST provider, a CE5/MIPSII target build, or a verified LAN2100 hardware transport.

The highest-priority engineering problem remains a supported application/service-callable path into the **same live stock JIPOD owner** used by Jungo USBware HOST 3.5.14.64, followed by a proven native EP0 transfer and safe completion/lifetime handling.

Do not describe the project as having working physical USB, CarPlay, or a head-unit-ready release.

## Target platform

- Head unit family: Kia LAN2100 / LGE_AU1350_YN.
- OS: Windows CE 5.0.
- CPU/ABI: MIPSII, 32-bit target.
- USB stack: Jungo USBware HOST 3.5.14.64.
- Ownership model under investigation: existing stock USBware/JIPOD owner.
- First control-IN request:
  - Setup: `C0 53 00 00 00 00 04 00`
  - Expected data: `01 00 00 00`
  - Required actual length: exactly 4 bytes.

## Current evidence snapshot

| Area | Current conclusion | What this does not prove |
|---|---|---|
| Public QUERY boundary sample | Host-testable C++98 boundary; repository documents 46/46 host assertions | No native USB calls, target build, or hardware transfer |
| A129 callback lifetime correction | C129 accepted the narrow source correction as `ACCEPT_SOURCE_ONLY` | Not integrated into canonical source; no target build/runtime proof |
| B131 registration/dispatch evidence | C131 accepted with limitations | No live parent-node identity, callable application entry, or EP0 ABI |
| B132 Production user-mode/owner-entry investigation | C133 accepted with limitations | No Production caller into UWD1/JIPOD and no native EP0 contract |
| Physical LAN2100 QUERY | Not proven | No claim of a successful target-side QUERY |
| Full CarPlay path | Not ready | Role switching, iAP2/authentication, NCM/IP and media session remain separate milestones |

## Accepted review records

These are public-safe summaries of review outcomes; the underlying OEM evidence packages are not included in this repository.

- **C129 / A129:** `ACCEPT_SOURCE_ONLY`. The review accepts only the scoped RX callback-retirement/lifetime source correction. It does not accept canonical integration, target compilation, hardware behavior, or production network readiness.
  - Review report SHA-256: `57978ea9282478878bc1a16249feb84b34299afdf009a09f2fc3993520c647b5`.
  - A129 handoff archive SHA-256: `2ed6ae291c9e9341dacf533f19fbea7e6233132508ba3776561e51dd36e792e5`.
  - **Integration warning:** A129 was prepared against a declared source preimage that differs from the source files in the historical LINK4 ZIP. Applying the diff to that ZIP does not reproduce the declared postimage hashes. Do not cherry-pick or claim integration without checking the exact current parent files and obtaining a parent-integration review.
- **C131 / B131:** `ACCEPT_WITH_LIMITATIONS`. Static registration/dispatch findings are accepted only within the bounded evidence scope.
  - Review report SHA-256: `cddb1cf967cd49c0017a5782265cbd41a8e75a1ce21356a6db623e8f77b2f85b`.
- **C133 / B132:** `ACCEPT_WITH_LIMITATIONS`. Package integrity and bounded Production-image findings are accepted; native USB remains not ready.
  - Review report SHA-256: `bb6838607fc265f824e278d2b54df609c98cd439b53bcb34b19bb2d5d464848e`.

A review verdict applies only to its stated scope. It is not a general project approval.

## Historical source ZIP audit

The audited archive was `AutoKitMIPS_LINK4_ABI_V2_SOURCE_ONLY_EXPERIMENTAL_20261105.zip`, SHA-256:

`24604573170c0f52cccdc45189ea5c424e0126a5dcd52df3ca4cfc1ad25c2eb5`.

It is a historical 2026-11-05 experimental source-only snapshot, not a current release baseline. Its VCP contains 185 source records and all referenced source files were present. It does not contain the later AKB native-owner provider source modules discussed in project work. Its UWD1 transport is logical Phaethon/iPod message plumbing, not proof of the required raw EP0 C0/53 transaction through the live JIPOD owner.

A host-only audit ran 38 test scripts: 25 passed and 13 returned nonzero. The nonzero results included two assertion aborts, one fixture/API mismatch, two source-pattern expectation failures, and eight version-lineage checks expecting an older version string. These results must be reconciled individually; they are not evidence of a CE5/MIPSII build failure or success.

## Immediate working rules

1. Keep `main` stable. Work on a named branch and use a pull request.
2. Never infer a callable ABI from a ROM address, symbol name, generic import, or a logical UWD1 command.
3. Do not invent IOCTLs, status constants, structure layouts, callbacks, or ownership tokens.
4. Do not synthesize a positive QUERY result. ACK only after genuine terminal native success, exact length/data, safe drain, and valid release are proven.
5. Do not bundle OEM SDKs, firmware images, private dumps, credentials, certificates, or confidential project handoffs.
6. Keep source acceptance, integration acceptance, target build, and hardware validation as separate gates.
7. Do not use the historical NK image as a substitute for the designated Production target. Any private target-image work must remain outside this public repository unless redistribution rights are explicit.

## Next milestone

Close the Production owner-entry gap with a documented, supported, version-matched mechanism that reaches the same live JIPOD owner and provides a valid device-generation/lifetime lease. Only then define and implement the native EP0 request contract.
