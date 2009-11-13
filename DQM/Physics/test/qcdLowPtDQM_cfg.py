# $Id: qcdLowPtDQM_cfg.py,v 1.2 2009/11/11 16:01:00 loizides Exp $

import FWCore.ParameterSet.Config as cms

process = cms.Process("QcdLowPtDQM")

process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('DQM/Physics/qcdLowPtDQM_cfi')

process.GlobalTag.globaltag = 'MC_31X_V9::All'

process.dqmSaver.workflow = cms.untracked.string('/Physics/QCDPhysics/LowPt')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
#    input = cms.untracked.int32(500)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#        'file:/opt/data/cms/CMSSW_3_3_2/RelValMinBias-GEN-SIM-DIGI-RAW-HLTDEBUG-MC_31X_V9-v1.root'
        'file:/opt/data/cms/CMSSW_3_3_2/RelValMinBias-GEN-SIM-RECO-MC_31X_V9-v2.root'
    )
)

process.siPixelDigis.InputLabel = cms.InputTag("source")

process.p = cms.Path(process.myRecoSequence*process.QcdLowPtDQM+process.dqmSaver)
