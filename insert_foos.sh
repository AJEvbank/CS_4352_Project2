#! bin/sh

touch ./testdir/dir1/foo
echo "This is ./testdir/dir1/foo." >> ./testdir/dir1/foo
touch ./testdir/dir1/dir2/foo
echo "This is ./testdir/dir1/dir2/foo." >> ./testdir/dir1/dir2/foo
touch ./testdir/dir3/foo
echo "This is ./testdir/dir3/foo." >> ./testdir/dir3/foo
touch ./testdir/foo
echo "This is ./testdir/foo." >> ./testdir/foo
touch ./testdir/bird
echo "This is the bird file." >> ./testdir/bird
touch ./testdir/dir1/dir2/dog
echo "This is the dog file." >> ./testdir/dir1/dir2/dog
echo "This is the alligator file." >> ./testdir/alligator
