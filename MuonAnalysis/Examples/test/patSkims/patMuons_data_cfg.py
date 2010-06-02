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
process.GlobalTag.globaltag = 'GR_R_36X_V11::All'

### source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'rfio:/castor/cern.ch/user/g/gpetrucc/7TeV/DATA/Muons_With_HLTDEBUG.root' 
        #'file:/afs/cern.ch/user/g/gpetrucc/scratch0/mu10/clean/CMSSW_3_5_6/src/hlt.root'
        #'rfio:/castor/cern.ch/user/g/gpetrucc/7TeV/DATA/Muons_With_HLTDEBUG_v9_Run134542_Ls44to52.root'
        #'file:/data/gpetrucc/Feb9Skims/Data_CollisionEvents_MuonSkim.root'
        'root://castorcms.cern.ch//castor/cern.ch/cms/store/data/Commissioning10/MinimumBias/RECO/v9/000/135/575/82F9EB50-B461-DF11-B959-001617E30CC8.root',
        'root://castorcms.cern.ch//castor/cern.ch/cms/store/data/Commissioning10/MinimumBias/RECO/v9/000/135/575/74CBD380-B561-DF11-978B-00151796CD80.root'
    ),
    inputCommands = cms.untracked.vstring("keep *", "drop *_MEtoEDMConverter_*_*")
)

### number of events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

### FILTERS for GoodCollision
process.load("L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff")
from HLTrigger.HLTfilters.hltLevel1GTSeed_cfi import hltLevel1GTSeed
hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
process.bptxAnd   = hltLevel1GTSeed.clone(L1SeedsLogicalExpression = cms.string('0'))
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

process.Flag_BPTX     = cms.Path(process.noScraping * process.oneGoodVertexFilter + process.bptxAnd)
process.Flag_BSC      = cms.Path(process.noScraping * process.oneGoodVertexFilter + process.bscFilter)
process.Flag_Bit0     = cms.Path(process.bptxAnd)
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

### Adding Info about the Muon Station involved to the PATMuon
# Requires MuonAnalysis/Examples V00-03-00+
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
    SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring("skim_RecoMuBscOR", "skim_RecoMuHLTMu3", "skim_GlbMu") ),
)
process.e = cms.EndPath(process.out)
