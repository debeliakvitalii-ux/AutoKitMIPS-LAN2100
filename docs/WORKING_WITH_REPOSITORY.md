# Working with this repository

## Repository purpose

This is the public collaboration surface for AutoKitMIPS LAN2100. It is not the private OEM evidence store and is not a release repository for a working head-unit binary yet.

## Start here

1. Read [development status](DEVELOPMENT_STATUS_2026-11-23.md).
2. Read [USBware owner findings](USBWARE_OWNER_FINDINGS_2026-11-23.md).
3. Read [implementation roadmap](IMPLEMENTATION_ROADMAP.md).
4. Review the [architecture/lifetime contract](ARCHITECTURE_AND_LIFETIME_CONTRACT.md).
5. Use the [contributor evidence template](CONTRIBUTOR_EVIDENCE_TEMPLATE.md) for new technical evidence.

## Branch and PR workflow

- Use a focused branch name such as `fix/rx-callback-retirement` or `docs/owner-entry-evidence`.
- Keep a PR limited to one reviewable objective.
- Include exact parent commit and file hashes for source patches.
- Include test commands, results, and limitations.
- Do not merge a patch that only applies with offsets unless exact postimage hashes and intended semantics are verified.
- Do not modify `main` directly for experimental work.

## What belongs here

Good public contributions:
- authored technical explanations;
- reproducible host tests and source code with clear license/provenance;
- platform-matched, redistributable ABI documentation;
- sanitized test logs and hash manifests;
- focused bug reports and reviewed fixes.

Do not publish:
- OEM firmware/NK images or raw dumps;
- proprietary SDKs, headers or libraries without redistribution rights;
- credentials, certificates, keys or private device identifiers;
- private project handoffs or internal review packages;
- guessed IOCTLs, internal ROM addresses presented as callable APIs, or synthetic success traces.

## Test the existing public sample

From the repository root:

```sh
sh examples/usb-query-boundary/tests/run_host_tests.sh
```

These tests exercise a host-side boundary with a fake provider. They do not access a USB device and do not validate Windows CE/MIPSII.
