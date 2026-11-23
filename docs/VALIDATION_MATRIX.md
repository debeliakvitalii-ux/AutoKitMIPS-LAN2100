# Validation matrix and release gates

## Evidence levels

| Level | Meaning | Example |
|---|---|---|
| L0 — documentation | Contract or design written down | API proposal, state machine |
| L1 — static evidence | Exact source/binary evidence, bounded to identified inputs | Callsite, import, registration path |
| L2 — host test | Reproducible tests on development host | C++98 tests with fake provider |
| L3 — target build | Built with target CE5/MIPSII toolchain | eVC4 MIPSII compile/link |
| L4 — target runtime | Executed on LAN2100 with traceable logs | Attach, QUERY, detach |
| L5 — independent acceptance | Reviewer verifies evidence and scope | C review / integration review |

A higher level does not automatically validate unrelated subsystems.

## Current public evidence

| Component | Evidence | Status |
|---|---|---|
| QUERY boundary sample | Host GCC assertions documented as 46/46 | Host sample only |
| USBware/JIPOD owner entry | B131/C131 and B132/C133 bounded static investigations | Accepted with limitations; callable entry not established |
| A129 RX callback retirement fix | C129 source-only review | Accepted source-only; not integrated |
| USBware native EP0 provider | No proven version-matched callable ABI | Missing |
| CE5/MIPSII build | No build result in this repository | Not run / not evidenced |
| LAN2100 physical QUERY | No target trace | Not proven |
| CarPlay session | No end-to-end evidence | Not ready |

## Required test categories for a real provider

- Normal completion with exact four-byte response.
- Native error completion.
- Short transfer and zero-length transfer.
- Timeout before submit, during submit, and while awaiting completion.
- Cancellation and drain.
- Detach during submit.
- Detach during callback.
- Reattach while old generation is retiring.
- Repeated start/stop and shutdown reentrancy.
- Callback racing with slot retirement.
- Double release and stale-token rejection.
- DMA/buffer lifetime under every completion path.

## Evidence record for every test

Record:
- test ID and objective;
- exact source revision and file hashes;
- target image/build identity where relevant (do not publish proprietary images);
- toolchain and test environment;
- setup and exact steps;
- raw result/log hash;
- expected vs actual result;
- pass/fail and limitations;
- reviewer and review scope.

## Release gate checklist

- [ ] Supported same-owner application/service entry proven.
- [ ] Real physical-device lease and attach generation proven.
- [ ] Native EP0 ABI and status semantics documented.
- [ ] Terminal completion and actual length proven.
- [ ] RX data verified.
- [ ] Timeout/cancel/detach behavior proven.
- [ ] DMA and callback retirement proven.
- [ ] Exact-parent source integration reviewed.
- [ ] CE5/MIPSII target build completed.
- [ ] LAN2100 hardware tests completed.
- [ ] Public redistribution/license review completed.
- [ ] Release artifacts and hashes independently verified.

A release must not be marked ready while any required gate remains unchecked.
