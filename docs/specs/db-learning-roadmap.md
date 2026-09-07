# Simple database in C — learning roadmap

A 60-step, first-principles path from the current CLI harness to a small but real embedded database engine. Each step builds on the previous one and should be small enough to be its own commit, validated with `make test`.

Related: [cli-command-harness.md](cli-command-harness.md)

## Phase 1 — C mechanics you'll actually use (1–10)

1. Review pointers vs. arrays vs. buffers in `cli.c`.
2. Add a `struct` to model one record (e.g., id + string value).
3. Practice manual struct initialization and `sizeof`.
4. Write a function that copies a struct by value vs. by pointer.
5. Add `const`-correctness pass across `cli.h`/`cli.c`.
6. Introduce a simple fixed-size array "table" of records in memory.
7. Implement linear insert into that array.
8. Implement linear scan/find by id.
9. Implement delete-by-id (swap-with-last or tombstone).
10. Write unit tests for insert/find/delete.

## Phase 2 — Wire records into the CLI (11–20)

11. Add `insert <id> <value>` command.
12. busted implementation of get command
13. Add `get <id>` command.
14. Add `delete <id>` command.
15. Add `list` command to dump all records.
16. Handle duplicate id on insert (reject or update).
17. Handle get/delete on missing id (clear error).
18. Cap table size and handle "table full" explicitly.
19. Extract table logic into `include/table.h` / `src/table.c`.
20. Add tests for table module independent of CLI parsing.
21. Update the spec/README with the new commands.

## Phase 3 — Persistence to disk (21–30)

22. Design a fixed-size binary record layout (id + fixed-length bytes).
23. Write one record to a file with `fwrite`.
24. Read one record back with `fread` and verify round-trip.
25. Implement `save <file>` command using the table.
26. Implement `load <file>` command populating the table.
27. Handle short reads / corrupt file length gracefully.
28. Add a file header (magic number, version, record count).
29. Validate the header on load; reject unknown versions.
30. Add tests using a temp file per test run.
31. Document the on-disk format in an ADR.

## Phase 4 — Pages and a real storage engine (31–40)

32. Introduce a fixed-size "page" (e.g., 4KB) as the unit of I/O.
33. Pack multiple records into one page with a small page header.
34. Implement page read/write functions (`read_page`, `write_page`).
35. Add a free-space or slot count per page.
36. Implement "append record" that finds a page with space or allocates a new one.
37. Implement full-table scan across all pages.
38. Implement delete-with-compaction inside a page.
39. Add a page cache (small fixed array of recently used pages).
40. Add cache eviction (simple LRU or clock).
41. Benchmark scan performance before/after caching.

## Phase 5 — Indexing (41–50)

42. Implement a simple sorted-array index of id → page/slot.
43. Implement binary search over the sorted index.
44. Keep the index in sync on insert.
45. Keep the index in sync on delete.
46. Rebuild the index from a full table scan (recovery path).
47. Replace linear `get` with index-based lookup.
48. Measure and compare lookup cost with/without the index.
49. Introduce a minimal B-tree node structure (conceptual step).
50. Implement B-tree search only (no insert yet) over static data.
51. Implement B-tree insert with leaf splitting.

## Phase 6 — Correctness, durability, concurrency basics (52–61)

52. Add a write-ahead log (WAL) record format.
53. Append to WAL before mutating in-memory state.
54. Implement crash-recovery replay of the WAL on startup.
55. Add a checkpoint step that truncates the WAL after persisting.
56. Add basic transaction boundaries: `begin`, `commit`, `rollback` commands.
57. Buffer writes during a transaction; apply atomically on commit.
58. Discard buffered writes on rollback.
59. Add a simple single-writer lock to prevent concurrent corruption.
60. Write a fault-injection test (simulate crash mid-write) and verify recovery.
61. Write a final ADR summarizing the storage architecture and remaining tradeoffs.
