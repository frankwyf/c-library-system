#!/usr/bin/env bash
# scripts/smoke-test.sh — Integration smoke test for the compiled binary.
# Launches the program with piped input and verifies expected output keywords.
#
# Usage:
#   ./scripts/smoke-test.sh [path-to-binary]
#
# Exit codes:
#   0  All checks passed
#   1  One or more checks failed

set -euo pipefail

BIN="${1:-./build/library}"
DATA="data/books.txt"
PASS=0
FAIL=0

pass() { ((++PASS)); echo "  ✓ $1"; }
fail() { ((++FAIL)); echo "  ✗ $1"; }

echo "=== Smoke Test: ${BIN} ==="
echo ""

# --- Test 1: No arguments → usage error ---
echo "[Test 1] No arguments → usage error"
OUTPUT=$(${BIN} 2>&1 || true)
if echo "${OUTPUT}" | grep -qi "usage"; then
    pass "Prints usage message"
else
    fail "Expected usage message"
fi

# --- Test 2: Invalid file → error message ---
echo "[Test 2] Invalid book file → error"
OUTPUT=$(${BIN} __nonexistent_file__.txt 2>&1 || true)
if echo "${OUTPUT}" | grep -qi "error\|cannot"; then
    pass "Reports file-open error"
else
    fail "Expected error for missing file"
fi

# --- Test 3: Normal startup + immediate exit ---
echo "[Test 3] Startup → Main menu → Exit"
OUTPUT=$(printf '3\n' | ${BIN} "${DATA}" 2>&1)
if echo "${OUTPUT}" | grep -qi "library management"; then
    pass "Shows banner"
else
    fail "Expected banner text"
fi
if echo "${OUTPUT}" | grep -qi "goodbye"; then
    pass "Exits cleanly"
else
    fail "Expected goodbye message"
fi

# --- Test 4: User login → list available → logout → exit ---
echo "[Test 4] User session: list available books"
# User menu: 1=view available, 5=logout (varies by impl); for this app:
# user_cli menu: 1=available, 2=borrow, 3=return, 4=my books, 5=logout
INPUT=$(printf '1\n1\n5\n3\n')
OUTPUT=$(echo "${INPUT}" | ${BIN} "${DATA}" 2>&1)
if echo "${OUTPUT}" | grep -qi "frank herbert\|isaac asimov\|available"; then
    pass "Lists books in user session"
else
    fail "Expected book listing output"
fi

# --- Test 5: Librarian login → list all → logout → exit ---
echo "[Test 5] Librarian session: list all books"
# librarian_cli menu: 1=all, 2=borrowed, 3=add, 4=remove, 5=logout
INPUT=$(printf '2\n1\n5\n3\n')
OUTPUT=$(echo "${INPUT}" | ${BIN} "${DATA}" 2>&1)
if echo "${OUTPUT}" | grep -qi "frank herbert\|dune"; then
    pass "Lists catalogue in librarian session"
else
    fail "Expected catalogue listing"
fi

# --- Test 6: User borrow and return flow ---
echo "[Test 6] Borrow and return a book"
# Login as user, view available (1), borrow index 0 (2, then 0),
# view my books (4), return index 0 (3, then 0), logout (5), exit (3)
INPUT=$(printf '1\n2\n0\n4\n3\n0\n5\n3\n')
OUTPUT=$(echo "${INPUT}" | ${BIN} "${DATA}" 2>&1)
if echo "${OUTPUT}" | grep -qi "borrow\|return\|borrowed"; then
    pass "Borrow/return flow works"
else
    fail "Expected borrow/return output"
fi

# --- Summary ---
echo ""
TOTAL=$((PASS + FAIL))
echo "Results: ${PASS}/${TOTAL} passed, ${FAIL} failed."
if [ "${FAIL}" -gt 0 ]; then
    exit 1
fi
exit 0
