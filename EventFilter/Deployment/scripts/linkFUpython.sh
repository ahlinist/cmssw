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

# makeLinksTo BASE DEST
function makeLinksTo() {
  local BASE="$1"
  local DEST="$2"

  for X in $(find $BASE); do
    T=$(echo $X | sed -e"s#^$BASE/#$DEST/#")
    if [ -e $T ]; then
      continue
    fi

    if [ -h $X ] && [ -f $X ]; then
      # symlink to a regular file
      ln -s $(readlink -f $X) $T
    elif [ -h $X ] && [ -d $X ]; then
      # symlinks to a directory
      mkdir -p $T
      makeLinksTo $(readlink -f $X) $T
    elif [ -f $X ]; then
      # regular file
      ln -s $X $T
    elif [ -d $X ]; then
      # directory
      mkdir $T
    else
      # unknown, make a symlink as default action
      echo "Unexpected object at $X"
      ln -s $X $T
    fi
  done

}

mkdir -p $TARGET/home $TARGET/path

# traverse the PYTHONHOME defined in $ENVFILE
for DIR in $( unset PYTHONHOME; source $ENVFILE; echo $PYTHONHOME | tr : \\n ); do
  makeLinksTo $DIR $TARGET/home
done

# traverse the PYTHONPATH defined in $ENVFILE
for DIR in $( unset PYTHONPATH; source $ENVFILE; echo $PYTHONPATH | tr : \\n ); do
  makeLinksTo $DIR $TARGET/path
done
