import FWCore.ParameterSet.Config as cms

faketracks = cms.EDProducer("PFPretendTrackProducer",
    debug = cms.int32(0),
    runinfo_cuts=cms.string("/castor/cern.ch/user/b/ballin/tbv5/testbeam_cuts.root")                           
)
