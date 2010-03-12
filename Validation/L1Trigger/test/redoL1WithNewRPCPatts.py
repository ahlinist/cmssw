# Auto generated configuration file
# using: 
# Revision: 1.163 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: --filein dd.root -s L1 -n 10 --conditions MC_3XY_V21::All --no_exec --python_filename tmfTEST.py
import FWCore.ParameterSet.Config as cms

process = cms.Process('L1')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.MixingNoPileUp_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.EventContent.EventContent_cff')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.163 $'),
    annotation = cms.untracked.string('--filein nevts:10'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-500)
)
process.options = cms.untracked.PSet(

)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
       '/store/relval/CMSSW_3_5_4/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_3XY_V24-v1/0004/F289CA2E-2D2C-DF11-A2A0-002618943939.root'
      ,'/store/relval/CMSSW_3_5_4/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_3XY_V24-v1/0003/C6D0B2BB-9D2B-DF11-A57E-003048679008.root'
      ,'/store/relval/CMSSW_3_5_4/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_3XY_V24-v1/0003/A62CC5BD-9D2B-DF11-89FB-00261894392F.root'
      ,'/store/relval/CMSSW_3_5_4/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_3XY_V24-v1/0003/7C01AEBB-9D2B-DF11-8C49-001731AF66A1.root'
    )
)

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    outputCommands = process.RECOSIMEventContent.outputCommands,
    fileName = cms.untracked.string('/tmp/fruboes/processed354SingleMuRelVals_pt100.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string(''),
        filterName = cms.untracked.string('')
    )
)
keepL1=cms.untracked.vstring('drop *', 'keep *_*_*_L1', 'keep *_genParticles_*_*')
#process.output.outputCommands.extend(keepL1)
process.output.outputCommands=keepL1

process.load("L1TriggerConfig.RPCTriggerConfig.L1RPCConfig_cff")
process.rpcconf.filedir = cms.untracked.string('Validation/L1Trigger/test/v5/')
#process.simRpcTriggerDigis.RPCTriggerDebug = 1


# Additional output definition

# Other statements
process.GlobalTag.globaltag = 'MC_3XY_V25::All'

# Path and EndPath definitions
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.endjob_step = cms.Path(process.endOfProcess)
process.out_step = cms.EndPath(process.output)

# Schedule definition
process.schedule = cms.Schedule(process.L1simulation_step,process.endjob_step,process.out_step)
