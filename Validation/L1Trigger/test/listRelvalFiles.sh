#! /bin/bash
# execute eval scram before using this

release=CMSSW_3_5_0_pre1
#release=CMSSW_3_5_4
#release=CMSSW_3_5_0
like="RelValSingleMuPt10 RelValSingleMuPt100 RelValSingleMuPt1000"

# give globaltag or leave empty
#globalTag=MC_3XY_V20-v1 
#globalTag=MC_3XY_V20-v2 
globalTag=""

for l in $like; do
query="find file where phygrp = RelVal and release = $release and dataset like /$l/*$globalTag/GEN-SIM-DIGI-RAW-HLTDEBUG and dataset.status like VALID*"
castorBase="/castor/cern.ch/cms"
coma=""
for f in `python $DBSCMD_HOME/dbsCommandLine.py --noheader -c search --query="$query"`; do
   status=`stager_qry -M $castorBase/$f | awk '{print $3}'`
   if [  "$status" == "STAGED" ]; then
     echo $coma\'$f\' 
     coma=","
     #rfcp $castorBase$f /tmp/fruboes/350RelVals/
   else 
     stager_get -M $castorBase/$f
     echo \# $f has wrong castor status - \"$status\"
   fi
done
done