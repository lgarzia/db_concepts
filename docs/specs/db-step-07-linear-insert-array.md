# Linear insert into the in-memory table

- Status: Proposed
- Owner: TBD
- Related ADR: None

## Goal

Add the first table mutation: append one record to the next available slot in the fixed-size in-memory table. The operation should make the table's linear storage model observable while preserving the simple C99 design used by the preceding step.

## Context

The table module currently owns a fixed-size inline array of records and tracks occupied entries with `count`, but callers can only initialize the table. Roadmap step 7 needs a focused insertion operation that stores records in array order and updates table state without dynamic allocation, persistence, CLI behavior, or indexing.

## Scope

- Add a public table insertion interface in `include/table.h`.
- Implement insertion in `src/table.c` by writing the record at the first unoccupied array position.
- Increment the table's occupied-record count after a successful insertion.
- Preserve the order in which records are inserted.
- Add focused unit tests for one insertion, multiple insertions, and the table count.

## Non-goals

- Duplicate-id rejection or update behavior; roadmap step 15 defines that policy.
- A user-facing or CLI `insert` command; roadmap step 11 covers CLI integration.
- Explicit full-table error reporting; roadmap step 17 covers capacity handling.
- Lookup, deletion, sorting, indexing, persistence, dynamic allocation, or transactions.

## Requirements and expected behavior

- Insertion accepts a table and a record and stores the record in the next available slot, beginning at index zero for an empty table.
- A successful insertion increases `count` by exactly one.
- Multiple successful insertions store records in insertion order at contiguous positions from index zero through `count - 1`.
- Insertion copies the record value into table-owned inline storage; later changes to the caller's record do not alter the stored record.
- The operation does not allocate memory, depend on CLI state, perform file I/O, or use process-global table state.
- The table invariant remains `count <= TABLE_CAPACITY` for calls within the supported insertion capacity; behavior when no slot is available remains outside this step's contract.
- Duplicate ids are accepted at this stage because duplicate handling is specified separately in roadmap step 15.

## Acceptance criteria

- [ ] The public table header declares a focused insertion interface using the existing `table` and `table_record` types.
- [ ] Inserting into an empty table stores the record at index zero and changes `count` from zero to one.
- [ ] Repeated successful insertions occupy contiguous slots in insertion order and increase `count` once per record.
- [ ] Stored records remain independent of subsequent changes to the source record.
- [ ] A focused table test covers normal insertion and verifies ids, values, slot positions, and count.
- [ ] The table test continues to build and run through `make test`.
- [ ] The implementation compiles with the project's C99 warning flags without new warnings.

## Test plan

1. Initialize an empty table, insert one record, and assert its count, slot index, id, and value.
2. Insert several distinct records and assert that they occupy contiguous slots in the same order as insertion.
3. Modify a source record after insertion and assert that the stored record is unchanged, confirming a struct-by-value copy into the array.
4. Insert records with duplicate ids and verify they are retained independently, leaving duplicate policy for roadmap step 15.
5. Run `make test` to build and execute the table test with the existing test programs.

## Implementation plan

1. Add the insertion declaration to `include/table.h`, choosing a return contract that can be extended for full-table handling in roadmap step 17 without requiring callers to access storage directly.
2. Implement the operation in `src/table.c` using the current `count` as the destination index, copying the record into the inline array, then updating `count`.
3. Add or extend `tests/test_table.c` with focused assertions for insertion order, count changes, and value-copy behavior.
4. Keep the Makefile and CLI unchanged unless the existing table test target needs a minimal adjustment to compile the updated module.

## Risks and dependencies

- This step depends on the record and table types established in roadmap step 2 and step 6.
- The current fixed capacity means the insertion interface will need a documented failure result or equivalent policy when full-table handling is added in step 17.
- Accepting duplicate ids temporarily is intentional, but later duplicate handling must not silently invalidate tests or callers written for this step.

## Open questions

- Should the insertion function return a boolean/status value now, even though explicit full-table behavior is deferred to step 17?
- Should full-table insertion leave the table unchanged once step 17 defines its error contract?

## References

- [db-learning-roadmap.md](db-learning-roadmap.md), roadmap step 7 and follow-on steps 11, 15, and 17.
- [db-step-06-fixed-size-array-table.md](db-step-06-fixed-size-array-table.md), the preceding table-storage specification.
- `include/table.h`, `src/table.c`, and `tests/test_table.c`.
