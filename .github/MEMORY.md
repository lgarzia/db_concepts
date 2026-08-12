# Repository memory

This file captures long-lived knowledge about the repository and the way the project is meant to be operated.

## Purpose

- Record project conventions and known pitfalls.
- Preserve architectural decisions and build lessons.
- Keep Copilot context aligned with the current repository state.

## Important facts

- This project is intentionally small and explanatory.
- The public API is defined in `include/hello.h`.
- The implementation is in `src/hello.c`.
- The executable entry point is `src/main.c`.
- Build/test targets are defined in the Makefile.
- The binary output lives in `bin/` and should not be committed.

## Current conventions

- Prefer C99-compatible code.
- Use small modules and simple interfaces.
- Keep generated artifacts out of version control.
- Use `make test` as the acceptance check for changes.

## Common pitfalls

- Changing a public header without updating usage sites.
- Forgetting to add or update tests for behavior changes.
- Leaving build artifacts in the repo root.
- Mixing implementation with interface declarations.
- Making broad refactors without a targeted validation step.
