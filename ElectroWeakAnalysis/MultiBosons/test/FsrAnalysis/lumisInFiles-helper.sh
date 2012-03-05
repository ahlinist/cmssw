#!/bin/bash
## Echos commands that extract the lumis in files listed
## files_${PRIMARY_DATASET}_${PROCESSED_DATASET}.dat.
## Loops over all PROCESSED_DATASETs listed in $SOURCE_LIST.
## Extracted lumis are stored in
## lumisInFiles_${PRIMARY_DATASET}_${PROCESSED_DATASET}.dat

## Usage: `. lumisInFiles-helper.sh | sh'

## Customization: Modify the values of PRIMARY_DATASET and SOURCE_LIST.

PRIMARY_DATASET=DoubleMu
#SOURCE_LIST=$(cat new_dirs_2.txt)
SOURCE_LIST='
    Run2011A-30Nov2011-v1_condor_Dimuon_AOD-42X-v10
    Run2011B-30Nov2011-v1_condor_Dimuon_AOD-42X-v10
    Run2011A-30Nov2011-v1_condor_Dimuon_AOD-42X-v10_DBS
    Run2011B-30Nov2011-v1_condor_Dimuon_AOD-42X-v10_DBS
    '

for PROCESSED_DATASET in $SOURCE_LIST; do
    INPUT=files_${PRIMARY_DATASET}_${PROCESSED_DATASET}.dat
    OUTPUT=lumisInFiles_${PRIMARY_DATASET}_${PROCESSED_DATASET}.json
    echo "edmLumisInFiles.py --output=$OUTPUT \$(cat $INPUT)"
done
