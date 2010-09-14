import FWCore.ParameterSet.Config as cms

process = cms.Process("Test")

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
    'file:job_20100913-221732/patTuple_standard_ttbar_madgraph.root_ttbar10_madAOD.root'
  )
)

## options and output report
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

## ---
## define object collections
## ---
from PhysicsTools.PatAlgos.cleaningLayer1.muonCleaner_cfi import *
process.looseMuons = cleanPatMuons.clone(
    preselection =
    'isGlobalMuon & isTrackerMuon &'
    'pt > 20. &'
    'abs(eta) < 2.1 &'
    '(trackIso+caloIso)/pt < 0.1 &'
    'innerTrack.numberOfValidHits > 10 &'
    'globalTrack.normalizedChi2 < 10.0 &'
    'globalTrack.hitPattern.numberOfValidMuonHits > 0 &'
    'abs(dB) < 0.02',
    checkOverlaps = cms.PSet(
      jets = cms.PSet(
        src                 = cms.InputTag("goodJets"),
        algorithm           = cms.string("byDeltaR"),
        preselection        = cms.string(""),
        deltaR              = cms.double(0.3),
        checkRecoComponents = cms.bool(False),
        pairCut             = cms.string(""),
        requireNoOverlaps   = cms.bool(True),
      )
    )
)

process.tightMuons = cleanPatMuons.clone(
    src = 'looseMuons',
    preselection = '(trackIso+caloIso)/pt < 0.05'
)

process.vetoMuons = cleanPatMuons.clone(
    preselection =
    'isGlobalMuon &'
    'pt > 10. &'
    'abs(eta) < 2.5 &'
    '(trackIso+caloIso)/pt < 0.2'
)

from PhysicsTools.PatAlgos.selectionLayer1.electronSelector_cfi import *
process.vetoElectrons = selectedPatElectrons.clone(
    src = 'selectedPatElectrons',
    cut =
    'et > 15. &'
    'abs(eta) < 2.5 &'
    '(dr03TkSumPt+dr03EcalRecHitSumEt+dr03HcalTowerSumEt)/et <  0.2'
 )

from PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi import *
process.goodJets = selectedPatJets.clone(
    src = 'selectedPatJets',
    cut =
    'pt > 30. &'
    'abs(eta) < 2.4 &'
    'emEnergyFraction > 0.01 &'
    'jetID.n90Hits > 1 &'
    'jetID.fHPD < 0.98'
)

## ---
## define selection steps 
## ---

from HLTrigger.HLTfilters.hltHighLevel_cfi import *
## trigger
process.step1  = hltHighLevel.clone(
    TriggerResultsTag = "TriggerResults::HLT",
    HLTPaths = ["HLT_Mu9"]
)

# vertex filter
process.step2  = cms.EDFilter(
    "VertexSelector",
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string("!isFake && ndof > 4 && abs(z) < 15 && position.Rho < 2"),
    filter = cms.bool(True),
)

## tight muon selection
from PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi import *
process.step3a = countPatMuons.clone(
    src = 'tightMuons',
    minNumber = 1,
    maxNumber = 1
)

## loose muon selection
process.step3b = countPatMuons.clone(
    src = 'looseMuons',
    minNumber = 1,
    maxNumber = 1
)

## additional muon veto
process.step4  = countPatMuons.clone(
    src = 'vetoMuons',
    maxNumber = 1
)

## additional electron veto
from PhysicsTools.PatAlgos.selectionLayer1.electronCountFilter_cfi import *
process.step5  = countPatMuons.clone(
    src = 'vetoElectrons',
    maxNumber = 0
)

## jet selection bins
from PhysicsTools.PatAlgos.selectionLayer1.jetCountFilter_cfi import *
process.step6a = countPatJets.clone(src = 'goodJets', minNumber = 1)
process.step6b = countPatJets.clone(src = 'goodJets', minNumber = 2)
process.step6c = countPatJets.clone(src = 'goodJets', minNumber = 3)
process.step7  = countPatJets.clone(src = 'goodJets', minNumber = 4)


## ---
## selection monitoring
## ---

from PhysicsTools.PatExamples.PatTopSelectionAnalyzer_cfi import *
process.dummy = analyzePatTopSelection

process.TFileService = cms.Service("TFileService",
  fileName = cms.string('analyzePatTopSelection.root')
)

## ---
## paths
## ---

process.looseSequence = cms.Path(
    process.goodJets      *
    process.vetoElectrons *
    process.looseMuons    *
    process.vetoMuons     *
    process.dummy  * 
    process.step1  *
    process.step2  *
    process.step3b *
    process.step4  *
    process.step5  *
    process.step6a *
    process.step6b *
    process.step6c
    )

process.tightSequence = cms.Path(
    process.goodJets      *
    process.vetoElectrons *
    process.looseMuons    *
    process.tightMuons    *
    process.vetoMuons     *
    process.step1  *
    process.step2  *
    process.step3a *
    process.step4  *
    process.step5  *
    process.step6a *
    process.step6b *
    process.step6c *
    process.step7
    )
