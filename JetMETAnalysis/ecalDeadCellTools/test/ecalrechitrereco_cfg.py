# Auto generated configuration file
# using:
# Revision: 1.149
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v
# with command line options: reco -s RAW2DIGI,RECO --conditions FrontierCoitions_GlobalTag,MC_31X_V9::All --eventcontent=FEVT --no_output --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('RECO')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/MixingNoPileUp_cff')
process.load('Configuration/StandardSequences/GeometryExtended_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/RawToDigi_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/EndOfProcess_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.3 $'),
    annotation = cms.untracked.string('reco nevts:1'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.default.limit = 100

# summary
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0022/E48AF8D8-A0BF-DF11-BEDB-001A92971B48.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0022/DC4FB659-A5BF-DF11-AB27-0018F3D09608.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0022/D4127CD8-A0BF-DF11-B42F-0018F3D096AE.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0022/D2EEAF70-E1BF-DF11-9DD8-002618FDA211.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0022/C6991C55-A1BF-DF11-9B71-001A9281174C.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0022/B422F6D6-A1BF-DF11-922C-0018F3D09658.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0022/8AB9AB6A-A0BF-DF11-9B08-002354EF3BDA.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0022/7C49F0FB-A2BF-DF11-AE03-0018F3D096D2.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0022/763ABC5D-FDBF-DF11-B286-001A928116BE.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0022/60E5BEC0-2AC0-DF11-B69E-002618943914.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0022/229B5FD8-A0BF-DF11-9412-0018F3D09704.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0021/CECC2D70-9EBF-DF11-BE5E-0018F3D095F2.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0021/CCF6A6F6-9EBF-DF11-B664-0018F3D0965E.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0021/CA5A9FF7-9DBF-DF11-97F5-0018F3D096F6.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0021/C20D996F-9FBF-DF11-9F34-003048678C3A.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0021/BA95257B-9BBF-DF11-AAD2-0018F3D09654.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0021/B84586D6-9FBF-DF11-8381-0018F3D096E0.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0021/A0EC716A-A0BF-DF11-B7C4-002618943896.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0021/9E9C1A6F-9FBF-DF11-8379-0030486790BE.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0021/9893EF79-97BF-DF11-A9A3-001A92971AA8.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0021/82CA6670-9FBF-DF11-8B58-0018F3D0970A.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0021/708C90E6-9ABF-DF11-8469-0018F3D09688.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0021/66BD2DF1-99BF-DF11-8C0A-003048678B12.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0021/5C5C76E2-98BF-DF11-BB9D-003048679182.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0021/5AD56AE2-96BF-DF11-A3DF-001BFCDBD154.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0021/58DF70E7-9CBF-DF11-8A14-0018F3D09702.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0021/3C79C96D-9ABF-DF11-99F5-00261894396B.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0021/3836AFE7-95BF-DF11-8D1B-00304867BFB0.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0021/206E75E4-9EBF-DF11-9160-00248C55CC97.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0021/16F8936B-9CBF-DF11-AA90-0026189438C1.root',
       '/store/relval/CMSSW_3_8_3/RelValQCD_Pt_3000_3500/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_38Y_V9-v1/0021/048CDAD6-9FBF-DF11-83AF-001A92971B8E.root'
##         '/store/mc/Summer09/QCDFlat_Pt15to3000/GEN-SIM-RAW/MC_31X_V3_7TeV-v1/0001/F41F0B6C-18D1-DE11-8EA5-001EC9ED949B.root',
##         '/store/mc/Summer09/QCDFlat_Pt15to3000/GEN-SIM-RAW/MC_31X_V3_7TeV-v1/0001/F0E6B404-27D1-DE11-B2EF-00215A45F882.root',
##         '/store/mc/Summer09/QCDFlat_Pt15to3000/GEN-SIM-RAW/MC_31X_V3_7TeV-v1/0001/E8BE0BAD-1BD1-DE11-AAF9-00145EDD7905.root',
##         '/store/mc/Summer09/QCDFlat_Pt15to3000/GEN-SIM-RAW/MC_31X_V3_7TeV-v1/0001/E401DC59-31D2-DE11-8D44-0024E85A4BEC.root',
##         '/store/mc/Summer09/QCDFlat_Pt15to3000/GEN-SIM-RAW/MC_31X_V3_7TeV-v1/0001/C870FD15-5CD1-DE11-A0B3-00145EDD7ACD.root',
##         '/store/mc/Summer09/QCDFlat_Pt15to3000/GEN-SIM-RAW/MC_31X_V3_7TeV-v1/0001/B6428D65-0DD1-DE11-958B-001A648F19A6.root',
##         '/store/mc/Summer09/QCDFlat_Pt15to3000/GEN-SIM-RAW/MC_31X_V3_7TeV-v1/0001/B46770C5-2DD2-DE11-871B-00145EDD7481.root',
##         '/store/mc/Summer09/QCDFlat_Pt15to3000/GEN-SIM-RAW/MC_31X_V3_7TeV-v1/0001/B0605225-5CD1-DE11-A7BE-00145EDD7AE9.root',
##         '/store/mc/Summer09/QCDFlat_Pt15to3000/GEN-SIM-RAW/MC_31X_V3_7TeV-v1/0001/A8E7B776-0DD1-DE11-AF65-001A648F1DC2.root',
##         '/store/mc/Summer09/QCDFlat_Pt15to3000/GEN-SIM-RAW/MC_31X_V3_7TeV-v1/0001/98AA6163-8CD1-DE11-B2C8-0022198273E8.root',
##         '/store/mc/Summer09/QCDFlat_Pt15to3000/GEN-SIM-RAW/MC_31X_V3_7TeV-v1/0001/6C301174-18D1-DE11-83F2-001EC9ED5AFA.root'
    )
)

process.output = cms.OutputModule("PoolOutputModule",
   splitLevel = cms.untracked.int32(0),
   outputCommands = process.RECOSIMEventContent.outputCommands,
   fileName = cms.untracked.string('rereco_ECALRecovery.root'),
   dataset = cms.untracked.PSet(
      dataTier = cms.untracked.string(''),
      filterName = cms.untracked.string('')
   )
)
# Additional output definition

# Other statements
#process.GlobalTag.globaltag = 'MC_38Y_V10::All'
process.GlobalTag.globaltag = 'MC_38Y_V9::All'

#############################
process.load("Configuration.StandardSequences.Generator_cff")
process.genJet_step = cms.Path(process.genJetMET)

process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)
process.endjob_step = cms.Path(process.endOfProcess)
process.out_step = cms.EndPath(process.output)

#############################
#from UserCode.EcalDeadCellEventFilter.ecalDeadCellEventFilter_cfi import *
process.load('UserCode.EcalDeadCellEventFilter.ecalDeadCellEventFilter_cff')
process.filter = cms.Path(process.EcalDeadCellEventFilter)

#############################
process.load("SimCalorimetry.Configuration.ecalDigiSequence_cff")

process.tpparams12 = cms.ESSource("EmptyESSource",
recordName = cms.string('EcalTPGPhysicsConstRcd'),
iovIsRunNotTime = cms.bool(True),
firstValid = cms.vuint32(1)
)
# Mixing Module
process.load("SimGeneral.MixingModule.mixNoPU_cfi")

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
#moduleSeeds = cms.PSet(
#simEcalUnsuppressedDigis = cms.untracked.uint32(9876),
simEcalUnsuppressedDigis = cms.PSet(
        initialSeed = cms.untracked.uint32(9876),
        engineName = cms.untracked.string('HepJamesRandom')
    ),

mix = cms.PSet(
        initialSeed = cms.untracked.uint32(9854876),
        engineName = cms.untracked.string('HepJamesRandom')
    ),
#),
#sourceSeed = cms.PSet(
source = cms.PSet(
        initialSeed = cms.untracked.uint32(135799753),
        engineName = cms.untracked.string('HepJamesRandom')
    ),
)
process.ecalGlobalUncalibRecHit.EBdigiCollection = 'simEcalDigis:ebDigis'
process.ecalGlobalUncalibRecHit.EEdigiCollection = 'simEcalDigis:eeDigis'

process.redigiNew = cms.Path(process.mix*process.ecalDigiSequence)

#############################

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step,process.genJet_step,process.filter,process.endjob_step,process.out_step)
#process.schedule = cms.Schedule(process.raw2digi_step,process.redigiNew,process.reconstruction_step,process.genJet_step,process.filter,process.endjob_step,process.out_step)
