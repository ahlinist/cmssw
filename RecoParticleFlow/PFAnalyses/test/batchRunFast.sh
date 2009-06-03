#!/bin/bash
# Script to submit Pflow reprocessing of testbeam data on batch system and copy results to CASTOR

WORKDIR=$PWD
echo Working directory: $WORKDIR

RUND=/afs/cern.ch/user/b/ballin/scratch0/cmssw/src/RecoParticleFlow/PFAnalyses/test/

SCRIPT=$RUND"fastsim_neutrals.py"

echo Script is: $SCRIPT


OUTTREE="Dikaon_"$1"GeV_10k_fast.root"
OUTPUT="Dikaon_Events_"$1"GeV_10k_fast.root"
DESTD=/castor/cern.ch/user/b/ballin/tbv8/
LOG="Dikaon_"$1"GeV_10k_fast.txt"

echo Outputtree is: $OUTTREE
echo Reprocessed file is: $OUTPUT
echo Logfile is: $LOG

echo

cd /afs/cern.ch/user/b/ballin/scratch0/cmssw/src/
eval `scramv1 ru -sh`
cd $WORKDIR

echo Starting cmsRun
cmsRun $SCRIPT beamEnergy=$1 kevents=10 fileSuffix=fast
echo cmsRun complete.

echo Directory listing of $PWD
ls -lh

echo Copying files to castor...
rfcp $OUTTREE $DESTD$OUTTREE
rfcp $OUTPUT $DESTD$OUTPUT
cp *.txt $RUND"tbv8/"

echo Done.
exit 0
