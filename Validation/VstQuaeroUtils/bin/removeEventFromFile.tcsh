#!/bin/tcsh
################################################################
# remove a particular event from a file
################################################################


setenv eventType $1
setenv runNumber $2
setenv f $3

if ( "$3" == "" ) then
  echo 'Usage:  removeEventFromFile.tcsh <eventType> <runNumber> <file>'
  exit
endif

cat $f | egrep -v '^'$1' +'$2' ' > tmp_$$.txt
mv tmp_$$.txt $f

