#!/bin/bash
# Script to submit Pflow reprocessing of testbeam data on batch system and copy results to CASTOR
if [ -z "$1" ]; then 
	echo usage: $0 energy
	exit
fi

WORKDIR=$PWD
echo Working directory: $WORKDIR
ENERGY=$1
SUFFIX=$2
RUND=/afs/cern.ch/user/b/ballin/scratch0/cmssw/src/RecoParticleFlow/PFAnalyses/test/

SCRIPT=$RUND"pflow_tb_general.py"

echo Script is: $SCRIPT

OUTTREE="outputtree_"$ENERGY"GeV"$SUFFIX".root"
OUTPUT="reprocessed_"$ENERGY"GeV"$SUFFIX".root"
DESTD=/castor/cern.ch/user/b/ballin/tbv7D/
LOG="log_"$ENERGY"GeV_"$SUFFIX".txt"

echo Outputtree is: $OUTTREE
echo Reprocessed file is: $OUTPUT
echo Logfile is: $LOG

echo

cd /afs/cern.ch/user/b/ballin/scratch0/cmssw/src/
eval `scramv1 ru -sh`
cd $WORKDIR

echo Starting cmsRun
cmsRun $SCRIPT beamEnergy=$ENERGY fileSuffix=$SUFFIX kevents=0 notracks=0
echo cmsRun complete.

echo Directory listing of $PWD
ls -lh

echo Copying files to castor...
rfcp $OUTTREE $DESTD$OUTTREE
rfcp $OUTPUT $DESTD$OUTPUT
cp $LOG $RUND"tbv7D"

echo Done.
exit 0
