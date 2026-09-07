# GitHub Copilot Instructions for db_concepts

This repository is a small but professional C project optimized for GitHub Copilot and VS Code workflows. Keep the code readable, testable, and easy to extend.

## Project context

- Language: C99
- Build tool: GNU Make
- Primary app: minimal hello-world program
- Goal: keep the project simple, portable, and agent-friendly

## Coding standards

- Use snake_case for function names and variables.
- Use UPPER_CASE for macros and constants.
- Keep public APIs in headers and implementation details in .c files.
- Use include guards in all headers.
- Prefer small, single-purpose functions.
- Avoid unnecessary abstractions or hidden global state.
- Treat compiler warnings as actionable issues.
- Prefer explicit, readable code over clever or terse constructs.
- Keep the code portable across Windows, Linux, and macOS when reasonable.

## File conventions

- `include/`: public headers
- `src/`: source files and the main program entry point
- `tests/`: automated tests
- `docs/`: architecture, design decisions, and specs
- `.github/`: Copilot and agent guidance
- `.vscode/`: editor configuration
- `.agent/`: ephemeral runtime and scratch state

## Build and test workflow

- Build: `make`
- Run app: `make run`
- Run tests: `make test`
- Clean: `make clean`

## Copilot usage expectations

- Prefer small, correct edits over broad refactors.
- Keep patch scope limited to the task.
- When adding or changing public APIs, update the matching header and tests.
- Add tests for public behavior when relevant.
- If a change affects the build, update the Makefile or documentation as needed.
- Keep comments focused on intent and non-obvious behavior.

## Review rules

- Prefer clarity and maintainability over micro-optimizations.
- Avoid introducing new dependencies unless they are essential.
- Ensure the project remains easy to build from a clean checkout.

## Runtime and memory guidance

- Keep memory management simple and explicit.
- Avoid unnecessary dynamic allocation in this starter project.
- Validate function contracts and avoid unsafe assumptions.

## Agent memory

This project uses memory files to preserve knowledge. Keep repo-level notes under `/memories/repo/` and session notes under `/memories/session/` when used by Copilot.

### Memory policy

- The canonical persistent memory for Copilot is the `/memories/` hierarchy, not a repo markdown file such as `MEMORY.md`.
- A file like `MEMORY.md` in the repo root or under `.github/` may be discovered by generic repo-scanning heuristics, but it is only an optional human-readable note unless explicitly referenced by a task.

## Subagent Delegation Rules

- When asked to analyze repository structure, `.vscode`, `.github`, or configuration setups, DO NOT run multiple directory listing or file reading tools in the main thread.
- Always delegate mapping tasks by invoking the `@Explore` subagent or running an isolated subagent task.
- Accept only the summarized response back into the active context window.

## Context Management Rules

- Do NOT request the user to paste full file contents into the chat window.
- Always ask the user to reference files using `#file:` or attached context.
- When inspecting code, request specific line ranges, methods, or functions rather than full-file reads.
