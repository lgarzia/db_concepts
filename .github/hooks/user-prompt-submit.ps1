# VS Code sends the UserPromptSubmit event as one JSON object on standard input.
# Read all of stdin before parsing because the hook input is delivered as a
# complete stream, not as PowerShell parameters or environment variables.
$inputJson = [Console]::In.ReadToEnd()

try {
    # Convert the incoming JSON text into a PowerShell object so the script can
    # safely read documented fields such as hook_event_name.
    # Stop on conversion errors; otherwise PowerShell could continue with a
    # partially parsed or null value.
    $hookInput = $inputJson | ConvertFrom-Json -ErrorAction Stop

    # This message is returned to VS Code as context for the agent. It is not
    # ordinary terminal output because hook stdout is consumed by VS Code.
    $message = "UserPromptSubmit received: $($hookInput.hook_event_name)"
}
catch {
    # Keep diagnostic text on stderr. Hook stdout is reserved for the JSON
    # response below, so writing an error there would make the response invalid.
    # Exit code 2 tells VS Code to treat the hook failure as a blocking error.
    [Console]::Error.WriteLine("UserPromptSubmit received invalid JSON: $($_.Exception.Message)")
    exit 2
}

# Return the hook response as compact JSON on stdout.
#
# continue = true allows the agent session to proceed.
# systemMessage displays the message to the agent/user as hook context.
# ConvertTo-Json is used instead of hand-building a JSON string so that values
# are escaped correctly if the message later contains special characters.
@{
    continue = $true
    systemMessage = $message
} | ConvertTo-Json -Compress

# 2026-08-20 07:37:56.974 [info] [#203] [UserPromptSubmit] Input: {"timestamp":"2026-08-20T12:37:56.968Z","hook_event_name":"UserPromptSubmit","session_id":"a561aa4a-a5fc-4c38-873d-616a8214cc40","transcript_path":"c:\\Users\\lgarzia\\AppData\\Roaming\\Code\\User\\workspaceStorage\\694198e3921ec671923ffb1afb6582c1\\GitHub.copilot-chat\\transcripts\\a561aa4a-a5fc-4c38-873d-616a8214cc40.jsonl","prompt":"comment heavily and explain everything ","cwd":"c:\\Users\\lgarzia\\Documents\\GitHub\\db_concepts"}
# 2026-08-20 07:37:57.859 [info] [#203] [UserPromptSubmit] Completed (Success) in 890ms
# 2026-08-20 07:37:57.860 [info] [#203] [UserPromptSubmit] Output: {"systemMessage":"UserPromptSubmit received: UserPromptSubmit","continue":true}