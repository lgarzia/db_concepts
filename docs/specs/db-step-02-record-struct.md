# DB roadmap step 2 - add a record struct

- Status: Implemented
- Owner: [person or team]
- Related ADR: None currently needed

## Goal

Introduce a small C struct that models one database record with an integer id and a fixed-capacity string value.

## Context

This is roadmap step 2 of [db-learning-roadmap.md](db-learning-roadmap.md) ("Add a `struct` to model one record (e.g., id + string value)"). The existing concepts module provides the nearest learning surface for this first-principles C work. The record type is intentionally only a data model; table behavior begins in later roadmap steps.

## Scope

- Define a public `record_t` type in `include/concepts.h`.
- Give the record an `int id` field.
- Give the record a fixed-size `char value` array using `RECORD_VALUE_CAPACITY`.
- Test manual initialization and field access in `tests/test_concepts.c`.

## Non-goals

- No in-memory table or collection of records.
- No insert, find, delete, or CLI commands.
- No persistence or serialization format.
- No struct-copy helper; that is roadmap step 4.

## Requirements and expected behavior

1. `record_t` must be available to code that includes `concepts.h`.
2. A record must contain an integer `id` and a fixed-capacity string buffer named `value`.
3. The value capacity must be represented by the `RECORD_VALUE_CAPACITY` macro.
4. A record must support C99 designated-initializer syntax.
5. The record definition must not introduce hidden state or dynamic allocation.

## Acceptance criteria

- [x] `concepts.h` defines `RECORD_VALUE_CAPACITY` as 32.
- [x] `concepts.h` defines `record_t` with `id` and `value` fields.
- [x] The test initializes a record with designated initializers.
- [x] The test verifies the id and string value.
- [x] The test verifies the value array capacity with `sizeof`.
- [x] `make test` passes.

## Test plan

1. Initialize `record_t` with `.id = 7` and `.value = "hello"`.
2. Assert that the initialized id is `7`.
3. Assert that the initialized value matches `"hello"`.
4. Assert that `sizeof(record.value)` equals `RECORD_VALUE_CAPACITY`.
5. Run `make test` to verify the concepts test and existing tests.

## Implementation plan

1. Add `RECORD_VALUE_CAPACITY` to `include/concepts.h`.
2. Define `record_t` in `include/concepts.h` with an `int id` and `char value[RECORD_VALUE_CAPACITY]`.
3. Extend `tests/test_concepts.c` with a designated-initializer example and assertions for the fields and buffer capacity.
4. Run `make test`.

## Risks and dependencies

- The fixed value capacity limits the string storage available in a record; later persistence work can formalize how that capacity is encoded on disk.
- The record type is public through `concepts.h`, so later table and storage steps can reuse it without duplicating the layout.

## Open questions

- None for this learning step.

## References

- `docs/specs/db-learning-roadmap.md`
- `include/concepts.h`
- `tests/test_concepts.c`
