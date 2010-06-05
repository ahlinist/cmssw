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
#process.GlobalTag.globaltag = 'START36_V9::All'
process.GlobalTag.globaltag = 'START3X_V26B::All'

### source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'root://pcmssd12.cern.ch//data/gpetrucc/7TeV/hlt/MuHLT_MinBiasMC357_185_1.root',
        #'root://pcmssd12.cern.ch//data/gpetrucc/7TeV/jpsi/ppMuX_Spring10_REDIGI_START3X_V26_S09_GEN-SIM-RECO_C0AC7DEB-8144-DF11-A1E1-00304867D838.root'
    )
)

### number of events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

### FILTERS for GoodCollision
process.load("L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff")
from HLTrigger.HLTfilters.hltLevel1GTSeed_cfi import hltLevel1GTSeed
hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
process.bscFilter = hltLevel1GTSeed.clone(L1SeedsLogicalExpression = cms.string('(40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)'))
process.bit40     = hltLevel1GTSeed.clone(L1SeedsLogicalExpression = cms.string('(40 OR 41)'))
process.haloVeto  = hltLevel1GTSeed.clone(L1SeedsLogicalExpression = cms.string('NOT (36 OR 37 OR 38 OR 39)'))
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

process.Flag_BSC      = cms.Path(process.noScraping * process.oneGoodVertexFilter + process.bscFilter)
process.Flag_Bit40    = cms.Path(process.bit40)
process.Flag_HaloVeto = cms.Path(process.haloVeto)

from HLTrigger.HLTfilters.hltHighLevelDev_cfi import hltHighLevelDev
process.bscMinBiasWithPrescale   = hltHighLevelDev.clone(HLTPaths = ['HLT_MinBiasBSC'],                      HLTPathsPrescales = [1])
process.bscMinBiasORWithPrescale = hltHighLevelDev.clone(HLTPaths = ['HLT_L1_BscMinBiasOR_BptxPlusORMinus'], HLTPathsPrescales = [1])
process.hltMu3                   = hltHighLevelDev.clone(HLTPaths = ['HLT_Mu3'],                             HLTPathsPrescales = [1])
process.hltL1MuOpen              = hltHighLevelDev.clone(HLTPaths = ['HLT_L1MuOpen'],                        HLTPathsPrescales = [1])

process.Count_BscOR  = cms.Path(process.preFilter * process.bscMinBiasORWithPrescale)
process.Count_Bsc    = cms.Path(process.preFilter * process.bscMinBiasWithPrescale)
process.Count_RecoMu = cms.Path(process.preFilter * process.recoMuFilter)
process.Count_GlbMu  = cms.Path(process.preFilter * process.globalMuFilter )
process.Count_RecoMu_BscOR = cms.Path(process.preFilter * process.recoMuFilter    * process.bscMinBiasORWithPrescale)
process.Count_GlbMu_BscOR  = cms.Path(process.preFilter * process.globalMuFilter  * process.bscMinBiasORWithPrescale)
process.Count_RecoMu_Bsc   = cms.Path(process.preFilter * process.recoMuFilter    * process.bscMinBiasWithPrescale)
process.Count_GlbMu_Bsc    = cms.Path(process.preFilter * process.globalMuFilter  * process.bscMinBiasWithPrescale)
process.Count_RecoMu_Mu3   = cms.Path(process.preFilter * process.recoMuFilter    * process.hltMu3)
process.Count_GlbMu_Mu3    = cms.Path(process.preFilter * process.globalMuFilter  * process.hltMu3)
process.Count_RecoMu_L1MuOpen = cms.Path(process.preFilter * process.recoMuFilter    * process.hltL1MuOpen)
process.Count_GlbMu_L1MuOpen  = cms.Path(process.preFilter * process.globalMuFilter  * process.hltL1MuOpen)

process.skim_GoodColl          = cms.Path(process.preFilter)
process.skim_RecoMu            = cms.Path(process.preFilter * process.recoMuFilter  )
process.skim_GlbMu             = cms.Path(process.preFilter * process.globalMuFilter)
process.skim_RecoMuBsc         = cms.Path(process.preFilter * process.recoMuFilter   * process.bscMinBiasWithPrescale)
process.skim_RecoMuBscOR       = cms.Path(process.preFilter * process.recoMuFilter   * process.bscMinBiasORWithPrescale)
process.skim_RecoMuHLTMu3      = cms.Path(process.preFilter * process.recoMuFilter   * process.hltMu3)
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
# Requires:
#   SimGeneral/TrackingAnalysis V04-01-00-02 (35X) or V04-01-03+ (37X+)
#   SimTracker/TrackAssociation V01-08-17    (35X+)
#   SimMuon/MCTruth             V02-05-00-01 (35X) or V02-06-00+ (37X+)
#   MuonAnalysis/MuonAssociators V01-08-05
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
# Requires MuonAnalysis/Examples V00-03-00+
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
    #SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring("skim_RecoMuBscOR", "skim_RecoMuHLTMu3", "skim_GlbMu") ),
)
process.e = cms.EndPath(process.out)
