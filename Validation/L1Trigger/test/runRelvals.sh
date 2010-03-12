#! /bin/bash
# execute eval scram before using this

#release=CMSSW_3_5_0_pre1
#release=CMSSW_3_5_0_pre5
release=CMSSW_3_5_4
like=RelValSingleMuPt100
globalTag=""

# give globaltag or leave empty
#globalTag=MC_3XY_V20-v1 
#globalTag=MC_3XY_V20-v2 
query="find file where phygrp = RelVal and release = $release and dataset like /$like/*$globalTag/GEN-SIM-DIGI-RAW-HLTDEBUG and dataset.status like VALID*"



let i=0 
castorBase="/castor/cern.ch/cms/"
for f in `python $DBSCMD_HOME/dbsCommandLine.py --noheader -c search --query="$query"`; do
   echo "####################" 
   status=`stager_qry -M $castorBase/$f | awk '{print $3}'`
   if [  "$status" == "STAGED" ]; then
     echo Running $f
     bsub -q 8nm runsingle.sh $castorBase/$f ${release}_$globalTag $i
     let i+=1
   else 
#     stager_get -M $castorBase/$f
     echo \# $f has wrong castor status - \"$status\"
   fi
done
