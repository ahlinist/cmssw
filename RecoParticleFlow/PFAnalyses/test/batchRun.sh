#!/bin/bash
# Script to submit Pflow reprocessing of testbeam data on batch system and copy results to CASTOR
if [ -z "$1" ]; then 
	echo usage: $0 energy
	exit
fi

WORKDIR=$PWD
echo Working directory: $WORKDIR
ENERGY=$1
SUFFIX="barrel_slack"
RUND=/afs/cern.ch/user/b/ballin/scratch0/cmssw/src/RecoParticleFlow/PFAnalyses/test/

SCRIPT=$RUND"pflow_tb_general.py"

echo Script is: $SCRIPT

DESTD=/castor/cern.ch/user/b/ballin/CMSSW312/
LOG="log_"$ENERGY"GeV_"$SUFFIX".txt"

echo Outputtree is: $OUTTREE
echo Reprocessed file is: $OUTPUT
echo Logfile is: $LOG

echo

cd /afs/cern.ch/user/b/ballin/scratch0/cmssw/src/
eval `scramv1 ru -sh`
cd $WORKDIR

echo Starting cmsRun
cmsRun $SCRIPT beamEnergy=$ENERGY  fileSuffix=$SUFFIX outputCollections=0
echo cmsRun complete.

#tail -n 50 *.txt

echo Directory listing of $PWD
ls -lh

echo Copying root files to CASTOR...
for i in `ls | grep root`
do
        echo Copying ${i}
        rfcp ${i} /castor/cern.ch/user/b/ballin/CMSSW312
done


echo Done.
exit 0
