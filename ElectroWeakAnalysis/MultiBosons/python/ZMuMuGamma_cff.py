#Muon Selection
from ElectroWeakAnalysis.MultiBosons.MuonSelection_cfi import *

goodGblMuons = muonPtrSelection.clone()
goodGblMuons.src = cms.InputTag('muons')
goodGblMuons.cut = cms.string('isGlobalMuon & pt > 0')

goodGblMuonsTkReq = muonPtrSelection.clone()
goodGblMuonsTkReq.src = cms.InputTag('goodGblMuons')
goodGblMuonsTkReq.cut = cms.string('innerTrack().found() >= 10')

goodGblMuonsIso = muonPtrSelection.clone()
goodGblMuonsIso.src = cms.InputTag('goodGblMuonsTkReq')
goodGblMuonsIso.cut = cms.string('isolationR03().emEt < 3 & isolationR03().hadEt < 5 & isolationR03().sumPt < 5')

selectedGblMuons = muonSelection.clone()
selectedGblMuons.src = cms.InputTag('goodGblMuonsIso')

goodGblAndTkMuons = muonPtrSelection.clone()
goodGblAndTkMuons.src = cms.InputTag('muons')
goodGblAndTkMuons.cut = cms.string('(isGlobalMuon | isTrackerMuon) & pt > 0')

goodGblAndTkMuonsTkReq = muonPtrSelection.clone()
goodGblAndTkMuonsTkReq.src = cms.InputTag('goodGblAndTkMuons')
goodGblAndTkMuonsTkReq.cut = cms.string('innerTrack().found() >= 10')

goodGblAndTkMuonsIso = muonPtrSelection.clone()
goodGblAndTkMuonsIso.src = cms.InputTag('goodGblAndTkMuonsTkReq')
goodGblAndTkMuonsIso.cut = cms.string('isolationR03().emEt < 3 & isolationR03().hadEt < 5 & isolationR03().sumPt < 5')

selectedGblAndTkMuons = muonSelection.clone()
selectedGblAndTkMuons.src = cms.InputTag('goodGblAndTkMuonsIso')

muonSelectionSeq = cms.Sequence(goodGblMuons*goodGblMuonsTkReq*goodGblMuonsIso*selectedGblMuons+
                                goodGblAndTkMuons*goodGblAndTkMuonsTkReq*goodGblAndTkMuonsIso*selectedGblAndTkMuons)

#define and configure Z selection
from ElectroWeakAnalysis.MultiBosons.ZSelectionChargedLeptons_cfi import *

#Z -> Mu MU
ZMuMuCandidatesGG = ZProducerChargedLeptons.clone()
ZMuMuCandidatesGG.decay = cms.string('selectedGblMuons@+ selectedGblMuons@-')
ZMuMuCandidatesGG.cut = cms.string('mass > 0')

zMuMuCandidatesGGMassCut = ZPtrSelectionChargedLeptons.clone()
zMuMuCandidatesGGMassCut.src = cms.InputTag('ZMuMuCandidatesGG')
zMuMuCandidatesGGMassCut.cut = cms.string('mass > 20')

selectedZMuMuCandidatesGG = ZSelectionChargedLeptons.clone()
selectedZMuMuCandidatesGG.src = cms.InputTag('zMuMuCandidatesGGMassCut')
selectedZMuMuCandidatesGG.cut = cms.string('')

ZMuMuCandidatesGT = ZProducerChargedLeptons.clone()
ZMuMuCandidatesGT.decay = cms.string('selectedGblAndTkMuons@+ selectedGblAndTkMuons@-')
ZMuMuCandidatesGT.cut = cms.string('mass > 0 & (muDaughter1().isGlobalMuon | muDaughter2().isGlobalMuon)')

ZMuMuCandidatesSeq = cms.Sequence(ZMuMuCandidatesGG*zMuMuCandidatesGGMassCut*selectedZMuMuCandidatesGG+
                                  ZMuMuCandidatesGT)

#define and configure Zgamma Selection
from ElectroWeakAnalysis.MultiBosons.ZGammaSelectionChargedLeptons_cfi import *

ZMuMuGammaCandidatesGG = ZGammaProducerChargedLeptons.clone()
ZMuMuGammaCandidatesGG.decay = cms.string('selectedZMuMuCandidatesGG photons')
ZMuMuGammaCandidatesGG.cut = cms.string('mass > 0')

ZMuMuGammaCandidatesGT = ZGammaProducerChargedLeptons.clone()
ZMuMuGammaCandidatesGT.decay = cms.string('ZMuMuCandidatesGT photons')
ZMuMuGammaCandidatesGT.cut = cms.string('mass > 0')

ZMuMuGammaCandidatesSeq = cms.Sequence(ZMuMuGammaCandidatesGG+ZMuMuGammaCandidatesGT)

#define workflow
ZMuMuGammaWorkflow = cms.Sequence(muonSelectionSeq*ZMuMuCandidatesSeq*ZMuMuGammaCandidatesSeq)
