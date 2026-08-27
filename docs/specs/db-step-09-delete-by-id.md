# Delete record by ID

- Status: Proposed
- Owner: db_concepts maintainers
- Related ADR: None

## Goal

Allow callers to remove one in-memory table record by its ID.

## Context

The table supports bounded insertion and linear lookup, but records cannot yet
be removed. Step 9 adds deletion so the fixed-size table can reclaim a slot
without preserving record order.

## Scope

- Expose a table deletion function that searches for an ID.
- Remove a matching record using swap-with-last compaction.
- Report whether a record was removed.

## Non-goals

- Preserving insertion order after deletion.
- CLI delete commands or CLI parsing.
- Duplicate-ID policy changes.
- Dynamic table allocation or persistence.

## Requirements and expected behavior

- `table_delete_by_id` accepts a table pointer and record ID and returns nonzero
  when it removes a matching record.
- On success, the table count decreases by one and the final record fills the
  removed record's slot.
- When no matching ID exists, it returns zero and leaves the table unchanged.

## Acceptance criteria

- [ ] Deleting an existing non-final ID returns success.
- [ ] A successful deletion decreases `table.count` by one.
- [ ] The record previously at the final occupied index replaces the deleted
  record's index.
- [ ] Deleting a missing ID returns zero without changing table contents or
  count.

## Test plan

1. Insert three records, delete the middle ID, and verify count and
   swap-with-last compaction.
2. Verify a missing ID leaves a populated table unchanged.
3. Run the project validation command, `make test`, after implementation.

## Implementation plan

1. Declare `table_delete_by_id` in `include/table.h`.
2. Implement a linear search and swap-with-last deletion in `src/table.c`.
3. Add focused deletion tests in `tests/test_table.c`.
4. No documentation or ADR update is needed beyond this step specification.

## Risks and dependencies

- Swap-with-last deletion intentionally changes record order; callers must use
  ID lookup rather than index-based ordering.
- This depends on the existing fixed-size `table` representation and
  `table.count` invariant.

## Open questions

- None.

## References

- `docs/specs/db-learning-roadmap.md` step 9
- `include/table.h`
- `src/table.c`
