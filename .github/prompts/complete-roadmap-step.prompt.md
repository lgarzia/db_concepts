---
description: "Complete exactly one db-learning-roadmap step through merge and push."
agent: Roadmap Supervisor
argument-hint: "<global-step-number>"
---

# Complete roadmap step

Roadmap step number: ${input:step}

This is the full-lifecycle workflow: branch, feature specification, one red
test, implementation, `make test`, merge, and push. It is not the TDD kickoff
workflow.

1. Accept exactly one decimal global roadmap step number from 1 through 60.
   Reject empty, non-numeric, out-of-range, local-phase, duplicate, or
   multi-step input.
2. Invoke **Roadmap Supervisor** directly with `complete step ${input:step}`.
   The supervisor is the sole identity resolver: it reads
   `.github/roadmap-step-identities.json` once, then passes the exact
   `step_id`, `roadmap_text`, `branch`, and `spec_path` fields unchanged to
   every stage. Do not pass a manually derived slug or path.
3. The supervisor owns the stage sequence and stops on the first failed
   precondition or failed structured handoff. Do not implement, merge, or push
   outside that agent's workflow.

<!-- /complete-roadmap-step step=12 -->
