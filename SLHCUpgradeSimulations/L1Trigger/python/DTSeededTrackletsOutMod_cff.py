import FWCore.ParameterSet.Config as cms

Out = cms.OutputModule(
  "PoolOutputModule",
  outputCommands = cms.untracked.vstring('drop *', 
                                         'keep *_DTL1slhcProd_*_*',
                                         'drop *_DTL1slhcProd_DTTFMuSorterProd_*',
                                         'keep reco*_genParticles_*_*',
                                         'keep *_TriggerResults_*_MuonsUpgrade'),
  fileName = cms.untracked.string('')
  )

HLTOut = cms.OutputModule(
  "PoolOutputModule",
  outputCommands = cms.untracked.vstring('drop *', 
                                         'keep *_DTL1slhcProd_*_*',
                                         'drop *_*_DTTFMuSorterProd_*',
                                         'keep reco*_genParticles_*_*',
                                         'keep *_TriggerResults_*_MuonsUpgrade'),
  SelectEvents = cms.untracked.PSet(
     SelectEvents = cms.vstring('HLT1MuonIso', 
                                'HLT1MuonNonIso', 
                                'HLT2MuonNonIso', 
                                'HLT1Electron', 
                                'HLT1ElectronRelaxed', 
                                'HLT2Electron', 
                                'HLT2ElectronRelaxed', 
                                'HLTXElectronMuon', 
                                'HLTXElectronMuonRelaxed', 
                                'HLTXElectronTau', 
                                'HLTXMuonTau', 
                                'HLT1Tau', 
                                'HLT1Tau1MET', 
                                'HLT2TauPixel')
     ),
     fileName = cms.untracked.string('')
  )



