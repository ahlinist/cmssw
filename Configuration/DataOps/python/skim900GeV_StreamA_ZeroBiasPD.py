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

# All events from Zero PD
process.hltHighLevelDev.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
process.hltHighLevelDev.HLTPaths = (
   'HLT_PhysicsDeclared',
    )

process.hltHighLevelDev.andOr = True # True = OR, False = AND
process.hltHighLevelDev.throw = False # throw exception on unknown path names
process.hltHighLevelDev.HLTPathsPrescales  = cms.vuint32(
   1,    #'HLT_PhysicsDeclared',
    )
process.hltHighLevelDev.HLTOverallPrescale = cms.uint32 (1)

# All events from Zero PD, prescaled by a factor 10
process.hltHighLevelDev2 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev2.HLTPaths = (
   'HLT_PhysicsDeclared',
    )

process.hltHighLevelDev2.andOr = True # True = OR, False = AND
process.hltHighLevelDev2.throw = False # throw exception on unknown path names
process.hltHighLevelDev2.HLTPathsPrescales  = cms.vuint32(
   1,    #'HLT_PhysicsDeclared',
    )
process.hltHighLevelDev2.HLTOverallPrescale = cms.uint32 (10)



process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('log/skimMinBias900GeV_PhysicsDeclaredRequest.log')
    )

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.123 $'),
    annotation = cms.untracked.string('skim'),
    name = cms.untracked.string('skim')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    'file:/data/rossin/CMS/store/mc/Summer09/MinBias/GEN-SIM-RAW/STARTUP3X_V8D_900GeV-v1/outputA_4_PhysDecl.root',
#    'file:/data/rossin/CMS/store/mc/Summer09/MinBias/GEN-SIM-RAW/STARTUP3X_V8D_900GeV-v1/outputA_3.root',
#    'file:/data/rossin/CMS/store/mc/Summer09/MinBias/GEN-SIM-RAW/STARTUP3X_V8D_900GeV-v1/outputA_4.root',
#    'file:/data/rossin/CMS/store/mc/Summer09/MinBias/GEN-SIM-RAW/STARTUP3X_V8D_900GeV-v1/outputA_5.root',
#    'file:/data/rossin/CMS/store/mc/Summer09/MinBias/GEN-SIM-RAW/STARTUP3X_V8D_900GeV-v1/outputA_6.root'
    )
)

process.hltTrigReport = cms.EDAnalyzer( "HLTrigReport",
    HLTriggerResults = cms.InputTag( 'TriggerResults','','HLT' )
)



# All zerobias
process.output1 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_AllZeroBias_900GeV_PhysDecl.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('FEVT'),
                                      filterName = cms.untracked.string('SD_AllZeroBias')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim1')
                                      )
)

# 10% of zerobias
process.output2 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_ZeroBias10_900GeV_PhysDecl.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('FEVT'),
                                      filterName = cms.untracked.string('SD_ZeroBias10')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim2')
                                      )
)

# the usage of trigger bits for selection is explained here:
## https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideEDMPathsAndTriggerBits#Selecting_Pass_for_Some_Trigger 

#process.pTfilter = cms.Path(process.ptHat_filter)
process.hlttrigreport = cms.Path(process.hltTrigReport)
process.skim1 = cms.Path(process.hltHighLevelDev)
process.skim2 = cms.Path(process.hltHighLevelDev2)


##process.out_step = cms.EndPath(process.output1+process.output2+process.output3+process.output4+process.output5+process.output6+process.output7+process.output8+process.output9+process.output10)
process.out_step = cms.EndPath(process.output1+process.output2)

#process.schedule = cms.Schedule(process.hlttrigreport,process.hlttrigreport2,process.skim1,process.skim2,process.skim3,process.skim4,process.skim5,process.skim6,process.skim7,process.skim8,process.skim9,process.skim10)
process.schedule = cms.Schedule(process.hlttrigreport,process.skim1,process.skim2)
process.schedule.append(process.out_step)
