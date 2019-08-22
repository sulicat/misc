#!/bin/sh

# the first argument is the program, then the other arguments are args passed to GDB as program arguments
if [ "$#" -le 0 ]; then
    echo "ლ(ಠ_ಠლ)"
fi

PROGRAM=$1
ARGS=""

i=0
for var in "$@"
do
    if [ $i -ne 0 ]; then
	ARGS="$ARGS $var"
    fi
    i+=1
done

dir=$(pwd)
PROGRAM="$dir/$PROGRAM"
COMMAND="gdb -i=mi -cd $dir --args $PROGRAM $ARGS"
echo ""
echo $COMMAND

cd pwd & emacs --eval "(gdb \"$COMMAND\")"
