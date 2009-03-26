import FWCore.ParameterSet.Config as cms

process = cms.Process("SiPixelInclusiveReader")



process.load('Configuration/StandardSequences/Services_cff')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/Sim_cff')
process.load('Configuration/StandardSequences/Digi_cff')
process.load('Configuration/StandardSequences/SimL1Emulator_cff')
process.load('Configuration/StandardSequences/L1TriggerDefaultMenu_cff')
process.load('Configuration/StandardSequences/DigiToRaw_cff')
process.load('Configuration/StandardSequences/RawToDigi_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'IDEAL_30X::All'


###### OUTPUT HISTOGRAM FILE NAME #######
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histo.root")
                                   )




##### DATABASE CONNECTION INFO ######

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
process.source = cms.Source("EmptySource",
    numberEventsInRun = cms.untracked.uint32(10),
    firstRun = cms.untracked.uint32(1)
)

process.Timing = cms.Service("Timing")

process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
    ignoreTotal = cms.untracked.int32(0)
)



####### GAIN READERS ######
process.SiPixelCondObjOfflineReader = cms.EDFilter("SiPixelCondObjOfflineReader",
    process.SiPixelGainCalibrationServiceParameters
)

process.SiPixelCondObjForHLTReader = cms.EDFilter("SiPixelCondObjForHLTReader",
    process.SiPixelGainCalibrationServiceParameters
)



####### LORENTZ ANGLE READER ######
process.SiPixelLorentzAngleReader = cms.EDFilter("SiPixelLorentzAngleReader")



####### CABLING MAP READER ######
process.SiPixelFedCablingMapAnalyzer = cms.EDAnalyzer("SiPixelFedCablingMapAnalyzer")


#######  QUALITY READER #######
process.SiPixelBadModuleReader = cms.EDAnalyzer("SiPixelBadModuleReader")


####### TEMPLATE OBJECT READER ######
#Change to True if you would like a more detailed error output
wantDetailedOutput = False
#Change to True if you would like to output the full template database object
wantFullOutput = False

process.SiPixelTemplateDBObjectReader = cms.EDFilter("SiPixelTemplateDBObjectReader",
                              siPixelTemplateCalibrationLocation = cms.string(
                             "CalibTracker/SiPixelESProducers"),
                              wantDetailedTemplateDBErrorOutput = cms.bool(wantDetailedOutput),
                              wantFullTemplateDBOutput = cms.bool(wantFullOutput))





####### DO ALL READERS (OR SELECT ONE YOU WANT) ########
process.p = cms.Path(process.SiPixelCondObjOfflineReader*process.SiPixelLorentzAngleReader*process.SiPixelFedCablingMapAnalyzer*process.SiPixelCondObjForHLTReader*process.SiPixelTemplateDBObjectReader*process.SiPixelBadModuleReader)



