#! /bin/bash
# execute eval scram before using this
wdir=/afs/cern.ch/user/d/dbart/scratch0/CMSSW_3_6_0_pre4/src/Validation/L1Trigger/test/
destdir=/afs/cern.ch/user/d/dbart/scratch0/Out598-606newMaching/
pyCfg=RPCTriggerEffTest.py
castorBase="/castor/cern.ch/cms"
release=*
#release=CMSSW_3_5_0_pre5
#release=CMSSW_3_5_4

#dataset="/StreamExpress/Commissioning10-RpcCalHLT-v7/ALCARECO"
#/MinimumBias/Commissioning10-PromptReco-v8/RECO //132605
#/ZeroBias/Commissioning10-Apr1ReReco-v2/RECO //132440
#/MinimumBias/Commissioning10-PromptReco-v8/RECO  //132592
#/MinimumBias/Commissioning10-PromptReco-v7/RECO //132440
dataset="/MinimumBias/Commissioning10-Muon_skim-v8/RAW-RECO"
#dataset1="=/MinimumBias/Commissioning10-Muon_skim-v8/RAW-RECO"
pwd=`pwd`
cd $wdir
eval `scramv1 ru -sh`
cd -
cp $wdir$pyCfg $pyCfg







castorBase="/castor/cern.ch/cms/"

OK=1
for (( ; ; ))
do
echo $@
for l in $@ ; do

query="find file where run = $l and release = $release and dataset = $dataset and dataset.status like VALID*"

for f in `python $DBSCMD_HOME/dbsCommandLine.py --noheader -c search --query="$query"`; do
   
   status=`stager_qry -M $castorBase/$f | awk '{print $3}'`
   if [  "$status" == "STAGED" ]; then
     echo  $f OK

     
   else 
     stager_get -M $castorBase/$f
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










echo "readFiles = cms.untracked.vstring()">> $pyCfg
echo "secFiles = cms.untracked.vstring()">> $pyCfg

echo "process.source = cms.Source ('PoolSource', fileNames=readFiles, secondaryFileNames=secFiles)">> $pyCfg
echo "readFiles.extend( [">> $pyCfg
coma=""
let i=0 

for l in $@ ; do

query="find file where run = $l and release = $release and dataset = * and dataset = $dataset and dataset.status like VALID*"

for f in `python $DBSCMD_HOME/dbsCommandLine.py --noheader -c search --query="$query"`; do
   echo "####################" 
   status=`stager_qry -M $castorBase/$f | awk '{print $3}'`
   if [  "$status" == "STAGED" ]; then
     echo  
	echo $coma\'$f\' >> $pyCfg
     coma=","
     let i+=1
	echo $i
 	if [ $i -gt 250 ] ; then
 	echo "])" >> $pyCfg
 	echo "secFiles.extend( [">> $pyCfg
	coma=""
	let i=0
 	fi
   else 
#     stager_get -M $castorBase/$f
     echo \# $f has wrong castor status - \"$status\"
   fi
done
# 	if [ $i > 250 ] ; then
# 	break
# 	fi

done



echo "])" >> $pyCfg
cat $pyCfg

if [ $i>0 ]; then 
cmsRun $pyCfg
echo "OK"

else
echo "NO File"
exit
fi

#outdir=$destdir`date +%d_%m_%y_Run`$1
outdir=$destdir$1$dataset
mkdir -p $outdir

#for r in `ls |grep root`;do
cp RPC_efficiency.root $outdir
#done
cp log.log $outdir
#rfcp Out.root $CASTOR_HOME
