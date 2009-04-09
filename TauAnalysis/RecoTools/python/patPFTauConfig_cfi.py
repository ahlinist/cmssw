import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.producersLayer1.tauProducer_cfi import *
from PhysicsTools.PatAlgos.cleaningLayer1.tauCleaner_cfi import * 

#--------------------------------------------------------------------------------  
# PAT layer 1 (particle flow)tau-jet configuration parameters
#--------------------------------------------------------------------------------

# add reconstructed tau decay mode
allLayer1Taus.addDecayMode = cms.bool(True)

# do not remove tau-jets failing discriminatorByIsolation
# (instead, leave this removal to the subsequent selector stage)
cleanLayer1Taus.preselection = cms.string('')

# do not remove tau-jets overlapping with electrons and muons
# (instead, leave this removal to the subsequent selector stage;
#  based on disriminatorAgainstElectrons, disriminatorAgainstMuons)
cleanLayer1Taus.checkOverlaps = cms.PSet()
