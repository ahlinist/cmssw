import FWCore.ParameterSet.Config as cms
import copy

pfCandidateHistManager = cms.PSet(
  pluginName = cms.string('pfCandidateHistManager'),
  pluginType = cms.string('PFCandidateHistManager'),
      
  pfCandidateSource = cms.InputTag(''),

  dqmDirectory_store = cms.string('PFCandidateQuantities')
)

allPFCandidateHistManager = copy.deepcopy(pfCandidateHistManager)
allPFCandidateHistManager.pluginName = cms.string('allPFCandidateHistManager')
allPFCandidateHistManager.pfCandidateSource = cms.InputTag("particleFlow")
allPFCandidateHistManager.dqmDirectory_store = cms.string('AllPFCandidateQuantities')

pfChargedHadronHistManager = copy.deepcopy(pfCandidateHistManager)
pfChargedHadronHistManager.pluginName = cms.string('pfChargedHadronHistManager')
pfChargedHadronHistManager.pfCandidateSource = cms.InputTag("pfAllChargedHadrons")
pfChargedHadronHistManager.dqmDirectory_store = cms.string('PFChargedHadronQuantities')

pfGammaHistManager = copy.deepcopy(pfCandidateHistManager)
pfGammaHistManager.pluginName = cms.string('pfGammaHistManager')
pfGammaHistManager.pfCandidateSource = cms.InputTag("pfAllPhotons")
pfGammaHistManager.dqmDirectory_store = cms.string('PFGammaQuantities')

pfNeutralHadronHistManager = copy.deepcopy(pfCandidateHistManager)
pfNeutralHadronHistManager.pluginName = cms.string('pfNeutralHadronHistManager')
pfNeutralHadronHistManager.pfCandidateSource = cms.InputTag("pfAllNeutralHadrons")
pfNeutralHadronHistManager.dqmDirectory_store = cms.string('PFNeutralHadronQuantities')
