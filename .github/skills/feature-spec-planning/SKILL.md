---
name: feature-spec-planning
description: Create or maintain concise, reviewable feature specifications.
---

# Feature spec planning skill

## Purpose

Use this skill to create or maintain concise, reviewable feature specifications with aligned requirements, acceptance criteria, test plans, and implementation plans.

## Workflow

1. For a roadmap step, require the caller's `step_id`, exact `roadmap_text`,
   `branch`, and exact `spec_path`. Validate all values against
   `.github/roadmap-step-identities.json`; do not derive a slug or output path.
2. Inspect only the target roadmap step from db-learning-roadmap.md and any directly related source or header files. Do not scan unrelated historical specifications in docs/specs/.
3. Define the problem, intended users, goal, scope, and non-goals.
4. Separate required behavior from implementation choices.
5. Identify affected modules, public interfaces, tests, build targets, and documentation.
6. Write acceptance criteria that are observable and testable.
7. Draft a test plan covering normal behavior, relevant boundaries or errors, and the project validation command.
8. Draft an implementation plan in dependency order, naming likely files and interfaces.
9. Record risks, dependencies, assumptions, open questions, and related ADRs.
10. **Read the template located at `.github/skills/feature-spec-planning/templates/spec-template.md`.**
11. **Format your final feature specification output to strictly match the structure, headings, and style of that template.**
12. Check that the spec is internally consistent and fits the repository conventions.

## Output location and naming

- **Exact target:** Write only to the exact validated registry `spec_path`; it
  must be a Markdown path directly under `docs/specs/`. Do not choose or
  rename the filename.
- **Roadmap identity:** Add `- Roadmap step: <step_id> - <roadmap_text>` using
  the exact validated values.
- **Direct creation:** Use the workspace file-writing tool to save the spec
  directly to the exact path rather than printing the markdown text in the
  chat window.
- **Post-write proof:** Re-read the exact path before reporting success.
  Confirm it exists, is nonempty, remains under `docs/specs/`, includes the
  exact roadmap-identity line, and has every heading from the template.

## Validation boundary

This skill is planning-only. Validate the specification’s structure, scope,
and internal consistency, but do not run build, test, lint, or application
commands unless the user explicitly requests execution or the task also
includes implementation changes.
