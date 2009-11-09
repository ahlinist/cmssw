import FWCore.ParameterSet.Config as cms
import copy

from RecoTauTag.TauAnalysisTools.TauNtupleProducer_cfi import *

'''

Tag and probe tau ntuple production for Heavy Flavor QCD fake rate studies

Author: Evan Friis, UC Davis (friis@physics.ucdavis.edu)

Strategy: Unbias trigger selection by requiring an HLT moun, matched to a
global muon, with pt > 15

'''

from HLTrigger.HLTfilters.hltHighLevel_cfi import hltHighLevel

jetTrigger = hltHighLevel.clone()
jetTrigger.HLTPaths = cms.vstring('HLT_L1Jet15')

# Simple selection on the potential probe taus
probeTauJets = cms.EDProducer("PFTauRefSelector",
      src = cms.InputTag('shrinkingConePFTauProducer'),
      cut = cms.string('abs(eta) < 2.5 && pfTauTagInfoRef().pfjetRef().pt() > 5.0')
)

atLeastTwoProbeTaus = cms.EDFilter("CandViewCountFilter",
      src = cms.InputTag('probeTauJets'),
      minNumber = cms.uint32(2)
)

dijetTagAndProbes = cms.EDProducer("TauFakeRateDijetProbesProducer",
      source = cms.InputTag("probeTauJets"),
      dataType = cms.string("PFTau"),
      expression = cms.string("pfTauTagInfoRef().pfjetRef().pt()"),
      threshold = cms.double(15.0)
)

atLeastOneTagTau = cms.EDFilter("CandViewCountFilter",
      src = cms.InputTag("dijetTagAndProbes", "tagObject"),
      minNumber = cms.uint32(1)
)

highestProbeJetNtupleProducer = protoRecoTauNtupleProducer.clone()
highestProbeJetNtupleProducer.source = cms.InputTag("dijetTagAndProbes", "highestPtProbe")
highestProbeJetNtupleProducer.alias = cms.string("highestPtJet")
highestProbeJetNtupleProducer.matchingOption = 'none'
highestProbeJetNtupleProducer.expressions = cms.PSet(common_expressions,
      pftau_expressions)
highestProbeJetNtupleProducer.discriminators = cms.PSet(pftau_discriminators,
      pftau_discriminators_extra)

secondProbeJetNtupleProducer = protoRecoTauNtupleProducer.clone()
secondProbeJetNtupleProducer.source = cms.InputTag("dijetTagAndProbes", "secondHighestPtProbe")
secondProbeJetNtupleProducer.alias = cms.string("secondPtJet")
secondProbeJetNtupleProducer.matchingOption = 'none'
secondProbeJetNtupleProducer.expressions = cms.PSet(common_expressions,
      pftau_expressions)
secondProbeJetNtupleProducer.discriminators = cms.PSet(pftau_discriminators,
      pftau_discriminators_extra)

heavyFlavorNtupleSequence = cms.Sequence(
      jetTrigger
      *probeTauJets
      *atLeastTwoProbeTaus # two, before the tag selection
      *dijetTagAndProbes
      *atLeastOneTagTau
      *highestProbeJetNtupleProducer
      *secondProbeJetNtupleProducer)

