# List all records command

- Status: Proposed
- Owner: Database learning project
- Roadmap step: 14 - Add `list` command to dump all records.
- Related ADR: None

## Goal

Allow the CLI to dump every record currently stored in the fixed-size table in a deterministic, readable format.

## Context

The project maintains a fixed-size in-memory table of records, and earlier steps add insert, get, and delete commands. There is currently no way to inspect all stored data from the CLI, which makes debugging, validation, and user workflows difficult.

## Scope

- Add a new `list` command recognized by the CLI parser.
- Display each stored record in a stable, readable format.
- Provide an explicit empty-table response when no records exist.

## Non-goals

- Persisting records to disk.
- Sorting records by value instead of table order.
- Supporting filters or partial record selection.
- Reworking the record schema or table storage model.

## Requirements and expected behavior

- The CLI accepts `list` as a command keyword without additional arguments.
- When the table is empty, the command prints a clear empty-state message.
- When the table contains records, the command prints each record in a deterministic order and includes both the record id and value.
- Invalid forms such as `list extra` are rejected instead of printing a partial or misleading result.
- The command does not modify stored records or their count.

## Acceptance criteria

- [ ] CLI accepts `list` and produces a valid record dump when the table contains records.
- [ ] Empty-table behavior prints a clear message instead of producing blank output.
- [ ] Extra arguments are rejected and do not produce partial output.
- [ ] The feature remains compatible with the existing insert/get/delete flow and the fixed-size table API.

## Test plan

1. Add parser tests to confirm that `cli_parse_line("list")` is recognized and that `cli_parse_line("list extra")` is rejected.
2. Add CLI or integration tests that initialize the table with multiple records and verify the output includes all records in a stable order.
3. Add an empty-table test that verifies the command prints the expected empty-state response.
4. Run the project validation command, `make test`, to confirm the feature integrates with the existing C build and test workflow. (This spec does not execute the command.)

## Implementation plan

1. Extend the public CLI parsing contract in the command enum and parser logic to include the `list` command.
2. Add the CLI handler or output routine that iterates the fixed-size table and prints each record using the existing table API.
3. Add focused tests covering empty and populated table states, plus malformed argument handling.
4. Update command documentation or README material if the project’s user-facing CLI documentation lists the supported commands.

## Risks and dependencies

- The output format must remain readable and safe for the fixed-size string buffers already used in the table.
- The command must align with the project’s simple C99 CLI design without changing the underlying table model.
- Existing insert/get/delete tests and parsing behavior must not regress.

## Open questions

- Should the dump use insertion order or current table order after deletes?
- Should the empty-state message be `No records.` or `Table is empty.`?

## References

- include/cli.h
- include/table.h
- src/cli.c
- src/main.c
- tests/test_cli.c
