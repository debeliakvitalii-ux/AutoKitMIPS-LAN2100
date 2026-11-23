# Implementation roadmap

This roadmap orders work by dependency. Do not skip a gate because a later component compiles on a host.

## P0 — Reach the real owner safely

**Goal:** an OEM-supported application/service path to the same live stock JIPOD owner.

Required deliverables:
- Version-matched ABI or broker/extension contract, with source provenance and redistribution permission.
- Exact call context and calling convention.
- Device acquisition and detach registration behavior.
- Owner token, device token, and attach-generation semantics.
- Lifetime guarantee that begins before the first pointer read.
- Independent review of the evidence.

Exit condition: reviewer can trace the call from a real application/service boundary to the currently attached stock owner and show how the object remains valid across detach.

## P0 — Native EP0 QUERY

Only after the owner-entry gate:

- Submit setup `C0 53 00 00 00 00 04 00`.
- Obtain genuine terminal native status.
- Require actual length exactly four.
- Require RX `01 00 00 00`.
- Define timeout and cancellation behavior.
- Prove DMA and callback retirement on success, failure, timeout, and detach.
- Release only resources owned by the same attach generation.
- Return ACK only when every success predicate is satisfied; otherwise fail closed.

Exit condition: independent review of source and target evidence, followed by a physical LAN2100 test with traceable logs.

## P1 — Canonical source integration

A source-only patch is not automatically a canonical integration.

For every patch:
1. Record exact parent file paths, sizes, and SHA-256.
2. Verify patch preimages against the intended parent.
3. Apply in a disposable worktree.
4. Verify exact postimage hashes.
5. Run focused host tests and relevant regression suites.
6. Obtain independent parent-integration review.
7. Only then merge through a pull request.

A129 is currently accepted source-only, not integrated. Its declared preimage does not match the historical LINK4 ZIP snapshot; a contextual patch application to that archive did not reproduce the declared postimage hashes. Reconcile against the actual current canonical parent before any integration claim.

## P1 — CE5/MIPSII build

- Use the actual eVC4/CE5 MIPSII toolchain and the designated target SDK.
- Record compiler/linker versions and complete build logs.
- Verify calling conventions, structure packing, pointer width, alignment, imports/exports, and linker dependencies.
- Produce reproducible artifact hashes.
- Do not call a host GCC/Clang build a target build.

Exit condition: successful target build with a manifest and independent review.

## P1 — Hardware validation

On the actual LAN2100 head unit:
- Confirm stock USB owner and physical device attach/detach.
- Capture generation and lease transitions.
- Execute the first QUERY only after the owner and EP0 gates pass.
- Capture native terminal status, actual length, RX bytes, timeout/cancel/drain evidence.
- Exercise unplug/detach races and repeated attach cycles.
- Preserve fail-closed behavior for every incomplete or ambiguous result.

No hardware-side operation should be performed without an explicit test plan and authorization.

## P2 — Higher-level CarPlay path

A successful first QUERY is only a control-transfer milestone. Separate subsequent work is required for:
- USB role-switch completion and role ownership.
- iAP2 session establishment.
- Identity/authentication and any required credentials/certificates.
- NCM or other network function enumeration and data path.
- IP configuration, DNS/service discovery as required.
- Media/control session and display/input integration.

Each layer needs its own observable acceptance tests. Do not label the whole product ready based on one USB control request.

## Suggested issue/PR discipline

- One issue per missing contract or reproducible defect.
- One PR per bounded source/documentation change.
- Keep hardware experiments out of documentation-only PRs.
- Include exact test commands and results.
- State what was not tested.
- Close an issue only when its explicit acceptance criteria are met.
