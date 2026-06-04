#!/bin/sh
# Validates repo health. Run at the start of a session to understand current state.
# Exits 0 if everything is clean, 1 if anything needs attention.
#
# Checks:
#   - All test suites pass (builds if needed)
#   - Generated files (uint16/uint32) are in sync with the uint8 template
#   - README version badge matches clib.json
#   - clib.json src lists all generated headers

set -e
status=0

check() {
    LABEL=$1
    shift
    if "$@" >/dev/null 2>&1; then
        printf "[ OK ] %s\n" "$LABEL"
    else
        printf "[FAIL] %s\n" "$LABEL"
        status=1
    fi
}

# --- Build and run tests ---
make test >/dev/null 2>&1 || true   # ensure binaries exist
check "cbuff_uint8 tests pass"  sh -c './cbuff_uint8_test  2>&1 | grep -q "ALL TESTS PASSED"'
check "cbuff_uint16 tests pass" sh -c './cbuff_uint16_test 2>&1 | grep -q "ALL TESTS PASSED"'
check "cbuff_uint32 tests pass" sh -c './cbuff_uint32_test 2>&1 | grep -q "ALL TESTS PASSED"'

# --- Generated files in sync with template ---
TMPDIR=$(mktemp -d)
cp cbuff_uint8.h cbuff_uint8_test.c generate.sh "$TMPDIR/"
(cd "$TMPDIR" && sh generate.sh >/dev/null 2>&1)

for f in cbuff_uint16.h cbuff_uint16_test.c cbuff_uint32.h cbuff_uint32_test.c; do
    check "$f in sync with template" diff "$f" "$TMPDIR/$f"
done
rm -rf "$TMPDIR"

# --- Stats table not stale ---
check "README stats table is current" python3 - << 'PYEOF'
import re, subprocess, sys
with open("README.md") as f:
    content = f.read()
m = re.search(r"<statsTable>(.*?)</statsTable>", content, re.DOTALL)
readme_table = m.group(1).strip() if m else ""
fresh = subprocess.run(["python3", "stats.py"], capture_output=True, text=True).stdout.strip()
sys.exit(0 if readme_table == fresh else 1)
PYEOF

# --- Version consistency ---
CLIB_VER=$(jq -r '.version' clib.json)
check "README version badge matches clib.json ($CLIB_VER)" \
    grep -q "version-${CLIB_VER}-blue" README.md

# --- clib.json src lists generated headers ---
check "clib.json includes cbuff_uint16.h" jq -e '.src | contains(["cbuff_uint16.h"])' clib.json
check "clib.json includes cbuff_uint32.h" jq -e '.src | contains(["cbuff_uint32.h"])' clib.json

echo ""
if [ $status -eq 0 ]; then
    echo "All checks passed."
else
    echo "Some checks failed — see [FAIL] lines above."
fi
exit $status
