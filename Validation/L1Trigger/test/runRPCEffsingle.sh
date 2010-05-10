#!/bin/sh
wdir=/afs/cern.ch/user/d/dbart/scratch0/CMSSW_3_6_0_pre4/src/Validation/L1Trigger/test/
destdir=/afs/cern.ch/user/d/dbart/scratch0/out900GeV/



file=$1
odir=$destdir/$2
mkdir -p $odir
num=$3
pyCfg=RPCTriggerEffTest.py

cd $wdir
eval `scramv1 ru -sh`
cd -
cp $wdir/$pyCfg ./
rfcp $file ./
bn=`basename $file`

echo "process.RPCTriggerVal.dev= cms.bool(False)" >> $pyCfg

echo "process.source.fileNames = cms.untracked.vstring(\"file:$bn\")" >> $pyCfg


cat $pyCfg
cmsRun $pyCfg


name=RPC_validation_$num.root
mv RPC_efficiency.root $name
cp $name  $odir
