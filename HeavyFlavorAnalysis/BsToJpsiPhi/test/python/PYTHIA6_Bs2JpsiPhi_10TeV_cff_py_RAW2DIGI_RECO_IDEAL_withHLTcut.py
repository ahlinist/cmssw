# Auto generated configuration file
# using: 
# Revision: 1.99.2.3 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: Configuration/GenProduction/python/PYTHIA6_Bs2JpsiPhi_10TeV_cff.py -s RAW2DIGI,RECO --filein file:test_100_HLT.root --eventcontent RECOSIM --datatier GEN-SIM-RECO --conditions FrontierConditions_GlobalTag,IDEAL_V11::All --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('RECO')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/MixingNoPileUp_cff')
process.load('Configuration/StandardSequences/GeometryPilot2_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('HLTrigger.HLTfilters.hltHighLevel_cfi')
process.load('Configuration/StandardSequences/RawToDigi_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/EndOfProcess_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.99.2.3 $'),
    annotation = cms.untracked.string('Configuration/GenProduction/python/PYTHIA6_Bs2JpsiPhi_10TeV_cff.py nevts:1'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:BsToJpsiPhi_uptoHLT_1.root')
)

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = process.RECOSIMEventContent.outputCommands,
    fileName = cms.untracked.string('BsToJpsiPhi_full_1.root'),
#    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('filter_step') ),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-RECO'),
        filterName = cms.untracked.string('')
    )
)

# Additional output definition

# Other statements
process.GlobalTag.globaltag = 'IDEAL_V11::All'

process.hltFilter = cms.EDFilter( "HLTHighLevel",
   TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
   HLTPaths = cms.vstring('HLT_DoubleMu3'),   # provide list of HLT paths (or patterns) you want
   andOr = cms.bool(False),             # how to deal with multiple triggers: True (OR) accept if ANY is true, False (AND) accept if ALL are true
)

# Path and EndPath definitions
#process.filter_step = cms.Path(process.hltFilter)
#process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.hltFilter*process.RawToDigi*process.reconstruction)
#process.endjob_step = cms.Path(process.endOfProcess)
process.out_step = cms.EndPath(process.output)

# Schedule definition
#process.schedule = cms.Schedule(process.filter_step,process.raw2digi_step,process.reconstruction_step,process.endjob_step,process.out_step)
process.schedule = cms.Schedule(process.reconstruction_step,process.out_step)
