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

process.hltHighLevelDev.HLTPaths = ("HLT_BTagIP_Jet50U","HLT_BTagMu_Jet10U","HLT_StoppedHSCP_8E29")
process.hltHighLevelDev.andOr = True # True = OR, False = AND
process.hltHighLevelDev.HLTPathsPrescales  = cms.vuint32(1,1,1)
process.hltHighLevelDev.HLTOverallPrescale = cms.uint32 (1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev2 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev2.HLTPaths = ("HLT_Mu3","HLT_Mu5","HLT_Mu9","HLT_IsoMu3","HLT_L2Mu9","HLT_L2Mu11","HLT_L1Mu20","HLT_L1Mu14_L1SingleEG10","HLT_L1Mu14_L1SingleJet6U","HLT_L1Mu14_L1ETM30","HLT_L1DoubleMuOpen","HLT_DoubleMu0","HLT_DoubleMu3",)
process.hltHighLevelDev2.andOr = True # True = OR, False = AND
process.hltHighLevelDev2.HLTPathsPrescales  = cms.vuint32(1,1,1,1,1,1,1,1,1,1,1,1,1)
process.hltHighLevelDev2.HLTOverallPrescale = cms.uint32 (1)

process.hltHighLevelDev3 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev3.HLTPaths = ("HLT_Mu9",)
process.hltHighLevelDev3.andOr = True # True = OR, False = AND
process.hltHighLevelDev3.HLTPathsPrescales  = cms.vuint32(1,)
process.hltHighLevelDev3.HLTOverallPrescale = cms.uint32 (1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev4 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev4.HLTPaths = ("HLT_L1Mu20","HLT_L2Mu9","HLT_L2Mu11","HLT_L1Mu14_L1SingleEG10","HLT_L1Mu14_L1SingleJet6U","HLT_L1Mu14_L1ETM30")
process.hltHighLevelDev4.andOr = True # True = OR, False = AND
process.hltHighLevelDev4.HLTPathsPrescales  = cms.vuint32(1,1,1,1,1,1)
process.hltHighLevelDev4.HLTOverallPrescale = cms.uint32 (1)

## Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
#process.hltHighLevelDev5 = process.hltHighLevelDev.clone(andOr = True)
#process.hltHighLevelDev5.HLTPaths = ("HLT_L1DoubleMuOpen","HLT_DoubleMu0","HLT_DoubleMu3")
#process.hltHighLevelDev5.andOr = True # True = OR, False = AND
#process.hltHighLevelDev5.HLTPathsPrescales  = cms.vuint32(5,1,1)
#process.hltHighLevelDev5.HLTOverallPrescale = cms.uint32 (1)
#

#############   pt_hat Filter  #####
process.ptHat_filter = cms.EDFilter("MCProcessFilter",
    ProcessID = cms.untracked.vint32(0),
    MinPthat = cms.untracked.vdouble(50),
    MaxPthat = cms.untracked.vdouble(150.)
)


##############   mu Filter as in PYTHIA6_InclusiveMu5_pt50_10TeV_cff.py #####
#process.mugenfilter = cms.EDFilter("MCSmartSingleParticleFilter",
#                           MinPt = cms.untracked.vdouble(5.,5.),
#                           MinEta = cms.untracked.vdouble(-2.5,-2.5),
#                           MaxEta = cms.untracked.vdouble(2.5,2.5),
#                           ParticleID = cms.untracked.vint32(13,-13),
#                           Status = cms.untracked.vint32(1,1),
#                           # Decay cuts are in mm
#                           MaxDecayRadius = cms.untracked.vdouble(2000.,2000.),
#                           MinDecayZ = cms.untracked.vdouble(-4000.,-4000.),
#                           MaxDecayZ = cms.untracked.vdouble(4000.,4000.)
#)

process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('log/skimInclusiveMu5_Pt50_Pt50to150.log')
    )

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.123 $'),
    annotation = cms.untracked.string('skim'),
    name = cms.untracked.string('skim')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(5000000)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'file:/data/rossin/CMS/Triggers/Summer09/InclusiveMu5_Pt50/GEN-SIM-RECO/MC_31X_V3-v1/0013/02CBD73C-8784-DE11-8E37-00144F0D6808.root',
#'file:/data/rossin/CMS/Triggers/Summer09/InclusiveMu5_Pt50/GEN-SIM-RECO/MC_31X_V3-v1/0013/18579BBD-9184-DE11-B892-00144F0D67FA.root',
#'file:/data/rossin/CMS/Triggers/Summer09/InclusiveMu5_Pt50/GEN-SIM-RECO/MC_31X_V3-v1/0013/0E4AC9CF-8B84-DE11-A140-00144F9E8C1A.root',
#'file:/data/rossin/CMS/Triggers/Summer09/InclusiveMu5_Pt50/GEN-SIM-RECO/MC_31X_V3-v1/0013/36429E51-9784-DE11-BD30-00144F0D68CE.root',
#'file:/data/rossin/CMS/Triggers/Summer09/InclusiveMu5_Pt50/GEN-SIM-RECO/MC_31X_V3-v1/0013/326B12E9-8E84-DE11-9E2B-00144F0D67FA.root',
#'file:/data/rossin/CMS/Triggers/Summer09/InclusiveMu5_Pt50/GEN-SIM-RECO/MC_31X_V3-v1/0013/185C3789-7F84-DE11-83F8-00144F0D67F6.root',
#'file:/data/rossin/CMS/Triggers/Summer09/InclusiveMu5_Pt50/GEN-SIM-RECO/MC_31X_V3-v1/0013/0A8B8C8C-8D84-DE11-B6EF-00144F0D67FC.root'
    )
)

process.hltTrigReport = cms.EDAnalyzer( "HLTrigReport",
    HLTriggerResults = cms.InputTag( 'TriggerResults','','HLT8E29' )
)


# Output definition
process.output1 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_BTag_HSCP_InclusiveMu5_Pt50to150.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_BTag_HSCP')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim1')
                                      )
)
# Yet another output definition
process.output2 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(99),
                                   outputCommands = process.AODSIMEventContent.outputCommands,
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/PD_Mu_InclusiveMu5_Pt50to150.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('AODSIM'),
                                      filterName = cms.untracked.string('PD_Mu_HLTMu5')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim2')
                                      )
)

# Yet another output definition
process.output3 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_Mu9_InclusiveMu5_Pt50toInf.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_Mu9')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim3')
                                      )
)

# Yet another output definition
process.output4 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_L1_L2_Mu_InclusiveMu5_Pt50toInf.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_L1_L2_Mu')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim4')
                                      )
)


# the usage of trigger bits for selection is explained here:
## https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideEDMPathsAndTriggerBits#Selecting_Pass_for_Some_Trigger 

#process.pTfilter = cms.Path(process.ptHat_filter)
process.hlttrigreport = cms.Path(process.ptHat_filter+process.hltTrigReport)
process.skim1 = cms.Path(process.ptHat_filter+process.hltHighLevelDev )
process.skim2 = cms.Path(process.ptHat_filter+process.hltHighLevelDev2)
process.skim3 = cms.Path(process.hltHighLevelDev3)
process.skim4 = cms.Path(process.hltHighLevelDev4)


process.out_step = cms.EndPath(process.output1+process.output2+process.output3+process.output4)

process.schedule = cms.Schedule(process.hlttrigreport,process.skim1,process.skim2,process.skim3,process.skim4)
process.schedule.append(process.out_step)

