#! /bin/bash

echo ./find
./find
echo

echo ./find .
./find .
echo

echo ./find dir1
./find dir1
echo

echo ./find ./dir1
./find ./dir1
echo

echo ./find ./dir1/dir2
./find ./dir1/dir2
echo

echo ./find ./dir4
./find ./dir4
echo

echo ./find -name foo
./find -name foo
echo

echo ./find -name foo -mmin +10
#Output depends on state of files.
./find -name foo -mmin +10
echo

echo ./find -inum 000
#Should display target not found.
./find -inum 000
echo

echo ./find -name alligator -exec cat
./find -name alligator -exec cat
echo

echo ./find -name alligator -exec rm
./find -name alligator -exec rm
echo

echo ./find -name bird -exec mv dir1/eagle
./find -name bird -exec mv dir1/eagle
echo
