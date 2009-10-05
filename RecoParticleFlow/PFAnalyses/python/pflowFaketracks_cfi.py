import FWCore.ParameterSet.Config as cms

from RecoParticleFlow.PFAnalyses.RunDict import *

faketracks = cms.EDProducer("PFPretendTrackProducer",
    debug = cms.int32(0),
    runinfo_cuts=cms.string(testbeam_cuts),
    justCreateEmptyCollections = cms.bool(False),
    #If you're simulating the endcap experiment, set this to true
    #hopefully temporary <sigh>
    endcapMode = cms.bool(False)
)
