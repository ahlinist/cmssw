import FWCore.ParameterSet.Config as cms

configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/Spring08Production/data/iCSA08_TrackerLaser.cff,v $'),
    annotation = cms.untracked.string('Simulation of the Tracker Laser Alignment System for CSA08')
)
source = cms.Source("LaserAlignmentSource",
    firstRun = cms.untracked.uint32(1)
)


