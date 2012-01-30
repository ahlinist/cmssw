#!/bin/tcsh

pushd $CMSSW_BASE/src

addpkg RecoMuon/GlobalTrackingTools
addpkg RecoMuon/GlobalMuonProducer

patch -p0 < MuonAnalysis/Cosmics/patches

scram b -j 4

popd
