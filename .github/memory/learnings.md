# Project learnings

## Build guidance

- The Makefile is the primary source of truth for build behavior.
- Cross-platform `.exe` handling is important on Windows.
- Keep build outputs isolated under `bin/`.

## Code guidance

- Small API boundaries are easier for Copilot to understand and modify correctly.
- Minimal public interfaces reduce bugs and make tests easier to write.

## Workflow guidance

- Keep Copilot instructions explicit.
- Prefer a small number of well-named modules over a monolithic design.
