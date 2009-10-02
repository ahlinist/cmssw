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

process.hltHighLevelDev.HLTPaths = ("HLT_Ele10_LW_L1R",)
process.hltHighLevelDev.andOr = True # True = OR, False = AND
process.hltHighLevelDev.HLTPathsPrescales  = cms.vuint32(10,)
process.hltHighLevelDev.HLTOverallPrescale = cms.uint32(1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev2 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev2.HLTPaths = ("HLT_Ele10_LW_EleId_L1R",)
process.hltHighLevelDev2.andOr = True # True = OR, False = AND
process.hltHighLevelDev2.HLTPathsPrescales  = cms.vuint32(1,)
process.hltHighLevelDev2.HLTOverallPrescale = cms.uint32(1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev3 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev3.HLTPaths = ("HLT_Ele15_LW_L1R","HLT_Ele15_SC10_LW_L1R","HLT_Ele20_LW_L1R")
process.hltHighLevelDev3.andOr = True # True = OR, False = AND
process.hltHighLevelDev3.HLTPathsPrescales  = cms.vuint32(1,1,1)
process.hltHighLevelDev3.HLTOverallPrescale = cms.uint32(1)

process.hltHighLevelDev4 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev4.HLTPaths = ("HLT_Photon15_L1R",)
process.hltHighLevelDev4.andOr = True # True = OR, False = AND
process.hltHighLevelDev4.HLTPathsPrescales  = cms.vuint32(5,)
process.hltHighLevelDev4.HLTOverallPrescale = cms.uint32(1)

process.hltHighLevelDev5 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev5.HLTPaths = ("HLT_Photon20_L1R","HLT_Photon30_L1R_8E29")
process.hltHighLevelDev5.andOr = True # True = OR, False = AND
process.hltHighLevelDev5.HLTPathsPrescales  = cms.vuint32(1,1)
process.hltHighLevelDev5.HLTOverallPrescale = cms.uint32(1)

process.hltHighLevelDev6 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev6.HLTPaths = ("HLT_SingleLooseIsoTau20",)
process.hltHighLevelDev6.andOr = True # True = OR, False = AND
process.hltHighLevelDev6.HLTPathsPrescales  = cms.vuint32(5,)
process.hltHighLevelDev6.HLTOverallPrescale = cms.uint32(1)

process.hltHighLevelDev7 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev7.HLTPaths = ("HLT_DoubleLooseIsoTau15",)
process.hltHighLevelDev7.andOr = True # True = OR, False = AND
process.hltHighLevelDev7.HLTPathsPrescales  = cms.vuint32(4,)
process.hltHighLevelDev7.HLTOverallPrescale = cms.uint32(1)


#############   pt_hat Filter  #####
process.ptHat_filter = cms.EDFilter("MCProcessFilter",
    ProcessID = cms.untracked.vint32(0),
    MinPthat = cms.untracked.vdouble(-1.0),
    MaxPthat = cms.untracked.vdouble(100000.)
)


process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('log/skimQCD_EMEnriched_Pt80to170.log')
    )

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.123 $'),
    annotation = cms.untracked.string('skim'),
    name = cms.untracked.string('skim')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(228000000)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'file:/data/rossin/CMS/Triggers/Summer09/QCD_EMEnriched_Pt80to170/GEN-SIM-RECO/MC_31X_V3-v1/0011/FC9054CA-6C89-DE11-A7CA-003048772DC2.root'
    )
)

process.hltTrigReport = cms.EDAnalyzer( "HLTrigReport",
    HLTriggerResults = cms.InputTag( 'TriggerResults','','HLT8E29' )
)


# Output definition
process.output1 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_Ele10_QCD_EMEnriched_Pt80to170.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_Ele10')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                   SelectEvents = cms.vstring('skim1') #defining the skim.
                                      )
)

# Yet another output definition
process.output2 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_Ele10_EleId_QCD_EMEnriched_Pt80to170.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_Ele10_EleId')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim2')
                                      )
)

# Yet another output definition
process.output3 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_Ele15_QCD_EMEnriched_Pt80to170.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_Ele15')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim3')
                                      )
)

# Yet another output definition
process.output4 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_Photon15_QCD_EMEnriched_Pt80to170.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_Photon15')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim4')
                                      )
)

# Yet another output definition
process.output5 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_Photon20_QCD_EMEnriched_Pt80to170.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_Photon20')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim5')
                                      )
)

# Yet another output definition
process.output6 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_SingleLooseTau20_QCD_EMEnriched_Pt80to170.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_SingleLooseTau20')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim6')
                                      )
)

# Yet another output definition
process.output7 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_DoubleLooseTau15_QCD_EMEnriched_Pt80to170.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_DoubleLooseTau15')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim7')
                                      )
)

# the usage of trigger bits for selection is explained here:
## https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideEDMPathsAndTriggerBits#Selecting_Pass_for_Some_Trigger 

#process.pTfilter = cms.Path(process.ptHat_filter)
process.hlttrigreport = cms.Path(process.ptHat_filter+process.hltTrigReport)
process.skim1 = cms.Path(process.ptHat_filter+process.hltHighLevelDev )
process.skim2 = cms.Path(process.ptHat_filter+process.hltHighLevelDev2)
process.skim3 = cms.Path(process.ptHat_filter+process.hltHighLevelDev3)
process.skim4 = cms.Path(process.ptHat_filter+process.hltHighLevelDev4)
process.skim5 = cms.Path(process.ptHat_filter+process.hltHighLevelDev5)
process.skim6 = cms.Path(process.ptHat_filter+process.hltHighLevelDev6)
process.skim7 = cms.Path(process.ptHat_filter+process.hltHighLevelDev7)


process.out_step = cms.EndPath(process.output1+process.output2+process.output3+process.output4+process.output5+process.output6+process.output7)

process.schedule = cms.Schedule(process.hlttrigreport,process.skim1,process.skim2,process.skim3,process.skim4,process.skim5,process.skim6,process.skim7)
process.schedule.append(process.out_step)

