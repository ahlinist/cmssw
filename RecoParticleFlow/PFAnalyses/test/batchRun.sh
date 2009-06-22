#!/bin/bash
# Script to submit Pflow reprocessing of testbeam data on batch system and copy results to CASTOR
if [ -z "$1" ]; then 
	echo usage: $0 energy
	exit
fi

WORKDIR=$PWD
echo Working directory: $WORKDIR
ENERGY=$1
SUFFIX=""
RUND=/afs/cern.ch/user/b/ballin/scratch0/cmssw/src/RecoParticleFlow/PFAnalyses/test/

SCRIPT=$RUND"pflow_tb_general.py"

echo Script is: $SCRIPT

OUTTREE="outputtree_"$ENERGY"GeV_notracks"$SUFFIX".root"
OUTPUT="reprocessed_"$ENERGY"GeV_notracks"$SUFFIX".root"
DESTD=/castor/cern.ch/user/b/ballin/tb310pre8/
LOG="log_"$ENERGY"GeV_notracks"$SUFFIX".txt"

echo Outputtree is: $OUTTREE
echo Reprocessed file is: $OUTPUT
echo Logfile is: $LOG

echo

cd /afs/cern.ch/user/b/ballin/scratch0/cmssw/src/
eval `scramv1 ru -sh`
cd $WORKDIR

echo Starting cmsRun
cmsRun $SCRIPT beamEnergy=$ENERGY kevents=0 notracks=1 copyToTmp=0
echo cmsRun complete.

tail -n 50 *.txt

echo Directory listing of $PWD
ls -lh

echo Copying files to castor...
rfcp $OUTTREE $DESTD
rfcp $OUTPUT $DESTD
cp $LOG $RUND"tb310pre8/"

echo Done.
exit 0
