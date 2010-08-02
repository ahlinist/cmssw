import FWCore.ParameterSet.Config as cms

process = cms.Process("MuonPlots")

# Messages
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.GlobalTag.globaltag = 'GR_R_38X_V8::All'

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        ## Produced with MuonAnalysis/Examples/test/patSkims/patMuons_data_cfg.py
        'root://pcmssd12.cern.ch//data/gpetrucc/7TeV/inclusiveMuons/inclusiveMuons_fromMay6th_v4/tupleData_42_1.root',
    )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.TFileService = cms.Service('TFileService', fileName=cms.string('inclusiveMuonPlots_Data_simple.root') )

from HLTrigger.HLTfilters.hltHighLevelDev_cfi import hltHighLevelDev
reskim  = hltHighLevelDev.clone(TriggerResultsTag = cms.InputTag('TriggerResults','',''))
process.recoMu     = reskim.clone(HLTPaths = ['skim_RecoMu'], HLTPathsPrescales = [1])
process.bscMinBias = hltHighLevelDev.clone(HLTPaths = ['HLT_L1_BscMinBiasOR_BptxPlusORMinus'], HLTPathsPrescales = [1])

## Common plots
from MuonAnalysis.Examples.inclusiveMuonPlots_cfi import makeInclusiveMuonPlots;
process.globalMuons = cms.EDAnalyzer("InclusiveMuonPlots",
    makeInclusiveMuonPlots(),
    muons     = cms.InputTag('patMuonsWithTrigger'),
    selection = cms.string("isGlobalMuon"),
    onlyLeadingMuon = cms.bool(False),
    primaryVertices = cms.InputTag("offlinePrimaryVertices"),
)

process.p = cms.Path(
    process.recoMu +
    process.bscMinBias +
    process.globalMuons 
)
