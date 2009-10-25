
import FWCore.ParameterSet.Config as cms




TauAnalysisRecoToolsEC = cms.PSet(
    outputCommands = cms.untracked.vstring(
    'keep *_selectedLayer1Taus_*_*',
    'keep *_selectedLayer1Electrons_*_*',
    'keep *_selectedLayer1Muons_*_*',
    'keep *_layer1METs_*_*',
    'keep *_recoDiTau_*_*',
    'keep *_recoZll_*_*'
    )
)
