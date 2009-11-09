import FWCore.ParameterSet.Config as cms
import copy

from RecoTauTag.TauAnalysisTools.TauNtupleProducer_cfi import *

'''

Tag and probe tau ntuple production for Heavy Flavor QCD fake rate studies

Author: Evan Friis, UC Davis (friis@physics.ucdavis.edu)

Strategy: Unbias trigger selection by requiring an HLT moun, matched to a
global muon, with pt > 9

'''

from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel

muonTrigger = hltHighLevel.clone()
muonTrigger.HLTPaths = cms.vstring('HLT_Mu9')

tagMuons = cms.EDFilter("MuonRefSelector",
      src = cms.InputTag("muons"),
      cut = cms.string('isGlobalMuon > 0 && pt > 9.0 && abs(eta) < 2.1')
)

# temp test
#tagMuons = cms.EDFilter("CandViewRefSelector",
#      src = cms.InputTag("shrinkingConePFTauProducer"),
#      cut = cms.string('charge() == 1')
#)

atLeastOneTagMuon = cms.EDFilter("CandViewCountFilter",
      src = cms.InputTag('tagMuons'),
      minNumber = cms.uint32(1),
)

# Simple selection on the potential probe taus
probeTaus = cms.EDProducer("CandViewRefSelector",
      src = cms.InputTag('shrinkingConePFTauProducer'),
      cut = cms.string('abs(eta) < 2.5 && pt > 5.0')
)

atLeastOneProbeTau = cms.EDFilter("CandViewCountFilter",
      src = cms.InputTag('probeTaus'),
      minNumber = cms.uint32(1)
)

# Match the base probe taus to the tag muon collection
#  so we can later remove any taus on top of the muon
tagProbeMatcher = cms.EDProducer("TrivialDeltaRViewMatcher",
      src = cms.InputTag("probeTaus"),
      matched = cms.InputTag("tagMuons"),
      distMin = cms.double(0.2)
)

probeTauNtupleProducer = protoRecoTauNtupleProducer.clone()

probeTauNtupleProducer.source = cms.InputTag("probeTaus")
probeTauNtupleProducer.matchingOption = 'unmatched'
probeTauNtupleProducer.matchingSource = 'tagProbeMatcher'
probeTauNtupleProducer.expressions = cms.PSet(common_expressions,
      pftau_expressions)

probeTauNtupleProducer.discriminators = cms.PSet(pftau_discriminators,
      pftau_discriminators_extra)

heavyFlavorNtupleSequence = cms.Sequence(
      muonTrigger
      *tagMuons
      *atLeastOneTagMuon
      *probeTaus
      *atLeastOneProbeTau
      *tagProbeMatcher
      *probeTauNtupleProducer)



