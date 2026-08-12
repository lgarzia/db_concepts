# Incident triage skill

## Purpose

Use this skill when a project issue needs a clear, focused investigation and recovery path.

## Workflow

1. Reproduce the problem.
2. Identify the failing component.
3. Check the smallest relevant build/test target.
4. Apply the smallest root-cause fix.
5. Validate with `make test` or the specifically relevant command.

## Standard output

- concise root cause summary
- affected files
- validation step and result
- follow-up recommendation if needed
