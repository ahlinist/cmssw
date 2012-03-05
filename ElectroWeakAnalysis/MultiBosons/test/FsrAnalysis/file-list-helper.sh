#!/bin/bash
## Echos commands that store lists of root files in
## $TIER2/$PRIMARY_DATASET/$PROCESSED_DATASET.
## Lists are stored in files_${PRIMARY_DATASET}_${PROCESSED_DATASET}.dat.
## Loops over all PROCESSED_DATASETs listed in $SOURCE_LIST.
## Filenames are stored with full path and prepended with `file:'.

## Usage: `. file-list-helper.sh | sh'
## Customization: Modify the values of PRIMARY_DATASET and SOURCE_LIST.
PRIMARY_DATASET='WJetsToLNu_TuneZ2_7TeV-madgraph-tauola'
## ZGToEEG_TuneZ2_7TeV-madgraph/Fall11-PU_S6_START42_V14B-v1_glite_Diphoton_AOD-42X-v10
#SOURCE_LIST=$(cat new_dirs_2.txt)
SOURCE_LIST='
    Fall11-PU_S6_START42_V14B-v1_condor_Dimuon_AOD-42X-v10
    '

for PROCESSED_DATASET in $SOURCE_LIST; do
    echo "listFiles.py $TIER2/$PRIMARY_DATASET/$PROCESSED_DATASET <<END"
    echo "files_${PRIMARY_DATASET}_${PROCESSED_DATASET}.dat"
    echo "END"
done
