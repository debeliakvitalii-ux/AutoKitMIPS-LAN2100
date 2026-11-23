# Engineering decision log

This file records current public-facing decisions. It is not a replacement for the append-only internal project tree/plan or independent review reports.

## D-001 — Preserve the stock USB owner

**Decision:** The intended native path must use the existing live USBware/JIPOD owner.

**Reason:** A second HCD, synthetic owner, or fabricated device identity would not establish ownership of the actual attached device and could conflict with the stock stack.

## D-002 — Do not infer native EP0 from UWD1

**Decision:** The existence of the stock `UWD1:` stream front-end is not treated as proof of an arbitrary EP0 control-transfer API.

**Reason:** The Production application-to-owner callsite and transfer contract have not been established.

## D-003 — Fail closed

**Decision:** Positive QUERY success requires genuine terminal native success, exact actual length and RX bytes, valid generation/lease, and proven DMA/callback retirement.

**Reason:** Submit acceptance, callback return, logical command success, or a software flag cannot substitute for hardware completion and safe lifetime.

## D-004 — Keep review scopes separate

**Decision:** Source-only acceptance, exact-parent integration, target build, hardware runtime, and product release are distinct states.

**Reason:** A129 is accepted source-only but not integrated; C131/C133 accept bounded evidence only.

## D-005 — Do not publish proprietary artifacts

**Decision:** Public repository content is limited to authored summaries, shareable source, and artifacts with confirmed redistribution rights.

**Reason:** Private OEM images, SDKs, dumps, credentials and internal handoff bundles may be confidential or non-redistributable.

## D-006 — Preserve main through PR workflow

**Decision:** New project documentation and future source changes should be proposed on topic branches and reviewed before merge.

**Reason:** The repository is a public collaboration point; experimental or partially reviewed work must not silently become the canonical baseline.

## D-007 — A129 requires exact-parent reconciliation

**Decision:** Do not integrate the A129 patch until its exact declared preimage is matched to the intended canonical parent and the postimage hashes reproduce.

**Reason:** The historical LINK4 ZIP does not match A129's declared preimage; a contextual application to that archive did not reproduce the declared postimages.
