#!/bin/bash
## Echos commands that extract the lumis in files listed
## files_${PRIMARY_DATASET}_${PROCESSED_DATASET}.dat.
## Loops over all PROCESSED_DATASETs listed in $SOURCE_LIST.
## Extracted lumis are stored in
## lumisInFiles_${PRIMARY_DATASET}_${PROCESSED_DATASET}.dat

## Usage: `. lumisInFiles-helper.sh | sh'

## Customization: Modify the values of PRIMARY_DATASET and SOURCE_LIST.

PRIMARY_DATASET=DoubleMu
SOURCE_LIST=new_dirs.txt

for PROCESSED_DATASET in $(cat $SOURCE_LIST); do
    INPUT=files_${PRIMARY_DATASET}_${PROCESSED_DATASET}.dat
    OUTPUT=lumisInFiles_${PRIMARY_DATASET}_${PROCESSED_DATASET}.json
    echo "edmLumisInFiles.py --output=$OUTPUT \$(cat $INPUT)"
done
