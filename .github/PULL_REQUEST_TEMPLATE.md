## Summary

<!-- One paragraph describing what this PR does and why. -->

## Related Issue

Closes #<!-- issue number -->

## Type of Change

- [ ] Bug fix (non-breaking change that fixes an issue)
- [ ] New feature (non-breaking change that adds functionality)
- [ ] Breaking change (fix or feature that would change existing behaviour)
- [ ] Refactor / code quality (no behaviour change)
- [ ] Documentation update
- [ ] CI / build system change

## Checklist

- [ ] I have read [CONTRIBUTING.md](CONTRIBUTING.md)
- [ ] The code follows the project's C99 style (passes `clang-format --dry-run --Werror`)
- [ ] New or changed logic is covered by unit tests in `tests/`
- [ ] All existing tests pass (`ctest --test-dir build --output-on-failure`)
- [ ] I have run `cppcheck` locally and resolved any new warnings
- [ ] I have updated the relevant documentation (README, header comments) if needed

## How to Test

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
ctest --test-dir build --output-on-failure -V
```

## Screenshots / Output (if applicable)

<!-- Paste relevant terminal output, before/after comparisons, etc. -->
