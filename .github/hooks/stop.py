"""Handle the VS Code agent Stop hook event.

VS Code starts this script as a command hook and sends the event payload as
JSON through standard input. The script must write its hook response as JSON
to standard output. Diagnostic text belongs on standard error so it does not
corrupt the JSON response that VS Code parses.
"""

import json
import sys


def main() -> int:
    """Read a Stop event and return a valid hook response."""
    # Read the complete stdin stream because VS Code provides the event as one
    # JSON document, rather than as command-line arguments.
    input_json = sys.stdin.read()

    try:
        # Parse the payload with the standard-library JSON decoder. This keeps
        # the hook dependency-free and lets us validate the input before using
        # any of its fields.
        hook_input = json.loads(input_json)
    except json.JSONDecodeError as error:
        # stdout is reserved for protocol JSON. Send the human-readable error
        # to stderr and use exit code 2, which tells VS Code to block on error.
        print(
            f"Stop received invalid JSON: {error}",
            file=sys.stderr,
        )
        return 2

    # The common hook payload includes hook_event_name. Use .get() so a
    # missing optional field produces a useful message instead of a traceback.
    event_name = hook_input.get("hook_event_name", "Stop")
    message = f"{event_name} received"

    # Return one compact JSON object on stdout. continue=true allows the agent
    # session to finish normally, while systemMessage provides hook context.
    response = {
        "continue": True,
        "systemMessage": message,
    }
    print(json.dumps(response, separators=(",", ":")))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
