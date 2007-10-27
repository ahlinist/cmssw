#!/bin/bash

#
#$Id: runall.sh,v 1.10 2007/10/15 20:11:13 gutsche Exp $
#
# Script to run all integration tests
#
#

tests=`cat RelVal.txt | grep -v '#'`

report=""

let nfail=0
let npass=0

echo "Tests to be run RelVal preparation and:" $tests

eval `scramv1 runtime -sh`

# Test Pickling Of Relval Parameter-Sets
echo "Preparing to run RelVal preparation"
let starttime=`date "+%s"`
python2.4 prepareRelVal.py --samples RelVal_runall.txt --cms-path dummy 
let exitcode=$?

let endtime=`date "+%s"`
let tottime=$endtime-$starttime;   

if [ $exitcode -ne 0 ] ;then
  echo "RelVal preparation FAILED - time: $tottime s - exit: $exitcode"
  report="$report \n RelVal preparation FAILED  - time: $tottime s - exit: $exitcode"
  let nfail+=1
else 
  echo "RelVal preparation PASSED - time: $tottime s"
  report="$report \n RelVal preparation PASSED  - time: $tottime s"
  let npass+=1
fi 

for file in $tests 
do
    echo Preparing to run $file
    logfile=$file".log"
    let starttime=`date "+%s"`
    cmsRun $file > $logfile 2>&1
    let exitcode=$?

    let endtime=`date "+%s"`
    let tottime=$endtime-$starttime;   

    if [ $exitcode -ne 0 ] ;then
      echo "cmsRun $file : FAILED - time: $tottime s - exit: $exitcode"
      report="$report \n cmsRun $file : FAILED  - time: $tottime s - exit: $exitcode"
      let nfail+=1
    else 
      echo "cmsRun $file : PASSED - time: $tottime s"
      report="$report \n cmsRun $file : PASSED  - time: $tottime s"
      let npass+=1
    fi 
done


report="$report \n \n $npass tests passed, $nfail failed \n"

echo -e "$report" 
rm -f runall-report.log
echo -e "$report" >& runall-report.log
