# Release Process

This document describes how to cut a new release.

## Prerequisites

- You must have push access to the `master` branch.
- All CI checks on `master` must be green.
- The `CHANGELOG.md` must be up-to-date with the new version's changes.

## Steps

### 1. Update version numbers

Update the version string in these files:

| File | Field |
|------|-------|
| `CMakeLists.txt` | `project(... VERSION x.y.z ...)` |
| `clib.json` | `"version": "x.y.z"` |
| `CHANGELOG.md` | New `## [x.y.z]` section at the top |

### 2. Commit the version bump

```bash
git add CMakeLists.txt clib.json CHANGELOG.md
git commit -m "chore: release vX.Y.Z"
```

### 3. Tag the release

```bash
git tag -a vX.Y.Z -m "Release vX.Y.Z"
```

### 4. Push

```bash
git push origin master --tags
```

### 5. Wait for CI

The `release` job in `.github/workflows/ci.yml` will automatically:

1. Build release binaries for **Linux**, **macOS**, and **Windows**.
2. Create a GitHub Release with auto-generated release notes.
3. Upload platform-specific archives:
   - `c-library-system-vX.Y.Z-linux-x86_64.tar.gz`
   - `c-library-system-vX.Y.Z-macos.tar.gz`
   - `c-library-system-vX.Y.Z-windows-x64.zip`

### 6. Verify

- Check the [Releases page](https://github.com/frankwyf/c-library-system/releases) for the new entry.
- Download and smoke-test the binary on at least one platform.

## Versioning policy

This project follows [Semantic Versioning](https://semver.org/):

| Change type | Version bump | Example |
|-------------|:------------:|---------|
| Bug fix (backward-compatible) | PATCH | 1.0.0 → 1.0.1 |
| New feature (backward-compatible) | MINOR | 1.0.1 → 1.1.0 |
| Breaking change | MAJOR | 1.1.0 → 2.0.0 |

## Hotfix workflow

For urgent fixes against a released version:

```bash
git checkout -b hotfix/vX.Y.Z vX.Y.Z
# make fix, commit, tag vX.Y.(Z+1), push
git push origin hotfix/vX.Y.Z --tags
```

Then merge back into `master`.
