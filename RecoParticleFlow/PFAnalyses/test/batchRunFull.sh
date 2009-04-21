#!/bin/bash
# Script to submit Pflow reprocessing of testbeam data on batch system and copy results to CASTOR

WORKDIR=$PWD
echo Working directory: $WORKDIR

RUND=/afs/cern.ch/user/b/ballin/scratch0/cmssw/src/RecoParticleFlow/PFAnalyses/test/

SCRIPT=$RUND"fullsim_neutrals.py"

echo Script is: $SCRIPT


OUTTREE="DipionDelegate_"$1"GeV_2k_notracks_full.root"
DESTD=/castor/cern.ch/user/b/ballin/tbv7D/
LOG="log_fullsim.txt"

echo Outputtree is: $OUTTREE
echo Logfile is: $LOG

echo

cd /afs/cern.ch/user/b/ballin/scratch0/cmssw/src/
eval `scramv1 ru -sh`
cd $WORKDIR

echo Starting cmsRun
cmsRun $SCRIPT beamEnergy=$1 kevents=2 fileSuffix=notracks_full
echo cmsRun complete.

echo Directory listing of $PWD
ls -lh

echo Copying files to castor...
rfcp $OUTTREE $DESTD$OUTTREE
cp $LOG $RUND"tbv7D"

echo Done.
exit 0
