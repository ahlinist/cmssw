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
process.hltHighLevelDev.HLTOverallPrescale = cms.uint32 (1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev2 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev2.HLTPaths = ("HLT_Jet15U",)
process.hltHighLevelDev2.andOr = True # True = OR, False = AND
process.hltHighLevelDev2.HLTPathsPrescales  = cms.vuint32(1,)
process.hltHighLevelDev2.HLTOverallPrescale = cms.uint32 (1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev3 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev3.HLTPaths = ("HLT_DiJetAve15U_8E29",)
process.hltHighLevelDev3.andOr = True # True = OR, False = AND
process.hltHighLevelDev3.HLTPathsPrescales  = cms.vuint32(1,)
process.hltHighLevelDev3.HLTOverallPrescale = cms.uint32 (1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev4 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev4.HLTPaths = ("HLT_L1MET20","HLT_MET45")
process.hltHighLevelDev4.andOr = True # True = OR, False = AND
process.hltHighLevelDev4.HLTPathsPrescales  = cms.vuint32(20,1)
process.hltHighLevelDev4.HLTOverallPrescale = cms.uint32 (1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev5 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev5.HLTPaths = ("HLT_L1MuOpen",)
process.hltHighLevelDev5.andOr = True # True = OR, False = AND
process.hltHighLevelDev5.HLTPathsPrescales  = cms.vuint32(1,)
process.hltHighLevelDev5.HLTOverallPrescale = cms.uint32 (1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev6 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev6.HLTPaths = ("HLT_L1Mu",)
process.hltHighLevelDev6.andOr = True # True = OR, False = AND
process.hltHighLevelDev6.HLTPathsPrescales  = cms.vuint32(1,)
process.hltHighLevelDev6.HLTOverallPrescale = cms.uint32 (1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev7 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev7.HLTPaths = ("HLT_ZeroBias",)
process.hltHighLevelDev7.andOr = True # True = OR, False = AND
process.hltHighLevelDev7.HLTPathsPrescales  = cms.vuint32(25,) #This amounts to ~1pb-1
process.hltHighLevelDev7.HLTOverallPrescale = cms.uint32 (1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev8 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev8.HLTPaths = ("HLT_MinBiasEcal","HLT_MinBiasHcal")
process.hltHighLevelDev8.andOr = True # True = OR, False = AND
process.hltHighLevelDev8.HLTPathsPrescales  = cms.vuint32(1,1)
process.hltHighLevelDev8.HLTOverallPrescale = cms.uint32 (20) #This amounts to ~1pb-1

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev9 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev9.HLTPaths = ("HLT_MinBiasPixel",)
process.hltHighLevelDev9.andOr = True # True = OR, False = AND
process.hltHighLevelDev9.HLTPathsPrescales  = cms.vuint32(15,)
process.hltHighLevelDev9.HLTOverallPrescale = cms.uint32 (1) #This amounts to ~1pb-1

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev10 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev10.HLTPaths = ("HLT_CSCBeamHalo","HLT_BackwardBSC","HLT_ForwardBSC","HLT_TrackerCosmics")
process.hltHighLevelDev10.andOr = True # True = OR, False = AND
process.hltHighLevelDev10.HLTPathsPrescales  = cms.vuint32(10,1,1,1)
process.hltHighLevelDev10.HLTOverallPrescale = cms.uint32 (1)


#############   pt_hat Filter  #####
process.ptHat_filter = cms.EDFilter("MCProcessFilter",
    ProcessID = cms.untracked.vint32(0),
    MinPthat = cms.untracked.vdouble(-1.0),
    MaxPthat = cms.untracked.vdouble(15.)
)

process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('log/skimMinBias.log')
    )

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.123 $'),
    annotation = cms.untracked.string('skim'),
    name = cms.untracked.string('skim')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(50000000)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/009BFA13-927F-DE11-B51F-00221503164C.root',
'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/0662FF6C-E47F-DE11-930E-001B243DEF3F.root',
'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/0AFF04A5-0F80-DE11-86E6-0016368478AC.root',
'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/1EA95BAE-797F-DE11-9D11-00E081345B96.root',
'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/2240D1A0-7B7F-DE11-81AE-001A92544626.root',
'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/26E55A93-0F80-DE11-B2A8-0016368E0CF0.root',
'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/28D8FC4B-B17F-DE11-8764-001E8C0EF928.root',
'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/347E4BE7-837F-DE11-8158-00E08134B780.root',
'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/36CB8434-8481-DE11-8C7A-001EC94B51D5.root',
'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/44200BD1-1880-DE11-955D-001EC9AAA2F1.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/46BE77DC-797F-DE11-8EAB-003048770DCE.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/48D1CFFB-8181-DE11-903B-00E08134051C.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/4E7E8545-957F-DE11-8FF8-00E081336478.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/54703DF9-A97F-DE11-97B7-001A9254460C.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/54C9627D-3880-DE11-9E37-00163691DC02.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/54F14FC2-707F-DE11-9C5E-00304867FD83.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/564ACF4D-8D7F-DE11-8CC2-00304867FE77.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/60C2F0E4-9D7F-DE11-AE32-003048770DCE.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/665B8AEF-FA7F-DE11-BEBB-001EC9AA99C8.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/6C9CA5BD-A77F-DE11-857A-00304867FE1F.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/702E8237-CA7F-DE11-85FE-00163691DEEA.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/76BE19FC-AA7F-DE11-8CE1-00E08133CD90.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/82C757A0-D67F-DE11-9A91-00163691D99E.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/8EED4E07-FB7F-DE11-946B-00163691DC62.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/92465E47-957F-DE11-9D25-0002B3E9064B.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/9C08ED70-847F-DE11-9D23-0030487D676A.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/9CA0AB71-7D7F-DE11-9245-00E081325020.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/9CC91D47-7E81-DE11-871E-001C23C105CA.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/AACB0C2B-CF7F-DE11-BFC7-00163691D236.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/B66C80BF-7C81-DE11-A1F9-00E081237967.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/CA73EEED-8A7F-DE11-9D75-003048770DCE.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/CE0A0BE0-3580-DE11-BC21-001EC9AA9F90.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/D2E13320-7881-DE11-B5DA-001C23C0DFDD.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/D6D25D64-E47F-DE11-8CEA-00163691D9A2.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/D83B802E-1E80-DE11-8F7D-00074305CD92.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/D8C8AB45-9A7F-DE11-93E0-00304867FE57.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/E02BBCC8-D77F-DE11-AE08-00163691DEF6.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/E05D3E26-7981-DE11-8E1B-001EC94BA3D6.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/E88EDB09-717F-DE11-B0DE-0002B3E92671.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/F8C17AF2-FA7F-DE11-A4A7-001EC9AAA319.root',
#'file:/data/rossin/CMS/Triggers/Summer09/MinBias/GEN-SIM-RECO/0016/FC84038B-727F-DE11-A46E-001E8C7B5650.root'
    )
)

process.hltTrigReport = cms.EDAnalyzer( "HLTrigReport",
    HLTriggerResults = cms.InputTag( 'TriggerResults','','HLT8E29' )
)


# Output definition
process.output1 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_L1Jet6U_Pt0to15.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_L1Jet6U')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim1') #defining the skim. This will be part of the 
                                      )
)

# Yet another output definition
process.output2 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_Jet15U_Pt0to15.root'),
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
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/CS_DiJetAve15U_Pt0to15.root'),
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
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_MET_Pt0to15.root'),
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
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_L1MuOpen_Pt0to15.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_L1MuOpen')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim5')
                                      )
)

# Yet another output definition
process.output6 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_L1Mu_Pt0to15.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_L1Mu')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim6')
                                      )
)

# Yet another output definition
process.output7 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_ZeroBias_Pt0toInf.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_ZeroBias')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim7')
                                      )
)

# Yet another output definition
process.output8 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_MinBiasCal_Pt0toInf.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_MinBiasCal')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim8')
                                      )
)

# Yet another output definition
process.output9 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_MinBiasPixel_Pt0toInf.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_MinBiasPixel')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim9')
                                      )
)

# Yet another output definition
process.output10 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_BH_Forward_Pt0toInf.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_BH_Forward')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim10')
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
process.skim7 = cms.Path(process.hltHighLevelDev7)
process.skim8 = cms.Path(process.hltHighLevelDev8)
process.skim9 = cms.Path(process.hltHighLevelDev9)
process.skim10 = cms.Path(process.hltHighLevelDev10)


process.out_step = cms.EndPath(process.output1+process.output2+process.output3+process.output4+process.output5+process.output6+process.output7+process.output8+process.output9+process.output10)

process.schedule = cms.Schedule(process.hlttrigreport,process.skim1,process.skim2,process.skim3,process.skim4,process.skim5,process.skim6,process.skim7,process.skim8,process.skim9,process.skim10)
process.schedule.append(process.out_step)
