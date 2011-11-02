#! /bin/bash 

# txt file with file list per dataset should be passed like
# example file: VBF_HToGG_M-120_TuneZ2_7TeV-pythia6.txt
# "rfio:///castor/cern.ch/user/f/fabstoec/Summer11Private/VBF_HToGG_M-120_TuneZ2_7TeV-pythia6/Summer11-PU_S4_START42_V11-v1/GEN-SIM-RECO/SUM11-HGGPRIV-0080-0440-0006.root"
# should contain hyphens and protocol to read the file 

if [ ! $CRABDIR ] ; then
   echo "Please set CRAB environment"
   exit
fi

dataset=$1

datasetName=`basename $dataset .txt`
rm -rf $datasetName
mkdir -p $datasetName
cp common_dump_config.py $datasetName
./runProduction_LOCAL.py $dataset
cd $datasetName
jobs=`ls *.cfg`

for job in $jobs; do
    jobName=`basename $job .cfg`
    echo "- Invoking crab: $jobName"
    while [ `jobs | wc -l` -gt 5 ]; do
	    
#	  echo "Sleeping 5"
	sleep 5 
    done 

    crab -create -submit -cfg $job 2>&1 > ${jobName}_submit.log &
done
