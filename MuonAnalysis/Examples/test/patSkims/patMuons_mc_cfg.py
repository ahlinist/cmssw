import FWCore.ParameterSet.Config as cms

process = cms.Process("PATMuons")

### standard includes
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000 
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

### global tag
process.GlobalTag.globaltag = 'START38_V8::All'

### source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/relval/CMSSW_3_8_0/RelValMinBias/GEN-SIM-RECO/START38_V7-v1/0005/9EC22559-2C95-DF11-9F1D-001A92811744.root',
        '/store/relval/CMSSW_3_8_0/RelValMinBias/GEN-SIM-RECO/START38_V7-v1/0005/24C78530-3D95-DF11-80F8-00261894393F.root',
        '/store/relval/CMSSW_3_8_0/RelValMinBias/GEN-SIM-RECO/START38_V7-v1/0004/1415F186-0195-DF11-97D0-003048678FE6.root',
    )
)

### number of events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

### FILTERS for GoodCollision
process.oneGoodVertexFilter = cms.EDFilter("VertexSelector",
   src = cms.InputTag("offlinePrimaryVertices"),
   cut = cms.string("!isFake && ndof >= 4 && abs(z) <= 15 && position.Rho <= 2"),
   filter = cms.bool(True),   # otherwise it won't filter the events, just produce an empty vertex collection.
)
process.noScraping = cms.EDFilter("FilterOutScraping",
    applyfilter = cms.untracked.bool(True),
    debugOn = cms.untracked.bool(False), ## Or 'True' to get some per-event info
    numtrack = cms.untracked.uint32(10),
    thresh = cms.untracked.double(0.25)
)

process.globalMuFilter = cms.EDFilter("TrackCountFilter",    src = cms.InputTag("globalMuons"), minNumber = cms.uint32(1))
process.recoMuFilter   = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("muons"),       minNumber = cms.uint32(1))

process.preFilter = cms.Sequence(process.oneGoodVertexFilter * process.noScraping)

from HLTrigger.HLTfilters.hltHighLevelDev_cfi import hltHighLevelDev
process.bscMinBiasOR = hltHighLevelDev.clone(HLTPaths = ['HLT_L1_BscMinBiasOR_BptxPlusORMinus'], HLTPathsPrescales = [1])
process.hltL1MuOpen  = hltHighLevelDev.clone(HLTPaths = ['HLT_L1MuOpen'],                        HLTPathsPrescales = [1])

process.skim_RecoMu            = cms.Path(process.preFilter * process.recoMuFilter  )
process.skim_GlbMu             = cms.Path(process.preFilter * process.globalMuFilter)
process.skim_RecoMuBscOR       = cms.Path(process.preFilter * process.recoMuFilter   * process.bscMinBiasOR)
process.skim_RecoMuHLTL1MuOpen = cms.Path(process.preFilter * process.recoMuFilter   * process.hltL1MuOpen)

### Adding Trigger Info from TriggerResultsSummary to the PATMuon
process.load("MuonAnalysis.MuonAssociators.patMuonsWithTrigger_8E29_cff")
from MuonAnalysis.MuonAssociators.patMuonsWithTrigger_8E29_cff import changeTriggerProcessName, switchOffAmbiguityResolution;

## ==== Change trigger process name ====
#changeTriggerProcessName(process, "HLT2")   # Custom re-run HLT
#changeTriggerProcessName(process, "REDIGI") # Spring10 ReDigi

## ==== Change which track is used for L1 matching ====
#process.muonL1Info.useTrack = 'global'
#process.muonL1Info.useState = 'outermost'
#process.muonMatchHLTL1.useTrack = 'global'
#process.muonMatchHLTL1.useState = 'outermost'

## === Switch off ambiguity resolution in trigger matching ====
## can be a good idea if you're working with events with a single true muon + ghosts
## and you're applying the quality cuts that kill the ghosts only afterwards
switchOffAmbiguityResolution(process)

### Adding MCtruth Info to the PATMuon
from MuonAnalysis.MuonAssociators.patMuonsWithTrigger_8E29_cff import addMCinfo
addMCinfo(process)

### Add MC classification by hits
process.load("MuonAnalysis.MuonAssociators.muonClassificationByHits_cfi")
from MuonAnalysis.MuonAssociators.muonClassificationByHits_cfi import addUserData as addClassByHits
addClassByHits(process.patMuonsWithoutTrigger, extraInfo=True)
## add two other classifications with preselection, to get different ghosts
process.classByHitsTMA = process.classByHitsTM.clone(
    muonPreselection = cms.string("muonID('TrackerMuonArbitrated')")
)
process.classByHitsGlbPT = process.classByHitsGlb.clone(
    muonPreselection = cms.string("muonID('GlobalMuonPromptTight')")
)
process.muonClassificationByHits.replace(process.classByHitsTM,  process.classByHitsTM  + process.classByHitsTMA)
process.muonClassificationByHits.replace(process.classByHitsGlb, process.classByHitsGlb + process.classByHitsGlbPT)
addClassByHits(process.patMuonsWithoutTrigger, labels=["classByHitsTMA","classByHitsGlbPT"], extraInfo=False)

### Adding Info about the Muon Station involved to the PATMuon
process.load("MuonAnalysis.Examples.muonStations_cfi")
from MuonAnalysis.Examples.muonStations_cfi import addUserData as addStations
addStations(process.patMuonsWithoutTrigger)


process.p = cms.Path(
    process.preFilter  +
    process.recoMuFilter  +
    process.muonClassificationByHits +
    process.muonStations +
    process.patMuonsWithTriggerSequence 
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('tupleMC.root'),
    outputCommands = cms.untracked.vstring('drop *',
        'keep patMuons_patMuonsWithTrigger__*',                    ## All PAT muons
        'keep edmTriggerResults_TriggerResults_*_*',               ## HLT info, per path (cheap)
        'keep l1extraL1MuonParticles_l1extraParticles_*_*',        ## L1 info (cheap)
        #--- Other things you might want ---
        'keep *_offlinePrimaryVertices__*',                  ## 
        'keep *_offlineBeamSpot__*',                         ##
        'keep recoTrackExtras_standAloneMuons_*_*',          ## track states at the muon system, to which patMuons sta tracks point (useful if you want variables of the innermost or outermost state)
        'keep TrackingRecHitsOwned_standAloneMuons_*_*',     ## muon rechits, to compute things like number of stations
        #--- Other things which can be expensive but you might still want them ---
        'keep triggerTriggerEvent_hltTriggerSummaryAOD__*',  ## HLT 4-vectors
        'keep *_patTrigger__*',                              ## As above, in PAT format
        #--- HLT DEBUG, usually not available ---
        'keep recoTracks_hltL2Muons_*_*',
        'keep recoTracks_hltL3Muons_*_*',
        'keep recoTrackExtras_hltL2Muons_*_*',
        'keep recoTrackExtras_hltL3Muons_*_*',
        'keep recoRecoChargedCandidates_hltL2MuonCandidates__*',
        'keep recoRecoChargedCandidates_hltL3MuonCandidates__*',
    ),
    SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring("skim_RecoMuBscOR") ),
)
process.e = cms.EndPath(process.out)
