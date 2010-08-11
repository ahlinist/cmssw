# Auto generated configuration file
# using: 
# Revision: 1.192 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: hlt -s HLT:GRun --processName=HLT38 --data --magField AutoFromDBCurrent --scenario pp --datatier RAW-HLT --eventcontent=FEVTDEBUGHLT --customise=HLTrigger/Configuration/customHLTData_Options.py --conditions=GR_R_38X_V7::All --python_filename=hlt_38x.py --no_exec --filein RAW.root
import FWCore.ParameterSet.Config as cms

process = cms.Process('HLT38')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('HLTrigger.Configuration.HLT_GRun_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.EventContent.EventContent_cff')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.192 $'),
    annotation = cms.untracked.string('hlt nevts:1'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
process.options = cms.untracked.PSet(

)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('RAW.root')
)

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
    fileName = cms.untracked.string('hlt_HLT.root'),
    dataset = cms.untracked.PSet(
        # KH   
        dataTier = cms.untracked.string('RAW'),
        #dataTier = cms.untracked.string('RAW-HLT'),
        filterName = cms.untracked.string('')
    )
)

# Additional output definition

# Other statements
process.GlobalTag.globaltag = 'GR_R_38X_V7::All'

# Path and EndPath definitions
process.endjob_step = cms.Path(process.endOfProcess)
process.out_step = cms.EndPath(process.output)

# Schedule definition
process.schedule = cms.Schedule()
process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.endjob_step,process.out_step])

# Automatic addition of the customisation function
def customise(process):
    if 'hltTrigReport' in process.__dict__:
        process.hltTrigReport.HLTriggerResults = cms.InputTag( 'TriggerResults','',process.name_() )

    if 'hltDQMHLTScalers' in process.__dict__:
        process.hltDQMHLTScalers.triggerResults = cms.InputTag( 'TriggerResults','',process.name_() )

    if 'hltDQML1SeedLogicScalers' in process.__dict__:
        process.hltDQML1SeedLogicScalers.processname = process.name_()

    process.options.wantSummary = cms.untracked.bool(True)
    process.MessageLogger.categories.append('TriggerSummaryProducerAOD')
    process.MessageLogger.categories.append('L1GtTrigReport')
    process.MessageLogger.categories.append('HLTrigReport')

    # drop on input the previous HLT results
    #process.source.inputCommands = cms.untracked.vstring (
    #    'keep *',
    #    'drop *_hltL1GtObjectMap_*_*',
    #    'drop *_TriggerResults_*_*',
    #    'drop *_hltTriggerSummaryAOD_*_*',
    #)

    # override RAW data name to rn on data
    process.hltGetRaw.RawDataCollection                  = "source"
    process.hltGtDigis.DaqGtInputTag                     = "source"
    process.hltGctDigis.inputLabel                       = "source"
    process.hltScalersRawToDigi.scalersInputTag          = "source"
    process.hltSiPixelDigis.InputLabel                   = "source"
    process.hltMuonCSCDigis.InputObjects                 = "source"
    process.hltMuonDTDigis.inputLabel                    = "source"
    process.hltDTTFUnpacker.DTTF_FED_Source              = "source"
    process.hltEcalRawToRecHitFacility.sourceTag         = "source"
    process.hltESRawToRecHitFacility.sourceTag           = "source"
    process.hltHcalDigis.InputLabel                      = "source"
    process.hltMuonRPCDigis.InputLabel                   = "source"
    process.hltSiStripRawToClustersFacility.ProductLabel = "source"
    process.hltCscTfDigis.producer                       = "source"
    process.hltL1EventNumberNZS.rawInput                 = "source"
    process.hltDTROMonitorFilter.inputLabel              = "source"
    process.hltEcalCalibrationRaw.rawInputLabel          = "source"
    process.hltHcalCalibTypeFilter.InputTag              = "source"
    process.hltDTDQMEvF.inputLabel                       = "source"
    process.hltEcalDigis.InputLabel                      = "source"
    process.hltEBHltTask.FEDRawDataCollection            = "source"
    process.hltEEHltTask.FEDRawDataCollection            = "source"
    process.hltL1tfed.rawTag                             = "source"
    process.hltSiPixelDigisWithErrors.InputLabel         = "source"
    process.hltSiPixelHLTSource.RawInput                 = "source"
    process.hltSiStripFEDCheck.RawDataTag                = "source"

    return(process)


# End of customisation function definition

process = customise(process)
