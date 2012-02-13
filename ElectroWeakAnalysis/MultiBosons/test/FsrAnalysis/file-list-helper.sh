#!/bin/bash
## Echos commands that store lists of root files in
## $TIER2/$PRIMARY_DATASET/$PROCESSED_DATASET.
## Lists are stored in files_${PRIMARY_DATASET}_${PROCESSED_DATASET}.dat.
## Loops over all PROCESSED_DATASETs listed in $SOURCE_LIST.
## Filenames are stored with full path and prepended with `file:'.

## Usage: `. file-list-helper.sh | sh'

## Customization: Modify the values of PRIMARY_DATASET and SOURCE_LIST.

#PRIMARY_DATASET=DoubleMu
#SOURCE_LIST=$(cat new_dirs_2.txt)

PRIMARY_DATASET=DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia
SOURCE_LIST='Summer11-PU_S4_START42_V11-v1_glite_Dimuon_RECO-42X-v9
             Summer11-PU_S4_START42_V11-v1_glidein_Dimuon_RECO-42X-v9'


for PROCESSED_DATASET in $SOURCE_LIST; do
    echo "listFiles.py $TIER2/$PRIMARY_DATASET/$PROCESSED_DATASET <<END"
    echo "files_${PRIMARY_DATASET}_${PROCESSED_DATASET}.dat"
    echo "END"
done
