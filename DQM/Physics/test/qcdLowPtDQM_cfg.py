# $Id: qcdLowPtDQM_cfg.py,v 1.3 2009/11/13 09:59:18 loizides Exp $

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

process.options = cms.untracked.PSet(
    FailPath = cms.untracked.vstring("ProductNotFound")
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

process.dump = cms.EDAnalyzer('EventContentAnalyzer')

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/opt/data/cms/CMSSW_3_3_2/RelValMinBias-GEN-SIM-RECO-MC_31X_V9-v2.root'
    )
)

# uncomment if you run on MC raw
#process.p1 = cms.Path(
#    process.myRecoSeq1
#)
#process.siPixelDigis.InputLabel = cms.InputTag("rawDataCollector")

process.p2 = cms.Path(
    process.myRecoSeq2  *
    process.QcdLowPtDQM +
    process.dqmSaver
)

process.dqmSaver.workflow = cms.untracked.string('/Physics/QCDPhysics/LowPt')
