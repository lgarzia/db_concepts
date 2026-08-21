# Feature spec planning skill

## Purpose

Use this skill to create or maintain concise, reviewable feature specifications with aligned requirements, acceptance criteria, test plans, and implementation plans.

## Workflow

1. Inspect only the target roadmap step from db-learning-roadmap.md and any directly related source or header files. Do not scan unrelated historical specifications in docs/specs/.
2. Define the problem, intended users, goal, scope, and non-goals.
3. Separate required behavior from implementation choices.
4. Identify affected modules, public interfaces, tests, build targets, and documentation.
5. Write acceptance criteria that are observable and testable.
6. Draft a test plan covering normal behavior, relevant boundaries or errors, and the project validation command.
7. Draft an implementation plan in dependency order, naming likely files and interfaces.
8. Record risks, dependencies, assumptions, open questions, and related ADRs.
9. **Read the template located at `.github/skills/feature-spec-planning/templates/spec-template.md`.**
10. **Format your final feature specification output to strictly match the structure, headings, and style of that template.**
11. Check that the spec is internally consistent and fits the repository conventions.

## Output location and naming

- **Target Directory:** All generated or updated feature specifications must be saved strictly inside the `docs/specs/` folder.
- **File Naming Convention:** Name files using the roadmap pattern matching the step number: `db-step-<NN>-<slug>.md` (e.g., `db-step-03-hash-table.md`).
- **File Creation:** Use the workspace file-writing tool to save the spec directly to disk at `docs/specs/<filename>.md` rather than just printing the markdown text in the chat window.

## Validation boundary

This skill is planning-only. Validate the specification’s structure, scope,
and internal consistency, but do not run build, test, lint, or application
commands unless the user explicitly requests execution or the task also
includes implementation changes.
