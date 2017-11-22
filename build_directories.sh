#! bin/sh

mkdir testdir
cd testdir
touch test1
echo "This is test1." >> test1
touch test2
echo "This is test2." >> test2
mkdir dir1
mkdir dir3
cd dir1
touch test3
echo "This is test3." >> test3
touch test4
echo "This is test4." >> test4
mkdir dir2
cd dir2
touch test5
echo "This is test5." >> test5
touch test6
echo "This is test6." >> test6
cd ..
cd ..
cd dir3
touch test7
echo "This is test7." >> test7
touch test8
echo "This is test8." >> test8
cd ..
cd ..
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
