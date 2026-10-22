# Contributing to AutoKitMIPS

Please begin with [the technical USB request](docs/USB_HELP.md) and [issue #1](https://github.com/debeliakvitalii-ux/AutoKitMIPS-LAN2100/issues/1).

## Useful technical replies

Include:

1. Exact platform, stack version and application/kernel execution context.
2. The entry point or documented IPC, its declaration and a shareable source reference.
3. How the live device is acquired and kept valid across detach.
4. How terminal status, actual length, timeout, cancellation and drain are reported.
5. What was actually tested: source analysis, host fixture, target compilation or hardware.

A small reproducible correction is welcome. A generic USB API name without its target binding does not close the integration gap. Do not submit guessed kernel calls or guessed IOCTLs as working implementations.

## Published sample

Run the existing host tests from the repository root:

```sh
sh examples/usb-query-boundary/tests/run_host_tests.sh
```

For Clang as well, if installed:

```sh
sh examples/usb-query-boundary/tests/run_host_tests.sh g++ clang++
```

See [validation scope](examples/usb-query-boundary/VALIDATION.md). These commands use a synthetic provider and do not access USB hardware.

## Sharing materials

Share only material you may redistribute and retain existing notices. State the source and applicable license for contributed code. Do not upload firmware dumps, confidential SDKs, credentials, certificates or private project conversations. This repository has no project-wide open-source license declaration; public visibility alone does not grant a license to third-party material.

Development and review precede publication of additional project files. A public sample or source review is not a release for installation on the head unit.
