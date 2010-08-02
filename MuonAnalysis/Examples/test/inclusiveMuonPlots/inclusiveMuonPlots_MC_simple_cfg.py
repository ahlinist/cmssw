import FWCore.ParameterSet.Config as cms

process = cms.Process("MuonPlots")

# Messages
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.GlobalTag.globaltag = 'START38_V8::All'

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        ## Produced with MuonAnalysis/Examples/test/patSkims/patMuons_mc_cfg.py
        'root://pcmssd12.cern.ch//data/gpetrucc/7TeV/inclusiveMuons/inclusiveMuons_MuHLT_MinBiasMC357_v5/tupleMC_1_1.root',
    )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.TFileService = cms.Service('TFileService', fileName=cms.string('inclusiveMuonPlots_MC_simple.class.root') )

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
## Now we make the sub-plots with classification by hits
process.globalMuonsGhost = process.globalMuons.clone(selection = "isGlobalMuon && (-5 <= userInt('classByHitsGlb') <= -1)")
process.globalMuonsPunch = process.globalMuons.clone(selection = "isGlobalMuon && ( 0 <= userInt('classByHitsGlb') <=  1)")
process.globalMuonsLight = process.globalMuons.clone(selection = "isGlobalMuon && (userInt('classByHitsGlb') == 2)")
process.globalMuonsHeavy = process.globalMuons.clone(selection = "isGlobalMuon && (userInt('classByHitsGlb') >= 3)")

process.p = cms.Path(
    process.recoMu +
    process.bscMinBias +
    process.globalMuons +    
    process.globalMuonsGhost +    
    process.globalMuonsPunch +    
    process.globalMuonsLight +    
    process.globalMuonsHeavy     
)
