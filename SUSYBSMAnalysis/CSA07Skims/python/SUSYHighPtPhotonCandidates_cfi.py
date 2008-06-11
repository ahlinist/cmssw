import FWCore.ParameterSet.Config as cms

from SimGeneral.HepPDTESSource.pythiapdt_cfi import *
from PhysicsTools.RecoAlgos.allTracks_cfi import *
#module allTracks = ChargedCandidateProducer {
#  InputTag src = ctfWithMaterialTracks
#  string particleType = "pi+"
#}
allPhotons = cms.EDProducer("PhotonShallowCloneProducer",
    src = cms.InputTag("photons")
)

allPhotonIsolations = cms.EDProducer("CandPtIsolationProducer",
    src = cms.InputTag("allPhotons"),
    d0Max = cms.double(1000000.0),
    dRMin = cms.double(0.015),
    dRMax = cms.double(0.2),
    elements = cms.InputTag("allTracks"),
    ptMin = cms.double(1.5),
    dzMax = cms.double(1000000.0)
)


