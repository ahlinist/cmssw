#!/bin/bash
# Script to submit Pflow reprocessing of testbeam data on batch system and copy results to CASTOR
if [ -z "$1" ]; then 
	echo usage: $0 energy
	exit
fi

WORKDIR=$PWD
echo Working directory: $WORKDIR
ENERGY=$1
RUND=/afs/cern.ch/user/b/ballin/scratch0/cmssw/src/RecoParticleFlow/PFAnalyses/test/

SCRIPT=$RUND"pflow_"$ENERGY"GeV.py"

echo Script is: $SCRIPT

OUTTREE="outputtree_"$ENERGY"GeV.root"
OUTPUT="reprocessed_"$ENERGY"GeV.root"
DESTD=/castor/cern.ch/user/b/ballin/tbv6/
LOG="log_"$ENERGY"GeV.txt"

echo Outputtree is: $OUTTREE
echo Reprocessed file is: $OUTPUT
echo Logfile is: $LOG

echo

cd /afs/cern.ch/user/b/ballin/scratch0/cmssw/src/
eval `scramv1 ru -sh`
cd $WORKDIR

echo Starting cmsRun
cmsRun $SCRIPT 
echo cmsRun complete.

echo Directory listing of $PWD
ls -lh

echo Copying files to castor...
rfcp $OUTTREE $DESTD$OUTTREE
rfcp $OUTPUT $DESTD$OUTPUT
cp log.txt $RUND"tbv6"$LOG

echo Done.
exit 0