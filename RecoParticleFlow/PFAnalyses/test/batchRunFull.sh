#!/bin/bash
# Script to submit Pflow reprocessing of testbeam data on batch system and copy results to CASTOR

WORKDIR=$PWD
echo Working directory: $WORKDIR

RUND=/afs/cern.ch/user/b/ballin/scratch0/tbSandbox/CMSSW_3_1_2/src/RecoParticleFlow/PFAnalyses/test/

SCRIPT=$RUND"fullsim_pions.py"
echo Script is: $SCRIPT


echo Outputtree is: $OUTTREE
echo Reprocessed file is: $OUTPUT
echo Logfile is: $LOG

echo

cd /afs/cern.ch/user/b/ballin/scratch0/tbSandbox/CMSSW_3_1_2/src
eval `scramv1 ru -sh`
cd $WORKDIR

echo Starting cmsRun with...
OPTIONS="$SCRIPT beamEnergy=$1 kevents=$2 zeroT=1 noZspSr=1 fileSuffix=slack_full  outputCollections=0 endcapMode=${3}"
echo cmsRun $OPTIONS
cmsRun $OPTIONS

echo cmsRun complete.

echo Directory listing of $PWD
ls -lh

echo Copying all files to castor...

for i in `ls | grep root`
do
	echo Copying ${i}
        rfcp ${i} /castor/cern.ch/user/b/ballin/CMSSW312
done

echo Done.
exit 0
