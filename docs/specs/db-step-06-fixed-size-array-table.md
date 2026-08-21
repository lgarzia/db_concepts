# In-memory fixed-size record table

- Status: Proposed
- Owner: TBD
- Related ADR: None

## Goal

Introduce a bounded in-memory table that owns a fixed-size array of records and tracks how many entries are currently present. This provides the storage foundation for the linear insert, lookup, delete, and unit-test steps that follow.

## Context

The project is a small C99 learning database with an existing CLI parser but no table storage abstraction. The table is intended for learners to practice arrays, structs, `sizeof`, and explicit state management without introducing dynamic allocation or persistence. It must remain simple, portable, and independently testable.

## Scope

- Define a record type compatible with the record model introduced in roadmap step 2.
- Define a fixed table capacity and a table type containing an inline array of records plus a count of occupied entries.
- Provide initialization for an empty table.
- Keep table storage in memory only and expose enough state for later table operations and focused tests.
- Add a dedicated table test target to the existing `make test` workflow.

## Non-goals

- Inserting, finding, or deleting records; those are roadmap steps 7–10.
- CLI commands or changes to CLI parsing; those begin in roadmap step 11.
- Dynamic allocation, file persistence, pages, indexes, transactions, or concurrency.
- Enforcing duplicate-id behavior or interpreting record values.

## Requirements and expected behavior

- The table stores records in a fixed-size inline array whose capacity is represented by a named UPPER_CASE constant.
- The table tracks the number of occupied records with a count initialized to zero.
- The table does not allocate memory dynamically and does not require cleanup beyond normal object lifetime.
- The table record and table declarations are available through a public header in `include/`.
- Initialization produces a valid empty table without requiring callers to zero-initialize the object first.
- The count cannot exceed the declared table capacity under the table API’s documented invariants.
- Record and table sizes are determined by normal C object layout and can be inspected with `sizeof`; the design does not depend on packed structs or binary serialization.
- The table module has no CLI, file, or process-global state dependencies.

## Acceptance criteria

- [ ] A public table header defines the record type, fixed capacity constant, table type, and empty-table initialization interface.
- [ ] An initialized table reports zero occupied records and has capacity for the declared maximum number of records.
- [ ] The table uses inline storage and introduces no dynamic-memory dependency.
- [ ] A focused table test verifies initialization, capacity, count state, and representative record storage access.
- [ ] The table test is built and executed by `make test` alongside the existing tests.
- [ ] The implementation compiles with the project’s C99 warning flags without new warnings.

## Test plan

1. Initialize a table and assert that its count is zero and its declared capacity is nonzero and fixed.
2. Construct a representative record, place it in an available table slot in the focused test, and verify its id and value remain intact.
3. Assert the table’s inline array has the expected relationship to its capacity using `sizeof` or equivalent compile-time-visible constants.
4. Run `make test` to build and execute the new table test together with the existing test programs.

## Implementation plan

1. Add `include/table.h` with the record type, capacity constant, table type, and initialization declaration, following the project’s include-guard and snake_case conventions.
2. Add `src/table.c` with the empty-table initialization implementation and no CRUD behavior.
3. Add `tests/test_table.c` for table state, inline storage, record layout, and initialization behavior.
4. Update `Makefile` with a table test target and include it in `test`; include the table module only where required by that target.
5. Keep CLI behavior unchanged and update this spec or a later roadmap spec if the record representation must change before step 7.

## Risks and dependencies

- This step depends conceptually on roadmap step 2’s record model; changing the record fields later could affect the public table header and tests.
- A fixed capacity limits the table by design and must not be mistaken for persistence or a production storage limit.
- Exposing an inline array makes the layout easy to learn and test but couples later modules to the table representation; later refactoring may be needed when pages are introduced.

## Open questions

- What exact record fields and string-value capacity were selected in roadmap step 2?
- Should empty-table initialization be a function, a macro initializer, or both once the record type is finalized?
- What fixed table capacity best supports the upcoming insert/full-table tests without obscuring the memory layout?

## References

- [db-learning-roadmap.md](db-learning-roadmap.md), roadmap step 6 and follow-on steps 7–10.
- `include/` and `src/` project layout defined by the repository instructions.
- Existing `Makefile` and `tests/test_cli.c` validation patterns.
