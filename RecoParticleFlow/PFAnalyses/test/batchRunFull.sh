#!/bin/bash
# Script to submit Pflow reprocessing of testbeam data on batch system and copy results to CASTOR

WORKDIR=$PWD
echo Working directory: $WORKDIR

RUND=/afs/cern.ch/user/b/ballin/scratch0/cmssw/src/RecoParticleFlow/PFAnalyses/test/

SCRIPT=$RUND"fullsim_neutrals.py"

echo Script is: $SCRIPT


OUTTREE="Dikaon_"$1"GeV_2k_full.root"
OUTPUT="Dikaon_Event_"$1"GeV_2k_full.root"
DESTD=/castor/cern.ch/user/b/ballin/tbv8/
LOG="Dikaon_"$1"GeV_2k_full.txt"
LOGSUM="sum_Dikaon_"$1"GeV_2k_full.txt"

echo Outputtree is: $OUTTREE
echo Reprocessed file is: $OUTPUT
echo Logfile is: $LOG

echo

cd /afs/cern.ch/user/b/ballin/scratch0/cmssw/src/
eval `scramv1 ru -sh`
cd $WORKDIR

echo Starting cmsRun
cmsRun $SCRIPT beamEnergy=$1 kevents=2 fileSuffix=full
echo cmsRun complete.

echo Directory listing of $PWD
ls -lh

echo Copying files to castor...
rfcp $OUTTREE $DESTD$OUTTREE
rfcp $OUTPUT $DESTD$OUTPUT
tail -n 200 $LOG > $LOGSUM

cp $LOGSUM "${RUND}tbv8/"

echo Done.
exit 0
