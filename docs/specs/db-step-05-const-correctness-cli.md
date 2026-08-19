# Const-correctness pass across the CLI

- Status: Proposed
- Owner: Repository maintainer
- Related ADR: None

## Goal

Make the CLI parser interface and implementation accurately express which input data is read-only while preserving the existing command parsing behavior.

## Context

Roadmap step 5 follows the initial CLI harness and the record-copy exercises. The current CLI already uses `const` for several string parameters, but the declarations and local pointer usage should be reviewed as one coherent learning exercise. The intended users are learners reading `cli.h`, `cli.c`, and the CLI tests. The pass must clarify ownership and mutation without introducing new commands, state, or abstractions.

## Scope

- Audit the public declarations in `include/cli.h` and the implementation in `src/cli.c` for const-correctness.
- Mark input strings and other data that the parser does not modify as read-only.
- Keep result objects and caller-owned output storage mutable wherever the implementation writes to them.
- Preserve the existing parser behavior, command enum values, result layout, and fixed argument capacity.
- Add focused tests or compile coverage that exercises the const-correct interface through the existing CLI test target.

## Non-goals

- No new CLI commands or changes to command syntax.
- No changes to `cli_command_result`, `cli_command_type`, or `CLI_ARGUMENT_CAPACITY`.
- No dynamic allocation, hidden global state, or parser refactor unrelated to const qualification.
- No changes to the interactive loop in `src/main.c` unless required to compile against the corrected public interface.
- No broad const-correctness pass across unrelated modules.

## Requirements and expected behavior

- `cli_parse_line` must accept input through a pointer-to-const character sequence because parsing does not modify the caller's line.
- Helper functions that only inspect command text or input buffers must use pointer-to-const parameters.
- Helper functions that write a parsed result must retain a mutable result parameter.
- Local pointer variables must be declared const-qualified when they are used only to inspect immutable input; pointers that advance through input must still prevent writes through the pointed-to data.
- The parser must continue to accept and reject exactly the same inputs as before, including null input, line endings, whitespace-separated commands, and over-capacity arguments.
- The public header and implementation must compile as C99 with the repository's warning flags without casts that hide an incorrect mutation contract.

## Acceptance criteria

- [ ] The public CLI parser declaration accurately marks its non-mutated input as read-only.
- [ ] All relevant helper parameters and local input pointers in `src/cli.c` prevent mutation of data that the parser only reads.
- [ ] Mutable result storage remains writable where parsing constructs the returned argument.
- [ ] Existing command results and `cli_command_result` layout remain unchanged.
- [ ] Tests continue to cover valid commands, invalid commands, null input, line-ending handling, and the argument-capacity boundary.
- [ ] A const-qualified input buffer can be passed to the public parser without a cast or diagnostic.
- [ ] `make test` builds and passes all existing test executables.

## Test plan

1. Pass a const-qualified character buffer containing a valid `read` or `write` command to `cli_parse_line` and assert the expected command and argument.
2. Confirm representative valid commands, including arguments with spaces and both supported line endings, retain their existing results.
3. Confirm empty, unknown, argument-less, and null inputs remain invalid.
4. Confirm an argument at the capacity boundary is still rejected rather than silently truncated.
5. Build the CLI test target with the repository's C99 warning flags and verify no const-related warnings or casts are needed.
6. Run `make test` to validate the CLI, concepts, and hello test executables.

## Implementation plan

1. Review the declarations in `include/cli.h` and identify each input, output, and mutable local data contract.
2. Update const qualifications in `include/cli.h` and `src/cli.c`, keeping function names, return types, enum values, result layout, and parser control flow stable.
3. Extend `tests/test_cli.c` with a const-qualified input-buffer case while retaining the existing behavior and boundary assertions.
4. Compile and run the focused CLI test target, then run `make test` for the complete project validation.

## Risks and dependencies

- Adding or removing a qualifier can expose callers that incorrectly rely on mutation, so the public header and all in-repository call sites must be compiled together.
- C permits a pointer variable to advance while still preventing writes through the pointed-to characters; the implementation should preserve that distinction.
- The repository's current warning flags may not report every const issue, so the tests and manual signature audit are both part of the learning objective.

## Open questions

- None.

## References

- [db-learning-roadmap.md](db-learning-roadmap.md), Phase 1 step 5
- [cli.h](../../include/cli.h)
- [cli.c](../../src/cli.c)
- [test_cli.c](../../tests/test_cli.c)
- [Makefile](../../Makefile)
