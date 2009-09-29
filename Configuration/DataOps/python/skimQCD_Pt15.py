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

process.hltHighLevelDev.HLTPaths = ("HLT_L1Jet6U",)
process.hltHighLevelDev.andOr = True # True = OR, False = AND
process.hltHighLevelDev.HLTPathsPrescales  = cms.vuint32(5,)
process.hltHighLevelDev.HLTOverallPrescale =cms.uint32(1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev2 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev2.HLTPaths = ("HLT_Jet15U",)
process.hltHighLevelDev2.andOr = True # True = OR, False = AND
process.hltHighLevelDev2.HLTPathsPrescales  = cms.vuint32(1,)
process.hltHighLevelDev2.HLTOverallPrescale =cms.uint32(1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev3 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev3.HLTPaths = ("HLT_DiJetAve15U_8E29",)
process.hltHighLevelDev3.andOr = True # True = OR, False = AND
process.hltHighLevelDev3.HLTPathsPrescales  = cms.vuint32(1,)
process.hltHighLevelDev3.HLTOverallPrescale =cms.uint32(1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev4 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev4.HLTPaths = ("HLT_Jet30U","HLT_QuadJet15U","HLT_FwdJet20U")
process.hltHighLevelDev4.andOr = True # True = OR, False = AND
process.hltHighLevelDev4.HLTPathsPrescales  = cms.vuint32(10,1,10)
process.hltHighLevelDev4.HLTOverallPrescale =cms.uint32(1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev5 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev5.HLTPaths = ("HLT_L1MET20","HLT_MET45")
process.hltHighLevelDev5.andOr = True # True = OR, False = AND
process.hltHighLevelDev5.HLTPathsPrescales  = cms.vuint32(20,1)
process.hltHighLevelDev5.HLTOverallPrescale =cms.uint32(1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev6 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev6.HLTPaths = ("HLT_L1MuOpen",)
process.hltHighLevelDev6.andOr = True # True = OR, False = AND
process.hltHighLevelDev6.HLTPathsPrescales  = cms.vuint32(1,)
process.hltHighLevelDev6.HLTOverallPrescale =cms.uint32(1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev7 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev7.HLTPaths = ("HLT_L1Mu",)
process.hltHighLevelDev7.andOr = True # True = OR, False = AND
process.hltHighLevelDev7.HLTPathsPrescales  = cms.vuint32(1,)
process.hltHighLevelDev7.HLTOverallPrescale = cms.uint32(1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev8 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev8.HLTPaths = ("HLT_Mu9",)
process.hltHighLevelDev8.andOr = True # True = OR, False = AND
process.hltHighLevelDev8.HLTPathsPrescales  = cms.vuint32(1,)
process.hltHighLevelDev8.HLTOverallPrescale =cms.uint32(1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev9 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev9.HLTPaths = ("HLT_L1Mu20","HLT_L2Mu9","HLT_L2Mu11","HLT_L1Mu14_L1SingleEG10","HLT_L1Mu14_L1SingleJet6U","HLT_L1Mu14_L1ETM30")
process.hltHighLevelDev9.andOr = True # True = OR, False = AND
process.hltHighLevelDev9.HLTPathsPrescales  = cms.vuint32(1,1,1,1,1,1)
process.hltHighLevelDev9.HLTOverallPrescale =cms.uint32(1)

# Once the hltHighLevel filter has been defined I can clone it and configure as needed
process.hltHighLevelDev10 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev10.HLTPaths = ("HLT_L1SingleEG5","HLT_L1SingleEG8")
process.hltHighLevelDev10.andOr = True # True = OR, False = AND
process.hltHighLevelDev10.HLTPathsPrescales  = cms.vuint32(5,1)
process.hltHighLevelDev10.HLTOverallPrescale = cms.uint32(1)

# Once the hltHighLevel filter has been defined I can clone it and configure as needed
process.hltHighLevelDev11 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev11.HLTPaths = ("HLT_L1DoubleEG5",)
process.hltHighLevelDev11.andOr = True # True = OR, False = AND
process.hltHighLevelDev11.HLTPathsPrescales  = cms.vuint32(1,)
process.hltHighLevelDev11.HLTOverallPrescale = cms.uint32(1)

# Once the hltHighLevel filter has been defined I can clone it and configure as needed
process.hltHighLevelDev12 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev12.HLTPaths = ("HLT_Photon10_L1R",)
process.hltHighLevelDev12.andOr = True # True = OR, False = AND
process.hltHighLevelDev12.HLTPathsPrescales  = cms.vuint32(1,)
process.hltHighLevelDev12.HLTOverallPrescale = cms.uint32(1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev13 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev13.HLTPaths = ("HLT_DoubleEle5_SW_L1R","HLT_DoublePhoton5_eeRes_L1R")
process.hltHighLevelDev13.andOr = True # True = OR, False = AND
process.hltHighLevelDev13.HLTPathsPrescales  = cms.vuint32(1,20)
process.hltHighLevelDev13.HLTOverallPrescale = cms.uint32(1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev14 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev14.HLTPaths = ("HLT_DoublePhoton5_Jpsi_L1R","HLT_DoublePhoton5_Upsilon_L1R")
process.hltHighLevelDev14.andOr = True # True = OR, False = AND
process.hltHighLevelDev14.HLTPathsPrescales  = cms.vuint32(1,1)
process.hltHighLevelDev14.HLTOverallPrescale = cms.uint32(1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev15 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev15.HLTPaths = ("HLT_DoublePhoton10_L1R",)
process.hltHighLevelDev15.andOr = True # True = OR, False = AND
process.hltHighLevelDev15.HLTPathsPrescales  = cms.vuint32(1,)
process.hltHighLevelDev15.HLTOverallPrescale = cms.uint32(1)


#############   pt_hat Filter  #####
process.ptHat_filter = cms.EDFilter("MCProcessFilter",
    ProcessID = cms.untracked.vint32(0),
    MinPthat = cms.untracked.vdouble(15.0),
    MaxPthat = cms.untracked.vdouble(30.0)
)

process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('log/skimQCD_Pt15.log')
    )

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.123 $'),
    annotation = cms.untracked.string('skim'),
    name = cms.untracked.string('skim')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100000000)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/081EAD49-8582-DE11-A3BE-001E68A99418.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/1C6D2A4D-7482-DE11-A96E-0030487CD7E8.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/1CBDF9B9-8882-DE11-8EDA-001E68A996EA.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/421A25CA-7982-DE11-BEE5-001E682F882A.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/50DDE73D-8B82-DE11-8046-001E68A993F4.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/5445E67C-6982-DE11-B8A3-001E6849DC22.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/5AA6B41F-7382-DE11-B295-0030487CD7E2.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/5ADBC3B7-6982-DE11-B5BE-0030487C6F56.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/66987C8D-8582-DE11-A336-001E682F8676.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/6AA93EF4-2883-DE11-A43F-003048D47677.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/6E0F6FF3-8582-DE11-A1E5-001E6837DC54.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/7CF9A534-8982-DE11-9964-001E68A993EE.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/8AEE301E-8A82-DE11-B03B-001E68A99418.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/8AF6A349-7582-DE11-812A-00163691D181.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/8CB8F8EC-8182-DE11-A577-001E6837DC54.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/8E3AD020-6B82-DE11-B4A4-001B2493298E.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/926E321F-8282-DE11-B891-003048C5D19A.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/9804EEB7-6982-DE11-9F6A-0030487CD7E2.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/9C2EC70A-6A82-DE11-BA35-0030487CD7E8.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/A6378C02-8B82-DE11-97B0-001E68A99400.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/A6749EF0-7582-DE11-9394-0030487CD7E2.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/AA15741C-6B82-DE11-98AD-0030487CD7E2.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/AE4AB95D-6A82-DE11-8A16-001E682F84DE.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/B2B3A2BF-7582-DE11-8F72-0030487C2FA2.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/B690A5F1-6982-DE11-8870-0030487C21B0.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/BA0B7E4C-6282-DE11-BBF0-001E682F8578.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/BC327C8B-6A82-DE11-AE33-001E68A994C6.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/BE0DE9E4-9482-DE11-85A0-00304820A72D.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/C6A7297E-6982-DE11-A626-003048917F50.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/DE3479E5-7B82-DE11-9D5A-001E68A99492.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/DE3510CF-6882-DE11-92A9-003048917F24.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/DE7F1BDC-8882-DE11-B399-0030487CAA07.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/E22D0636-7482-DE11-B470-0030487C21B0.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/E29D035A-6A82-DE11-A0D2-001B241DE88F.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/EA4FDCF9-8282-DE11-AADA-0030487DFC68.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/F0AF8445-6A82-DE11-BF65-001E682F87E6.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/F45F9B9A-9582-DE11-830F-003048917F56.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt15/FA9B6DEB-7682-DE11-A260-0030487CD7E2.root'
    )
)

process.hltTrigReport = cms.EDAnalyzer( "HLTrigReport",
    HLTriggerResults = cms.InputTag( 'TriggerResults','','HLT8E29' )
)


# Output definition
process.output1 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_L1Jet6U_Pt15toInf.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_L1Jet6U')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                   SelectEvents = cms.vstring('skim1') #defining the skim.
                                      )
)

# Yet another output definition
process.output2 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_Jet15U_Pt15toInf.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_Jet15U')
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
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/CS_DiJetAve15U_Pt15toInf.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('USER'),
                                      filterName = cms.untracked.string('CS_DiJetAve15U')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim3')
                                      )
)

# Yet another output definition
process.output4 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_Jet30U_Quad_Fwd_Pt15to30.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_Jet30U_Quad_Fwd')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim4')
                                      )
)

# Yet another output definition
process.output5 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_MET_Pt15to30.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_MET')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim5')
                                      )
)

# Yet another output definition
process.output6 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_L1MuOpen_Pt15toInf.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_L1MuOpen')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim6')
                                      )
)

# Yet another output definition
process.output7 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_L1Mu_Pt15toInf.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_L1Mu')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim7')
                                      )
)

# Yet another output definition
process.output8 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_Mu9_Pt15to30.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_Mu9')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim8')
                                      )
)

# Yet another output definition
process.output9 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_L1_L2_Mu_Pt15to30.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_L1_L2_Mu')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim9')
                                      )
)

# Yet another output definition
process.output10 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_L1SingleEG5_Pt15toInf.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_L1SingleEG5')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                   SelectEvents = cms.vstring('skim10')
                                      )
)

# Yet another output definition
process.output11 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_L1DoubleEG5_Pt15toInf.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_L1DoubleEG5')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim11')
                                      )
)

# Yet another output definition
process.output12 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_Photon10_Pt15toInf.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_Photon10')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim12')
                                      )
)

process.output13 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_DoubleEG5_Pt15to30.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_DoubleEG5')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim13')
                                      )
)

# Yet another output definition
process.output14 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_DoublePhoton5_JPsi_Upsilon_Pt15to30.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_DoublePhoton5_JPsi_Upsilon')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim14')
                                      )
)

# Yet another output definition
process.output15 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_DoublePhoton10_Pt15to30.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_DoublePhoton10')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim15')
                                      )
)


# the usage of trigger bits for selection is explained here:
## https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideEDMPathsAndTriggerBits#Selecting_Pass_for_Some_Trigger 

#process.pTfilter = cms.Path(process.ptHat_filter)
process.hlttrigreport = cms.Path(process.ptHat_filter+process.hltTrigReport)
process.skim1 = cms.Path(process.hltHighLevelDev )
process.skim2 = cms.Path(process.hltHighLevelDev2)
process.skim3 = cms.Path(process.hltHighLevelDev3)
process.skim4 = cms.Path(process.ptHat_filter+process.hltHighLevelDev4)
process.skim5 = cms.Path(process.ptHat_filter+process.hltHighLevelDev5)
process.skim6 = cms.Path(process.hltHighLevelDev6)
process.skim7 = cms.Path(process.hltHighLevelDev7)
process.skim8 = cms.Path(process.ptHat_filter+process.hltHighLevelDev8)
process.skim9 = cms.Path(process.ptHat_filter+process.hltHighLevelDev9)
process.skim10 = cms.Path(process.hltHighLevelDev10)
process.skim11 = cms.Path(process.hltHighLevelDev11)
process.skim12 = cms.Path(process.hltHighLevelDev12)
process.skim13 = cms.Path(process.ptHat_filter+process.hltHighLevelDev13)
process.skim14 = cms.Path(process.ptHat_filter+process.hltHighLevelDev14)
process.skim15 = cms.Path(process.ptHat_filter+process.hltHighLevelDev15)


process.out_step = cms.EndPath(process.output1+process.output2+process.output3+process.output4+process.output5+process.output6+process.output7+process.output8+process.output9+process.output10+process.output11+process.output12+process.output13+process.output14+process.output15)

process.schedule = cms.Schedule(process.hlttrigreport,process.skim1,process.skim2,process.skim3,process.skim4,process.skim5,process.skim6,process.skim7,process.skim8,process.skim9,process.skim10,process.skim11,process.skim12,process.skim13,process.skim14,process.skim15)
process.schedule.append(process.out_step)

