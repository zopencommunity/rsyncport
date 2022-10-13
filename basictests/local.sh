#!/bin/sh
#
# Local test just copies files from <src> to <dst> locally using rsync
#
#set -x

if ! whence rsync >/dev/null ; then
  echo "Unable to find rsync - needs to be in your PATH" >&2
  exit 4
fi

#
# Create some files in /tmp/rsync/src
#
testdir="/tmp/rsync$$"

if rm -rf "${testdir}" && mkdir "${testdir}" && mkdir "${testdir}/src" && mkdir "${testdir}/dst" && mkdir "${testdir}/log"; then
  :
else
  echo "Unable to set up test directories under $testdir" >&2
  exit 4
fi

if echo "File f1 text" >"${testdir}/src/f1.txt" && echo "File f2 text" >"${testdir}/src/f2.txt" ; then
  :
else 
  echo "Unable to create f1.txt and f2.txt" >&2
fi


#
# Copy from src to dst with rsync
#
if ! rsync -vvvv -r "${testdir}/src/" "${testdir}/dst/" >"${testdir}/log/basic.out.log" 2>"${testdir}/log/basic.err.log" ; then
  echo "rsync failed. output in ${testdir}/log/basic.*.log" >&2
  exit 4
fi
if ! diff "${testdir}/src" "${testdir}/dst" ; then
  echo "${testdir}/src and ${testdir}/dst should be identical but they are not" >&2 
  exit 4
fi

#
# Cleanup
#
rm -rf "${testdir}"
