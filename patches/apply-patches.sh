#!/bin/bash
#
# apply-patches.sh - Apply all z/OS CCSID conversion fixes using filename-based patches
#
# Usage: ./apply-patches.sh [rsync-source-directory]
#

set -e  # Exit on error

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RSYNC_DIR="${1:-../rsync-3.5.0}"

echo "╔══════════════════════════════════════════════════════════════════════╗"
echo "║     z/OS rsync CCSID Conversion Fixes - Patch Application           ║"
echo "╚══════════════════════════════════════════════════════════════════════╝"
echo

# Verify we're in the right place
if [ ! -f "$RSYNC_DIR/receiver.c" ]; then
    echo "ERROR: Cannot find rsync source directory at: $RSYNC_DIR"
    echo "Usage: $0 [rsync-source-directory]"
    echo
    echo "Example:"
    echo "  cd /home/haritha/code/rsyncreview_dh_28aug/rsyncport/patches"
    echo "  ./apply-patches.sh ../rsync-3.5.0"
    exit 1
fi

cd "$RSYNC_DIR"
echo "Working directory: $(pwd)"
echo

# Define patches using filename-based names
PATCHES=(
    "receiver.c.patch"
    "hlink.c.patch"
    "rsyncfns.py.patch"
    "daemon-include-maxconn_test.py.patch"
    "msg-io-timeout-zero_test.py.patch"
    "zos-ccsid-conversion_test.py.patch"
    "zos-ccsid-performance_test.py.patch"
    "zos-ccsid-regression_test.py.patch"
    "run-ccsid-tests.sh.patch"
    "CCSID_TEST_SUITE.md.patch"
)

echo "═══════════════════════════════════════════════════════════════════"
echo "  Applying patches..."
echo "═══════════════════════════════════════════════════════════════════"
echo

APPLIED=0
FAILED=0

for patch in "${PATCHES[@]}"; do
    patch_file="$SCRIPT_DIR/$patch"
    
    if [ ! -f "$patch_file" ]; then
        echo "✗ ERROR: Patch file not found: $patch"
        FAILED=$((FAILED + 1))
        continue
    fi
    
    printf "Applying %-50s ... " "$patch"
    
    if patch -p1 < "$patch_file" > /tmp/patch.$$.log 2>&1; then
        echo "✓"
        APPLIED=$((APPLIED + 1))
    else
        echo "✗ FAILED"
        echo "  Error details:"
        cat /tmp/patch.$$.log | sed 's/^/    /'
        FAILED=$((FAILED + 1))
        rm -f /tmp/patch.$$.log
        
        # Ask user if they want to continue
        echo
        read -p "Continue with remaining patches? [y/N] " -n 1 -r
        echo
        if [[ ! $REPLY =~ ^[Yy]$ ]]; then
            echo "Aborted."
            exit 1
        fi
    fi
done

rm -f /tmp/patch.$$.log

echo
echo "═══════════════════════════════════════════════════════════════════"
echo "  Patch Application Summary"
echo "═══════════════════════════════════════════════════════════════════"
echo "  Applied: $APPLIED"
echo "  Failed:  $FAILED"
echo "  Total:   ${#PATCHES[@]}"
echo "═══════════════════════════════════════════════════════════════════"

if [ $FAILED -eq 0 ]; then
    echo
    echo "✓ All patches applied successfully!"
    echo
    
    # Set executable permissions on test runner
    if [ -f "testsuite/run-ccsid-tests.sh" ]; then
        chmod +x testsuite/run-ccsid-tests.sh
        echo "✓ Set executable permission on run-ccsid-tests.sh"
    fi
    
    echo
    echo "Files modified:"
    echo "  • receiver.c           - CCSID conversion fixes"
    echo "  • hlink.c              - Hard-link tag preservation"
    echo "  • testsuite/rsyncfns.py - Runtime capability check"
    echo "  • testsuite/daemon-include-maxconn_test.py"
    echo "  • testsuite/msg-io-timeout-zero_test.py"
    echo
    echo "Files created:"
    echo "  • testsuite/zos-ccsid-conversion_test.py"
    echo "  • testsuite/zos-ccsid-performance_test.py"
    echo "  • testsuite/zos-ccsid-regression_test.py"
    echo "  • testsuite/run-ccsid-tests.sh"
    echo "  • testsuite/CCSID_TEST_SUITE.md"
    echo
    echo "Next steps:"
    echo "  1. Build rsync:    make clean && ./configure && make"
    echo "  2. Run tests:      cd testsuite && ./run-ccsid-tests.sh quick"
    echo "  3. Full test:      ./run-ccsid-tests.sh full"
    echo
    exit 0
else
    echo
    echo "✗ Some patches failed to apply"
    echo
    echo "Check for .rej files:"
    echo "  find . -name '*.rej'"
    echo
    echo "To revert changes:"
    echo "  git checkout ."
    echo "  # or manually revert affected files"
    echo
    exit 1
fi
