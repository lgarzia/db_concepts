# CLI command harness

- Status: Proposed
- Owner: [person or team]
- Related ADR: None currently needed

## Goal

Add a very small interactive command-line harness that accepts database-like commands without connecting to a database yet. The first version exists to provide a simple, testable place to learn C before adding real database behavior.

## Context

The repository currently contains a minimal C program with a public header, implementation file, executable entry point, and assertion-based test. The CLI should preserve that simple structure while introducing command parsing and a repeatable input/output loop.

The first commands intentionally do not perform database operations:

- `read <string>` echoes `<string>`.
- `write <string>` echoes `<string>`.
- `quit` ends the command session without echoing anything.

The command names establish a future vocabulary for database work while keeping the initial behavior easy to understand.

## Scope

- Read commands from standard input one line at a time.
- Recognize the commands `read`, `write`, and `quit`.
- Require a non-empty string argument after the command.
- Echo the argument exactly once followed by a newline.
- Continue accepting commands until the user enters `quit`.
- Treat end-of-file as a graceful fallback termination condition.
- Keep command parsing separate enough that it can be unit tested without depending entirely on interactive terminal input.
- Add extensive explanatory comments around input handling, parsing, control flow, and ownership of buffers.
- Add focused automated tests and preserve the existing `make test` workflow.

## Non-goals

- No database connection, storage, SQL, or persistence.
- No command history, scripting language, authentication, or configuration.
- No quoting or escaping rules in the first version.
- No aliases, abbreviated commands, or case-insensitive matching.
- No alternate exit commands, signals, or interactive command history.
- No broad refactor of the existing greeting API beyond what is needed to introduce the CLI cleanly.

## Requirements and expected behavior

### Valid commands

1. Input matching `read <string>` must write `<string>` to standard output followed by a newline.
2. Input matching `write <string>` must write `<string>` to standard output followed by a newline.
3. The command keyword must be lowercase and separated from the argument by whitespace.
4. The argument is the remainder of the line after the command separator, with the line ending removed.
5. A string may contain spaces after the command separator.
6. The CLI must process multiple valid commands in one session, one line at a time.
7. Input matching exactly `quit` must terminate the command loop successfully without echoing `quit`.
8. End-of-file must also terminate the loop successfully if it occurs before `quit`.

### Invalid commands

1. An empty line, unknown command, or command without a non-empty argument must not terminate the process unexpectedly.
2. Invalid input must produce a concise diagnostic on standard error and allow the CLI to process the next line.
3. The diagnostic must identify the accepted forms: `read <string>`, `write <string>`, and `quit`.
4. The first version does not need to preserve or echo invalid input.

### Learning-oriented implementation

1. The implementation must use C99-compatible constructs and the repository naming conventions.
2. Buffer size, line-reading behavior, and truncation behavior must be explicit in the code and documented with comments.
3. The code must avoid hidden global state and unnecessary dynamic allocation.
4. Public declarations must remain in headers and implementation details in source files.
5. Comments should explain intent and C concepts that are not obvious, especially buffer ownership, newline removal, command matching, and return values.

## Acceptance criteria

- [ ] The executable accepts `read hello` and prints `hello` followed by a newline.
- [ ] The executable accepts `write hello` and prints `hello` followed by a newline.
- [ ] Arguments containing spaces are echoed as one string.
- [ ] Multiple commands can be entered before end-of-file, and valid outputs appear in order.
- [ ] The `quit` command ends the session without producing echoed output.
- [ ] Commands entered after `quit` are not processed.
- [ ] Empty, unknown, and argument-less commands produce a diagnostic and the process continues.
- [ ] End-of-file exits the command loop successfully when no `quit` command is entered.
- [ ] Command parsing can be tested without requiring a human to type into an interactive terminal.
- [ ] Tests cover valid commands, invalid commands, and at least one input-boundary case.
- [ ] `make test` passes.
- [ ] The implementation includes substantial explanatory comments without changing the public behavior.
- [ ] Existing project build and test conventions remain intact.

## Test plan

1. Add unit tests for the command parser or command-dispatch function using representative input lines.
2. Verify `read hello` produces the expected command and argument.
3. Verify `write hello world` preserves the argument text after the command separator.
4. Verify `quit` returns the termination result and does not echo its command text.
5. Verify commands after `quit` are not processed.
6. Verify empty input, an unknown command, and a missing argument are rejected with the expected error result.
7. Verify a line ending is removed without removing meaningful argument characters.
8. Verify a line at or near the input buffer limit has an explicit, tested behavior rather than silent ambiguity.
9. Add a small integration-style test for processing multiple commands and stopping at `quit` if it can be done portably within the existing C test harness.
10. Run `make test` and confirm the test executable passes.
11. Manually run `make run` with multiple commands followed by `quit` to demonstrate the normal workflow.

## Implementation plan

1. Decide whether to retain the greeting API as a separate example or replace the executable entry point; avoid deleting the existing API without an explicit decision.
2. Add a small public command-processing interface, likely in `include/cli.h`, if separating parsing from the input loop improves testability.
3. Implement line parsing in a focused source file, likely `src/cli.c`.
4. Use a named constant for the input buffer size and document why the initial size is sufficient for a learning harness.
5. Read one line at a time in `src/main.c`, remove the line ending, and pass the line to the parser or dispatcher.
6. Match only the exact lowercase command keywords `read`, `write`, and `quit`.
7. Echo the parsed argument for `read` and `write`, return a termination result for `quit`, and return a clear error result for invalid input.
8. Stop dispatching input as soon as `quit` is recognized.
9. Keep standard output for valid echoed strings and standard error for invalid-command diagnostics.
10. Add or update `tests/test_cli.c` with assertions for parsing, dispatch, termination, and post-termination behavior.
11. Update the `Makefile` so the CLI and CLI tests are built through the existing `make`, `make run`, and `make test` targets.
12. Add comments while implementing each non-obvious block, then keep the comments accurate as the code evolves.
13. Update the README with all three commands and the `quit` termination workflow after implementation.

## Risks and dependencies

- Fixed-size input buffers introduce a maximum command length. The limit and behavior must be explicit and tested.
- Interactive terminal behavior can be difficult to automate portably on Windows, so parsing should be tested independently from terminal input.
- Changing `src/main.c` may alter the current hello-world demonstration; the implementation should preserve it or document the replacement decision.
- Shell-pipeline tests may behave differently across Windows and Unix-like environments; prefer C-level tests for core behavior.

## Open questions

- Should the existing hello-world executable remain available as a separate target while the CLI becomes the default executable?
- What initial input buffer size is appropriate for the learning goals?
- Should overlong input be rejected, truncated with an error, or processed across multiple reads?
- Should the CLI display a prompt such as `> `, or should it remain output-only for easier automated testing?

## References

- `docs/specs/README.md`
- `include/hello.h`
- `src/main.c`
- `tests/test_hello.c`
- `Makefile`
