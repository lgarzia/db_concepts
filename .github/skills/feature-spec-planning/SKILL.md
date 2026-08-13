# Feature spec planning skill

## Purpose

Use this skill to create or maintain concise, reviewable feature specifications with aligned requirements, acceptance criteria, test plans, and implementation plans.

## Workflow

1. Inspect the relevant repository files and existing documentation.
2. Define the problem, intended users, goal, scope, and non-goals.
3. Separate required behavior from implementation choices.
4. Identify affected modules, public interfaces, tests, build targets, and documentation.
5. Write acceptance criteria that are observable and testable.
6. Draft a test plan covering normal behavior, relevant boundaries or errors, and the project validation command.
7. Draft an implementation plan in dependency order, naming likely files and interfaces.
8. Record risks, dependencies, assumptions, open questions, and related ADRs.
9. Check that the spec is internally consistent and fits the repository conventions.

## Granularity rules

- Prefer one spec per independently discussable or deliverable feature.
- Do not create one spec per function, struct, or source file.
- Keep test and implementation plans in the feature spec by default.
- Split plans into separate documents only when they are independently owned, reviewed, or complex.

## Boundaries

- This skill produces planning documents; it does not implement the feature.
- Use an ADR for the reasoning behind a significant architectural decision.
- Use instructions for rules that apply repeatedly across tasks.
- Use memory for stable repository facts and lessons, not active task plans.
- Keep temporary session notes out of the durable spec unless they become a project decision.

## Validation checklist

- The goal and intended outcome are clear.
- Scope and non-goals prevent uncontrolled expansion.
- Requirements describe observable behavior.
- Acceptance criteria can be checked by a reviewer or test.
- The test plan covers the acceptance criteria.
- The implementation plan names likely affected surfaces and preserves public API boundaries.
- Risks, dependencies, assumptions, and open questions are explicit.
- The document identifies related ADRs or states that none are needed.
