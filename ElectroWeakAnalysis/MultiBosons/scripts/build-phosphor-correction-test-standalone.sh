#!/bin/bash
##
## This script is an example of how to use the PHOSPHOR correction
## class PhosphorCorrectionFunctor in stand-alone ROOT without 
## the CMSSW environment using g++ to compile it.
## It generates a ROOT dictionary for the class, compiles both the class
## and the dictionary and finally it compiles a test which uses the 
## correction in a stand-alone executable linking the correction and its
## dictionary.
##
## New files created in the current dirrectory are:
##    PhosphorCorrectionFunctorDict.h - ROOT dictionary header files
##    PhosphorCorrectionFunctorDict.cc - ROOT dictionary implementation
##    PhosphorCorrectionFunctor.o - the compiled correction object file
##    PhosphorCorrectionFunctorDict.o - the compiled dictionary object file
##    test-phosphor-correction.exe - the final executable
## 
## Usage: 
##    ./build-phosphor-correction-test-standalone.sh 
## 
## Jan Veverka, Caltech, 2 April 2012

## Shorthand for g++ options
ROOT_LIBS="-lNew -lRint -lTree -lTreePlayer -lCint -lThread -lGraf -lGraf3d \
    -lHist -lHtml -lMatrix -lMinuit -lPostscript -lProof -lThread -lCore \
    -lGX11 -lPhysics -lGpad -lGui -lTreeViewer"

## Shorthand for some g++ flags
GXX_FLAGS="-Wall -Wno-long-long -fPIC"

## Shorthand for the name of the dictionary file
DICT_FILE=PhosphorCorrectionFunctorDict.cc

if [[ -n "$CMSSW_BASE" ]]; then
    ## CMSSW environment is defined
    HEADER_FILE_DIR=$CMSSW_BASE/src
    ## Shortcut for a long dir name used a couple times
    BASE=$CMSSW_BASE/src/ElectroWeakAnalysis/MultiBosons
    HEADER_FILE=$BASE/interface/PhosphorCorrectionFunctor.h
    CC_FILE=$BASE/src/PhosphorCorrectionFunctor.cc
    LINKDEF_FILE=$BASE/src/PhosphorCorrectionFunctorLinkDef.h
    MAIN_FILE=$BASE/bin/testPhosphorCorrection.cc
else
    HEADER_FILE_DIR=.
    HEADER_FILE=PhosphorCorrectionFunctor.h
    CC_FILE=PhosphorCorrectionFunctor.cc
    LINKDEF_FILE=PhosphorCorrectionFunctorLinkDef.h
    MAIN_FILE=testPhosphorCorrection.cc
fi  
    
## Generate the ROOT dictionary for the correction class
rootcint -l -f $DICT_FILE -c -I$HEADER_FILE_DIR -I$ROOTSYS/include \
    $HEADER_FILE $LINKDEF_FILE

## Compile the correction class and its dictionary
g++ -c $GXX_FLAGS \
    $CC_FILE $DICT_FILE \
    -I$HEADER_FILE_DIR -I$ROOTSYS/include \
    -L$ROOTSYS/lib $ROOT_LIBS
    
## Compile the correction test
g++ $GXX_FLAGS \
    PhosphorCorrectionFunctor.o \
    PhosphorCorrectionFunctorDict.o \
    $MAIN_FILE \
    -o test-phosphor-correction.exe \
    -pthread -DCTHREAD_POSIX -D_THREAD_SAFE -D_REENTRANT \
    -I$HEADER_FILE_DIR \
    -I$ROOTSYS/include -L$ROOTSYS/lib  \
    $ROOT_LIBS \
    -L/usr/X11R6/lib -lm -ldl -L/usr/lib -lpthread -rdynamic 
