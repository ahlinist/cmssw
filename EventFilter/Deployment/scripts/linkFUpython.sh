#! /bin/bash

ENVFILE=$1
TARGET=$2

[ -f "$ENVFILE" ] || {
    echo "ERROR: file $ENVFILE does not exist or cannot be read" 1>&2
    exit 1
}
grep -q "PYTHONPATH=" $ENVFILE || {
    echo "ERROR: file $ENVFILE does not define PYTHONPATH" 1>&2
    exit 1
}

mkdir -p $TARGET/home $TARGET/path

# traverse the PYTHONHOME defined in $ENVFILE in reverse order
for DIR in $( unset PYTHONHOME; source $ENVFILE; echo $PYTHONHOME | tr : \\n | tac ); do
    echo -e "\t************************************************"
    echo -e "\tPYTHONHOME part: $DIR"
    cp -r -s -f $DIR/* $TARGET/home/
    echo -e "\t************************************************"
done
# traverse the PYTHONPATH defined in $ENVFILE in reverse order
for DIR in $( unset PYTHONPATH; source $ENVFILE; echo $PYTHONPATH | tr : \\n | tac ); do
    echo -e "\t************************************************"
    echo -e "\tPYTHONPATH part: $DIR"
    cp -r -s -f $DIR/* $TARGET/path/
    echo -e "\t************************************************"
done
