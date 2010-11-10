import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.producersLayer1.tauProducer_cfi import *
from PhysicsTools.PatAlgos.cleaningLayer1.tauCleaner_cfi import * 

# import helper function to build the pat efficiency PSet
from RecoTauTag.TauAnalysisTools.PFTauEfficiencyAssociator_cfi import *

#--------------------------------------------------------------------------------  
# PAT layer 1 (particle flow)tau-jet configuration parameters
#--------------------------------------------------------------------------------

# set Pt thresholds for PFChargedHadron (PFGamma) isolation to 1.0 (1.5) GeV,
#  matching the thresholds used when computing the tau iso. discriminators
#  in RecoTauTag/RecoTau/python/PFRecoTauDiscriminationByIsolation_cfi.py)
patTaus.userIsolation.pfChargedHadron.threshold = cms.double(1.0)
patTaus.userIsolation.pfGamma.threshold = cms.double(1.5)

# embed track references in pat::Tau
# (so that analysis can access track information if running on PAT-tuples)
patTaus.embedLeadTrack = cms.bool(True)
patTaus.embedSignalTracks = cms.bool(True)
patTaus.embedIsolationTracks = cms.bool(True)

# add tau id. efficiencies & fake-rates
patTaus.efficiencies = cms.PSet()
build_pat_efficiency_loader(shrinkingConeZTTEffSimAssociator, None, patTaus.efficiencies)
build_pat_efficiency_loader(shrinkingConeWJets, None, patTaus.efficiencies)
build_pat_efficiency_loader(shrinkingConeMuEnrichedQCDAssociator, None, patTaus.efficiencies)
build_pat_efficiency_loader(shrinkingConeDiJetHighPt, None, patTaus.efficiencies)
build_pat_efficiency_loader(shrinkingConeDiJetSecondPt, None, patTaus.efficiencies)
patTaus.addEfficiencies = cms.bool(False)

# do not remove tau-jets failing discriminatorByIsolation
# (instead, leave this removal to the subsequent selector stage)
cleanPatTaus.preselection = cms.string('')

# do not remove tau-jets overlapping with electrons and muons
# (instead, leave this removal to the subsequent selector stage;
#  based on disriminatorAgainstElectrons, disriminatorAgainstMuons)
cleanPatTaus.checkOverlaps = cms.PSet()
