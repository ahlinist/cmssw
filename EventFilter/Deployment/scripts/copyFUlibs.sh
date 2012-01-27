#! /bin/bash

ENVFILE=$1
TARGET=$2

[ -f "$ENVFILE" ] || {
    echo "ERROR: file $ENVFILE does not exist or cannot be read" 1>&2
    exit 1
}
grep -q "LD_LIBRARY_PATH=" $ENVFILE || {
    echo "ERROR: file $ENVFILE does not define LD_LIBRARY_PATH" 1>&2
    exit 1
}

mkdir -p $TARGET

# traverse the LD_LIBRARY_PATH defined in $ENVFILE in reverse order
for DIR in $( unset LD_LIBRARY_PATH; source $ENVFILE; echo $LD_LIBRARY_PATH | tr : \\n | tac ); do
    cp -r -s -f $DIR/* $TARGET/
done
