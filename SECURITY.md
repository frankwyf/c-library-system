# Security Policy

## Supported Versions

| Version | Supported |
|---------|:---------:|
| Latest (`master`) | ✓ |
| Tagged releases | ✓ |
| Older releases | ✗ |

## Reporting a Vulnerability

**Please do not open a public GitHub issue for security vulnerabilities.**

If you discover a security vulnerability in this project, please report it
privately so we can address it before public disclosure.

### How to Report

1. Go to the **Security** tab of this repository.
2. Click **"Report a vulnerability"** to open a private advisory draft.
3. Fill in as much detail as possible:
   - Description of the vulnerability
   - Steps to reproduce (proof-of-concept if available)
   - Potential impact
   - Suggested fix (optional)

Alternatively, you may e-mail the maintainer directly (address listed in the
GitHub profile).

### Response Timeline

| Milestone | Target |
|-----------|--------|
| Acknowledgement | Within **48 hours** |
| Initial assessment | Within **7 days** |
| Fix / patch released | Within **30 days** (depending on severity) |
| Public disclosure | After the fix is released |

We follow responsible disclosure principles and will credit you in the release
notes unless you prefer to remain anonymous.

## Scope

This project is a **console application** with no network exposure, no
authentication subsystem, and no database backend.  The primary security
concerns are:

- **Buffer overflows** in C string handling (`strncpy` guard lengths, fixed
  array sizes in `structures.h`)
- **File-path injection** when passing a book-data file on the command line

Out-of-scope reports (e.g. theoretical issues with no practical exploit
path on a system where the user already has local code execution) will still
be reviewed, but fixes may be lower priority.
