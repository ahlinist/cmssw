#!/bin/bash
## Echos commands that store lists of root files in
## $TIER2/$PRIMARY_DATASET/$PROCESSED_DATASET.
## Lists are stored in files_${PRIMARY_DATASET}_${PROCESSED_DATASET}.dat.
## Loops over all PROCESSED_DATASETs listed in $SOURCE_LIST.
## Filenames are stored with full path and prepended with `file:'.

## Usage: `. file-list-helper.sh | sh'

## Customization: Modify the values of PRIMARY_DATASET and SOURCE_LIST.

PRIMARY_DATASET=DoubleMu
SOURCE_LIST=new_dirs.txt

for PROCESSED_DATASET in $(cat $SOURCE_LIST); do
    echo "listFiles.py $TIER2/$PRIMARY_DATASET/$PROCESSED_DATASET <<END"
    echo "files_${PRIMARY_DATASET}_${PROCESSED_DATASET}.dat"
    echo "END"
done
