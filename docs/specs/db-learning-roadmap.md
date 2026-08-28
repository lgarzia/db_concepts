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
10. Write unit tests for

## Phase 2 — Wire records into the CLI (11–20)

11. Add `insert <id> <value>` command.
12. Add `get <id>` command.
13. Add `delete <id>` command.
14. Add `list` command to dump all records.
15. Handle duplicate id on insert (reject or update).
16. Handle get/delete on missing id (clear error).
17. Cap table size and handle "table full" explicitly.
18. Extract table logic into `include/table.h` / `src/table.c`.
19. Add tests for table module independent of CLI parsing.
20. Update the spec/README with the new commands.

## Phase 3 — Persistence to disk (21–30)

21. Design a fixed-size binary record layout (id + fixed-length bytes).
22. Write one record to a file with `fwrite`.
23. Read one record back with `fread` and verify round-trip.
24. Implement `save <file>` command using the table.
25. Implement `load <file>` command populating the table.
26. Handle short reads / corrupt file length gracefully.
27. Add a file header (magic number, version, record count).
28. Validate the header on load; reject unknown versions.
29. Add tests using a temp file per test run.
30. Document the on-disk format in an ADR.

## Phase 4 — Pages and a real storage engine (31–40)

31. Introduce a fixed-size "page" (e.g., 4KB) as the unit of I/O.
32. Pack multiple records into one page with a small page header.
33. Implement page read/write functions (`read_page`, `write_page`).
34. Add a free-space or slot count per page.
35. Implement "append record" that finds a page with space or allocates a new one.
36. Implement full-table scan across all pages.
37. Implement delete-with-compaction inside a page.
38. Add a page cache (small fixed array of recently used pages).
39. Add cache eviction (simple LRU or clock).
40. Benchmark scan performance before/after caching.

## Phase 5 — Indexing (41–50)

41. Implement a simple sorted-array index of id → page/slot.
42. Implement binary search over the sorted index.
43. Keep the index in sync on insert.
44. Keep the index in sync on delete.
45. Rebuild the index from a full table scan (recovery path).
46. Replace linear `get` with index-based lookup.
47. Measure and compare lookup cost with/without the index.
48. Introduce a minimal B-tree node structure (conceptual step).
49. Implement B-tree search only (no insert yet) over static data.
50. Implement B-tree insert with leaf splitting.

## Phase 6 — Correctness, durability, concurrency basics (51–60)

51. Add a write-ahead log (WAL) record format.
52. Append to WAL before mutating in-memory state.
53. Implement crash-recovery replay of the WAL on startup.
54. Add a checkpoint step that truncates the WAL after persisting.
55. Add basic transaction boundaries: `begin`, `commit`, `rollback` commands.
56. Buffer writes during a transaction; apply atomically on commit.
57. Discard buffered writes on rollback.
58. Add a simple single-writer lock to prevent concurrent corruption.
59. Write a fault-injection test (simulate crash mid-write) and verify recovery.
60. Write a final ADR summarizing the storage architecture and remaining tradeoffs.
