#!/bin/sh
set -e
rm -f s_*.gz
./gzipsplit test/ab.img

if [ ! -f s_0.gz ]; then
       echo "file s_0.gz is not found" >&2
       exit 1
fi

if [ ! -f s_1.gz ]; then
	echo "file s_1.gz is not found" >&2
	exit 1
fi

if [ -f s_2.gz ]; then
	echo "extra file s_2.gz has been found" >&2
fi

zcat s_0.gz > s_0.txt
if ! cmp -s s_0.txt test/a.txt; then
	echo "1st file does not match expected test/a.txt" >&2
	exit 1
fi
zcat s_1.gz > s_1.txt

if ! cmp -s s_1.txt test/b.txt; then
	echo "1st file does not match expected test/b.txt" >&2
	exit 1
fi

exit 0
