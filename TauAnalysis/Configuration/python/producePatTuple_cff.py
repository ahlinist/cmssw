import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# produce all objects specific to TauAnalysis which get stored in PATTuple
#--------------------------------------------------------------------------------

#
# import "official" PAT production sequences
#
from PhysicsTools.PatAlgos.patLayer0_cff import *
from PhysicsTools.PatAlgos.patLayer1_cff import *

#--------------------------------------------------------------------------------
#
# CV: disable PAT photon sequences for now
# (until tags are more stable...)
#
# NOTE: temporary fix for CMSSW_2_2_x only !!
#
patBeforeLevel0Reco.remove(patAODPhotonIsolation)
patLayer0Cleaners.remove(allLayer0Photons)
patHighLevelReco_withoutPFTau.remove(patPhotonId)
patHighLevelReco_withoutPFTau.remove(patLayer0PhotonIsolation)
patMCTruth_LeptonPhoton.remove(photonMatch)
patTrigMatch.remove(photonTrigMatchHLT1PhotonRelaxed)
allObjects.remove(layer1Photons)
allObjects.remove(layer1Hemispheres)

eleIsoDepositHcalFromTowers.ExtractorPSet = cms.PSet(EleIsoHcalFromTowersExtractorBlock)
#--------------------------------------------------------------------------------

#
# reconstruct objects needed by TauAnalysis customized PAT sequences
# (AOD level electron id. and isolation)
#
from TauAnalysis.RecoTools.prePatProduction_cff import *
#
# customize configuration parameters for pat::Electrons,
# pat::Muons and pat::(PF)Taus
#
from TauAnalysis.RecoTools.patElectronConfig_cfi import *
from TauAnalysis.RecoTools.patMuonConfig_cfi import *
from TauAnalysis.RecoTools.patPFTauConfig_cfi import *
#
# produce collections of pat::Electrons, pat::Muons and
# pat::(PF)Taus passing different selection criteria
#
from TauAnalysis.RecoTools.patLeptonSelection_cff import *
#
# produce collection of pat::Jets passing Et threshold and
# Eta acceptance cuts and not overlapping with any object
# passing selection criteria for pat::Electron, pat::Muon or pat::(PF)Tau
# (pat::Jet collection to be considered for central-jet veto)
#
from TauAnalysis.RecoTools.patJetSelection_cff import *
#
# produce TauAnalysis specific objects not related to PAT sequences
# (reconstruction of generator level information and
#  selection of primary event vertex)
#
from TauAnalysis.RecoTools.postPatProduction_cff import *
#
# produce combinations of e + tau-jet, mu + tau-jet
# and tau-jet + tau-jet pairs
#
from TauAnalysis.CandidateTools.diTauPairProductionAllKinds_cff import *
#
# produce collections of e + tau-jet, mu + tau-jet and
# tau-jet + tau-jet pairs passing different selection criteria
#
from TauAnalysis.CandidateTools.diTauPairSelectionAllKinds_cff import *

producePatTuple = cms.Sequence( producePrePat         # comment-out if running on "officially" produced PAT-tuples
                               +patLayer0 + patLayer1 # comment-out if running on "officially" produced PAT-tuples
                               +producePostPat
                               +produceLayer1SelLeptons + selectLayer1Jets
                               +produceDiTauPairsAllKinds
                               +selectDiTauPairsAllKinds )
