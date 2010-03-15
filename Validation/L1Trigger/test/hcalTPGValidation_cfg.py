import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
       '/store/relval/CMSSW_3_5_4/RelValQCD_FlatPt_15_3000/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_3XY_V24-v1/0004/D24293FA-2C2C-DF11-988E-00261894380A.root',
       '/store/relval/CMSSW_3_5_4/RelValQCD_FlatPt_15_3000/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_3XY_V24-v1/0003/EC9BD81D-9D2B-DF11-89FA-003048678FF8.root',
       '/store/relval/CMSSW_3_5_4/RelValQCD_FlatPt_15_3000/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_3XY_V24-v1/0003/E4DCAD2B-9F2B-DF11-8C1E-003048678FFE.root',
       '/store/relval/CMSSW_3_5_4/RelValQCD_FlatPt_15_3000/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_3XY_V24-v1/0003/D8618CC1-9D2B-DF11-900D-001731AF6867.root',
       '/store/relval/CMSSW_3_5_4/RelValQCD_FlatPt_15_3000/GEN-SIM-DIGI-RAW-HLTDEBUG/MC_3XY_V24-v1/0003/C8BCE998-9B2B-DF11-913C-0018F3D09710.root'
   )
)

#DQM
process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")
#Save root file to /tmp/user
import os
user=os.getenv("USER")
process.dqmSaver.dirName='/tmp/%s'%user
process.dqmSaver.workflow = cms.untracked.string('/A/N/C')

process.load('SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.GlobalTag.globaltag = "MC_3XY_V24::All"
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')

process.load('SimCalorimetry.HcalTrigPrimProducers.hcaltpdigi_cff')

process.load('Validation.L1Trigger.hcalTPGValidation_cfi')

process.p = cms.Path(
   process.hcalTPGValidation +
   process.dqmSaver
)
