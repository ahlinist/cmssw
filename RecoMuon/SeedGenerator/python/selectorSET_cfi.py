import FWCore.ParameterSet.Config as cms

# The services
from RecoMuon.TrackingTools.MuonServiceProxy_cff import *
# needed for the SET algo
#from RecoMuon.MuonSeedGenerator.ptSeedParameterization_cfi import *
from RecoMuon.MuonSeedGenerator.ptSeedParameterization_38T_cfi import *
#MuonSeedTester  = cms.EDProducer("MuonSeedTester",
SETMuonSeed  = cms.EDFilter("SETMuonProducer",
#selectorSET = cms.EDProducer("SETProducer",
    MuonServiceProxy,
    SETTrajBuilderParameters = cms.PSet(
        ptSeedParameterization, 
#        UseBWLightFit = cms.bool(True),
        UseBWLightFit = cms.bool(False),
        Apply_prePruning = cms.bool(True),
        EnableRPCMeasurement = cms.bool(True),
        FilterParameters = cms.PSet(
            FitDirection = cms.string('insideOut'),
            DTRecSegmentLabel = cms.InputTag("dt4DSegments"),
            EnableRPCMeasurement = cms.bool(True),
            CSCRecSegmentLabel = cms.InputTag("cscSegments"),
            EnableDTMeasurement = cms.bool(True),
            RPCRecSegmentLabel = cms.InputTag("rpcRecHits"),
            Propagator = cms.string('SteppingHelixPropagatorAny'),
            EnableCSCMeasurement = cms.bool(True)
        )
    )
)



