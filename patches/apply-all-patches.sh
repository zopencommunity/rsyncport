#!/bin/bash
#
# apply-all-patches.sh - Apply all z/OS CCSID conversion fixes
#
# Usage: ./apply-all-patches.sh [--check-only]
#

set -e  # Exit on error

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RSYNC_DIR="${1:-../rsync-3.5.0}"
CHECK_ONLY=0

# Parse arguments
if [ "$1" = "--check-only" ]; then
    CHECK_ONLY=1
    RSYNC_DIR="${2:-../rsync-3.5.0}"
fi

echo "╔══════════════════════════════════════════════════════════════════════╗"
echo "║     z/OS rsync CCSID Conversion Fixes - Patch Application Tool      ║"
echo "╚══════════════════════════════════════════════════════════════════════╝"
echo

# Verify we're in the right place
if [ ! -f "$RSYNC_DIR/receiver.c" ]; then
    echo "ERROR: Cannot find rsync source directory at: $RSYNC_DIR"
    echo "Usage: $0 [rsync-source-directory]"
    exit 1
fi

cd "$RSYNC_DIR"
echo "Working directory: $(pwd)"
echo

# Define patches in dependency order
PATCHES=(
    "01-receiver-ccsid-fixes.patch"
    "02-hlink-tag-preservation.patch"
    "03-rsyncfns-capability-check.patch"
    "04-daemon-maxconn-capability-check.patch"
    "05-msg-io-timeout-capability-check.patch"
    "new-zos-ccsid-conversion_test.py.patch"
    "new-zos-ccsid-performance_test.py.patch"
    "new-zos-ccsid-regression_test.py.patch"
    "new-run-ccsid-tests.sh.patch"
)

# Check mode - verify patches without applying
if [ $CHECK_ONLY -eq 1 ]; then
    echo "═══════════════════════════════════════════════════════════════════"
    echo "  CHECK MODE - Verifying patches (not applying)"
    echo "═══════════════════════════════════════════════════════════════════"
    echo
    
    FAILURES=0
    for patch in "${PATCHES[@]}"; do
        patch_file="$SCRIPT_DIR/$patch"
        if [ ! -f "$patch_file" ]; then
            echo "✗ MISSING: $patch"
            FAILURES=$((FAILURES + 1))
            continue
        fi
        
        printf "Checking %-50s ... " "$patch"
        if patch -p1 --dry-run --silent < "$patch_file" 2>/dev/null; then
            echo "✓ OK"
        else
            echo "✗ FAIL"
            FAILURES=$((FAILURES + 1))
        fi
    done
    
    echo
    if [ $FAILURES -eq 0 ]; then
        echo "✓ All patches can be applied cleanly"
        echo
        echo "To apply patches, run:"
        echo "  $0 $RSYNC_DIR"
        exit 0
    else
        echo "✗ $FAILURES patch(es) failed verification"
        echo
        echo "Possible causes:"
        echo "  - Patches already applied"
        echo "  - Local modifications conflict"
        echo "  - Wrong rsync version"
        exit 1
    fi
fi

# Apply mode
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
