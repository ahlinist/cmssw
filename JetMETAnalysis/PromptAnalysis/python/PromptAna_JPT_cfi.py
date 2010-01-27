import FWCore.ParameterSet.Config as cms

import JetMETCorrections.Configuration.JetPlusTrackCorrections_cff
JetPlusTrackZSPCorrectorAntiKt5ForDQM = JetMETCorrections.Configuration.JetPlusTrackCorrections_cff.JetPlusTrackZSPCorrectorAntiKt5.clone()
JetPlusTrackZSPCorrectorAntiKt5ForDQM.ElectronIds = 'eidTight'
JetPlusTrackZSPCorrectorAntiKt5ForDQM.label = 'JetPlusTrackZSPCorrectorAntiKt5ForDQM'
JetPlusTrackZSPCorrectorAntiKt5ForDQM.JetTracksAssociationAtVertex = cms.InputTag('ak5JetTracksAssociatorAtVertex')
JetPlusTrackZSPCorrectorAntiKt5ForDQM.JetTracksAssociationAtCaloFace = cms.InputTag('ak5JetTracksAssociatorAtCaloFace')
from JetMETCorrections.Configuration.ZSPJetCorrections219_cff import *
from JetMETCorrections.Configuration.JetPlusTrackCorrections_cff import *

promptanaJPTak5 = cms.EDProducer("PromptAna_JPT",
  InputTag = cms.InputTag('ak5CaloJets'),
  JPTCorrectorName = cms.string('JetPlusTrackZSPCorrectorAntiKt5ForDQM'),
  ZSPCorrectorName = cms.string('ZSPJetCorrectorAntiKt5'),
  Prefix = cms.string('JPTak5Jet'),
  Suffix = cms.string(''),
  AllVariables = cms.bool(False),
  # Jet ID configuration
  jetID = cms.PSet(
    useRecHits = cms.bool(True),
    hbheRecHitsColl = cms.InputTag("hbhereco"),
    hoRecHitsColl   = cms.InputTag("horeco"),
    hfRecHitsColl   = cms.InputTag("hfreco"),
    ebRecHitsColl   = cms.InputTag("ecalRecHit", "EcalRecHitsEB"),
    eeRecHitsColl   = cms.InputTag("ecalRecHit", "EcalRecHitsEE")
  )
)
