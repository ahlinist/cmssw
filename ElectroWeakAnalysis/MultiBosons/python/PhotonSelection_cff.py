from ElectroWeakAnalysis.MultiBosons.PhotonSelection_cfi import *

from PhysicsTools.PatAlgos.patSequences_cff import *

photonETCut = photonPtrSelection.clone()
photonETCut.src = cms.InputTag('selectedPatPhotons')
photonETCut.cut = cms.string('et > 25')

selectedPhotons = photonPtrSelection.clone()
selectedPhotons.src = cms.InputTag('photonETCut')
selectedPhotons.cut = cms.string('')

vGammaPhotonSequence = cms.Sequence(patDefaultSequence*
                              photonETCut*
                              selectedPhotons)
