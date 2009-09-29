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

process.hltHighLevelDev.HLTPaths = ("HLT_Mu3","HLT_Mu5","HLT_Mu9","HLT_IsoMu3","HLT_L2Mu9","HLT_L2Mu11","HLT_L1Mu20","HLT_L1Mu14_L1SingleEG10","HLT_L1Mu14_L1SingleJet6U","HLT_L1Mu14_L1ETM30","HLT_L1DoubleMuOpen","HLT_DoubleMu0","HLT_DoubleMu3",)
process.hltHighLevelDev.andOr = True # True = OR, False = AND
process.hltHighLevelDev.HLTPathsPrescales  = cms.vuint32(1,1,1,1,1,1,1,1,1,1,1,1,1)
process.hltHighLevelDev.HLTOverallPrescale = cms.uint32 (1)

process.hltHighLevelDev2 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev2.HLTPaths = ("HLT_Mu3",)
process.hltHighLevelDev2.andOr = True # True = OR, False = AND
process.hltHighLevelDev2.HLTPathsPrescales  = cms.vuint32(10,)
process.hltHighLevelDev2.HLTOverallPrescale = cms.uint32 (1)

# Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
process.hltHighLevelDev3 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev3.HLTPaths = ("HLT_L1DoubleMuOpen","HLT_DoubleMu0","HLT_DoubleMu3")
process.hltHighLevelDev3.andOr = True # True = OR, False = AND
process.hltHighLevelDev3.HLTPathsPrescales  = cms.vuint32(5,1,1)
process.hltHighLevelDev3.HLTOverallPrescale = cms.uint32 (1)


##############   pt_hat Filter  #####
#process.ptHat_filter = cms.EDFilter("MCProcessFilter",
#    ProcessID = cms.untracked.vint32(0),
#    MinPthat = cms.untracked.vdouble(50),
#    MaxPthat = cms.untracked.vdouble(150.)
#)

##############   mumugenfilter as in PYTHIA6_ppMuMuX_10TeV_cff.py  #####
process.mumugenfilter = cms.EDFilter("MCParticlePairFilter",
    Status = cms.untracked.vint32(1, 1),
    MinPt = cms.untracked.vdouble(2.5, 2.5),
    MaxEta = cms.untracked.vdouble(2.5, 2.5),
    MinEta = cms.untracked.vdouble(-2.5, -2.5),
    ParticleCharge = cms.untracked.int32(-1),
    ParticleID1 = cms.untracked.vint32(13),
    ParticleID2 = cms.untracked.vint32(13)
)



process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('log/skimppMuX.log')
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
'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/0A007135-5583-DE11-A145-001AA0095119.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/0CBE0977-5D81-DE11-8667-00E08130092C.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/14558B10-5683-DE11-98F9-001AA0093636.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/1C3C79F5-5783-DE11-9312-001AA009F71A.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/1E8B0D53-EE81-DE11-833B-001E688652D5.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/32D207C2-6983-DE11-B3D0-001AA00AA80E.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/3C4CDD12-5683-DE11-82B8-001AA00B418A.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/3CF17252-5983-DE11-A578-001AA00DF708.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/424B4A61-5483-DE11-BD7D-001AA00BAF3B.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/4806F52F-5683-DE11-B8EB-001AA009F92B.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/4CB83A4F-5983-DE11-907B-001AA00AB0FE.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/542AC870-BE81-DE11-9002-00E0813006C6.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/5E2A7953-5683-DE11-AEC3-001AA00C55FB.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/607F2909-5883-DE11-A352-001AA009A484.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/62DEB124-5883-DE11-B575-001AA009EF21.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/6CEF9F25-5883-DE11-8350-001AA00A70B8.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/6E6CAAA2-5683-DE11-B1F1-001AA00DDC67.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/70458A20-5083-DE11-982D-001AA00B5FB0.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/765E8F03-5883-DE11-8AD8-001AA00CBED3.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/82C5D017-5883-DE11-BB7A-001AA009EF21.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/90618B21-5883-DE11-8BCB-001AA00DF48F.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/94F8EBB7-5783-DE11-8D00-001AA00BAF2F.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/B0977A0C-2A81-DE11-9868-001D090BE0A7.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/B67957B9-5683-DE11-9AD9-001AA00DBB38.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/B6CEEAD8-F581-DE11-80AD-00188B7AD27C.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/BCCD9A22-5583-DE11-B571-00221917D9AA.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/BE2A80AF-5683-DE11-A61A-001AA00B4852.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/C0D24EC7-0481-DE11-9DDB-00E081300930.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/C6C18CD0-5783-DE11-B476-001AA00B4E26.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/CEFB31C2-5683-DE11-9F36-001AA00DD851.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuX/GEN-SIM-RECO/MC_31X_V3-v1/0010/D202BB51-5683-DE11-B3EB-001AA00A6C97.root'
    )
)

process.hltTrigReport = cms.EDAnalyzer( "HLTrigReport",
    HLTriggerResults = cms.InputTag( 'TriggerResults','','HLT8E29' )
)


# Output definition
process.output1 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(99),
                                   outputCommands = process.AODSIMEventContent.outputCommands,
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/PD_Mu_ppMuX.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('AODSIM'),
                                      filterName = cms.untracked.string('PD_Mu_HLTMu3')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim1')
                                      )
)

# Yet another output definition
process.output2 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_Mu3_ppMuX.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_Mu3')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim2')
                                      )
)

# Yet another output definition
process.output3 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_DoubleMu_ppMuX.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_DoubleMu')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('skim3')
                                      )
)


# the usage of trigger bits for selection is explained here:
## https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideEDMPathsAndTriggerBits#Selecting_Pass_for_Some_Trigger 

#process.pTfilter = cms.Path(process.ptHat_filter)
process.hlttrigreport = cms.Path(~process.mumugenfilter+process.hltTrigReport)
process.skim1 = cms.Path(~process.mumugenfilter+process.hltHighLevelDev )
process.skim2 = cms.Path(~process.mumugenfilter+process.hltHighLevelDev2)
process.skim3 = cms.Path(~process.mumugenfilter+process.hltHighLevelDev3)


process.out_step = cms.EndPath(process.output1+process.output2+process.output3)

process.schedule = cms.Schedule(process.hlttrigreport,process.skim1,process.skim2,process.skim3)
process.schedule.append(process.out_step)

