#!/bin/sh

PATH=/bin:/usr/bin
WORKDIR=/tmp/coen175.$LOGNAME

function die() {
    rm -rf $WORKDIR
    exit 1
}

trap die 2

if [ $# -ne 2 ]; then
    echo "$0 submission-file examples-file" 1>&2
    exit 1
fi

rm -rf $WORKDIR && mkdir -m 700 $WORKDIR && mkdir $WORKDIR/my_examples || die

echo "Checking environment ..."
hostname -f | grep 'linux.*.engr.scu.edu' >/dev/null ||
    { echo "Must be run on an ECC Linux system" 1>&2; die; }

echo "Checking submission ..."
test -r $1 && test `wc -c < $1` -gt 1000000 \
    && echo "Submission too large" 1>&2 && die

echo "Extracting submission ..."
tar -C $WORKDIR -xf $1 || die

echo "Compiling project ..."
(cd $WORKDIR && cd phase2 && rm -f *.o scc core && make) || die

echo "Extracting examples ..."
tar -C $WORKDIR -xf $2 || die

echo "Running examples ..."
exec 3> CHECKSUB.diff

(cd $WORKDIR/my_examples && for FILE in *.c; do
    echo -n "$FILE ... "
    echo "$FILE ..." 1>&3
    (ulimit -t 1; ../phase2/scc) < $FILE 2>/dev/null |
	cmp - `basename $FILE .c`.out 1>&3 2>/dev/null && echo ok || echo failed
done)

rm -rf $WORKDIR
exit 0
