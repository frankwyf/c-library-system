# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [Unreleased]

### Added

- **`test_utility.c`** — 10 unit tests covering `util_trim_newline` edge cases.
- **5 edge-case tests** for `test_library.c` (single book, multiple blanks, no trailing newline, orphan author, Windows line endings).
- **5 edge-case tests** for `test_librarian.c` (long strings, empty strings, shift verification, add-remove-add cycle).
- **5 edge-case tests** for `test_user.c` (borrow-return-borrow, fill & drain, middle compaction, boundary indices).
- **CI: Windows** — build & test on `windows-latest` with MSVC.
- **CI: Code coverage** — gcov + lcov with Codecov upload.
- **CI: Valgrind** — full memory-leak check for all test suites.
- **CI: AddressSanitizer + UBSan** — runtime memory-error detection.
- **CI: clang-format** — formatting gate (dry-run `--Werror`).
- **CI: Dependabot** — auto-PR for GitHub Actions version bumps.
- **CI: Multi-platform release** — Linux + macOS + Windows binaries on version tags.
- **CMake options**: `ENABLE_COVERAGE`, `ENABLE_ASAN`, `ENABLE_UBSAN`.
- **Makefile targets**: `test`, `coverage`, `format`, `format-check`.
- **Dockerfile** — multi-stage (builder → test → runtime) with minimal footprint.
- **`.clang-format`** — LLVM-based style rules (4-space indent, 80-col limit).
- **`.editorconfig`** — cross-editor indent / charset / EOL settings.
- **`.dockerignore`** — lean Docker build context.
- **`codecov.yml`** — Codecov coverage thresholds and ignore rules.
- **`.github/dependabot.yml`** — weekly action-version update checks.
- **`.github/ISSUE_TEMPLATE/bug_report.yml`** — structured bug report form.
- **`.github/ISSUE_TEMPLATE/feature_request.yml`** — structured feature request form.
- **`.github/PULL_REQUEST_TEMPLATE.md`** — PR checklist.
- **`SECURITY.md`** — vulnerability disclosure policy.
- **`docs/RELEASE.md`** — release process guide for maintainers.
- **`scripts/install-hooks.sh`** — installs pre-commit clang-format hook.

### Changed

- `CONTRIBUTING.md` expanded with dev-environment table, coverage/ASan/hook instructions, and PR guidelines.
- `README.md` updated with Codecov badge, coverage section, and revised project tree.
- `clib.json` regenerated with full `src` listing and keyword metadata.

---

## [1.0.0] – 2026-06-30

### Added

- **Core library management system** with two roles: *User* and *Librarian*.
- User: browse available books, borrow (up to `MAX_BORROWED`, default 4), return books, view borrowed list.
- Librarian: view all/borrowed books, add new books, remove available books.
- `library_read_books()` — reads a plain-text book catalogue; robust against blank lines and Windows `\r\n` endings.
- `user_do_borrow()` / `user_do_return()` — non-interactive, unit-testable borrow/return logic.
- `librarian_do_add()` / `librarian_do_remove()` — non-interactive catalogue management.
- `util_read_index()` — validated, non-interactive integer input (rejects non-digit characters).
- **26 unit tests** across 3 modules using the [Unity](https://github.com/ThrowTheSwitch/Unity) C testing framework.
- CMake build system with `FetchContent` for Unity; no manual dependency installation required.
- GNU Makefile for Linux / macOS convenience builds (`make`, `make run`, `make clean`).
- **GitHub Actions CI/CD**: build + test on Ubuntu and macOS; `cppcheck` static analysis; automated binary release on version tags.
- MIT Licence.
- Documentation in **English**, **Chinese (简体中文)**, and **Japanese (日本語)**.

### Fixed (vs. original coursework)

- Renamed `available` field to `is_borrowed` (0 = available, 1 = borrowed) for clarity.
- Fixed off-by-one in `returnBook`: now validates `borrow_idx < num_borrowed` instead of `borrow_idx < maxBorrowed`.
- `library_free()` zeroes dangling `borrowed[]` pointers before freeing the catalogue.
- Library functions return `LIB_OK` / `LIB_ERR` instead of calling `exit()` directly.
- Removed all personally identifiable information (student IDs, names, dates) from source code.

[Unreleased]: https://github.com/frankwyf/c-library-system/compare/v1.0.0...HEAD
[1.0.0]: https://github.com/frankwyf/c-library-system/releases/tag/v1.0.0
