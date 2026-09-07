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

function Invoke-UserPromptSubmitHook
{
    param(
        [string]$hook_path,
        [string]$payload
    )

    $powershell_executable = Join-Path $PSHOME "powershell.exe"
    if (-not (Test-Path -LiteralPath $powershell_executable))
    {
        $powershell_executable = "powershell.exe"
    }

    $start_info = New-Object System.Diagnostics.ProcessStartInfo
    $start_info.FileName = $powershell_executable
    $start_info.Arguments = '-NoProfile -ExecutionPolicy Bypass -File "{0}"' -f $hook_path
    $start_info.UseShellExecute = $false
    $start_info.RedirectStandardInput = $true
    $start_info.RedirectStandardOutput = $true
    $start_info.RedirectStandardError = $true
    $start_info.CreateNoWindow = $true

    $process = New-Object System.Diagnostics.Process
    $process.StartInfo = $start_info

    Assert-Condition $process.Start() "The hook process should start."
    $process.StandardInput.Write($payload)
    $process.StandardInput.Close()

    $stdout = $process.StandardOutput.ReadToEnd()
    $stderr = $process.StandardError.ReadToEnd()
    $process.WaitForExit()

    return [pscustomobject]@{
        ExitCode = $process.ExitCode
        Stdout = $stdout
        Stderr = $stderr
    }
}

$repository_root = Split-Path -Parent $PSScriptRoot
$hook_path = Join-Path $repository_root ".github\hooks\user-prompt-submit.ps1"

Assert-Condition (Test-Path -LiteralPath $hook_path) "The UserPromptSubmit hook must exist."

$valid_result = Invoke-UserPromptSubmitHook $hook_path '{"hook_event_name":"UserPromptSubmit","prompt":"complete step 12"}'
$valid_stdout = $valid_result.Stdout.Trim()

Assert-Condition ($valid_result.ExitCode -eq 0) "Valid JSON must return exit code 0."
Assert-Condition ([string]::IsNullOrWhiteSpace($valid_result.Stderr)) "Valid JSON must not write diagnostics to stderr."
Assert-Condition ($valid_stdout -match '^\{.*\}$') "Valid JSON must produce one JSON object on stdout."
Assert-Condition ($valid_stdout -notmatch "[`r`n]") "Hook stdout must be compact JSON."

$valid_response = $valid_stdout | ConvertFrom-Json -ErrorAction Stop
Assert-Condition ([bool]$valid_response.PSObject.Properties["continue"].Value) "Hook response must set continue to true."
Assert-Condition ($valid_response.systemMessage -eq "UserPromptSubmit received: UserPromptSubmit") "Hook response must identify the event."

$invalid_result = Invoke-UserPromptSubmitHook $hook_path '{"hook_event_name":'

Assert-Condition ($invalid_result.ExitCode -eq 2) "Malformed JSON must return exit code 2."
Assert-Condition ([string]::IsNullOrWhiteSpace($invalid_result.Stdout)) "Malformed JSON must not write a success response to stdout."
Assert-Condition ($invalid_result.Stderr -match "UserPromptSubmit received invalid JSON:") "Malformed JSON must emit a useful stderr diagnostic."

Write-Output "PASS: UserPromptSubmit hook valid and malformed stdin contracts."
