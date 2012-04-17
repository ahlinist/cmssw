#!/bin/bash
## Takes the version string as an argument and creates a tar ball in the working
## directory with all the files related to the PHOSPHOR correction.
## Precondition: CMSSW environment has been set up and the dev branch of the
##  ElectroWeakAnalysis/MultiBosons has been checked out and built.
## Jan Veverka, Caltech, 21 March 2012 - 17 April 2012

VERSION=${1:-v0.2}
TARBALL_DIR=PHOSPHOR_Correction_${VERSION}
BASE_DIR=$CMSSW_BASE/src/ElectroWeakAnalysis/MultiBosons
FILE_LIST='bin/testPhosphorCorrection.cc
           interface/PhosphorCorrectionFunctor.h
           src/PhosphorCorrectionFunctor.cc
           src/PhosphorCorrectionFunctorLinkDef.h
           scripts/build-phosphor-correction-test-standalone.sh
           test/Phosphor/README
           test/Phosphor/runPhosphorCorrectionTest.C'

SUBSTITUTION_COMMAND='s?ElectroWeakAnalysis/MultiBosons/interface/??'

mkdir $TARBALL_DIR && \
    for FILE in $FILE_LIST; do
        cp $BASE_DIR/$FILE $TARBALL_DIR 
    done && \
    sed -i.bak $SUBSTITUTION_COMMAND $TARBALL_DIR/*.* && \
    rm $TARBALL_DIR/*.bak && \
    tar czvf ${TARBALL_DIR}.tgz $TARBALL_DIR && \
    rm -rf $TARBALL_DIR
