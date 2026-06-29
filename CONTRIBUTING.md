# Contributing to C Library Management System

Thank you for your interest in contributing!

## How to contribute

1. **Fork** the repository on GitHub.
2. **Clone** your fork: `git clone https://github.com/<your-username>/c-library-system.git`
3. **Create a branch**: `git checkout -b feature/my-new-feature`
4. **Write code** following the style guide below.
5. **Add or update tests** in `tests/` for any new or changed behaviour.
6. **Ensure all tests pass**: `ctest --test-dir build --output-on-failure`
7. **Commit** with a clear message (see below).
8. **Push** and open a **Pull Request** targeting the `main` branch.

## Code style

| Rule | Detail |
|------|--------|
| Standard | C99 (`-std=c99`) |
| Naming | `snake_case` for all identifiers |
| Indentation | 4 spaces, no tabs |
| Line length | ≤ 100 characters |
| Headers | Every public function documented with a brief comment in the `.h` file |
| Global state | No global mutable state outside test fixtures |

## Commit message convention

Use [Conventional Commits](https://www.conventionalcommits.org/):

```
<type>: <short summary in present tense>
```

Common types: `feat`, `fix`, `docs`, `test`, `refactor`, `chore`.

Examples:

```
feat: add search by author
fix: off-by-one in user_do_return index check
docs: update Japanese README
test: add edge case for empty catalogue removal
```

## Running tests locally

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure -V
```

> **Windows note**: `tmpfile()` (used in `test_library.c`) may require administrator
> privileges on some Windows configurations. Run tests under WSL or Linux for best results.

## Running static analysis

```bash
cppcheck --enable=warning,style,performance \
         --suppress=missingIncludeSystem    \
         -I src src/
```

## Reporting issues

Please open a [GitHub Issue](https://github.com/frankwyf/c-library-system/issues) and include:

- Your OS and compiler version (`gcc --version`)
- Steps to reproduce the problem
- Expected behaviour vs. actual behaviour

## Licence

By contributing, you agree that your contributions will be licensed under the
[MIT License](LICENSE).
