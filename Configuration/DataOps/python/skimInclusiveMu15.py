import FWCore.ParameterSet.Config as cms

process = cms.Process('SKIM')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/EventContent/EventContent_cff')

# module to select events based on HLT Trigger bits
process.load('HLTrigger/HLTfilters/hltHighLevelDev_cfi')

# Loading "hltHighLevelDev_cfi" defines an EDFilter called hltHighLevelDev
# now we can configure it 

process.hltHighLevelDev.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT8E29")

process.hltHighLevelDev.HLTPaths = ("HLT_Mu9",)
process.hltHighLevelDev.andOr = True # True = OR, False = AND
process.hltHighLevelDev.HLTPathsPrescales  = cms.vuint32(1,)
process.hltHighLevelDev.HLTOverallPrescale = cms.uint32 (4)


process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('log/skimInclusiveMu15.log')
    )

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.123 $'),
    annotation = cms.untracked.string('skim'),
    name = cms.untracked.string('skim')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10000000)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#'file:/data/rossin/CMS/Triggers/Summer09/InclusiveMu15/GEN-SIM-RECO/MC_31X_V3-v1/0026/4EF3899B-0383-DE11-9996-001EC9AAA288.root',
#'file:/data/rossin/CMS/Triggers/Summer09/InclusiveMu15/GEN-SIM-RECO/MC_31X_V3-v1/0026/7A41229D-0383-DE11-9867-001EC9AA9158.root',
'file:/data/rossin/CMS/Triggers/Summer09/InclusiveMu15/GEN-SIM-RECO/MC_31X_V3-v1/0026/9848C69C-0383-DE11-BF03-00074305CEE0.root'
    )
)

process.hltTrigReport = cms.EDAnalyzer( "HLTrigReport",
    HLTriggerResults = cms.InputTag( 'TriggerResults','','HLT8E29' )
)


# Output definition
process.output1 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_Mu9_InclusiveMu15.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_Mu9')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim1')
                                      )
)


# the usage of trigger bits for selection is explained here:
## https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideEDMPathsAndTriggerBits#Selecting_Pass_for_Some_Trigger 

#process.pTfilter = cms.Path(process.ptHat_filter)
process.hlttrigreport = cms.Path(process.hltTrigReport)
process.skim1 = cms.Path(process.hltHighLevelDev)


process.out_step = cms.EndPath(process.output1)

process.schedule = cms.Schedule(process.hlttrigreport,process.skim1)
process.schedule.append(process.out_step)

