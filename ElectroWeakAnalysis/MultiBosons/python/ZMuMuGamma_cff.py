#Muon Selection
from ElectroWeakAnalysis.MultiBosons.MuonSelection_cfi import *

goodGlobalMuons = muonSelection.clone()
goodGlobalMuons.cut = cms.string('isGlobalMuon & pt > 0')

goodGlobalMuonsTkTrackRequirement = muonSelection.clone()
goodGlobalMuonsTkTrackRequirement.cut = cms.string('innerTrack().found() >= 10')

goodGlobalAndTrackerMuons = muonSelection.clone()
goodGlobalAndTrackerMuons.cut = cms.string('isGlobalMuon | isTrackerMuon & pt > 0')

goodGlobalAndTrackerMuonsTkTrackRequirement = muonSelection.clone()
goodGlobalAndTrackerMuonsTkTrackRequirement.cut = cms.string('innerTrack().found() >= 10')

muonSelectionSeq = cms.Sequence(goodGlobalMuons*goodGlobalMuonsTkTrackRequirement+
                                goodGlobalAndTrackerMuons*goodGlobalAndTrackerMuonsTkTrackRequirement)

#define and configure Z selection
from ElectroWeakAnalysis.MultiBosons.ZSelectionChargedLeptons_cfi import *

#Z -> Mu MU
ZMuMuCandidates_gg = ZSelection.clone()
ZMuMuCandidates_gg.decay = cms.string('goodGlobalMuons@+ goodGlobalMuons@-')
ZMuMuCandidates_gg.cut = cms.string('mass > 0')

ZMuMuCandidates_gt = ZSelection.clone()
ZMuMuCandidates_gt.decay = cms.string('goodGlobalAndTrackerMuons@+ goodGlobalandTrackerMuons@-')
ZMuMuCandidates_gt.cut = cms.string('mass > 0 & (muDaughter1().isGlobalMuon | muDaughter2().isGlobalMuon)')

ZMuMuCandidatesSeq = cms.Sequence(ZMuMuCandidates_gg+ZMuMuCandidates_gt)

#define and configure Zgamma Selection
from ElectroWeakAnalysis.MultiBosons.ZGammaSelectionChargedLeptons_cfi import *

ZMuMuGammaCandidates_gg = ZGammaSelectionChargedLeptons.clone()
ZMuMuGammaCandidates_gg.decay = cms.string('ZMuMuCandidates_gg photons')
ZMuMuGammaCandidates_gg.cut = cms.string('mass > 0')

ZMuMuGammaCandidates_gt = ZGammaSelectionChargedLeptons.clone()
ZMuMuGammaCandidates_gt.decay = cms.string('ZMuMuCandidates_gt photons')
ZMuMuGammaCandidates_gt.cut = cms.string('mass > 0')

ZMuMuGammaCandidatesSeq = cms.Sequence(ZMuMuGammaCandidates_gg+ZMuMuGammaCandidates_gt)

#define workflow
ZMuMuGammaWorkflow = cms.Sequence(muonSelectionSeq*ZMuMuCandidatesSeq*ZMuMuGammaCandidatesSeq)
