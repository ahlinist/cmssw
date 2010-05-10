#!/bin/sh

wdir=/afs/cern.ch/user/d/dbart/scratch0/CMSSW_3_6_0_pre4/src/Validation/L1Trigger/test/
#wdir=/afs/cern.ch/user/f/fruboes/scratch0/2010.03.GMTProblem/CMSSW_3_5_0/src/Validation/L1Trigger/test/

file=$1
odir=$wdir/$2
mkdir $odir
num=$3
pyCfg=RPCTriggerValidationTest.py

cd $wdir
eval `scramv1 ru -sh`
cd -
cp $wdir/$pyCfg ./
rfcp $file ./
bn=`basename $file`
echo "process.source.fileNames = cms.untracked.vstring(\"file:$bn\")" >> $pyCfg


cat $pyCfg
cmsRun $pyCfg


name=RPC_validation_$num.root
mv RPC_validation.root $name
cp $name  $odir


