#!/bin/bash
# Fix UTF-8 encoding for z/OS CCSID test files
# Run this after pulling on z/OS systems where tests fail with encoding errors

set -e

cd "$(dirname "$0")"

echo "================================================================"
echo "Fixing UTF-8 encoding for z/OS CCSID test files"
echo "================================================================"
echo

echo "Step 1: Pulling latest .gitattributes..."
git pull origin bump-rsync-version-3.5.0

echo
echo "Step 2: Removing corrupted Python files..."
rm -f rsync-3.5.0/testsuite/zos-ccsid-conversion_test.py
rm -f rsync-3.5.0/testsuite/zos-ccsid-performance_test.py
rm -f rsync-3.5.0/testsuite/zos-ccsid-regression_test.py

echo
echo "Step 3: Re-checking out files in UTF-8 (binary mode)..."
git checkout HEAD -- rsync-3.5.0/testsuite/zos-ccsid-conversion_test.py
git checkout HEAD -- rsync-3.5.0/testsuite/zos-ccsid-performance_test.py
git checkout HEAD -- rsync-3.5.0/testsuite/zos-ccsid-regression_test.py

echo
echo "Step 4: Tagging files as UTF-8..."
chtag -t -c 1208 rsync-3.5.0/testsuite/zos-ccsid-conversion_test.py
chtag -t -c 1208 rsync-3.5.0/testsuite/zos-ccsid-performance_test.py
chtag -t -c 1208 rsync-3.5.0/testsuite/zos-ccsid-regression_test.py

echo
echo "Step 5: Verifying file tags..."
ls -T rsync-3.5.0/testsuite/zos-ccsid-*.py

echo
echo "Step 6: Verifying UTF-8 encoding..."
file -i rsync-3.5.0/testsuite/zos-ccsid-*.py

echo
echo "================================================================"
echo "✓ Done! Files are now in UTF-8 encoding."
echo "================================================================"
echo
echo "To run tests:"
echo "  cd rsync-3.5.0"
echo "  make check"
echo
