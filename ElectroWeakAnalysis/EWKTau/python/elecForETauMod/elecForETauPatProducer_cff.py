from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import *
from ElectroWeakAnalysis.EWKTau.elecForETauMod.elecForETauPatConfig_cfi import *
from ElectroWeakAnalysis.EWKTau.elecForETauMod.elecForETauPatSelectors_cfi import *
from ElectroWeakAnalysis.EWKTau.elecForETauMod.elecForETauPatAntiSelectors_cfi import *
from ElectroWeakAnalysis.EWKTau.elecForETauMod.elecForETauId_cff import *
from ElectroWeakAnalysis.EWKTau.elecForETauMod.elecForETauIso_cff import *
#from PhysicsTools.PatAlgos.mcMatchLayer0.mcMatchSequences_cff import *
from PhysicsTools.PatAlgos.triggerLayer0.trigMatchSequences_cff import *
electronTrigMatchCandHLT1ElectronStartup.src = "allLayer0ElecForETau"
electronTrigMatchHLT1Electron.src = "allLayer0ElecForETau"
electronTrigMatchHLT1ElectronRelaxed.src = "allLayer0ElecForETau"

patTrigMatchElecForETau = cms.Sequence(
                                        (patCandHLT1ElectronStartup * electronTrigMatchCandHLT1ElectronStartup)
	                               +(patHLT1ElectronRelaxed * electronTrigMatchHLT1ElectronRelaxed)
                                       +(patHLT1Electron * electronTrigMatchHLT1Electron) 
                                      )

patHighLevelRecoElecForETau = cms.Sequence(patElecIdForETau * patAODElectronIsolation * patLayer0ElectronIsolation)
layer0ElecForETau = cms.Sequence(allLayer0ElecForETau * patTrigMatchElecForETau * patHighLevelRecoElecForETau)
layer1ElecForETau = cms.Sequence(
                                  allLayer1ElecForETau
                                 *selectEtaElecForETau
                                 *selectKinElecForETau
                                 *selectMchHLTElecForETau
                                 *selectTrkIsoAfterMchHLTElecForETau
                                 *selectEcalIsoAfterTrkIsoElecForETau
                                 *selectHcalIsoAfterEcalIsoElecForETau
                                 *selectIdAfterIsoElecForETau
                                 *selectIpAfterIsoElecForETau
                                 *selectTrkIsoElecForETau
                                 *selectEcalIsoElecForETau
                                 *selectHcalIsoElecForETau
                                 *selectIdElecForETau
                                 *selectIpElecForETau                                
                              )

