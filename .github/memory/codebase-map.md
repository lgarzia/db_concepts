# Codebase map

## High-level structure

- `include/`: public headers and API contracts
- `src/`: implementation and executable entry point
- `tests/`: unit tests
- `docs/`: architecture guidance and decisions
- `.github/`: Copilot and agent guidance
- `.vscode/`: editor configuration
- `.agent/`: ephemeral operational state

## Module map

- `hello.h` defines the public function contract.
- `hello.c` implements the function.
- `main.c` wires the implementation into the console program.
- `test_hello.c` validates expected behavior.

## Build path

- `make` builds the application.
- `make test` builds and runs the test program.
- `make run` executes the main executable.
