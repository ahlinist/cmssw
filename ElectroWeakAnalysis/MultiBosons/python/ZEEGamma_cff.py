#Electron Selection
from ElectroWeakAnalysis.MultiBosons.ElectronSelection_cfi import *

goodElectrons = electronSelection.clone()
goodElectrons.cut = cms.string('et > 0')

#define and configure Z selection
from ElectroWeakAnalysis.MultiBosons.ZSelectionChargedLeptons_cfi import *

#Z -> E E
ZEECandidates = ZSelectionChargedLeptons.clone()
ZEECandidates.decay = cms.string('goodElectrons@+ goodElectrons@-')
ZEECandidates.cut = cms.string('mass > 0')

ZEECandidatesSeq = cms.Sequence(ZEECandidates)

#define and configure Zgamma Selection
from ElectroWeakAnalysis.MultiBosons.ZGammaSelectionChargedLeptons_cfi import *

ZEEGammaCandidates = ZGammaSelectionChargedLeptons.clone()
ZEEGammaCandidates.decay = cms.string('ZEECandidates photons')
ZEEGammaCandidates.cut = cms.string('mass > 0')

ZEEGammaCandidatesSeq = cms.Sequence(ZEEGammaCandidates)

#setup sequences
ZEEGammaWorkflow = cms.Sequence(ZEECandidatesSeq*ZEEGammaCandidatesSeq)
