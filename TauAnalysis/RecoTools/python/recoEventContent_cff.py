import FWCore.ParameterSet.Config as cms

TauAnalysisRecoToolsEC = cms.PSet(
    outputCommands = cms.untracked.vstring(
        'keep *_selectedPatTaus_*_*',
        'keep *_selectedPatElectrons_*_*',
        'keep *_selectedPatMuons_*_*',
        'keep *_patMETs_*_*',
        'keep *_recoDiTau_*_*',
        'keep *_recoZll_*_*'
    )
)
