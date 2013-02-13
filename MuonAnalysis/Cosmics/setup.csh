#!/bin/tcsh

pushd $CMSSW_BASE/src

scram b -j 4

popd
