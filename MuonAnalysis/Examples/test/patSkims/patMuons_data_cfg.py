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
process.GlobalTag.globaltag = 'GR_R_38X_V8::All'

### source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'root://castorcms.cern.ch//castor/cern.ch/cms/store/data/Run2010A/MinimumBias/RECO/v4/000/140/401/C08FEE9D-0493-DF11-96AB-0030487C5CE2.root',
        'root://castorcms.cern.ch//castor/cern.ch/cms/store/data/Run2010A/MinimumBias/RECO/v4/000/140/401/BE79BB59-0C93-DF11-9434-0019B9F72CE5.root',
        'root://castorcms.cern.ch//castor/cern.ch/cms/store/data/Run2010A/MinimumBias/RECO/v4/000/140/401/B2F7A0EC-0393-DF11-B645-0030487CD7B4.root',
	'root://castorcms.cern.ch//castor/cern.ch/cms/store/data/Run2010A/MinimumBias/RECO/v4/000/140/401/AC14EE84-0293-DF11-ADAF-003048F11C58.root',
	'root://castorcms.cern.ch//castor/cern.ch/cms/store/data/Run2010A/MinimumBias/RECO/v4/000/140/401/A44D9EB0-FF92-DF11-A969-003048F1C836.root',
	'root://castorcms.cern.ch//castor/cern.ch/cms/store/data/Run2010A/MinimumBias/RECO/v4/000/140/401/90722D59-0C93-DF11-9F00-0030487CD700.root',
	'root://castorcms.cern.ch//castor/cern.ch/cms/store/data/Run2010A/MinimumBias/RECO/v4/000/140/401/68F430EB-0393-DF11-A908-000423D94494.root',
	'root://castorcms.cern.ch//castor/cern.ch/cms/store/data/Run2010A/MinimumBias/RECO/v4/000/140/401/582FF9F0-0A93-DF11-B09E-003048D2BB58.root',
	'root://castorcms.cern.ch//castor/cern.ch/cms/store/data/Run2010A/MinimumBias/RECO/v4/000/140/401/48B4C919-0193-DF11-B2FC-000423D94908.root',
	'root://castorcms.cern.ch//castor/cern.ch/cms/store/data/Run2010A/MinimumBias/RECO/v4/000/140/401/405754D8-3093-DF11-BB2B-003048F11114.root',
	'root://castorcms.cern.ch//castor/cern.ch/cms/store/data/Run2010A/MinimumBias/RECO/v4/000/140/401/361B4872-0793-DF11-8A4F-0030487CD704.root',
	'root://castorcms.cern.ch//castor/cern.ch/cms/store/data/Run2010A/MinimumBias/RECO/v4/000/140/401/22C496EC-0393-DF11-843F-000423D9890C.root',
	'root://castorcms.cern.ch//castor/cern.ch/cms/store/data/Run2010A/MinimumBias/RECO/v4/000/140/401/22365323-0893-DF11-88DC-0030487CAF0E.root',
	'root://castorcms.cern.ch//castor/cern.ch/cms/store/data/Run2010A/MinimumBias/RECO/v4/000/140/401/1C6DD547-FE92-DF11-A087-003048CFB40C.root',
    ),
    inputCommands = cms.untracked.vstring("keep *", "drop *_MEtoEDMConverter_*_*")
)

### number of events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

### FILTERS for GoodCollision
process.oneGoodVertexFilter = cms.EDFilter("VertexSelector",
   src = cms.InputTag("offlinePrimaryVertices"),
   cut = cms.string("!isFake && ndof >= 4 && abs(z) <= 30 && position.Rho <= 2"),
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

### Adding Info about the Muon Station involved to the PATMuon
process.load("MuonAnalysis.Examples.muonStations_cfi")
from MuonAnalysis.Examples.muonStations_cfi import addUserData as addStations
addStations(process.patMuonsWithoutTrigger)


process.p = cms.Path(
    process.preFilter  +
    process.recoMuFilter  +
    process.muonStations +
    process.patMuonsWithTriggerSequence 
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('tupleData.root'),
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
