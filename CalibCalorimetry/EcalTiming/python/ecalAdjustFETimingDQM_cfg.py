import FWCore.ParameterSet.Config as cms

process = cms.Process("EcalAdjustFETimingDQM")

# Global Tag -- for geometry
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = 'GR_R_43_V3::All'
process.GlobalTag.globaltag = 'GR_R_44_V1::All'

process.load("Configuration.StandardSequences.Geometry_cff")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.source = cms.Source("EmptySource",
       numberEventsInRun = cms.untracked.uint32(1),
       firstRun = cms.untracked.uint32(888888), # Use last IOV for event setup info
)

#process.load("FWCore.MessageService.MessageLogger_cfi")


# For the DQM files, see: https://cmsweb.cern.ch/dqm/online/data/browse/Original
# and /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/data/OnlineData/original
process.adjustTiming = cms.EDAnalyzer('EcalAdjustFETimingDQM',
       EBDQMFileName = cms.string("DQM_V0001_EcalBarrel_R000177140.root"),
       EEDQMFileName = cms.string("DQM_V0001_EcalEndcap_R000177140.root"),
       XMLFileNameBeg = cms.string("towerDelaysAdjusted_"),
       TextFileName = cms.string("adjustmentsToTowers.txt"),
       RootFileNameBeg = cms.string("ecalAdjustFETimingDQM."),
       ReadExistingDelaysFromDB = cms.bool(False) # True requires running at P5 (and probably ecalpro access)
)


process.p = cms.Path(process.adjustTiming)
