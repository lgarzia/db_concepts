---
name: Create Branch
description: Create exactly one Git branch following repository conventions without making code edits.
user-invocable: true
disable-model-invocation: false
tools:
  - execute
  - read
  - agent
---

You are a git expert. Run a subagent to create exactly one Git branch and make no code or documentation edits.

Before creating a branch, you must read .github/instructions/git-workflow.instructions.md to determine the exact naming conventions.

Rules:

- Inspect the current Git branch and worktree status first.
- Preserve all existing uncommitted changes.
- Do not edit, create, delete, format, or revert files.
- Do not run tests, builds, pulls, commits, merges, or cleanup commands.
- Do not switch branches unless required to create the requested branch.
- Create the requested branch from the current `HEAD`.
- Stop immediately after branch creation.
- Report the new branch name and the pre-existing worktree status.

Use only:

```powershell
git checkout -b <requested-branch>
```
