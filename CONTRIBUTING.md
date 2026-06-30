# Contributing to C Library Management System

Thank you for your interest in contributing!

## Getting Started

1. **Fork** the repository on GitHub.
2. **Clone** your fork: `git clone https://github.com/<your-username>/c-library-system.git`
3. **Create a branch**: `git checkout -b feature/my-new-feature`
4. **Write code** following the style guide below.
5. **Add or update tests** in `tests/` for any new or changed behaviour.
6. **Ensure all tests pass**: `ctest --test-dir build --output-on-failure`
7. **Run the formatter**: `make format` (or `clang-format -i src/*.c src/*.h tests/*.c`)
8. **Commit** with a clear message (see below).
9. **Push** and open a **Pull Request** targeting the `master` branch.

## Development Environment

| Tool | Required | Minimum version | Purpose |
|------|:--------:|-----------------|---------|
| GCC or Clang | ✓ | GCC ≥ 7 / Clang ≥ 6 | Compile |
| CMake | ✓ | ≥ 3.14 | Build system + tests |
| Git | ✓ | any | Version control |
| clang-format | Recommended | ≥ 14 | Code formatting |
| cppcheck | Recommended | ≥ 2.0 | Static analysis |
| lcov | Optional | ≥ 1.16 | Coverage reports |
| Valgrind | Optional | ≥ 3.18 | Memory-leak checking |

## Code style

| Rule | Detail |
|------|--------|
| Standard | C99 (`-std=c99`) |
| Naming | `snake_case` for all identifiers |
| Indentation | 4 spaces, no tabs |
| Line length | ≤ 80 characters (enforced by `.clang-format`) |
| Headers | Every public function documented with a brief comment in the `.h` file |
| Global state | No global mutable state outside test fixtures |
| Formatting | Must pass `clang-format --dry-run --Werror` (CI will check) |

> A `.clang-format` and `.editorconfig` are included — most editors will pick these up automatically.

## Commit message convention

Use [Conventional Commits](https://www.conventionalcommits.org/):

```
<type>(<scope>): <short summary in present tense>
```

Common types: `feat`, `fix`, `docs`, `test`, `refactor`, `ci`, `chore`.

Examples:

```
feat(user): add search by author
fix(user): off-by-one in user_do_return index check
docs: update Japanese README
test(librarian): add edge case for empty catalogue removal
ci: add Valgrind memory check job
```

## Running tests locally

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
ctest --test-dir build --output-on-failure -V
```

### With coverage report

```bash
make coverage
# → opens coverage-html/index.html
```

### With AddressSanitizer (GCC/Clang only)

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_ASAN=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

> **Windows note**: `tmpfile()` (used in `test_library.c`) may require administrator
> privileges on some Windows configurations. Run tests under WSL or Linux for best results.

## Running static analysis

```bash
cppcheck --enable=warning,style,performance \
         --suppress=missingIncludeSystem    \
         -I src src/
```

## Pre-commit hook (optional)

Install a Git hook that auto-formats staged files before committing:

```bash
./scripts/install-hooks.sh
```

## Reporting issues

Please open a [GitHub Issue](https://github.com/frankwyf/c-library-system/issues) using
one of the provided templates (bug report or feature request).

## Pull Request Guidelines

- One logical change per PR (small and focused is better).
- All CI checks must pass before merging.
- Squash trivial "fix typo" commits before requesting review.
- If your PR adds a public API function, include tests **and** update the relevant header comments.

## Licence

By contributing, you agree that your contributions will be licensed under the
[MIT License](LICENSE).
