import FWCore.ParameterSet.Config as cms

process = cms.Process("testGctValidation")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("DQMServices.Core.DQM_cfg")
process.load("Validation.L1Trigger.gctValidation_cfi")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(

        '/store/relval/CMSSW_3_1_1/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V2-v1/0002/ECA5F052-756B-DE11-9274-000423D99658.root',
        '/store/relval/CMSSW_3_1_1/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V2-v1/0002/EA2FCF1E-756B-DE11-B718-000423D944F0.root',
        '/store/relval/CMSSW_3_1_1/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V2-v1/0002/CA0A2AB9-746B-DE11-ABC1-000423D99896.root',
        '/store/relval/CMSSW_3_1_1/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V2-v1/0002/9C462FBF-746B-DE11-A306-000423D98C20.root',
        '/store/relval/CMSSW_3_1_1/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V2-v1/0002/88CBBCAC-746B-DE11-BFA2-000423D9970C.root',
        '/store/relval/CMSSW_3_1_1/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V2-v1/0002/7E56E50C-756B-DE11-A3F9-000423D99658.root',
        '/store/relval/CMSSW_3_1_1/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V2-v1/0002/741B9B5F-756B-DE11-B0E4-000423D999CA.root',
        '/store/relval/CMSSW_3_1_1/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V2-v1/0002/6E0D73E2-746B-DE11-B117-001D09F28E80.root',
        '/store/relval/CMSSW_3_1_1/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V2-v1/0002/6A256C88-766B-DE11-9C66-000423D996C8.root',
        '/store/relval/CMSSW_3_1_1/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V2-v1/0002/3891C5C5-E16B-DE11-902C-001D09F244BB.root',
        '/store/relval/CMSSW_3_1_1/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V2-v1/0002/2EE801A9-756B-DE11-AE4C-001D09F2514F.root',
        '/store/relval/CMSSW_3_1_1/RelValZEE/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_31X_V2-v1/0002/284864FC-746B-DE11-83EA-001D09F2546F.root'
        
        #'/store/relval/CMSSW_3_1_2/RelValTTbar/GEN-SIM-RECO/STARTUP31X_V2-v1/0006/0CC00E3B-5A78-DE11-A2AB-000423D94A04.root'


    )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = cms.string('STARTUP31X_V1::All')
#process.GlobalTag.globaltag = cms.string('MC_31X_V1::All')

process.myPath = cms.Path(
	
    process.gctValidation
        
)
