#! bin/sh

touch ./testdir/dir1/foo
echo "This is ./testdir/dir1/foo." >> ./testdir/dir1/foo
touch ./testdir/dir1/dir2/foo
echo "This is ./testdir/dir1/dir2/foo." >> ./testdir/dir1/dir2/foo
touch ./testdir/dir3/foo
echo "This is ./testdir/dir3/foo." >> ./testdir/dir3/foo
touch ./testdir/foo
echo "This is ./testdir/foo." >> ./testdir/foo
