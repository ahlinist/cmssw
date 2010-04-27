import FWCore.ParameterSet.Config as cms

#Retrieve Z selection from inclusive analysis
from ElectroWeakAnalysis.Skimming.zMuMu_SubskimPaths_cff import *

dimuonsVisitor = cms.ExpandVisitor(cms.Sequence)
dimuonsPath.visit(dimuonsVisitor)

zMuMuSequence = dimuonsVisitor.result()
if zMuMuSequence.remove(dimuonsFilter):
    print 'zMuMuGamma: Removed dimuonsFilter from zMuMuSequence'

dimuonsOneTrackVisitor = cms.ExpandVisitor(cms.Sequence)
dimuonsOneTrackPath.visit(dimuonsOneTrackVisitor)

zMuMuOneTrackSequence = dimuonsOneTrackVisitor.result()
if zMuMuOneTrackSequence.remove(dimuonsOneTrackFilter):
    print 'zMuMuGamma: Removed dimuonsOneTrackFilter from zMuMuOneTrackSequence'
    
#Retrieve photon selection
from ElectroWeakAnalysis.MultiBosons.PhotonSelection_cff import *

#define and configure Zgamma Selection
from ElectroWeakAnalysis.MultiBosons.VGammaSelection_cfi import *

ZMuMuGammaCandidates = VGammaProducer.clone()
ZMuMuGammaCandidates.decay = cms.string('dimuonsGlobal selectedPhotons')
ZMuMuGammaCandidates.cut = cms.string('mass > 0')

ZMuMuGammaCandidatesOneTrack = VGammaProducer.clone()
ZMuMuGammaCandidatesOneTrack.decay = cms.string('dimuonsOneTrack selectedPhotons')
ZMuMuGammaCandidatesOneTrack.cut = cms.string('mass > 0')

ZMuMuGammaCandidatesSeq = cms.Sequence(ZMuMuGammaCandidates)
ZMuMuGammaCandidatesOneTrackSeq = cms.Sequence(ZMuMuGammaCandidatesOneTrack)

#define workflow
ZMuMuGammaWorkflow = cms.Sequence(vGammaPhotonSequence*zMuMuSequence*ZMuMuGammaCandidatesSeq +
                                  vGammaPhotonSequence*zMuMuOneTrackSequence*ZMuMuGammaCandidatesOneTrackSeq)
