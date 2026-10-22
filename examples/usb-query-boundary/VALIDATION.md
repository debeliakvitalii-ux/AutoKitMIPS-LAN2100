# Host validation

Validation date: 2026-10-22. Scope: this standalone public extraction only.

| Check | Observed result |
| --- | --- |
| GCC 13.3.0, C++98, `-Wall -Wextra -Werror -pthread` | Build passed |
| AddressSanitizer + UndefinedBehaviorSanitizer | 46/46 assertions passed; no diagnostics in this run |
| Clang rerun | Not run: `clang++` unavailable in this environment |
| Windows CE / MIPS build | Not performed |
| Real USBware provider / head-unit USB transfer | Not performed; provider absent |

The harness covers missing callbacks, withdrawal during drain, withdrawal at dispatcher entry, withdrawal inside release, serialized parallel withdrawal, generation isolation and one-shot admission. Fake handles and synthetic replies are used throughout. Passing assertions do not establish target ABI compatibility or physical device lifetime safety.

The runner defaults to GCC. Pass compiler names explicitly to reproduce additional builds. Leak detection is disabled (`ASAN_OPTIONS=detect_leaks=0`); the result makes no leak-validation claim.
