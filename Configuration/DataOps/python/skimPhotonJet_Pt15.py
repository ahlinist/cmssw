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

process.hltHighLevelDev.HLTPaths = ("HLT_Photon15_L1R",)
process.hltHighLevelDev.andOr = True # True = OR, False = AND
process.hltHighLevelDev.HLTPathsPrescales  = cms.vuint32(5,)
process.hltHighLevelDev.HLTOverallPrescale = cms.uint32(1)


process.hltHighLevelDev2 = process.hltHighLevelDev.clone(andOr = True)
process.hltHighLevelDev2.HLTPaths = ("HLT_Photon20_L1R","HLT_Photon30_L1R_8E29")
process.hltHighLevelDev2.andOr = True # True = OR, False = AND
process.hltHighLevelDev2.HLTPathsPrescales  = cms.vuint32(1,1)
process.hltHighLevelDev2.HLTOverallPrescale = cms.uint32(1)

## Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
#process.hltHighLevelDev3 = process.hltHighLevelDev.clone(andOr = True)
#process.hltHighLevelDev3.HLTPaths = ("HLT_DoubleEle5_SW_L1R","HLT_DoublePhoton5_eeRes_L1R")
#process.hltHighLevelDev3.andOr = True # True = OR, False = AND
#process.hltHighLevelDev3.HLTPathsPrescales  = cms.vuint32(1,20)
#process.hltHighLevelDev3.HLTOverallPrescale = cms.uint32(1)
#
## Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
#process.hltHighLevelDev4 = process.hltHighLevelDev.clone(andOr = True)
#process.hltHighLevelDev4.HLTPaths = ("HLT_DoublePhoton5_Jpsi_L1R","HLT_DoublePhoton5_Upsilon_L1R")
#process.hltHighLevelDev4.andOr = True # True = OR, False = AND
#process.hltHighLevelDev4.HLTPathsPrescales  = cms.vuint32(1,1)
#process.hltHighLevelDev4.HLTOverallPrescale = cms.uint32(1)
#
## Once the hltHighLevelDev filter has been defined I can clone it and configure as needed
#process.hltHighLevelDev5 = process.hltHighLevelDev.clone(andOr = True)
#process.hltHighLevelDev5.HLTPaths = ("HLT_DoublePhoton10_L1R",)
#process.hltHighLevelDev5.andOr = True # True = OR, False = AND
#process.hltHighLevelDev5.HLTPathsPrescales  = cms.vuint32(1,)
#process.hltHighLevelDev5.HLTOverallPrescale = cms.uint32(1)
#
##############   pt_hat Filter  #####
#process.ptHat_filter = cms.EDFilter("MCProcessFilter",
#    ProcessID = cms.untracked.vint32(0),
#    MinPthat = cms.untracked.vdouble(-1.0),
#    MaxPthat = cms.untracked.vdouble(100000.)
#)

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
    destinations = cms.untracked.vstring('log/skimPhotonJet_15.log')
    )

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.123 $'),
    annotation = cms.untracked.string('skim'),
    name = cms.untracked.string('skim')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(153600000)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'file:/data/rossin/CMS/Triggers/Summer09/PhotonJet_Pt15/GEN-SIM-RECO/MC_31X_V3-v1/0014/749C0E38-B982-DE11-8661-001F29C9A554.root'
    )
)

process.hltTrigReport = cms.EDAnalyzer( "HLTrigReport",
    HLTriggerResults = cms.InputTag( 'TriggerResults','','HLT8E29' )
)


# Output definition
process.output1 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_Photon15_PhotonJet_15.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_Photon15')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                   SelectEvents = cms.vstring('skim1') #defining the skim.
                                      )
)

process.output2 = cms.OutputModule("PoolOutputModule",
                                   splitLevel = cms.untracked.int32(0),
                                   outputCommands = cms.untracked.vstring('keep *'),
                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_Photon20_PhotonJet_15.root'),
                                   dataset = cms.untracked.PSet(
                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
                                      filterName = cms.untracked.string('SD_Photon20')
                                      ),
                                   SelectEvents = cms.untracked.PSet(
                                   SelectEvents = cms.vstring('skim2') #defining the skim.
                                      )
)

#process.output3 = cms.OutputModule("PoolOutputModule",
#                                   splitLevel = cms.untracked.int32(0),
#                                   outputCommands = cms.untracked.vstring('keep *'),
#                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_DoubleEG5_PhotonJet_15.root'),
#                                   dataset = cms.untracked.PSet(
#                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
#                                      filterName = cms.untracked.string('SD_DoubleEG5')
#                                      ),
#                                   SelectEvents = cms.untracked.PSet(
#                                      SelectEvents = cms.vstring('skim3')
#                                      )
#)
#
## Yet another output definition
#process.output4 = cms.OutputModule("PoolOutputModule",
#                                   splitLevel = cms.untracked.int32(0),
#                                   outputCommands = cms.untracked.vstring('keep *'),
#                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_DoublePhoton5_JPsi_Upsilon_PhotonJet_15.root'),
#                                   dataset = cms.untracked.PSet(
#                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
#                                      filterName = cms.untracked.string('SD_DoublePhoton5_JPsi_Upsilon')
#                                      ),
#                                   SelectEvents = cms.untracked.PSet(
#                                      SelectEvents = cms.vstring('skim4')
#                                      )
#)
#
## Yet another output definition
#process.output5 = cms.OutputModule("PoolOutputModule",
#                                   splitLevel = cms.untracked.int32(0),
#                                   outputCommands = cms.untracked.vstring('keep *'),
#                                   fileName = cms.untracked.string('/data/rossin/CMS/Triggers/SDexercise/SD_DoublePhoton10_PhotonJet_15.root'),
#                                   dataset = cms.untracked.PSet(
#                                      dataTier = cms.untracked.string('GEN-SIM-RECO'),
#                                      filterName = cms.untracked.string('SD_DoublePhoton10')
#                                      ),
#                                   SelectEvents = cms.untracked.PSet(
#                                      SelectEvents = cms.vstring('skim5')
#                                      )
#)
#

# the usage of trigger bits for selection is explained here:
## https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideEDMPathsAndTriggerBits#Selecting_Pass_for_Some_Trigger 

#process.pTfilter = cms.Path(process.ptHat_filter)
process.hlttrigreport = cms.Path(process.hltTrigReport)
process.skim1 = cms.Path(process.hltHighLevelDev)
process.skim2 = cms.Path(process.hltHighLevelDev2)
#process.skim3 = cms.Path(process.hltHighLevelDev3)
#process.skim4 = cms.Path(process.hltHighLevelDev4)
#process.skim5 = cms.Path(process.hltHighLevelDev5)


process.out_step = cms.EndPath(process.output1+process.output2)

process.schedule = cms.Schedule(process.hlttrigreport,process.skim1,process.skim2)
process.schedule.append(process.out_step)


