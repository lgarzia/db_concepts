# DB roadmap step 1 — review pointers vs. arrays vs. buffers

- Status: Proposed
- Owner: [person or team]
- Related ADR: None currently needed

## Goal

Build a working, first-principles understanding of pointers vs. arrays vs. buffers in C by adding a heavily commented demonstration that is callable from the CLI and verified by tests — not just read about, but exercised.

## Context

This is roadmap step 1 of [db-learning-roadmap.md](db-learning-roadmap.md) ("Review pointers vs. arrays vs. buffers in `cli.c`"), on branch `db/01-review-pointers-arrays-buffers` per [git-workflow.instructions.md](../../.github/instructions/git-workflow.instructions.md).

`cli.c` already exercises these concepts implicitly (`argument` is a pointer into the `line` buffer, `cli_command_result.argument` is a fixed-size array). This step makes the concepts explicit and teachable via a new `concepts` CLI command, rather than only reviewing existing code silently.

## Scope

- Add a new `concepts` command (no argument) to the CLI, alongside `read`, `write`, and `quit`.
- Add a demonstration function that prints an explanation of:
  - array-to-pointer decay,
  - pointer arithmetic vs. array indexing,
  - the difference between a buffer's capacity and its logical (string) length,
  - why a `char *` parameter cannot report the size of the array it decayed from.
- Extract at least one small, pure (non-printing) helper used by the demonstration so it can be unit tested deterministically.
- Add unit tests for that helper.
- Keep all new code heavily commented, explaining the concept at each step, per the roadmap's "comment heavily" learning goal.

## Non-goals

- No new persistent state, records, or storage (that starts at step 6+).
- No changes to `read`/`write`/`quit` parsing behavior.
- No interactive prompt or formatting system beyond plain `printf`/`puts` output.

## Requirements and expected behavior

1. Entering `concepts` at the CLI prints a short walkthrough covering the four concepts listed in Scope.
2. The walkthrough output is produced by calling a documented function, not inlined in `main.c`.
3. At least one helper function performs a real computation (e.g., manually computing a buffer's logical length via pointer walking) whose result can be asserted in a test, independent of any printed output.
4. `concepts` is parsed the same way `quit` is: an exact, whitespace-trimmed match with no argument.
5. Existing `read`, `write`, `quit`, and invalid-command behavior are unchanged.

## Acceptance criteria

- [x] Entering `concepts` prints the demonstration and the CLI continues accepting commands afterward.
- [x] The demonstration function has a one-line comment above each concept it demonstrates.
- [x] A pure helper function used by the demonstration is unit tested with at least two cases (typical input, empty input).
- [x] `read`, `write`, `quit`, and invalid-command tests still pass unchanged.
- [x] `make test` passes.

## Test plan

1. Unit test the pure helper (e.g., manual buffer-length-via-pointer function) against known strings, including an empty string.
2. Unit test that `cli_parse_line("concepts")` returns the new command type with no argument.
3. Re-run existing `test_cli` assertions to confirm no regression in `read`/`write`/`quit`/invalid parsing.
4. Run `make test`.
5. Manually run `make run`, enter `concepts`, confirm output appears, then enter `quit`.

## Implementation plan

1. Add `include/concepts.h` declaring a pure helper (`size_t concepts_pointer_strlen(const char *buffer)`) and a demonstration entry point (`void concepts_demonstrate(void)`).
2. Add `src/concepts.c` implementing both, with a one-line comment per concept inside `concepts_demonstrate()`.
3. Add `CLI_COMMAND_CONCEPTS` to `cli_command_type` in `include/cli.h`.
4. Parse `concepts` in `src/cli.c` the same way as `quit` (exact match, no argument).
5. Dispatch `CLI_COMMAND_CONCEPTS` in `src/main.c` to call `concepts_demonstrate()`, then continue the loop.
6. Add `tests/test_concepts.c` asserting the pure helper's behavior.
7. Add a `cli_parse_line("concepts")` assertion to `tests/test_cli.c`.
8. Update `Makefile` to compile `src/concepts.c` into the app and CLI test targets, and add a `test_concepts` target run by `make test`.
9. Run `make test`; fix any regressions before merging per the git workflow.

## Risks and dependencies

- Adding a new command type must not change parsing of existing commands; covered by re-running existing `test_cli` assertions.
- The demonstration must stay side-effect-only (printing) so the testable logic lives entirely in the pure helper.

## Open questions

- None currently; if a future step needs a different demo command name, this one can be renamed before roadmap step 6 introduces stored records.

## References

- `docs/specs/db-learning-roadmap.md`
- `docs/specs/cli-command-harness.md`
- `include/cli.h`
- `src/cli.c`
- `src/main.c`
- `tests/test_cli.c`
- `Makefile`
