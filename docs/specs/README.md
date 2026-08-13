# Specs

This folder is for structured planning documents and feature specifications.

Use it for:

- design proposals
- feature requests
- implementation notes
- architecture changes

## Organization

Prefer one spec per independently discussable or deliverable feature, not one spec per source file or function. Keep a feature's test plan and implementation plan in the same document by default. Split them only when they become independently owned, reviewed, or complex.

For a minimal starter project, keep specs short and outcome-focused. A useful spec normally includes:

- goal and context
- scope and non-goals
- requirements and expected behavior
- acceptance criteria
- test plan
- implementation plan
- risks, dependencies, and open questions

Use `.github/skills/feature-spec-planning/templates/spec-template.md` as a starting point. The planning workflow is described by `.github/agents/feature-spec.agent.md` and `.github/skills/feature-spec-planning/SKILL.md`.

## Document boundaries

- A spec describes what should be built and how completion will be verified.
- An ADR records why a significant architectural decision was made.
- Instructions define rules that apply repeatedly across tasks.
- Skills define reusable workflows for specialized tasks.
- Memory records stable repository facts and lessons.
