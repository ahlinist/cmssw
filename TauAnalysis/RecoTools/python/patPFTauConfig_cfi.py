import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.producersLayer1.tauProducer_cfi import *
from PhysicsTools.PatAlgos.cleaningLayer1.tauCleaner_cfi import * 

# import helper function to build the pat efficiency PSet
from RecoTauTag.TauTagTools.PFTauEfficiencyAssociator_cfi import *

#--------------------------------------------------------------------------------  
# PAT layer 1 (particle flow)tau-jet configuration parameters
#--------------------------------------------------------------------------------

# set Pt thresholds for PFChargedHadron (PFGamma) isolation to 1.0 (1.5) GeV,
#  matching the thresholds used when computing the tau iso. discriminators
#  in RecoTauTag/RecoTau/python/PFRecoTauDiscriminationByIsolation_cfi.py)
allLayer1Taus.isolation.pfChargedHadron.threshold = cms.double(1.0)
allLayer1Taus.isolation.pfGamma.threshold = cms.double(1.5)

# add reconstructed tau decay mode
allLayer1Taus.addDecayMode = cms.bool(True)

# add tau id. efficiencies & fake-rates
#allLayer1Taus.efficiencies = build_pat_efficiency_loader(shrinkingConeEfficienciesProducerFromFile)
#allLayer1Taus.addEfficiencies = cms.bool(True)

# do not remove tau-jets failing discriminatorByIsolation
# (instead, leave this removal to the subsequent selector stage)
cleanLayer1Taus.preselection = cms.string('')

# do not remove tau-jets overlapping with electrons and muons
# (instead, leave this removal to the subsequent selector stage;
#  based on disriminatorAgainstElectrons, disriminatorAgainstMuons)
cleanLayer1Taus.checkOverlaps = cms.PSet()
