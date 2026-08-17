# Copy a record by value and by pointer

- Status: Proposed
- Owner: Repository maintainer
- Related ADR: None

## Goal

Demonstrate and verify the difference between copying an existing `record_t` by value and copying it through a pointer destination, using small reusable helpers in the existing `concepts` module.

## Context

Roadmap step 4 follows the introduction of `record_t` and manual initialization in steps 2 and 3. The project is a small C99 learning codebase, so the feature should make copy semantics observable without introducing table state, dynamic allocation, or CLI behavior.

The intended users are learners reading the `concepts` API and its tests. The copy operations must preserve the complete record, including the fixed-size character array, rather than merely copying the pointer to a value.

## Scope

- Add a helper that returns a copy of a `record_t` by value.
- Add a helper that copies a `record_t` into a caller-provided destination through pointers.
- Add focused assertions and output in the concepts test executable demonstrating that both copies contain the source data.
- Keep the feature within `include/concepts.h`, `src/concepts.c`, and `tests/test_concepts.c`.

## Non-goals

- No table, insert, find, delete, or CLI command behavior.
- No dynamic allocation or serialization to disk.
- No changes to the `record_t` layout or `RECORD_VALUE_CAPACITY`.
- No performance benchmark or replacement of compiler-generated struct assignment.

## Requirements and expected behavior

- The by-value helper accepts a `const record_t *` source and returns a `record_t` containing the same `id` and string contents.
- The pointer-based helper accepts a `const record_t *` source and a `record_t *` destination, then copies the complete record into the destination.
- Copying must include all members of `record_t`, including the inline `value` array.
- Modifying either copied record after the operation must not modify the source record.
- The pointer-based helper returns `0` after a successful copy and `-1` when either pointer is null, without dereferencing invalid input.
- The public declarations must be available from `concepts.h` and use C99-compatible types and naming.

## Acceptance criteria

- [ ] A public by-value copy helper is declared in `include/concepts.h` and returns an independent `record_t` copy.
- [ ] A public pointer-based copy helper is declared in `include/concepts.h` and copies into caller-owned storage.
- [ ] Tests verify that `id` and the full string value are preserved by both helpers.
- [ ] Tests modify each copied record and verify that the original record remains unchanged.
- [ ] The pointer-based helper handles the agreed null-pointer case without undefined behavior.
- [ ] `make test` builds and passes all existing test executables.

## Test plan

1. Initialize a source record with a non-default id and value, copy it by value, and assert that both members match.
2. Copy the source into a separately initialized destination through the pointer-based helper and assert that both members match.
3. Modify the by-value copy's id and value, then assert that the source is unchanged.
4. Modify the pointer-based copy's id and value, then assert that the source is unchanged.
5. Exercise the pointer helper's null-pointer contract and assert that it returns `-1` without modifying the valid argument.
6. Run `make test` to validate the concepts tests and the existing hello and CLI tests.

## Implementation plan

1. Add focused declarations to `include/concepts.h`, likely a by-value function such as `concepts_copy_record(const record_t *source)` and a status-returning pointer helper such as `concepts_copy_record_to(const record_t *source, record_t *destination)`.
2. Implement both helpers in `src/concepts.c` using ordinary C struct assignment; the pointer helper should validate pointers according to the selected contract before assignment.
3. Extend `tests/test_concepts.c` with independent-copy and null-input checks while preserving the existing step 2 and step 3 demonstrations.
4. Run `make test`; update this spec or add an ADR only if implementation reveals a project-wide design decision.

## Risks and dependencies

- The pointer helper uses `0` for success and `-1` for invalid pointers; callers can test the result without exceptions or hidden state.
- Struct assignment copies the inline array as part of the containing struct, but tests should explicitly verify the string member so this learning objective remains visible.
- Existing test output is intentionally instructional; added output should remain concise and readable.

## Open questions

- None.

## References

- [db-learning-roadmap.md](db-learning-roadmap.md), Phase 1 step 4
- [concepts.h](../../include/concepts.h)
- [concepts.c](../../src/concepts.c)
- [test_concepts.c](../../tests/test_concepts.c)
