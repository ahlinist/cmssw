#!/bin/tcsh
###################################################
# Install hint in Quaero
# Usage:  Quaero/utilities/installHint.tcsh <colliderRun> <experiment> <finalState> <sumPt>
#
# $1 = colliderRun
# $2 = experiment
# $3 = finalState
# $4 = sumPt
###################################################

setenv colliderRun $1
setenv experiment $2
setenv finalState $3
setenv sumPt $4
setenv expDir $RELEASE_DIR/Quaero/experiments/

if ( $4 != "" ) then
  mkdir $expDir/$colliderRun/hint/ >& /dev/null
  cd $expDir/$colliderRun/hint/
  rm meta*.txt >& /dev/null
  rm -r simulate >& /dev/null
  ln -sf ../$experiment/simulate .
  refine 0 $colliderRun $experiment ../$experiment/metaData.txt > metaData.txt
  refine 0 $colliderRun $experiment ../$experiment/metaBkg.txt > metaBkg.txt
  echo $finalState $sumPt > hint_specs.txt
  cd check; ./go; cd -;
else
  echo 'Usage:  $RELEASE_DIR/installHint.tcsh <colliderRun> <experiment> <finalState> <sumPt>'
endif

