# Application-side USB QUERY boundary

C++98 source sample for evaluating the missing USBware HOST provider integration. Native USBware calls are not implemented. The sample is not an EXE, driver or automatic CarPlay startup implementation.

## Contents

- `include/` and `src/`: QUERY state machine and terminal/owner gate.
- `tests/test_drain_withdraw_release.cpp`: fake-provider concurrency harness.
- `tests/run_host_tests.sh`: Selectable compiler builds with AddressSanitizer and UndefinedBehaviorSanitizer.
- `VALIDATION.md`: scope and observed test result.
- `SHA256SUMS`: checksums of the published sample files.

## Reproduce on Linux

Install GCC and its sanitizer runtimes, then from the repository root run:

```sh
sh examples/usb-query-boundary/tests/run_host_tests.sh
```

The tests do not access USB hardware. Each build must print `HOST_QUERY_BOUNDARY_ASSERTIONS 46/46 PASS` and exit successfully.

To also run Clang when installed:

```sh
sh examples/usb-query-boundary/tests/run_host_tests.sh g++ clang++
```

## Integration contract

Read [USB_HELP.md](../../docs/USB_HELP.md) before connecting callbacks. No callback in this package is a vendor export. A real provider must protect device/context lifetime for the entire operation; the gate's final dispatcher lease only protects the release transaction. Stop requests latch state but do not cancel native I/O. Use fresh, zero-initialized storage for each single-use generation.

`akusb01_manual_query_once` is a retained compatibility helper name; this package adds no button, UI or startup action. Autonomous application integration remains outside this sample.

Host tests validate state transitions using fake pointers and synthetic completion values. They do not validate CE/MIPS ABI, hardware lifetime, native status mappings or USB success. On a 64-bit Linux host the fallback `DWORD`/`LONG` typedef widths differ from Windows CE; this is not a target ABI build.

This public extraction preserves implementation logic; comments, the header guard and the test output label were normalized for standalone use. No OEM implementation or third-party SDK is included.
