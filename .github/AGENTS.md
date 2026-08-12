# Agent definitions

This repository is designed for GitHub Copilot and VS Code workflows. The goal is to keep code generation consistent, predictable, and easy to validate.

## Core principles

- Prefer small, explicit changes.
- Let the code and tests define the behavior.
- Keep the build reproducible and terminal-friendly.
- Treat the Makefile as the canonical build contract.

## Default workflow

1. Understand the current task and its scope.
2. Read the relevant source and header files.
3. Keep the change minimal and targeted.
4. Update or add tests when behavior changes.
5. Run `make test` and confirm the output.
6. Document any design decision that affects architecture or behavior.

## Agent roles

### Code agent

- Writes or updates code in source or headers.
- Keeps conventions consistent with the C starter project.

### Test agent

- Adds or updates assertions in `tests/`.
- Verifies behavior through the project test target.

### Build agent

- Fixes compile issues and keeps the project building.
- Respects the Makefile and compiler settings.

### Documentation agent

- Updates docs when architecture, interfaces, or usage change.
- Keeps ADRs and specs brief and practical.

## Good prompts

- "Explain the current C project structure and build workflow."
- "Add a unit test for this public function."
- "Fix the build issue without changing the public API."
- "Document the design decision and update the relevant spec."

## Bad prompts

- "Rewrite everything in a better way."
- "Change the architecture without context."
- "Add broad refactors with no tests."

## Project conventions

- One responsibility per source file.
- Public API in headers.
- Keep generated outputs under `bin/`.
- Keep runtime or scratch state in `.agent/` and ignore it in Git.
