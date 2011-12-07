#!/bin/tcsh

pushd $CMSSW_BASE/src

addpkg RecoMuon/GlobalTrackingTools
addpkg RecoMuon/GlobalMuonProducer
addpkg DataFormats/MuonReco
addpkg RecoMuon/CosmicMuonProducer

patch -p0 < MuonAnalysis/Cosmics/patches

scram b -j 4

popd
