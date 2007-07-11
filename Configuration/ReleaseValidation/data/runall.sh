#!/bin/bash

#
#$Id: runall.sh,v 1.4 2007/07/09 09:16:52 klute Exp $
#
#Dummy script to run all integration tests
#
#

tests="single_mu_pt_100_negative_allReco.cfg single_mu_pt_1_positive_allReco.cfg Zprime_Dijets_700_allReco.cfg b_jets_gen_rec_50_120_allReco.cfg c_jets_gen_rec_50_120_allReco.cfg single_e_pt_35_allReco.cfg single_gamma_pt35_allReco.cfg ttbar_gen_rec_allReco.cfg QCD_pt800_1000_allReco.cfg single_e_pt_35_allReco_L1.cfg single_mu_pt_100_positive_allReco_L1.cfg  ttbar_gen_rec_allReco_L1.cfg"

report=""

let nfail=0
let npass=0

echo "Tests to be run : " $tests

eval `scramv1 runtime -sh`

for file in $tests 
do
    echo Preparing to run $file
    let starttime=`date "+%s"`
    cmsRun $file
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
