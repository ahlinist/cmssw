import FWCore.ParameterSet.Config as cms

process = cms.Process('SKIM')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/EventContent/EventContent_cff')
process.load('JetMETCorrections.Configuration.jecHLTFilters_cfi')

# module to select events based on HLT Trigger bits
process.load('HLTrigger/HLTfilters/hltHighLevelDev_cfi')

# Loading "hltHighLevelDev_cfi" defines an EDFilter called hltHighLevelDev
# now we can configure it 

process.hltHighLevelDev.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT8E29")

process.hltHighLevelDev.HLTPaths = ("HLT_Jet30U","HLT_QuadJet15U","HLT_FwdJet20U")
process.hltHighLevelDev.andOr = True # True = OR, False = AND
process.hltHighLevelDev.HLTPathsPrescales  = cms.vuint32(10,1,10)
process.hltHighLevelDev.HLTOverallPrescale =cms.uint32(1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev2 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev2.HLTPaths = ("HLT_Jet50U",)
process.hltHighLevelDev2.andOr = True # True = OR, False = AND
process.hltHighLevelDev2.HLTPathsPrescales  = cms.vuint32(1,)
process.hltHighLevelDev2.HLTOverallPrescale = cms.uint32(1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev3 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev3.HLTPaths = ("HLT_DiJetAve30U_8E29",)
process.hltHighLevelDev3.andOr = True # True = OR, False = AND
process.hltHighLevelDev3.HLTPathsPrescales  = cms.vuint32(1,)
process.hltHighLevelDev3.HLTOverallPrescale = cms.uint32(1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev4 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev4.HLTPaths = ("HLT_L1MET20","HLT_MET45")
process.hltHighLevelDev4.andOr = True # True = OR, False = AND
process.hltHighLevelDev4.HLTPathsPrescales  = cms.vuint32(20,1)
process.hltHighLevelDev4.HLTOverallPrescale =cms.uint32(1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev5 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev5.HLTPaths = ("HLT_BTagIP_Jet50U","HLT_BTagMu_Jet10U","HLT_StoppedHSCP_8E29")
process.hltHighLevelDev5.andOr = True # True = OR, False = AND
process.hltHighLevelDev5.HLTPathsPrescales  = cms.vuint32(1,1,1)
process.hltHighLevelDev5.HLTOverallPrescale =cms.uint32(1)

process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('log/skimQCD_Pt80.log')
    )

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.123 $'),
    annotation = cms.untracked.string('skim'),
    name = cms.untracked.string('skim')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(2000000)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt80/GEN-SIM-RECO/MC_31X_V3-v1/0018/0412405D-7D85-DE11-914E-003048976AE4.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt80/GEN-SIM-RECO/MC_31X_V3-v1/0018/14783849-A585-DE11-BD8F-001F29C96530.root',
'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt80/GEN-SIM-RECO/MC_31X_V3-v1/0018/68384BBD-8C85-DE11-BF0B-001F29C9A546.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt80/GEN-SIM-RECO/MC_31X_V3-v1/0018/82BB985F-B985-DE11-A041-001F29C9C492.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt80/GEN-SIM-RECO/MC_31X_V3-v1/0018/8C72B710-7E85-DE11-A9AA-001E6837DC54.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt80/GEN-SIM-RECO/MC_31X_V3-v1/0018/8E2F8148-CB86-DE11-8F27-0030487E4B00.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt80/GEN-SIM-RECO/MC_31X_V3-v1/0018/A261D8AF-C785-DE11-A0DE-00215E2222C2.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt80/GEN-SIM-RECO/MC_31X_V3-v1/0018/AA6E93B1-8285-DE11-A28D-001A4B0A273E.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt80/GEN-SIM-RECO/MC_31X_V3-v1/0018/B8F662CF-7C85-DE11-AF20-0019BBEBF534.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt80/GEN-SIM-RECO/MC_31X_V3-v1/0018/D2B432DB-9685-DE11-94CC-00E081327C25.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt80/GEN-SIM-RECO/MC_31X_V3-v1/0018/D8F25FCA-8685-DE11-8880-003048C559CA.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt80/GEN-SIM-RECO/MC_31X_V3-v1/0018/E80ED860-B585-DE11-8B3C-001F29C9C5A8.root'
    )
)

process.hltTrigReport = cms.EDAnalyzer( "HLTrigReport",
    HLTriggerResults = cms.InputTag( 'TriggerResults','','HLT8E29' )
)


# Output definition
process.output1 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_Jet30U_Quad_Fwd_Pt80toInf.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_Jet30U_Quad_Fwd')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                   SelectEvents = cms.vstring('skim1') #defining the skim.
                                      )
)

# Yet another output definition
process.output2 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_Jet50U_Pt80toInf.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_Jet50U')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim2')
                                      )
)

# Yet another output definition
process.output3 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(99),
    outputCommands = cms.untracked.vstring(
        'drop *',
        'keep *_sisCone5CaloJets_*_*',
        'keep *_sisCone7CaloJets_*_*',
        'keep *_kt4CaloJets_*_*',
        'keep *_kt6CaloJets_*_*',
        'keep *_antikt5CaloJets_*_*',
        'keep *_iterativeCone5CaloJets_*_*',  
        'keep *_sisCone5PFJets_*_*',
        'keep *_sisCone7PFJets_*_*',
        'keep *_kt4PFJets_*_*',
        'keep *_kt6PFJets_*_*',
        'keep *_antikt5PFJets_*_*',
        'keep *_iterativeCone5CaloJets_*_*',
        'keep *_iterativeCone5JetExtender_*_*',
        'keep *_sisCone5JetExtender_*_*',
        'keep *_kt4JetExtender_*_*',
        'keep *_TriggerResults_*_*',
        'keep *_hltTriggerSummaryAOD_*_*', 
        'keep *_towerMaker_*_*',
        'keep *_EventAuxilary_*_*',
        'keep *_pixelVertices_*_*',
        'keep *_metHO_*_*',
        'keep *_metNoHF_*_*',
        'keep *_metNoHFHO_*_*', 
        'keep *_met_*_*'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/CS_DiJetAve30U_Pt80toInf.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('USER'),
                                      filterName = cms.untracked.string('CS_DiJetAve30U')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim3')
                                      )
)

# Yet another output definition
process.output4 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_MET_Pt80toInf.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_MET')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim4')
                                      )
)

# Yet another output definition
process.output5 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_BTag_HSCP_Pt80toInf.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_BTag_HSCP')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim5')
                                      )
)

# the usage of trigger bits for selection is explained here:
## https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideEDMPathsAndTriggerBits#Selecting_Pass_for_Some_Trigger 

#process.pTfilter = cms.Path(process.ptHat_filter)
process.hlttrigreport = cms.Path(process.hltTrigReport)
process.skim1 = cms.Path(process.hltHighLevelDev )
process.skim2 = cms.Path(process.hltHighLevelDev2)
process.skim3 = cms.Path(process.hltHighLevelDev3)
process.skim4 = cms.Path(process.hltHighLevelDev4)
process.skim5 = cms.Path(process.hltHighLevelDev5)


process.out_step = cms.EndPath(process.output1+process.output2+process.output3+process.output4+process.output5)

process.schedule = cms.Schedule(process.hlttrigreport,process.skim1,process.skim2,process.skim3,process.skim4,process.skim5)
process.schedule.append(process.out_step)

