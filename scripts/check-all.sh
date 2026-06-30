#!/usr/bin/env bash
# scripts/check-all.sh — Run the same checks that CI performs, locally.
# Useful before pushing to verify nothing will break.
#
# Usage:
#   ./scripts/check-all.sh
#
# Requirements: cmake, gcc/clang, cppcheck, clang-format, valgrind (optional)

set -euo pipefail

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

PASS=0
FAIL=0

section() { echo -e "\n${YELLOW}━━━ $1 ━━━${NC}"; }
pass()    { ((++PASS)); echo -e "  ${GREEN}✓${NC} $1"; }
fail()    { ((++FAIL)); echo -e "  ${RED}✗${NC} $1"; }

BUILD_DIR="build-check"

# --- 1. Format check ---
section "Format Check (clang-format)"
if command -v clang-format &>/dev/null; then
    if find src tests -name '*.c' -o -name '*.h' | xargs clang-format --dry-run --Werror 2>/dev/null; then
        pass "All files formatted correctly"
    else
        fail "Formatting issues detected — run: make format"
    fi
else
    echo "  (skipped — clang-format not installed)"
fi

# --- 2. Static analysis ---
section "Static Analysis (cppcheck)"
if command -v cppcheck &>/dev/null; then
    if cppcheck --enable=warning,style,performance \
                --error-exitcode=1 \
                --suppress=missingIncludeSystem \
                -I src src/ 2>&1 | tail -5; then
        pass "cppcheck passed"
    else
        fail "cppcheck found issues"
    fi
else
    echo "  (skipped — cppcheck not installed)"
fi

# --- 3. Build ---
section "Build (CMake Debug)"
cmake -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON >/dev/null 2>&1
if cmake --build "${BUILD_DIR}" --parallel 2>&1 | tail -3; then
    pass "Build succeeded"
else
    fail "Build failed"
    echo -e "${RED}Cannot continue without a successful build.${NC}"
    exit 1
fi

# --- 4. Unit tests ---
section "Unit Tests (CTest)"
if ctest --test-dir "${BUILD_DIR}" --output-on-failure -V 2>&1 | tail -10; then
    pass "All tests passed"
else
    fail "Some tests failed"
fi

# --- 5. Valgrind (optional) ---
section "Memory Check (Valgrind)"
if command -v valgrind &>/dev/null; then
    VFAIL=0
    for suite in library user librarian utility; do
        if ! valgrind --error-exitcode=1 --leak-check=full --quiet \
             "./${BUILD_DIR}/test_${suite}" >/dev/null 2>&1; then
            fail "valgrind: test_${suite}"
            VFAIL=1
        fi
    done
    if [ "${VFAIL}" -eq 0 ]; then
        pass "No memory errors detected"
    fi
else
    echo "  (skipped — valgrind not installed)"
fi

# --- 6. Smoke test ---
section "Integration Smoke Test"
if [ -x "./${BUILD_DIR}/library" ]; then
    if ./scripts/smoke-test.sh "./${BUILD_DIR}/library" 2>&1 | tail -5; then
        pass "Smoke test passed"
    else
        fail "Smoke test failed"
    fi
else
    echo "  (skipped — binary not found)"
fi

# --- Cleanup ---
rm -rf "${BUILD_DIR}"

# --- Summary ---
echo ""
TOTAL=$((PASS + FAIL))
echo -e "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo -e "Results: ${GREEN}${PASS}${NC}/${TOTAL} passed, ${RED}${FAIL}${NC} failed."
echo -e "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

if [ "${FAIL}" -gt 0 ]; then
    exit 1
fi
exit 0
