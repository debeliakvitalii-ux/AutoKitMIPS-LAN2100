# Contributor evidence template

Copy this template into a focused issue or pull request. Do not attach confidential or non-redistributable material.

## Summary

- Claim / proposed fix:
- Exact blocker addressed:
- Expected acceptance criterion:

## Platform and provenance

- USB stack and exact version:
- Windows CE version:
- BSP / board:
- CPU / ABI:
- Execution context (application, service, kernel, owner callback):
- Source/document origin:
- License / redistribution permission:

## ABI or call contract

- Entry point / IPC name:
- Exact declaration and calling convention:
- Structure sizes, alignment and offsets:
- Handle/object origin:
- Is this the same live stock JIPOD owner? Evidence:
- Attach-generation mechanism:
- Lifetime guarantee before first pointer access:

## Completion and safety

- Submit semantics:
- Terminal status and documented success value:
- Actual-length semantics:
- RX buffer ownership and lifetime:
- Timeout semantics:
- Cancellation semantics:
- DMA/controller drain guarantee:
- Callback retirement guarantee:
- Detach/re-attach behavior:
- Final release serialization:

## Reproduction

- Source revision / commit:
- Build command and toolchain:
- Test command:
- Hardware used:
- Raw logs (sanitized):
- SHA-256 of relevant artifacts:
- Expected result:
- Actual result:
- Known limitations:

## Scope statement

Explicitly state what this evidence does **not** prove. A generic API name, internal address, submit return, host fake, or PC-only observation is not sufficient evidence of a Production LAN2100 native USB path.
