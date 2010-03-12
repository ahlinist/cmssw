
import FWCore.ParameterSet.Config as cms

process = cms.Process("DQM")

#
#  DQM SERVICES
#
process.load("DQMServices.Core.DQM_cfg")

#
#  DQM SOURCES
#
process.load("CondCore.DBCommon.CondDBSetup_cfi")

process.load("Configuration.StandardSequences.Geometry_cff")

process.load("DQMServices.Components.DQMEnvironment_cfi")

process.load("DQMOffline.Trigger.FourVectorHLTOffline_cfi")
process.load("DQMOffline.Trigger.FourVectorHLTOfflineClient_cfi")
process.load("DQMOffline.Trigger.JetMETHLTOfflineSource_cfi")
process.load("DQMOffline.Trigger.HLTJetMETQualityTester_cfi")
# configure HLT
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('40 OR 41')

from DQMOffline.Trigger.FourVectorHLTOfflineClient_cfi import *
from DQMOffline.Trigger.JetMETHLTOfflineClient_cfi import *

process.load("DQMServices.Components.DQMStoreStats_cfi")

process.hltclient = cms.Sequence(hltFourVectorClient)

hltFourVectorClient.prescaleLS = cms.untracked.int32(-1)
hltFourVectorClient.monitorDir = cms.untracked.string('')
hltFourVectorClient.prescaleEvt = cms.untracked.int32(1)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.source = cms.Source("PoolSource",
    fileNames =
cms.untracked.vstring('/store/relval/CMSSW_3_3_6/RelValQCD_Pt_80_120/GEN-SIM-RECO/MC_3XY_V9A-v1/0009/E42A8884-3DE4-DE11-979E-002618943986.root')
)

process.MessageLogger = cms.Service("MessageLogger",
    detailedInfo = cms.untracked.PSet(
        threshold = cms.untracked.string('DEBUG')
    ),
    critical = cms.untracked.PSet(
        threshold = cms.untracked.string('ERROR')
    ),
    debugModules = cms.untracked.vstring('*'),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING'),
        WARNING = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        noLineBreaks = cms.untracked.bool(True)
    ),
    destinations = cms.untracked.vstring('detailedInfo', 
        'critical', 
        'cout')
)



process.psource = cms.Path(process.hltLevel1GTSeed*process.jetMETHLTOfflineSource*process.hltJetMETOfflineQualityTests*process.dqmStoreStats) 

process.p = cms.EndPath(process.dqmSaver)
process.DQMStore.verbose = 0
process.DQM.collectorHost = ''
process.dqmSaver.convention = 'Online'
process.dqmSaver.saveByRun = 1
process.dqmSaver.saveAtJobEnd = True


