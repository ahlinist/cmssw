import FWCore.ParameterSet.Config as cms

from  ElectroWeakAnalysis.EWKTau.triggerLayer.patTrigMatcher_cfi import *

from ElectroWeakAnalysis.EWKTau.triggerLayer.patTrigMatcher_cfi import *

electronTrigMatchHLT1Electron.src = "allLayer0ElecForETau"
electronTrigMatchHLT1ElectronRelaxed.src = "allLayer0ElecForETau"
electronTrigMatchHLTXElectronTau.src = "allLayer0ElecForETau"
electronTrigMatchHLTXElectronMuon.src = "allLayer0ElecForETau"

muonTrigMatchHLT1MuonIso.src = "allLayer0MuForEWKTau"
muonTrigMatchHLT1MuonNonIso.src = "allLayer0MuForEWKTau"
muonTrigMatchHLTXMuonTau.src = "allLayer0MuForEWKTau"
muonTrigMatchHLTXElectronMuon.src = "allLayer0MuForEWKTau"

patTrigMatchElecForETau = cms.Sequence(
                                      patTrigMatchHLT1Electron
                                     +patTrigMatchHLT1ElectronRelaxed
                                     +(patHLTXElectronTau *electronTrigMatchHLTXElectronTau)
                                     +(patHLTXElectronMuon * electronTrigMatchHLTXElectronMuon)


)

patTrigMatchMuForEWKTau = cms.Sequence(

                                       patTrigMatchHLT1MuonNonIso 
                                      +patTrigMatchHLT1MuonIso 
                                      +(patHLTXMuonTau * muonTrigMatchHLTXMuonTau)
                                      +(patHLTXElectronMuon * muonTrigMatchHLTXElectronMuon) 

)






