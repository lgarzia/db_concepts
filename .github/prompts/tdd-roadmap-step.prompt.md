---
description: "Kick off TDD for a db-learning-roadmap step: creates the branch, writes the spec, and adds one failing (red) test. Stops after red — no implementation, merge, or push."
---

# TDD roadmap step kickoff

Roadmap step number: ${input:step}

Steps:

1. Read `docs/specs/db-learning-roadmap.md` and find the numbered step matching `${input:step}` exactly.
2. Zero-pad the step number to two digits (NN).
3. Derive a 3-5 word kebab-case slug summarizing that step's text (e.g., step 7 -> `linear-insert-array`).
4. Resolve:
   - branch name: `db/<NN>-<slug>`
   - spec file path: `docs/specs/db-step-<NN>-<slug>.md`
5. Invoke the `tdd-roadmap-step` agent, passing the roadmap step number, derived slug, resolved branch name, and resolved spec file path.

This flow stops after a confirmed failing (red) test is added and reported. Do not implement the feature, merge, or push as part of this prompt.

<!-- /tdd-roadmap-step step=7 -->
