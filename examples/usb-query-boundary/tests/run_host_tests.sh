#!/bin/sh
set -eu
root=$(CDPATH='' cd -- "$(dirname "$0")/.." && pwd)
if [ "$#" -eq 0 ]; then set -- g++; fi
for compiler in "$@"; do
  command -v "$compiler" >/dev/null 2>&1 || { echo "Compiler unavailable: $compiler" >&2; exit 127; }
  outfile=$(mktemp "${TMPDIR:-/tmp}/akusb-release-XXXXXX")
  trap 'rm -f "$outfile"' EXIT HUP INT TERM
  "$compiler" -std=c++98 -Wall -Wextra -Werror -pthread \
     -fsanitize=address,undefined -fno-omit-frame-pointer -I"$root/include" \
     "$root/src/UsbwareSyncEp0QueryBackend.cpp" \
     "$root/src/akusb01_query_terminal_gate.cpp" \
     "$root/tests/test_drain_withdraw_release.cpp" -o "$outfile"
  ASAN_OPTIONS=detect_leaks=0 "$outfile"
  rm -f "$outfile"
done
