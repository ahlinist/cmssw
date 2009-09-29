import FWCore.ParameterSet.Config as cms

process = cms.Process('SKIM')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/EventContent/EventContent_cff')
process.load('JetMETCorrections.Configuration.jecHLTFilters_cfi')

# module to select events based on HLT Trigger bits
process.load('HLTrigger/HLTfilters/hltHighLevelDev_cfi')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')

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

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev6 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev6.HLTPaths = ("HLT_Mu9",)
process.hltHighLevelDev6.andOr = True # True = OR, False = AND
process.hltHighLevelDev6.HLTPathsPrescales  = cms.vuint32(1,)
process.hltHighLevelDev6.HLTOverallPrescale =cms.uint32(1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev7 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev7.HLTPaths = ("HLT_L1Mu20","HLT_L2Mu9","HLT_L2Mu11","HLT_L1Mu14_L1SingleEG10","HLT_L1Mu14_L1SingleJet6U","HLT_L1Mu14_L1ETM30")
process.hltHighLevelDev7.andOr = True # True = OR, False = AND
process.hltHighLevelDev7.HLTPathsPrescales  = cms.vuint32(1,1,1,1,1,1)
process.hltHighLevelDev7.HLTOverallPrescale =cms.uint32(1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev8 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev8.HLTPaths = ("HLT_DoubleEle5_SW_L1R","HLT_DoublePhoton5_eeRes_L1R")
process.hltHighLevelDev8.andOr = True # True = OR, False = AND
process.hltHighLevelDev8.HLTPathsPrescales  = cms.vuint32(1,20)
process.hltHighLevelDev8.HLTOverallPrescale = cms.uint32(1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev9 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev9.HLTPaths = ("HLT_DoublePhoton5_Jpsi_L1R","HLT_DoublePhoton5_Upsilon_L1R")
process.hltHighLevelDev9.andOr = True # True = OR, False = AND
process.hltHighLevelDev9.HLTPathsPrescales  = cms.vuint32(1,1)
process.hltHighLevelDev9.HLTOverallPrescale = cms.uint32(1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev10 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev10.HLTPaths = ("HLT_DoublePhoton10_L1R",)
process.hltHighLevelDev10.andOr = True # True = OR, False = AND
process.hltHighLevelDev10.HLTPathsPrescales  = cms.vuint32(1,)
process.hltHighLevelDev10.HLTOverallPrescale = cms.uint32(1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev11 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev11.HLTPaths = ("HLT_SingleLooseIsoTau20",)
process.hltHighLevelDev11.andOr = True # True = OR, False = AND
process.hltHighLevelDev11.HLTPathsPrescales  = cms.vuint32(5,)
process.hltHighLevelDev11.HLTOverallPrescale = cms.uint32(1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev12 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev12.HLTPaths = ("HLT_DoubleLooseIsoTau15",)
process.hltHighLevelDev12.andOr = True # True = OR, False = AND
process.hltHighLevelDev12.HLTPathsPrescales  = cms.vuint32(4,)
process.hltHighLevelDev12.HLTOverallPrescale = cms.uint32(1)

#############   pt_hat Filter  #####
process.ptHat_filter = cms.EDFilter("MCProcessFilter",
    ProcessID = cms.untracked.vint32(0),
    MinPthat = cms.untracked.vdouble(30.0),
    MaxPthat = cms.untracked.vdouble(80.0)
)
#############  second pt_hat Filter  #####
process.ptHat_filter2 = process.ptHat_filter.clone()
process.ptHat_filter2.MaxPthat=cms.untracked.vdouble(50.0)

#############  EMEnrichingFilter as in PYTHIA6_QCD_Pt_20_30_10TeV_EMEnrichingFilter_cff.py #####
process.genParticlesForFilter = cms.EDProducer("GenParticleProducer",
    saveBarCodes = cms.untracked.bool(True),
    src = cms.InputTag("generator"),
    abortOnUnknownPDGCode = cms.untracked.bool(True)
)

process.emenrichingfilter = cms.EDFilter("EMEnrichingFilter",
                                 filterAlgoPSet = cms.PSet(isoGenParETMin=cms.double(20.),
                                                           isoGenParConeSize=cms.double(0.1),
                                                           clusterThreshold=cms.double(20.),
                                                           isoConeSize=cms.double(0.2),
                                                           hOverEMax=cms.double(0.5),
                                                           tkIsoMax=cms.double(5.),
                                                           caloIsoMax=cms.double(10.),
                                                           requireTrackMatch=cms.bool(False),
                                                           genParSource = cms.InputTag("genParticlesForFilter")
                                                           )
                                 )

process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('log/skimQCD_Pt30.log')
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
'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt30/A0B80633-AB82-DE11-ABB8-001517256129.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt30/B4BE0220-8A82-DE11-85F7-00096BB5C092.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt30/CABA2D0E-8782-DE11-BE69-0030487CA7C9.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt30/EA4EAE34-8682-DE11-95C6-001E682F8B72.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt30/GEN-SIM-RECO/MC_31X_V3-v1/0006/0EB0578F-F07F-DE11-82DD-003048D47661.root',
#'file:/data/rossin/CMS/Triggers/Summer09/QCD_Pt30/GEN-SIM-RECO/MC_31X_V3-v1/0006/10DBB067-1980-DE11-A44B-001E68A993D2.root'
    )
)

process.hltTrigReport = cms.EDAnalyzer( "HLTrigReport",
    HLTriggerResults = cms.InputTag( 'TriggerResults','','HLT8E29' )
)


# Output definition
process.output1 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_Jet30U_Quad_Fwd_QCD_Pt30to80.root'),
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
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_Jet50U_QCD_Pt30to80.root'),
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
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/CS_DiJetAve30U_QCD_Pt30to80.root'),
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
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_MET_Pt30to80.root'),
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
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_BTag_HSCP_Pt30to80.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_BTag_HSCP')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim5')
                                      )
)

# Yet another output definition
process.output6 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_Mu9_Pt30to50.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_Mu9')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim6')
                                      )
)

# Yet another output definition
process.output7 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_L1_L2_Mu_Pt30to50.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_L1_L2_Mu')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim7')
                                      )
)

process.output8 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_DoubleEG5_Pt30toInf.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_DoubleEG5')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim8')
                                      )
)

# Yet another output definition
process.output9 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_DoublePhoton5_JPsi_Upsilon_Pt30toInf.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_DoublePhoton5_JPsi_Upsilon')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim9')
                                      )
)

# Yet another output definition
process.output10 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_DoublePhoton10_Pt30toInf.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_DoublePhoton10')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim10')
                                      )
)

# Yet another output definition
process.output11 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_SingleLooseTau20_QCD_Pt30toInf.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_SingleLooseTau20')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim11')
                                      )
)

# Yet another output definition
process.output12 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_DoubleLooseTau15_QCD_BCtoE_Pt30toInf.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_DoubleLooseTau15')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim12')
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
process.skim6 = cms.Path(process.ptHat_filter2+process.hltHighLevelDev6)
process.skim7 = cms.Path(process.ptHat_filter2+process.hltHighLevelDev7)
process.skim8 = cms.Path(process.hltHighLevelDev8)
process.skim9 = cms.Path(process.hltHighLevelDev9)
process.skim10 = cms.Path(process.hltHighLevelDev10)
process.skim11 = cms.Path(process.genParticlesForFilter+~process.emenrichingfilter+process.hltHighLevelDev11)
process.skim12 = cms.Path(process.genParticlesForFilter+~process.emenrichingfilter+process.hltHighLevelDev12)


process.out_step = cms.EndPath(process.output1+process.output2+process.output3+process.output4+process.output5+process.output6+process.output7+process.output8+process.output9+process.output10+process.output11+process.output12)

process.schedule = cms.Schedule(process.hlttrigreport,process.skim1,process.skim2,process.skim3,process.skim4,process.skim5,process.skim6,process.skim7,process.skim8,process.skim9,process.skim10,process.skim11,process.skim12)
process.schedule.append(process.out_step)

