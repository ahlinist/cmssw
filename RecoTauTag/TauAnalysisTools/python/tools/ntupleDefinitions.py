import FWCore.ParameterSet.Config as cms

'''
Common defintions for quanities prodcues in TauNtuples
'''

common_expressions = cms.PSet(
      pt       = cms.string("pt()"),
      phi      = cms.string("phi()"),
      eta      = cms.string("eta()"),
      mass     = cms.string("mass()"),
      charge   = cms.string("charge()"),
)

pftau_expressions = cms.PSet(
      jetPt    = cms.string('pfTauTagInfoRef().pfjetRef().pt()'),
      jetEta   = cms.string('pfTauTagInfoRef().pfjetRef().eta()'),
      jetWidth = cms.string('sqrt(pfTauTagInfoRef().pfjetRef().etaetaMoment()^2 + pfTauTagInfoRef().pfjetRef().phiphiMoment()^2)'),
      nTrks    = cms.string("signalTracks().size()")
)

# Discriminator types.  Use RecoTauTag.TauAnalysisTools.tools.ntupleTools to
#  adapt these to arbritrary collections.

pftau_discriminators = cms.PSet(
      ByLeadTrack      = cms.InputTag("shrinkingConePFTauDiscriminationByLeadingTrackFinding"),
      ByLeadTrackPt    = cms.InputTag("shrinkingConePFTauDiscriminationByLeadingTrackPtCut"),
      ByLeadPionPt     = cms.InputTag("shrinkingConePFTauDiscriminationByLeadingPionPtCut"),
      ByTrackIsolation = cms.InputTag("shrinkingConePFTauDiscriminationByTrackIsolation"),
      ByEcalIsolation  = cms.InputTag("shrinkingConePFTauDiscriminationByECALIsolation"),
      ByIsolation      = cms.InputTag("shrinkingConePFTauDiscriminationByIsolation"),
      AgainstElectron  = cms.InputTag("shrinkingConePFTauDiscriminationAgainstElectron"),
      AgainstMuon      = cms.InputTag("shrinkingConePFTauDiscriminationAgainstMuon"),
)

pftau_discriminators_extra = cms.PSet(
      DecayMode       = cms.InputTag("shrinkingConePFTauDecayModeIndexProducer"),
      ByTaNCRaw       = cms.InputTag("shrinkingConePFTauDiscriminationByTaNC"),
      ByTaNCfrOne     = cms.InputTag("shrinkingConePFTauDiscriminationByTaNCfrOnePercent"),
      ByTaNCfrHalf    = cms.InputTag("shrinkingConePFTauDiscriminationByTaNCfrHalfPercent"),
      ByTaNCfrQuarter = cms.InputTag("shrinkingConePFTauDiscriminationByTaNCfrQuarterPercent"),
      ByTaNCfrTenth   = cms.InputTag("shrinkingConePFTauDiscriminationByTaNCfrTenthPercent"),
)
