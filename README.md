# db_concepts

A minimal C project starter optimized for GitHub Copilot and Visual Studio Code workflows.

## What is included

- a simple hello-world C program
- a public header and implementation split
- a test harness
- a Makefile-based build workflow
- Copilot-oriented repository metadata under `.github/`
- VS Code configuration under `.vscode/`
- runtime scratchpad and checkpoints under `.agent/`

## Quick start

```bash
make
make run
make test
```

## CLI commands

The application reads one command per line:

```text
read hello
hello
write hello world
hello world
quit
```

`read <string>` and `write <string>` echo their string argument. Enter `quit`
to end the session. End-of-file also exits successfully when input is piped or
closed.

## Project layout

```text
.
├── .github/
│   ├── copilot-instructions.md
│   ├── AGENTS.md
│   ├── MEMORY.md
│   ├── agents/
│   ├── instructions/
│   ├── memory/
│   ├── skills/
│   └── prompts/
├── .vscode/
│   ├── settings.json
│   ├── tasks.json
│   └── mcp.json
├── .agent/
│   ├── scratchpad.md
│   ├── checkpoints/
│   └── execution.log
├── docs/
│   ├── adr/
│   └── specs/
├── include/
│   ├── cli.h
│   └── hello.h
├── src/
│   ├── cli.c
│   ├── hello.c
│   └── main.c
├── tests/
│   ├── test_cli.c
│   └── test_hello.c
├── Makefile
├── .gitignore
├── README.md
└── LICENSE
```

## Best practices

- Keep public APIs in headers.
- Keep implementation separated from declarations.
- Keep tests focused on public behavior.
- Use `bin/` for generated output and exclude it from Git.
- Keep Copilot guidance explicit and repo-specific.

## License

This starter is intentionally lightweight and suitable for extension.
