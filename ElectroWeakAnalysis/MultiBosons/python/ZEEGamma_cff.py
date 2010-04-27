import FWCore.ParameterSet.Config as cms

#Retrieve Z selection from inclusive analysis (when they put it in CVS?)

#Retrieve Photon Selection
from ElectroWeakAnalysis.MultiBosons.PhotonSelection_cff import *

#define and configure Zgamma Selection
from ElectroWeakAnalysis.MultiBosons.VGammaSelection_cfi import *

ZEEGammaCandidates = VGammaSelection.clone()
ZEEGammaCandidates.decay = cms.string('ZEECandidates photons')
ZEEGammaCandidates.cut = cms.string('mass > 0')

ZEEGammaCandidatesSeq = cms.Sequence(ZEEGammaCandidates)

#setup sequences
ZEEGammaWorkflow = cms.Sequence(vGammaPhotonSequence*ZEEGammaCandidatesSeq)
