# Git workflow instructions

Apply these rules when creating branches or merging work for `docs/specs/db-learning-roadmap.md` steps.

## Branch naming

Use `db/<NN>-<slug>`:

- `db/` — namespaces this work from other feature branches.
- `<NN>` — zero-padded, two-digit roadmap step number (`01`–`60`) from `db-learning-roadmap.md`, matching the step exactly.
- `<slug>` — 3–5 word kebab-case summary of that step's text.

Examples: `db/01-record-struct`, `db/09-delete-by-id`, `db/24-save-command`, `db/60-final-adr`.

Use the global step number, not a phase prefix. The phase is always derivable from the roadmap; encoding it in the branch name risks drift if phases are reordered.

## Orchestrated roadmap identity

For Copilot-orchestrated roadmap work, do not derive a new slug from these
rules. Read `.github/roadmap-step-identities.json` and use its exact branch and
specification path for the supplied global step ID. It is the deterministic
identity source. Legacy branch aliases in that registry are only for detecting
already merged historical steps; do not reuse or rename them.
