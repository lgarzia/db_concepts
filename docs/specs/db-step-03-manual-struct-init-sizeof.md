# DB roadmap step 3 — manual struct initialization and sizeof

- Status: Proposed
- Owner: [person or team]
- Related ADR: None currently needed

## Goal

Make the C data model explicit by practicing manual struct initialization, field access, and `sizeof` on a record type so the team can reason about memory layout before building tables or persistence.

## Context

This is roadmap step 3 of [db-learning-roadmap.md](db-learning-roadmap.md) ("Practice manual struct initialization and `sizeof`"). The repository already includes a `record_t` model introduced in step 2 and a pure helper demonstration in the concepts module. This step focuses on teaching the mechanics of struct initialization and layout in a way that is observable and testable from C code, not just conceptual.

The learning objective is to build intuition for how C stores data in memory: the compiler lays out fields in order, arrays have a fixed number of bytes, and `sizeof` reports the byte footprint of a field or object. That understanding is the foundation for later steps involving tables, binary files, and page layouts.

## Scope

- Add or refine a record initialization example that demonstrates manual struct assignment and designated initialization.
- Exercise `sizeof(record_t)` and `sizeof(record.value)` in the test or demonstration code.
- Keep the examples readable, explicit, and aligned with the roadmap's learning-first approach.
- Use the already-established `record_t` in `include/concepts.h` as the central type for the exercise.

## Non-goals

- No table, insert, delete, or array-of-record logic.
- No CLI commands or persistence features.
- No hidden helper abstractions beyond small, explicit test code.
- No change in the public storage format or on-disk layout.

## Requirements and expected behavior

- A `record_t` instance can be initialized manually in code using explicit field assignment or designated initializers.
- The test must demonstrate that `sizeof(record.value)` matches the declared capacity constant rather than relying on an implicit assumption.
- The code must make the relationship between `record_t` memory layout and `char value[RECORD_VALUE_CAPACITY]` explicit.
- The example must be simple enough to understand in a single test or small demonstration, while still being realistic for later database work.
- Existing step 1 and step 2 behavior remains unchanged.

## Acceptance criteria

- [ ] A test or demonstration initializes a `record_t` object manually and verifies the expected `id` and `value` content.
- [ ] The code asserts `sizeof(record.value) == RECORD_VALUE_CAPACITY`.
- [ ] The code also verifies the size relationship for the enclosing `record_t` object in a way that makes struct layout visible.
- [ ] `make test` passes without regressions.
- [ ] The examples remain readable and are suitable as a learning artifact for the next roadmap steps.

## Test plan

1. Initialize a `record_t` with `.id = 7` and `.value = "hello"`.
2. Assert that the `id` field matches `7` and the string value matches `"hello"`.
3. Assert that `sizeof(record.value)` equals `RECORD_VALUE_CAPACITY`.
4. Add a second assertion showing the object size is at least the expected layout of `id` plus the fixed-length value array.
5. Run `make test` to confirm the new spec is satisfied and no prior tests regressed.

## Implementation plan

1. Reuse the existing `record_t` declaration from `include/concepts.h`.
2. Extend `tests/test_concepts.c` with a focused manual initialization example and `sizeof` assertions.
3. Keep the new assertions explicit and small; prefer readability and teaching value over cleverness.
4. Run `make test` and resolve any issues before considering the step complete.

## Risks and dependencies

- If the capacity constant changes later, the tests must be updated to reflect the new layout; this is a normal dependency of learning-oriented code.
- The step is intentionally shallow and should not add operational complexity; future steps will build on the record type with tables and persistence.

## Open questions

- None for this step; the behavior is well scoped and follows directly from the roadmap and current codebase.

## References

- `docs/specs/db-learning-roadmap.md`
- `docs/specs/db-step-02-record-struct.md`
- `include/concepts.h`
- `tests/test_concepts.c`
