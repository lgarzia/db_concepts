# C coding instructions

Apply these rules to all C files in this repository.

- Use C99-compatible constructs.
- Use `snake_case` naming for functions and variables.
- Use include guards in all headers.
- Prefer `const` where data is not modified.
- Avoid hidden state and unnecessary globals.
- Keep functions focused and easy to reason about.
- Use small helper functions rather than large monolithic blocks.
- Prefer explicit return values and simple behavior.

When implementing a new feature:

1. declare the API in the header,
2. implement it in a source file,
3. add or update tests,
4. run `make test`.
