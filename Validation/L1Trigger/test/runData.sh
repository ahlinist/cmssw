#! /bin/bash

wdir=/afs/cern.ch/user/d/dbart/scratch0/CMSSW_3_6_0_pre4/src/Validation/L1Trigger/test/

pyCfg=RPCTriggerEffTest.py
castorBase="/castor/cern.ch/cms"
release=*
#release=CMSSW_3_5_0_pre5
#release=CMSSW_3_5_4


dataset="/MinimumBias/Commissioning10-Muon_skim-v9/RAW-RECO"
#dataset="/MinimumBias/Commissioning10-PromptReco-v9/RECO"

pwd=`pwd`
cd $wdir
eval `scramv1 ru -sh`
cd -



OK=1
for (( ; ; ))
do
echo $@
for l in $@ ; do

query="find file where run = $l and release = $release and dataset = $dataset and dataset.status like VALID*"

for f in `python $DBSCMD_HOME/dbsCommandLine.py --noheader -c search --query="$query"`; do
   
   status=`stager_qry -M $castorBase$f | awk '{print $3}'`
   if [  "$status" == "STAGED" ]; then
     echo  $f OK

     
   else 
     stager_get -M $castorBase$f
     echo \# $f has wrong castor status - \"$status\"
	let OK=0
   fi
done
done
    if [ $OK == 1 ]; then
     break
    else
   sleep 30
    let OK=1
    fi

done


let i=0


for l in $@ ; do

query="find file where run = $l and release = $release and dataset = * and dataset = $dataset and dataset.status like VALID*"

for f in `python $DBSCMD_HOME/dbsCommandLine.py --noheader -c search --query="$query"`; do
   echo "####################" 
   status=`stager_qry -M $castorBase/$f | awk '{print $3}'`
   if [  "$status" == "STAGED" ]; then
     bsub -q cmscaf1nh runRPCEffsingle.sh $castorBase/$f $dataset $i
     let i+=1
   else 

     echo \# $f has wrong castor status - \"$status\"
   fi
done

done
