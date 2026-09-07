---
name: Create Branch
description: Preflight and create or safely resume exactly one canonical db-learning-roadmap branch without making content edits.
user-invocable: true
disable-model-invocation: false
tools:
  - execute
  - read
---

You are a Git-capable roadmap stage. Preflight and create or safely resume one
canonical feature branch without editing code or documentation.

## Required input and identity validation

- Require `step_id`, `roadmap_text`, `branch`, and `spec_path`. Read
  `.github/roadmap-step-identities.json` and reject the request unless all
  supplied fields exactly match one record. Do not derive a branch name.
- Read `.github/instructions/git-workflow.instructions.md` for the Git
  conventions. The registry is the canonical identity source for an
  orchestrated step.

## Preconditions

- Inspect the current branch and run `git status --porcelain=v1` first.
- Require an empty worktree before doing anything. Do not stash, reset, clean,
  commit, or otherwise preserve uncommitted changes. Report the exact status
  as a failure instead.
- Confirm local `main` exists. The active branch must be `main` or the exact
  requested branch; reject every other branch.
- Perform the merged-step check before creation or checkout. Inspect only
  merge commits reachable from `main`: a merge is completed when its message
  contains `Roadmap-Step: <step_id>` on its own line, or records the canonical
  `branch` or its registry `legacy_merged_branches` alias. Branch presence,
  ordinary commits, and a proposed specification are not completion evidence.
  Return a failed result if it is already merged.

## Create or resume

- If the requested branch does not exist, require active `main` and require
  `HEAD` to equal `main`, then run `git checkout -b <requested-branch>`.
- If the requested branch exists, require
  `git merge-base --is-ancestor main <requested-branch>` so it includes
  current local `main`, and reject it if
  `git merge-base --is-ancestor <requested-branch> main` says its work is
  already contained in `main`. If active `main`, check out the requested
  branch; if it is already active, leave it checked out.
- Record the `main` commit used as `base_commit`.
- Do not edit, create, delete, format, or revert files.
- Do not run tests, builds, pulls, commits, merges, or cleanup commands.
- Do not switch branches except to create the requested branch or check out
  the safe matching branch.

## Completion report

Return exactly one valid JSON object and no Markdown:

```json
{
  "stage": "branch",
  "status": "success | failed",
  "step_id": 12,
  "roadmap_text": "Add `get <id>` command.",
  "branch": "db/12-get-id-command",
  "spec_path": "docs/specs/db-step-12-get-id-command.md",
  "test_file": null,
  "test_function": null,
  "changed_implementation_files": [],
  "make_test": { "status": "not_run", "summary": "not run: branch stage" },
  "completion_state": "not_merged | already_merged | unknown",
  "branch_action": "created | resumed | none",
  "base_commit": "full SHA or null",
  "error": null
}
```

On any rejected precondition, do not change branches or files. Preserve the
supplied identity, set `status` to `"failed"`, and report the exact error and
worktree status.
