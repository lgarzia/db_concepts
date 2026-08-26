# Linear scan / find by id in the in-memory table

- Status: Proposed
- Owner: TBD
- Related ADR: None

## Goal

Add a read-only lookup operation that finds a record by id using a linear scan over the table's occupied entries, making stored records retrievable while preserving the simple C99 design used by the preceding step.

## Context

The table module can initialize a table and insert records in order (roadmap step 7), but callers have no way to retrieve a previously inserted record without reaching into `records` directly. Roadmap step 8 needs a focused find-by-id operation that scans the occupied portion of the array (`records[0]` through `records[count - 1]`) and reports whether a matching record was found, without dynamic allocation, persistence, CLI behavior, or indexing.

## Scope

- Add a public table lookup interface in `include/table.h`.
- Implement the lookup in `src/table.c` by scanning occupied entries from index zero through `count - 1` and comparing ids.
- Report whether a record with the given id was found, and provide the matched record to the caller on success.
- Leave the table and all stored records unmodified by a lookup.
- Add focused unit tests for a found id, a not-found id, and lookup on an empty table.

## Non-goals

- Delete-by-id or any mutation of the table; roadmap step 9 covers deletion.
- A user-facing or CLI `get` command; roadmap step 12 covers CLI integration.
- Clear error reporting/messages for a missing id; roadmap step 16 covers that behavior.
- Duplicate-id policy; roadmap step 15 covers rejecting or updating duplicates. This step only defines which match is returned when duplicates exist.
- Sorting, indexing (binary search, B-tree), persistence, dynamic allocation, or transactions.

## Requirements and expected behavior

- The lookup accepts a table and an id and searches only the occupied entries, i.e. indices `0` through `count - 1`.
- If a record with the given id exists, the operation reports success and makes that record's value available to the caller.
- If no record with the given id exists, including when the table is empty, the operation reports that no match was found and does not expose an unspecified or partially-initialized record.
- When multiple stored records share the same id (permitted per roadmap step 7's non-goals), the operation returns the first match encountered when scanning from index zero.
- The operation does not allocate memory, depend on CLI state, perform file I/O, use process-global table state, or modify `table->count` or any stored record.

## Acceptance criteria

- [ ] The public table header declares a focused find-by-id interface using the existing `table` and `table_record` types.
- [ ] Looking up an id that exists reports success and yields a copy of the matching stored record with the correct id and value.
- [ ] Looking up an id that does not exist, on a non-empty table, reports that no match was found.
- [ ] Looking up any id on an empty table reports that no match was found.
- [ ] When duplicate ids are present, the lookup returns the record at the lowest matching index.
- [ ] A lookup call does not change `count` or the contents of `records`.
- [ ] A focused table test covers a found id, a not-found id, and lookup on an empty table.
- [ ] The table test continues to build and run through `make test`.
- [ ] The implementation compiles with the project's C99 warning flags without new warnings.

## Test plan

1. Insert one or more records, look up an id known to exist, and assert success plus the correct id and value on the returned record.
2. Look up an id that was never inserted into a non-empty table and assert the lookup reports not-found.
3. Initialize an empty table and look up any id, asserting the lookup reports not-found.
4. Insert two records sharing the same id and assert the lookup returns the first-inserted (lowest-index) match.
5. Assert `table->count` and the stored records are unchanged before and after a lookup call.
6. Run `make test` to build and execute the table test with the existing test programs.

## Implementation plan

1. Add the find-by-id declaration to `include/table.h`, choosing a return contract (e.g., a boolean success value with an out-parameter record, mirroring the extensible style used for `table_insert`) that later steps such as delete-by-id can reuse or parallel.
2. Implement the operation in `src/table.c` as a linear scan over `records[0]` through `records[count - 1]`, comparing each entry's id and copying the first match into the caller-provided output on success.
3. Add or extend `tests/test_table.c` with focused assertions for found, not-found, empty-table, and duplicate-id scenarios, following the existing `test_table_inserts_records_in_order` style (arrange/act/assert with brief `printf` trace lines).
4. Keep the Makefile and CLI unchanged unless the existing table test target needs a minimal adjustment to compile the updated module.

## Risks and dependencies

- This step depends on the table storage model from roadmap step 6 and the insertion behavior from roadmap step 7, including its acceptance of duplicate ids.
- The lookup's duplicate-id resolution (return the lowest-index match) must remain compatible with whatever policy roadmap step 15 later adopts for duplicate insertion.
- Choosing a return contract now (status plus out-parameter vs. pointer-into-storage) affects how naturally step 9 (delete-by-id) and step 16 (clear missing-id errors) can build on it.

## Open questions

- Should the lookup return a pointer to the record inside `table->records`, or copy the record out through an out-parameter, to keep the table's internal storage encapsulated?
- Should the "not found" case be signaled with a boolean return, a sentinel index, or another status convention, given that step 16 will later add explicit error messaging on top of this contract?

## References

- [db-learning-roadmap.md](db-learning-roadmap.md), roadmap step 8 and follow-on steps 9, 12, 15, and 16.
- [db-step-07-linear-insert-array.md](db-step-07-linear-insert-array.md), the preceding insertion specification.
- `include/table.h`, `src/table.c`, and `tests/test_table.c`.
