#! /bin/bash 

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
