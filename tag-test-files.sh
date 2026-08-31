#!/bin/bash
# Tag z/OS CCSID test files as UTF-8 after clone/checkout
# These files contain UTF-8 test data and must be tagged correctly

cd "$(dirname "$0")/rsync-3.5.0/testsuite"

echo "Tagging z/OS CCSID test files as UTF-8..."

chtag -tc UTF-8 zos-ccsid-conversion_test.py
chtag -tc UTF-8 zos-ccsid-performance_test.py  
chtag -tc UTF-8 zos-ccsid-regression_test.py

echo
echo "Verifying tags:"
ls -T zos-ccsid-*.py

echo
echo "â Done! Test files are now tagged as UTF-8"
