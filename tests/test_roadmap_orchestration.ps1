Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

function Assert-Condition
{
    param(
        [bool]$condition,
        [string]$message
    )

    if (-not $condition)
    {
        throw "Assertion failed: $message"
    }
}

function Read-Utf8File
{
    param([string]$path)

    return Get-Content -Raw -Encoding UTF8 -LiteralPath $path
}

function Assert-Contains
{
    param(
        [string]$content,
        [string]$expected,
        [string]$description
    )

    Assert-Condition ($content.Contains($expected)) $description
}

$repository_root = Split-Path -Parent $PSScriptRoot
$registry_path = Join-Path $repository_root ".github\roadmap-step-identities.json"
$roadmap_path = Join-Path $repository_root "docs\specs\db-learning-roadmap.md"
$hook_format_path = Join-Path $repository_root ".github\hooks\__format.json"
$hook_path = Join-Path $repository_root ".github\hooks\user-prompt-submit.ps1"
$completion_prompt_path = Join-Path $repository_root ".github\prompts\complete-roadmap-step.prompt.md"

$registry = (Read-Utf8File $registry_path) | ConvertFrom-Json -ErrorAction Stop
$steps = @($registry.steps)
$roadmap = Read-Utf8File $roadmap_path
$roadmap_matches = [regex]::Matches($roadmap, '(?m)^(?<id>[1-9]|[1-5][0-9]|60)\.\s+(?<text>.+)$')

Assert-Condition ($registry.version -eq 1) "The identity registry must have version 1."
Assert-Condition ($registry.roadmap_path -eq "docs/specs/db-learning-roadmap.md") "The registry must identify the authoritative roadmap."
Assert-Condition ($steps.Count -eq 60) "The registry must contain all 60 roadmap steps."
Assert-Condition ($roadmap_matches.Count -eq 60) "The roadmap must contain all 60 numbered steps."

foreach ($id in 1..60)
{
    $records = @($steps | Where-Object { $_.id -eq $id })
    $roadmap_records = @($roadmap_matches | Where-Object { [int]$_.Groups["id"].Value -eq $id })

    Assert-Condition ($records.Count -eq 1) "Registry step $id must appear exactly once."
    Assert-Condition ($roadmap_records.Count -eq 1) "Roadmap step $id must appear exactly once."

    $record = $records[0]
    $roadmap_text = $roadmap_records[0].Groups["text"].Value.TrimEnd("`r")
    $expected_branch = "db/{0:D2}-{1}" -f $id, $record.slug
    $expected_spec_path = "docs/specs/db-step-{0:D2}-{1}.md" -f $id, $record.slug

    Assert-Condition ($record.text -eq $roadmap_text) "Registry text for step $id must exactly match the roadmap."
    Assert-Condition ($record.slug -match '^[a-z0-9]+(?:-[a-z0-9]+){2,4}$') "Registry slug for step $id must contain 3 to 5 kebab-case words."
    Assert-Condition ($record.branch -eq $expected_branch) "Registry branch for step $id must be derived from its canonical slug."
    Assert-Condition ($record.spec_path -eq $expected_spec_path) "Registry spec path for step $id must be derived from its canonical slug."
    Assert-Condition ($null -ne $record.legacy_merged_branches) "Registry step $id must define legacy merge aliases, even when empty."
}

$step_12 = @($steps | Where-Object { $_.id -eq 12 })[0]
Assert-Condition ($step_12.text -eq 'Add `get <id>` command.') "Step 12 must retain its exact roadmap text."
Assert-Condition ($step_12.slug -eq "get-id-command") "Step 12 must retain its canonical slug."
Assert-Condition ($step_12.branch -eq "db/12-get-id-command") "Step 12 must resolve to db/12-get-id-command."
Assert-Condition ($step_12.spec_path -eq "docs/specs/db-step-12-get-id-command.md") "Step 12 must resolve to its exact specification path."

$hook_format = (Read-Utf8File $hook_format_path) | ConvertFrom-Json -ErrorAction Stop
$windows_hook_command = $hook_format.hooks.UserPromptSubmit[0].windows
$hook_script = Read-Utf8File $hook_path

Assert-Condition ($windows_hook_command -match '-File \.github\\hooks\\user-prompt-submit\.ps1') "Windows hook configuration must use PowerShell -File."
Assert-Contains $hook_script '[Console]::In.ReadToEnd()' "The hook must read its JSON event from stdin."
Assert-Contains $hook_script '[Console]::Error.WriteLine' "The hook must reserve stderr for diagnostics."
Assert-Contains $hook_script 'ConvertTo-Json -Compress' "The hook must emit compact JSON."

$completion_prompt = Read-Utf8File $completion_prompt_path
$supervisor_agent = Read-Utf8File (Join-Path $repository_root ".github\agents\roadmap-supervisor.agent.md")

Assert-Condition ($completion_prompt -match '(?m)^agent:\s*Roadmap Supervisor\s*$') "The full-lifecycle prompt must explicitly target Roadmap Supervisor."
Assert-Contains $completion_prompt 'Roadmap step number: ${input:step}' "The full-lifecycle prompt must accept one supplied step number."
Assert-Contains $completion_prompt 'Invoke **Roadmap Supervisor** directly' "The full-lifecycle prompt must explicitly invoke Roadmap Supervisor."
Assert-Condition ($supervisor_agent -match 'resolve each requested ID\s+once') "The supervisor must resolve canonical identity once."
Assert-Contains $supervisor_agent "Pass that unchanged identity object to every stage." "The supervisor must pass canonical identity unchanged."
Assert-Contains $supervisor_agent '"stage": "branch | spec | red_test | implementation | merge"' "The supervisor must require structured stage handoffs."
Assert-Contains $supervisor_agent "Roadmap step: <step_id> - <roadmap_text>" "The supervisor must verify exact specification identity."

$contract_checks = @(
    @{ Path = ".github\agents\feature-spec.agent.md"; Text = 'Write the specification directly to the supplied `spec_path`'; Description = "Feature-spec agent must direct-write the exact output path." },
    @{ Path = ".github\skills\feature-spec-planning\SKILL.md"; Text = "Post-write proof:"; Description = "Feature-spec skill must require post-write verification." },
    @{ Path = ".github\agents\write-red-test.agent.md"; Text = "Reject a missing or mismatched spec."; Description = "Red-test agent must reject an invalid spec handoff." },
    @{ Path = ".github\agents\write-red-test.agent.md"; Text = '"status": "failed"'; Description = "Red-test stage must report its required failing test state." },
    @{ Path = ".github\agents\implement-roadmap-step.agent.md"; Text = "Reject a missing or mismatched specification."; Description = "Implementation agent must reject an invalid spec handoff." },
    @{ Path = ".github\agents\implement-roadmap-step.agent.md"; Text = '"status": "passed"'; Description = "Implementation stage must report a passing make test result." },
    @{ Path = ".github\agents\create-branch.agent.md"; Text = "Require an empty worktree before doing anything."; Description = "Branch stage must fail on a dirty worktree." },
    @{ Path = ".github\agents\create-branch.agent.md"; Text = "git merge-base --is-ancestor main <requested-branch>"; Description = "Branch stage must validate a resumed branch base." },
    @{ Path = ".github\agents\merge-branch.agent.md"; Text = "reject staged files,"; Description = "Merge stage must reject unsafe worktree changes." },
    @{ Path = ".github\agents\merge-branch.agent.md"; Text = 'git merge --no-ff <branch>'; Description = "Merge stage must use an explicit no-fast-forward merge." },
    @{ Path = ".github\agents\tdd-roadmap-step.agent.md"; Text = "Do not derive a slug or path."; Description = "TDD kickoff must use the canonical identity." }
)

foreach ($check in $contract_checks)
{
    $content = Read-Utf8File (Join-Path $repository_root $check.Path)
    Assert-Contains $content $check.Text $check.Description
}

Write-Output "PASS: Canonical registry, hook routing, and step-12 Roadmap Supervisor sentinel are valid."
