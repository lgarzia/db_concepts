# Git workflow instructions

Apply these rules when creating branches or merging work for `docs/specs/db-learning-roadmap.md` steps.

## Branch naming

Use `db/<NN>-<slug>`:

- `db/` — namespaces this work from other feature branches.
- `<NN>` — zero-padded, two-digit roadmap step number (`01`–`60`) from `db-learning-roadmap.md`, matching the step exactly.
- `<slug>` — 3–5 word kebab-case summary of that step's text.

Examples: `db/01-record-struct`, `db/09-delete-by-id`, `db/24-save-command`, `db/60-final-adr`.

Use the global step number, not a phase prefix. The phase is always derivable from the roadmap; encoding it in the branch name risks drift if phases are reordered.

## Per-step workflow

```powershell
git checkout main
git pull
git checkout -b db/<NN>-<slug>

# implement the step, then:
make test

git add -A
git commit -m "db step <NN>: <short description>"
git checkout main
git merge --no-ff db/<NN>-<slug>
git branch -d db/<NN>-<slug>
```

- Use `--no-ff` so each step stays visible as a discrete merge in history.
- Commit messages start with `db step <NN>:` so `git log --grep "db step"` gives a fast progress audit.
- Delete the branch after merge; only in-progress steps should have live branches.
- Never merge a step into `main` before `make test` passes.
