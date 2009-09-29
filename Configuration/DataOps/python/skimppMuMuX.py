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

###############   mumugenfilter as in PYTHIA6_ppMuMuX_10TeV_cff.py  #####
#process.mumugenfilter = cms.EDFilter("MCParticlePairFilter",
#    Status = cms.untracked.vint32(1, 1),
#    MinPt = cms.untracked.vdouble(2.5, 2.5),
#    MaxEta = cms.untracked.vdouble(2.5, 2.5),
#    MinEta = cms.untracked.vdouble(-2.5, -2.5),
#    ParticleCharge = cms.untracked.int32(-1),
#    ParticleID1 = cms.untracked.vint32(13),
#    ParticleID2 = cms.untracked.vint32(13)
#)



process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('log/skimppMuMuX.log')
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
'file:/data/rossin/CMS/Triggers/Summer09/ppMuMuX/GEN-SIM-RECO/MC_31X_V3-v1/0003/4C67560F-ED8C-DE11-AB31-001AA009FBC6.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuMuX/GEN-SIM-RECO/MC_31X_V3-v1/0003/D294CC8A-9E8B-DE11-BA1B-00188B798D8C.root',
#'file:/data/rossin/CMS/Triggers/Summer09/ppMuMuX/GEN-SIM-RECO/MC_31X_V3-v1/0003/F65D34D0-F88C-DE11-9CDD-001AA009A197.root'
    )
)

process.hltTrigReport = cms.EDAnalyzer( "HLTrigReport",
    HLTriggerResults = cms.InputTag( 'TriggerResults','','HLT8E29' )
)


# Output definition
process.output1 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(99),
                                   outputCommands = process.AODSIMEventContent.outputCommands,
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/PD_Mu_ppMuMuX.root'),
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
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_Mu3_ppMuMuX.root'),
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
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_DoubleMu_ppMuMuX.root'),
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
process.hlttrigreport = cms.Path(process.hltTrigReport)
process.skim1 = cms.Path(process.hltHighLevelDev )
process.skim2 = cms.Path(process.hltHighLevelDev2)
process.skim3 = cms.Path(process.hltHighLevelDev3)


process.out_step = cms.EndPath(process.output1+process.output2+process.output3)

process.schedule = cms.Schedule(process.hlttrigreport,process.skim1,process.skim2,process.skim3)
process.schedule.append(process.out_step)

