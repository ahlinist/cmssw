import FWCore.ParameterSet.Config as cms

process = cms.Process("RAW")

process.load("Configuration.StandardSequences.Services_cff")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

process.source = cms.Source("MCFileSource",
    fileNames = cms.untracked.vstring('file:starlight_pbpb_up1s.mumu.hepmc-01.dat')
)

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.VtxSmearedEarly10TeVCollision_cff")


# Conditions: fake or frontier
# process.load("Configuration.StandardSequences.FakeConditions_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
# https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions
process.GlobalTag.globaltag = 'STARTUP_V4::All'

process.load("Configuration.StandardSequences.Generator_cff")
process.load("Configuration.StandardSequences.Sim_cff")
process.load("Configuration.StandardSequences.MixingNoPileUp_cff")
process.load("Configuration.StandardSequences.Digi_cff")
process.load("Configuration.StandardSequences.L1Emulator_cff")
process.load("Configuration.StandardSequences.DigiToRaw_cff")
# Choose a menu/prescale/mask from one of the choices in L1TriggerConfig.L1GtConfigProducers.Luminosity
#process.load("L1TriggerConfig.L1GtConfigProducers.Luminosity.lumi1030.L1Menu2008_2E30_Unprescaled_cff")

# L1 GT EventSetup
process.load("L1TriggerConfig.L1GtConfigProducers.L1GtConfig_cff")
process.load("L1TriggerConfig.L1GtConfigProducers.Luminosity.startup.L1Menu_startup2_v2_Unprescaled_cff")

process.hltL1gtTrigReport = cms.EDAnalyzer( "L1GtTrigReport",
                                            UseL1GlobalTriggerRecord = cms.bool( False ),
                                            L1GtRecordInputTag = cms.InputTag( "hltGtDigis" )
                                            )


process.doDigiL1Raw = cms.Path(process.pgen+process.psim+process.pdigi+process.L1Emulator+process.DigiToRaw)

process.load("Configuration.EventContent.EventContent_cff")
process.FEVT = cms.OutputModule("PoolOutputModule",
    process.FEVTDEBUGHLTEventContent,
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RAW')
    ),
    basketSize = cms.untracked.int32(4096),
    fileName = cms.untracked.string('file:HepMc_Digi_Raw.root')
)
process.REVT = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RAW')
    ),
    fileName = cms.untracked.string('file:HepMc_Pure_Raw.root'),
    basketSize = cms.untracked.int32(4096),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep FEDRawDataCollection_rawDataCollector_*_RAW')
)
process.e1 = cms.EndPath(process.FEVT)
process.e2 = cms.EndPath(process.REVT)
