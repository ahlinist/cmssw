#!/bin/bash 

#Setup script for MVA Met studies in CMSSW_5_3_3 
#Point of this script is to get the root files
#To start you must first
# cvs co -r METPU_5_3_X_v0 JetMETCorrections/METPUSubtraction
tag=METPU_5_3_X_v0

cd ../../../
cvs co -r HEAD -d pharrisTmp UserCode/pharris/MVAMet/data
cp  -d pharrisTmp/*53.root                JetMETCorrections/METPUSubtraction/data/
cp  -d pharrisTmp/*53_UnityResponse.root  JetMETCorrections/METPUSubtraction/data/
rm -rf pharrisTmp

cvs co -r $tag RecoJets/JetProducers
cvs up -r CMSSW_5_3_3 RecoJets/JetProducers/src/JetSpecific.cc

cvs co -r $tag                 DataFormats/METReco
cvs co -r V01-05-06            RecoTauTag/RecoTau 
cd -