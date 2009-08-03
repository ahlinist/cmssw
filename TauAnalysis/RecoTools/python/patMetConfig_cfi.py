import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.recoLayer0.jetMETCorrections_cff import *

from PhysicsTools.PatAlgos.producersLayer1.metProducer_cfi import *

from JetMETCorrections.Type1MET.TauMetCorrections_cff import * 

#--------------------------------------------------------------------------------  
# PAT layer 0 MET configuration parameters
#--------------------------------------------------------------------------------

# apply tau-jet specific corrections to reco::MET per default
# (comment-out the next line in case you prefer to **not** apply the tau-jet specific MET corrections)

tauMetCorr.InputCaloJetsLabel = metJESCorIC5CaloJet.inputUncorJetsLabel
tauMetCorr.jetPTthreshold = metJESCorIC5CaloJet.jetPTthreshold
tauMetCorr.jetEMfracLimit = metJESCorIC5CaloJet.jetEMfracLimit
tauMetCorr.correctorLabel = metJESCorIC5CaloJet.corrector
tauMetCorr.InputMETLabel = cms.string('metJESCorIC5CaloJetMuons')

patMETCorrections._seq = patMETCorrections._seq * MetTauCorrections

#--------------------------------------------------------------------------------  
# PAT layer 1 MET configuration parameters
#--------------------------------------------------------------------------------

# change input for pat::MET production to reco::MET collection
# containing the tau-jet specific corrections
# (comment-out the next line in case you prefer to **not** apply the tau-jet specific MET corrections)
layer1METs.metSource  = cms.InputTag("tauMetCorr")
