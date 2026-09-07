# Get ID Command

- Status: Proposed
- Owner: [Unassigned]
- Roadmap step: 12 - Add `get <id>` command.
- Related ADR: None

## Goa l

Add a `get <id>` command that lets a CLI user retrieve the first matching ID/value record from the process-lifetime in-memory table.

## Context

The CLI already parses integer IDs for `insert` and `delete`, and the table module already provides `table_find_by_id` with first-match semantics. This step connects valid get input to that existing lookup API without changing table storage or lookup behavior.

## Scope

- Parse `get <id>` input into a CLI command result containing an `int` ID.
- Look up the ID in the initialized in-memory table through `table_find_by_id`.
- Report the retrieved record when a matching ID is found.
- Preserve existing command behavior and add focused parser and command-dispatch coverage.

## Non-goals

- Changing `table_find_by_id` or its linear-scan and first-match semantics.
- Defining the final missing-ID error behavior; that is covered by roadmap step 16.
- Adding `delete`, `list`, persistence, duplicate-ID enforcement, or indexes.
- Changing the table capacity or record representation.

## Requirements and expected behavior

- The command grammar is `get <id>`, with whitespace separating the command and ID.
- `<id>` must be a nonempty base-10 integer representable by `int`; signed values are allowed consistently with the existing ID parsers.
- LF and CRLF input are handled equivalently.
- A missing ID, nonnumeric or out-of-range ID, trailing non-whitespace input, or otherwise malformed command is invalid and does not access or modify the table.
- `main` keeps one initialized table for the session and uses `table_find_by_id` for a valid get command.
- When the ID exists, the command reports the first matching record's ID and value without changing the table count or stored records.
- Duplicate IDs retain the existing table behavior: the first matching record is returned.
- When the ID does not exist, the command does not fabricate a record; its clear error message is deferred to step 16.
- Existing `read`, `write`, `insert`, `delete`, `quit`, and `concepts` command behavior remains unchanged.

## Acceptance criteria

- [ ] `get 42` produces a get command containing ID `42`.
- [ ] Signed IDs and CRLF input are accepted consistently with the existing ID command parsing.
- [ ] Inputs with a missing ID, invalid or out-of-range ID, or trailing arguments are rejected.
- [ ] A valid get for an inserted ID reports the stored ID and value.
- [ ] A get for a duplicate ID reports the first matching record and leaves the table unchanged.
- [ ] A get for a missing ID does not report fabricated data and does not alter the table.
- [ ] Existing command parsing, insertion, deletion, and session behavior is preserved.

## Test plan

1. Extend `tests/test_cli.c` for valid get commands, signed IDs, LF/CRLF input, malformed arguments, trailing arguments, and `int` bounds.
2. Add focused command-dispatch or executable-level coverage for retrieving an inserted record, returning the first record for duplicate IDs, leaving the table unchanged, and not fabricating data for a missing ID.
3. Run `make test` after implementation.

## Implementation plan

1. Update `include/cli.h` to add a get command type and expose the parsed integer ID through the existing `cli_command_result` structure.
2. Update `src/cli.c` with get-specific integer parsing and command recognition, reusing the established validation and newline handling conventions without accepting trailing arguments.
3. Update `src/main.c` to call `table_find_by_id` for a get result, copy the found record into a local result, and print the defined successful retrieval format while preserving the existing session table.
4. Update `tests/test_cli.c` and the focused command-dispatch or executable-level test seam used for the application behavior; update the Makefile only if a new test target is needed.
5. Update command usage text or documentation only where the existing user-facing command list is maintained.

## Risks and dependencies

- The parser must reject malformed or out-of-range IDs consistently with `insert` and `delete` so lookup never receives an unintended value.
- Duplicate IDs require preserving `table_find_by_id` first-match behavior; changing lookup order would alter the existing table contract.
- The exact successful output format should be selected consistently with the existing `Inserted record` and `Deleted record` messages before output assertions are finalized.
- This work depends on the existing `table_init` and `table_find_by_id` contracts.

## Open questions

- What exact success text should the interactive CLI print for a retrieved record?
- Should the missing-ID error text be standardized now or left entirely to roadmap step 16?

## References

- `docs/specs/db-learning-roadmap.md` — Step 12: `Add get <id> command.`
- `include/cli.h`
- `src/cli.c`
- `include/table.h`
- `src/table.c`
- `src/main.c`
- `tests/test_cli.c`
- `tests/test_table.c`
- `docs/specs/db-step-11-insert-id-value-command.md`
