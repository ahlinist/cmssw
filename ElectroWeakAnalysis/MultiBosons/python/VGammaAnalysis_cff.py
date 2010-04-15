import FWCore.ParameterSet.Config as cms

#Muon Selection
from ElectroWeakAnalysis.MultiBosons.MuonSelection_cfi import *

goodGlobalMuons = muonSelection.clone()
goodGlobalMuons.cut = cms.string('isGlobalMuon & pt > 0 & innerTrack().found() >= 15')

goodGlobalAndTrackerMuons = muonSelection.clone()
goodGlobalAndTrackermuons.cut = cms.string('isGlobalMuon | isTrackerMuon & pt > 0 & innerTrack().found() >= 15')

#Electron Selection
from ElectroWeakAnalysis.MultiBosons.ElectronSelection_cfi import *

goodElectrons = electronSelection.clone()
goodElectrons.cut = cms.string('et > 0')

#define and configure Z selection
from ElectroWeakAnalysis.MultiBosons.ZSelectionChargedLeptons_cfi import *

#Z -> Mu MU
ZMuMuCandidates = ZSelection.clone()
ZMuMuCandidates.decay = cms.string('goodGlobalMuons@+ goodGlobalMuons@-')
ZMuMuCandidates.cut = cms.string('mass > 0')

#Z -> E E
ZEECandidates = ZSelection.clone()
ZEECandidates.decay = cms.string('goodElectrons@+ goodElectrons@-')
ZEECandidates.cut = cms.string('mass > 0')

#define and configure Zgamma Selection
from ElectroWeakAnalysis.MultiBosons.ZGammaSelectionChargedLeptons_cfi import *

ZMuMuGammaCandidates = ZGammaSelectionChargedLeptons.clone()
ZMuMuGammaCandidates.decay = cms.string('ZMuMuCandidates photons')
ZMuMuGammaCandidates.cut = cms.string('mass > 0')

ZEEGammaCandidates = ZGammaSelectionChargedLeptons.clone()
ZEEGammaCandidates.decay = cms.string('ZEECandidates photons')
ZEEGammaCandidates.cut = cms.string('mass > 0')

#setup sequences
ZGammaWorkflowChargedLeptons = cms.Sequence(ZMuMuCandidates*ZMuMUGammaCandidates+
                                            ZEECandidates*ZEEGammaCandidates)

VGammaAnalysis = cms.Sequence(ZGammaWorkFlowChargedLeptons)
                             

