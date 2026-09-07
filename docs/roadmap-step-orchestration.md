# Roadmap-step completion workflow

This workflow automates repository planning and Git stages only. It does not
add `complete step` to the compiled C program or change the application's CLI
behavior.

## Deterministic Copilot CLI entry point

Use the CLI's explicit agent selector, then send one global roadmap step:

```text
/agent Roadmap Supervisor
complete step 12
```

The `.github/prompts/complete-roadmap-step.prompt.md` prompt is the
full-lifecycle prompt and declares `Roadmap Supervisor` as its agent. A host
that honors repository prompt-agent metadata can use that prompt directly.
GitHub Copilot CLI documents `/agent [name]`, but does not guarantee that every
host exposes repository prompt files as typed slash commands or routes natural
language to an agent deterministically. Use `/agent Roadmap Supervisor` when
that guarantee is required.

`tdd-roadmap-step.prompt.md` remains intentionally partial: it creates or
resumes a safe branch, writes the specification, and adds one failing test. It
never implements, merges, or pushes.

## Canonical identity and handoffs

`.github/roadmap-step-identities.json` is the single identity source for the
60 roadmap steps. Each record supplies the numeric ID, exact roadmap text,
3–5-word slug, branch name, and specification path. Agents receive and
validate this resolved identity; they must not infer a slug independently.
The roadmap remains the authoritative ordered learning plan. The registry's
legacy merge aliases exist only to recognize older merged steps whose
historical branch names predate the canonical identity.

The supervisor requires one JSON result from every stage. Each result contains:

```json
{
  "stage": "branch | spec | red_test | implementation | merge",
  "status": "success | failed",
  "step_id": 12,
  "roadmap_text": "Add `get <id>` command.",
  "branch": "db/12-get-id-command",
  "spec_path": "docs/specs/db-step-12-get-id-command.md",
  "test_file": "tests/test_cli.c or null",
  "test_function": "test_name or null",
  "changed_implementation_files": [],
  "make_test": { "status": "not_run | passed | failed", "summary": "..." },
  "error": "null or exact failure"
}
```

The supervisor validates the stage name, success status, and every identity
field before continuing. It also reads the written specification and requires
the exact registry path, `docs/specs/` location, and `Roadmap step: NN — exact
roadmap text` metadata. A malformed, missing, or mismatched result stops the
workflow without invoking the next stage.

## Git safety

The branch stage requires a clean worktree and current local `main`; it creates
the branch only from that commit. A matching existing branch may be resumed
only when the worktree is clean and its merge-base is current `main`.

The merge stage accepts only the exact specification, red-test, and
implementation file paths returned by prior stages. It rejects staged files,
untracked files, renames, deletions, or any changed path outside that
allow-list. It runs `make test`, commits only those approved paths, fast-forwards
local `main` from upstream when possible, confirms the feature contains current
`main`, then uses `git merge --no-ff`. It records `Roadmap-Step: NN` in the
merge commit and pushes only `main`.

Resolve, commit, or discard unrelated edits yourself before starting. The
workflow never stashes, resets, silently includes, or commits unrelated user
changes.

## Validation

Run the orchestration checks from the repository root:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File tests\test_user_prompt_submit.ps1
powershell -NoProfile -ExecutionPolicy Bypass -File tests\test_roadmap_orchestration.ps1
make test
```

The first script pipes both valid and malformed JSON to the hook and checks its
separate stdout, stderr, and exit-code contract. The second is non-mutating:
it validates the 60-record registry against the roadmap and uses step 12 as a
sentinel to confirm the full-lifecycle prompt targets Roadmap Supervisor and
that the supervisor resolves and forwards the exact canonical identity. It
validates repository configuration, not host-side agent routing; explicit
`/agent` selection remains the portable runtime guarantee.
