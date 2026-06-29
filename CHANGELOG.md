# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

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

[1.0.0]: https://github.com/frankwyf/c-library-system/releases/tag/v1.0.0
