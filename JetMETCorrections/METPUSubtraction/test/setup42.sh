#!/bin/bash 

#Setup script for MVA Met studies in CMSSW_5_3_3 
#Point of this script is to get the root files
#To start you must first
# cvs co -r METPU_5_3_X_v0 JetMETCorrections/METPUSubtraction
tag=METPU_5_3_X_v3

cd ../../../
cvs co -r HEAD -d pharrisTmp UserCode/pharris/MVAMet/data
cp  -d pharrisTmp/*42.root                JetMETCorrections/METPUSubtraction/data/
cp  -d pharrisTmp/*42_UnityResponse.root  JetMETCorrections/METPUSubtraction/data/
rm -rf pharrisTmp

cvs co -r $tag RecoJets/JetProducers
cvs up -r CMSSW_5_3_3 RecoJets/JetProducers/src/JetSpecific.cc

cvs co -r V03-03-18           DataFormats/METReco
cvs co -r V05-00-16           DataFormats/JetReco
cvs co -r V01-05-06           RecoTauTag/RecoTau 
cvs co -r V03-04-07           RecoMET/METAlgorithms
cvs co -r V01-02-02           DataFormats/TauReco
cvs co -r V01-02-03           RecoTauTag/TauTagTools
cvs co -r V00-04-01           CondFormats/EgammaObjects
#cvs co -r V02-03-01           JetMETCorrections/Algorithms


cvs update -r  V03-02-01-01  RecoMET/METAlgorithms/src/CSCHaloAlgo.cc 
cvs update -r  V03-02-01-01  RecoMET/METAlgorithms/interface/CSCHaloAlgo.h 
cvs update -r  V03-04-02     RecoMET/METAlgorithms/src/PFClusterSpecificAlgo.cc 
cvs update -r  V03-04-02     RecoMET/METAlgorithms/src/PFClusterSpecificAlgo.cc 
cvs update -r  V05-00-13     DataFormats/JetReco/interface/PFClusterJet.h 
cvs update -r  V05-00-13     DataFormats/JetReco/src/PFClusterJet.cc 
cvs update -r  V05-07-00     RecoJets/JetProducers/plugins/FastjetJetProducer.cc
cvs update -r  V05-07-00     RecoJets/JetProducers/plugins/FastjetJetProducer.h
cvs update -r  V05-08-07     RecoJets/JetProducers/plugins/VirtualJetProducer.*
cvs update -r  V05-08-07     RecoJets/JetProducers/interface/PileUpSubtractor.h 
cvs update -r  V05-08-07     RecoJets/JetProducers/src/PileUpSubtractor.cc 
cvs co     -r  CMSSW_4_2_8_patch7 RecoMET/METAlgorithms/interface/SigInputObj.h
cvs co     -r  CMSSW_4_2_8_patch7 RecoMET/METAlgorithms/interface/SigInputObj.cc

rm RecoJets/JetProducers/plugins/FixedGridRhoProducer*
rm JetMETCorrections/METPUSubtraction/plugins/NoPileUpPFMEtDataProducer.*
rm JetMETCorrections/METPUSubtraction/python/noPileUpPFMET_cff.py
cd -