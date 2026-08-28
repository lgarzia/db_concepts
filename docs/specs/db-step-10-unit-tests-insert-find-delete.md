# Unit tests for insert, find, and delete

- Status: Implemented
- Owner: db_concepts maintainers
- Related ADR: None

## Goal

Establish focused automated coverage for the table insert, find, and delete APIs so their observable behavior remains stable as later database features build on them.

## Context

The in-memory fixed-size table is the foundation for subsequent CLI commands. Its public operations need tests for successful data handling, missing records, and invalid table pointers.

## Scope

- Add focused unit-test coverage for `table_insert`, `table_find_by_id`, and `table_delete_by_id`.
- Verify a null table passed to insertion is handled safely.

## Non-goals

- Changing record storage, capacity, or deletion strategy.
- Adding CLI commands or persistence.
- Defining duplicate-id validation.

## Requirements and expected behavior

- `table_insert` must return `1` after appending a record and `0` when passed a
  null table pointer or a full table.
- Existing insertion, lookup, and deletion behavior must remain unchanged.

## Acceptance criteria

- [x] Calling `table_insert(NULL, record)` returns `0` and does not crash.
- [x] Insertion into a table at capacity returns `0` without changing its count.
- [x] Existing table unit tests continue to cover insertion order, lookup success and failure, and deletion success and failure.
- [x] `make test` completes successfully after the required behavior is implemented.

## Test plan

1. Call `table_insert` with a null table and verify the test process continues.
2. Retain coverage for insert/find/delete normal paths and missing-id lookups and deletions.
3. Run the project validation command, `make test`, after implementation; this command is not run while creating the spec.

## Implementation plan

1. Add a focused null-table insertion test in `tests/test_table.c`.
2. Add a null-pointer guard to `table_insert` in `src/table.c`.
3. Run the table test target and then `make test`.
4. No public API, documentation, or ADR update is needed.

## Risks and dependencies

- The test depends on the existing public `table_insert` API and test target in the Makefile.
- Later requirements may choose a return status for insertion; this step preserves the current void API.

## Open questions

- None.

## References

- `docs/specs/db-learning-roadmap.md` step 10
- `include/table.h`
- `src/table.c`
- `tests/test_table.c`
