#!/bin/bash

# Simple script to loop over a CASTOR directory and print the bad files to
# stdout.  Some progress info is printed to stderr.
# You can pipe the output to "xargs -n 1 rfrm" to directly delete the
# files.  The safe option is to write it to a temporary file that you can check.
#
# Author: Evan K. Friis (UC Davis)
# 
# Examples:
# 
# ./printBadCastorFiles.sh | xargs -n 1 rfrm
#
# Safer:
# ./printBadCastorFiles.sh > badfiles.list 
# after checking badfiles.list is okay,
# cat badfiles.list | xargs -n 1 rfrm


function checker {
  echo Testing file: $1 >&2
  # Can add additional errors to grep for by added  -e "myerrorregex"
  clean=`edmFileUtil rfio:$1 | grep -e "FatalRootError"`
  if [[ "$clean" != "" ]]
  then
    echo $1 is bad! >&2
    echo $1 
  fi
}

COUNTER=0
for file in `nsls $1`
do
  # Background the check
  checker $1/$file &
  let COUNTER=COUNTER+1
  # If counter is 50, wait until current checks finish
  if [ $COUNTER -gt 50 ]; 
  then
    echo "Waiting to submit more requests..." >&2
    wait
    COUNTER=0
  fi
done
