---
description: "Kick off TDD for a db-learning-roadmap step: creates the branch, writes the spec, and adds one failing (red) test. Stops after red — no implementation, merge, or push."
agent: TDD Roadmap Step
argument-hint: "<global-step-number>"
---

# TDD roadmap step kickoff

Roadmap step number: ${input:step}

Steps:

1. Accept exactly one decimal global roadmap step number from 1 through 60.
2. Read `.github/roadmap-step-identities.json` and resolve that single record.
   Do not derive a slug, branch name, or specification path from prose.
3. Invoke the `TDD Roadmap Step` agent with the exact registry values for
   `step_id`, `roadmap_text`, `branch`, and `spec_path`.

This flow stops after a confirmed failing (red) test is added and reported. Do not implement the feature, merge, or push as part of this prompt.

<!-- /tdd-roadmap-step step=7 -->
