#!/bin/bash
## Echos commands that extract the lumis in files listed
## files_${PRIMARY_DATASET}_${PROCESSED_DATASET}.dat.
## Loops over all PROCESSED_DATASETs listed in $SOURCE_LIST.
## Extracted lumis are stored in
## lumisInFiles_${PRIMARY_DATASET}_${PROCESSED_DATASET}.dat

## Usage: `. lumisInFiles-helper.sh | sh'

## Customization: Modify the values of PRIMARY_DATASET and SOURCE_LIST.

#PRIMARY_DATASET=DoubleMu
#SOURCE_LIST=$(cat new_dirs_2.txt)

PRIMARY_DATASET=DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia
SOURCE_LIST='Summer11-PU_S4_START42_V11-v1_glite_Dimuon_RECO-42X-v9
             Summer11-PU_S4_START42_V11-v1_glidein_Dimuon_RECO-42X-v9'



for PROCESSED_DATASET in $SOURCE_LIST; do
    INPUT=files_${PRIMARY_DATASET}_${PROCESSED_DATASET}.dat
    OUTPUT=lumisInFiles_${PRIMARY_DATASET}_${PROCESSED_DATASET}.json
    echo "edmLumisInFiles.py --output=$OUTPUT \$(cat $INPUT)"
done
