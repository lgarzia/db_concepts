# Insert ID/Value Command

**Status:** Proposed  
**Owner:** [Unassigned]  
**Related ADR:** N/A

## Goal

Add an `insert <id> <value>` command that lets a CLI user append an ID/value record to the process-lifetime in-memory table.

## Context

The CLI already parses single-argument commands, and the table module already stores fixed-capacity `table_record` values. This step connects valid insert input to that existing storage API without changing table semantics.

## Scope

- Parse `insert <id> <value>` input.
- Store a successfully parsed record through `table_insert`.
- Report successful, malformed, and full-table insert attempts.
- Add focused parser and command-dispatch test coverage.

## Non-goals

- Adding `get`, `delete`, persistence, transactions, or indexes.
- Enforcing unique IDs or changing `table_insert` behavior.
- Accepting values that exceed the existing fixed-capacity buffers.

## Requirements and expected behavior

- The command grammar is `insert <id> <value>`, with whitespace separating all fields.
- `<id>` must be a nonempty base-10 integer representable by `int`; signed values are allowed.
- `<value>` must be nonempty and fit in `TABLE_VALUE_CAPACITY`, including its terminating null character. Internal value whitespace is preserved.
- LF and CRLF input are handled equivalently.
- Malformed commands, including missing fields, nonnumeric or out-of-range IDs, and oversized values, are invalid and do not modify the table.
- `main` initializes one table before reading commands. Each valid insert appends one `table_record` by calling `table_insert`.
- A successful insertion reports success. If the table is full, the command reports failure and does not overwrite an existing record or change the count.
- Duplicate IDs remain valid because the current table contract permits them.
- Existing `read`, `write`, `quit`, and `concepts` command behavior remains unchanged.

## Acceptance criteria

- [ ] `insert 42 example` produces an insert command containing ID `42` and value `example`.
- [ ] `insert -1 value with spaces\r\n` preserves the ID and internal value spaces.
- [ ] Inputs with a missing ID, invalid or out-of-range ID, missing value, or oversized value are rejected.
- [ ] A valid insert appends the expected ID and value to the initialized table.
- [ ] A full table rejects a further insert without changing its count or stored records.
- [ ] Existing command parsing and dispatch behavior is preserved.

## Test plan

1. Extend CLI parser tests for valid inserts, signed IDs, internal value whitespace, LF/CRLF, malformed fields, `int` bounds, and value-capacity boundaries.
2. Add or extend command-dispatch coverage to verify successful insertion, duplicate-ID insertion, and full-table rejection without mutation.
3. Run `make test` after implementation.

## Implementation plan

1. Update `include/cli.h` so `cli_command_type` and `cli_command_result` represent an insert command with a parsed `int` ID and copied value.
2. Update `src/cli.c` to recognize and validate `insert <id> <value>` while preserving current parser ownership and size-limit behavior.
3. Update `src/main.c` to initialize a `table`, construct a `table_record` from the parsed command, call `table_insert`, and emit defined insertion outcomes.
4. Update `tests/test_cli.c` and any focused command-dispatch tests required by the chosen test seam.

## Risks and dependencies

- The parsed value must respect both `CLI_ARGUMENT_CAPACITY` and `TABLE_VALUE_CAPACITY`; it must be rejected rather than truncated.
- User-facing result text should be defined consistently before output assertions are added.
- This work depends on the existing `table_init` and `table_insert` contracts.

## Open questions

- What exact success and failure text should the interactive CLI print?
- Should a leading plus sign be accepted for numeric IDs?

## References

- `docs/specs/db-learning-roadmap.md` — Step 11: `Add insert <id> <value> command.`
- `include/cli.h`
- `src/cli.c`
- `include/table.h`
- `src/table.c`
- `src/main.c`
- `tests/test_cli.c`
- `tests/test_table.c`
