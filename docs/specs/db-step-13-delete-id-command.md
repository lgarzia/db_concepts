# Delete ID Command

- Status: Proposed
- Owner: [Unassigned]
- Roadmap step: 13 - Add `delete <id>` command.
- Related ADR: None

## Goal

Add a `delete <id>` command that lets a CLI user remove a matching record from the process-lifetime in-memory table.

## Context

The CLI already parses integer IDs for `insert` and `get`, and the table module already provides the deletion semantics needed to remove a matching record from its fixed-capacity storage. This step connects valid delete input to that existing table API without changing the underlying record model or table structure.

## Scope

- Parse `delete <id>` input into a CLI command result containing an integer ID.
- Delete the matching record from the initialized in-memory table through the existing table delete API.
- Report success or a clear not-found condition for a valid delete attempt.
- Preserve existing command behavior and add focused parser and dispatch coverage.

## Non-goals

- Changing the table storage model, fixed capacity, or record layout.
- Replacing the existing delete-by-id behavior with a new algorithm or persistence layer.
- Defining duplicate-ID or table-full semantics beyond the established table contract.
- Adding `list`, `save`, `load`, or persistence features.

## Requirements and expected behavior

- The command grammar is `delete <id>`, with whitespace separating the command and ID.
- `<id>` must be a nonempty base-10 integer representable by `int`; signed values are allowed consistently with the existing ID parsers.
- LF and CRLF input are handled equivalently.
- A missing ID, nonnumeric or out-of-range ID, trailing non-whitespace input, or otherwise malformed command is invalid and does not access or modify the table.
- `main` keeps one initialized table for the session and uses the existing delete-by-id operation for a valid delete command.
- When the ID exists, the command removes the matching record without changing unrelated table state.
- When the ID does not exist, the command does not fabricate a record and reports a clear not-found result as defined by the current contract and step 16 guidance.
- Existing `read`, `write`, `insert`, `get`, `quit`, and `concepts` command behavior remains unchanged.

## Acceptance criteria

- [ ] `delete 42` produces a delete command containing ID `42`.
- [ ] Signed IDs and CRLF input are accepted consistently with the existing ID command parsing.
- [ ] Inputs with a missing ID, invalid or out-of-range ID, or trailing arguments are rejected.
- [ ] A valid delete removes the matching record from the initialized table.
- [ ] A delete for a missing ID reports the expected clear failure state without mutating the table.
- [ ] Existing command parsing, insertion, retrieval, and session behavior is preserved.

## Test plan

1. Extend `tests/test_cli.c` for valid delete commands, signed IDs, LF/CRLF input, malformed arguments, trailing arguments, and `int` bounds.
2. Add focused command-dispatch or executable-level coverage for deleting an inserted record and leaving the table unchanged when the ID is missing.
3. Run the project validation command, such as `make test`, but those commands are not run while creating the spec.

## Implementation plan

1. Update `include/cli.h` to add a delete command type and expose the parsed integer ID through the existing `cli_command_result` structure.
2. Update `src/cli.c` with delete-specific integer parsing and command recognition, reusing the established validation and newline handling conventions without accepting trailing arguments.
3. Update `src/main.c` to call the table delete operation for a valid delete result, report an explicit success or not-found outcome, and preserve the existing session table.
4. Update `tests/test_cli.c` and the focused command-dispatch or executable-level test seam used for the application behavior; update the Makefile only if a new test target is needed.
5. Update user-facing command text or documentation where the existing CLI usage list is maintained.

## Risks and dependencies

- The parser must reject malformed or out-of-range IDs consistently with `insert` and `get` so delete never receives an unintended value.
- The table contract for delete-by-id semantics must remain stable; a change in removal order or identity handling would affect later commands.
- The exact success and missing-ID output text should be selected consistently with the established CLI messages before output assertions are finalized.
- This work depends on the existing `table_init` and delete-by-id contracts.

## Open questions

- What exact success text should the interactive CLI print after a successful delete?
- Should the missing-ID error wording be standardized now or left for the dedicated missing-ID behavior step?

## References

- `docs/specs/db-learning-roadmap.md` — Step 13: `Add delete <id> command.`
- `include/cli.h`
- `src/cli.c`
- `include/table.h`
- `src/table.c`
- `src/main.c`
- `tests/test_cli.c`
- `tests/test_table.c`
- `docs/specs/db-step-12-get-id-command.md`
