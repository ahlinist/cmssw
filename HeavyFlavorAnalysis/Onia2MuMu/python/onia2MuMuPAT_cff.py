import FWCore.ParameterSet.Config as cms

##    __  __                        ____      _       __  __                       
##   |  \/  | ___ _ __ __ _  ___   / ___|__ _| | ___ |  \/  |_   _  ___  _ __  ___ 
##   | |\/| |/ _ \ '__/ _` |/ _ \ | |   / _` | |/ _ \| |\/| | | | |/ _ \| '_ \/ __|
##   | |  | |  __/ | | (_| |  __/ | |__| (_| | | (_) | |  | | |_| | (_) | | | \__ \
##   |_|  |_|\___|_|  \__, |\___|  \____\__,_|_|\___/|_|  |_|\__,_|\___/|_| |_|___/
##                    |___/                                                        
##   
## ==== Merge CaloMuons into the collection of reco::Muons  ====
from RecoMuon.MuonIdentification.calomuons_cfi import calomuons;
mergedMuons = cms.EDProducer("CaloMuonMerger",
    muons     = cms.InputTag("muons"), 
    caloMuons = cms.InputTag("calomuons"),
    minCaloCompatibility = calomuons.minCaloCompatibility
)

##    __  __  ____   _____           _   _     
##   |  \/  |/ ___| |_   _| __ _   _| |_| |__  
##   | |\/| | |       | || '__| | | | __| '_ \ 
##   | |  | | |___    | || |  | |_| | |_| | | |
##   |_|  |_|\____|   |_||_|   \__,_|\__|_| |_|
##                                             
##   
from PhysicsTools.PatAlgos.mcMatchLayer0.muonMatch_cfi import muonMatch
muonMatch.src = 'mergedMuons'

##    __  __       _          ____   _  _____   __  __                       
##   |  \/  | __ _| | _____  |  _ \ / \|_   _| |  \/  |_   _  ___  _ __  ___ 
##   | |\/| |/ _` | |/ / _ \ | |_) / _ \ | |   | |\/| | | | |/ _ \| '_ \/ __|
##   | |  | | (_| |   <  __/ |  __/ ___ \| |   | |  | | |_| | (_) | | | \__ \
##   |_|  |_|\__,_|_|\_\___| |_| /_/   \_\_|   |_|  |_|\__,_|\___/|_| |_|___/
##                                                                           
##   
### ==== Make PAT Muons ====
import PhysicsTools.PatAlgos.producersLayer1.muonProducer_cfi
patMuonsWithoutTrigger = PhysicsTools.PatAlgos.producersLayer1.muonProducer_cfi.patMuons.clone(
    muonSource = 'mergedMuons',
    # embed the tracks, so we don't have to carry them around
    embedTrack          = True,
    embedCombinedMuon   = True,
    embedStandAloneMuon = True,
    # then switch off some features we don't need
    #addTeVRefits = False, ## <<--- this doesn't work. PAT bug ??
    embedPickyMuon = False,
    embedTpfmsMuon = False, 
    userIsolation = cms.PSet(),   # no extra isolation beyond what's in reco::Muon itself
    isoDeposits = cms.PSet(), # no heavy isodeposits
    addGenMatch = True,       # no mc: T&P doesn't take it from here anyway.
    embedGenMatch = True,
    genParticleMatch = 'muonMatch'
)

##    _____     _                         __  __       _       _     
##   |_   _| __(_) __ _  __ _  ___ _ __  |  \/  | __ _| |_ ___| |__  
##     | || '__| |/ _` |/ _` |/ _ \ '__| | |\/| |/ _` | __/ __| '_ \ 
##     | || |  | | (_| | (_| |  __/ |    | |  | | (_| | || (__| | | |
##     |_||_|  |_|\__, |\__, |\___|_|    |_|  |_|\__,_|\__\___|_| |_|
##                |___/ |___/                                        
##   
### ==== Unpack trigger, and match ====
from PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cfi import patTrigger
patTrigger.onlyStandAlone = True

### ==== Then perform a match for all HLT triggers of interest
from PhysicsTools.PatAlgos.triggerLayer1.triggerMatcher_cfi import muonTriggerMatchHLTMu3
muonTriggerMatchHLTMu3.src = 'patMuonsWithoutTrigger'
muonTriggerMatchHLTMu3.andOr = False # i.e. 'AND'
### == For HLT triggers which are just L1s, we need a different matcher
from MuonAnalysis.MuonAssociators.muonHLTL1Match_cfi import muonHLTL1Match
muonMatchL1 = muonHLTL1Match.clone(
    src     = muonTriggerMatchHLTMu3.src,
    matched = muonTriggerMatchHLTMu3.matched,
    maxDeltaR     = cms.double(1.2),
    maxDeltaEta   = cms.double(0.2),
    fallbackToME1 = cms.bool(True),
)

### Single Mu L1
muonMatchHLTL1MuOpen = muonMatchL1.clone(pathNames = [ 'HLT_L1MuOpen' ], filterLabels = ['hltL1MuOpenL1Filtered0' ])
### Single Mu L2
muonMatchHLTL2Mu0 = muonTriggerMatchHLTMu3.clone(pathNames = [ "HLT_L2Mu0" ], filterLabels = ['hltL2Mu0L2Filtered0'] )
muonMatchHLTL2Mu3 = muonTriggerMatchHLTMu3.clone(pathNames = [ "HLT_L2Mu3" ], filterLabels = ['hltSingleMu3L2Filtered3'] )
### Single Mu L3
muonMatchHLTMu3 = muonTriggerMatchHLTMu3.clone(pathNames = [ "HLT_Mu3" ], filterLabels = ['hltSingleMu3L3Filtered3'] )
muonMatchHLTMu5 = muonTriggerMatchHLTMu3.clone(pathNames = [ "HLT_Mu5" ], filterLabels = ['hltSingleMu5L3Filtered5'] )

### Double Mu L1
muonMatchHLTL1DoubleMuOpen = muonMatchL1.clone(pathNames = [ 'HLT_L1DoubleMuOpen' ], filterLabels = ['hltDoubleMuLevel1PathL1OpenFiltered' ])
### Double Mu L2
muonMatchHLTL2DoubleMu0 = muonTriggerMatchHLTMu3.clone(pathNames = [ "HLT_L2DoubleMu0" ], filterLabels = ['hltDiMuonL2PreFiltered0'] )
### Double Mu L3
muonMatchHLTDoubleMu0 = muonTriggerMatchHLTMu3.clone(pathNames = [ "HLT_DoubleMu0" ], filterLabels = ['hltDiMuonL3PreFiltered0'] )
muonMatchHLTDoubleMu3 = muonTriggerMatchHLTMu3.clone(pathNames = [ "HLT_DoubleMu3" ], filterLabels = ['hltDiMuonL3PreFiltered']  )

### Mu L3 + L1MuOpen: 
#---- L3 part
muonMatchHLTMu0L1MuOpenL3 = muonTriggerMatchHLTMu3.clone(pathNames = [ "HLT_Mu0_L1MuOpen" ], filterLabels = [ 'hltMu0L1MuOpenL3Filtered0' ])
muonMatchHLTMu3L1MuOpenL3 = muonTriggerMatchHLTMu3.clone(pathNames = [ "HLT_Mu3_L1MuOpen" ], filterLabels = [ 'hltMu3L1MuOpenL3Filtered3' ])
muonMatchHLTMu5L1MuOpenL3 = muonTriggerMatchHLTMu3.clone(pathNames = [ "HLT_Mu5_L1MuOpen" ], filterLabels = [ 'hltMu5L1MuOpenL3Filtered5' ])
#---- L1 part
muonMatchHLTMu0L1MuOpenL1 = muonMatchL1.clone(pathNames = [ 'HLT_Mu0_L1MuOpen' ], filterLabels = ['hltMu0L1MuOpenL1Filtered0' ])
muonMatchHLTMu3L1MuOpenL1 = muonMatchL1.clone(pathNames = [ 'HLT_Mu3_L1MuOpen' ], filterLabels = ['hltMu3L1MuOpenL1Filtered0' ])
muonMatchHLTMu5L1MuOpenL1 = muonMatchL1.clone(pathNames = [ 'HLT_Mu5_L1MuOpen' ], filterLabels = ['hltMu5L1MuOpenL1Filtered0' ])

### Mu L3 + L2Mu0
#---- L3 part
muonMatchHLTMu0L2Mu0L3 = muonTriggerMatchHLTMu3.clone(pathNames = [ "HLT_Mu0_L2Mu0" ], filterLabels = [ 'hltMu0L2Mu0L3Filtered0' ])
muonMatchHLTMu3L2Mu0L3 = muonTriggerMatchHLTMu3.clone(pathNames = [ "HLT_Mu3_L2Mu0" ], filterLabels = [ 'hltMu3L2Mu0L3Filtered0' ])
muonMatchHLTMu5L2Mu0L3 = muonTriggerMatchHLTMu3.clone(pathNames = [ "HLT_Mu5_L2Mu0" ], filterLabels = [ 'hltMu5L2Mu0L3Filtered0' ])
#---- L2 part (actually, they're all the same (L2DoubleMu0)
muonMatchHLTMu0L2Mu0L2 = muonTriggerMatchHLTMu3.clone(pathNames = [ "HLT_Mu0_L2Mu0" ], filterLabels = [ 'hltDiMuonL2PreFiltered0' ])
muonMatchHLTMu3L2Mu0L2 = muonTriggerMatchHLTMu3.clone(pathNames = [ "HLT_Mu3_L2Mu0" ], filterLabels = [ 'hltDiMuonL2PreFiltered0' ])
muonMatchHLTMu5L2Mu0L2 = muonTriggerMatchHLTMu3.clone(pathNames = [ "HLT_Mu5_L2Mu0" ], filterLabels = [ 'hltDiMuonL2PreFiltered0' ])

### Mu L3 + Track
#---- muon part
muonMatchHLTMu0Tk0Mu  = muonTriggerMatchHLTMu3.clone(
    pathNames      = [ "HLT_Mu0_Track0_Jpsi" ], 
    filterLabels   = [ 'hltMu0TrackJpsiTrackMassFiltered' ], 
    collectionTags = [ 'hltL3MuonCandidates::HLT' ] 
)
muonMatchHLTMu3Tk0Mu = muonMatchHLTMu0Tk0Mu.clone(pathNames = [ "HLT_Mu3_Track0_Jpsi" ], filterLabels = [ 'hltMu3TrackJpsiTrackMassFiltered' ])
muonMatchHLTMu5Tk0Mu = muonMatchHLTMu0Tk0Mu.clone(pathNames = [ "HLT_Mu5_Track0_Jpsi" ], filterLabels = [ 'hltMu5TrackJpsiTrackMassFiltered' ])
#---- track part 
# v-- this works and it's easier --v
muonMatchHLTCtfTrack  = muonTriggerMatchHLTMu3.clone(pathNames = [ "*" ], collectionTags = ['hltMuTrackJpsiCtfTrackCands::HLT'])


## ==== Embed ====
patMuonsWithTrigger = cms.EDProducer( "PATTriggerMatchMuonEmbedder",
    src     = cms.InputTag(  "patMuonsWithoutTrigger" ),
    matches = cms.VInputTag( 
        # HLT Matches
        cms.InputTag('muonMatchHLTL1MuOpen','propagatedReco'), # fake, will match if and only if he muon did propagate to station 2
        cms.InputTag('muonMatchHLTL1MuOpen'),
        cms.InputTag('muonMatchHLTL2Mu0'),
        cms.InputTag('muonMatchHLTL2Mu3'),
        cms.InputTag('muonMatchHLTMu3'),
        cms.InputTag('muonMatchHLTMu5'),
        cms.InputTag('muonMatchHLTL1DoubleMuOpen'),
        cms.InputTag('muonMatchHLTL2DoubleMu0'),
        cms.InputTag('muonMatchHLTDoubleMu0'),
        cms.InputTag('muonMatchHLTDoubleMu3'),
        cms.InputTag('muonMatchHLTMu0L1MuOpenL3'),
        cms.InputTag('muonMatchHLTMu3L1MuOpenL3'),
        cms.InputTag('muonMatchHLTMu5L1MuOpenL3'),
        cms.InputTag('muonMatchHLTMu0L1MuOpenL1'),
        cms.InputTag('muonMatchHLTMu3L1MuOpenL1'),
        cms.InputTag('muonMatchHLTMu5L1MuOpenL1'),
        cms.InputTag('muonMatchHLTMu0L2Mu0L3'),
        cms.InputTag('muonMatchHLTMu3L2Mu0L3'),
        cms.InputTag('muonMatchHLTMu5L2Mu0L3'),
        cms.InputTag('muonMatchHLTMu0L2Mu0L2'),
        cms.InputTag('muonMatchHLTMu3L2Mu0L2'),
        cms.InputTag('muonMatchHLTMu5L2Mu0L2'),
        cms.InputTag('muonMatchHLTMu0Tk0Mu'),
        cms.InputTag('muonMatchHLTMu3Tk0Mu'),
        cms.InputTag('muonMatchHLTMu5Tk0Mu'),
        #cms.InputTag('muonMatchHLTMu0Tk0Tk'),
        #cms.InputTag('muonMatchHLTMu3Tk0Tk'),
        #cms.InputTag('muonMatchHLTMu5Tk0Tk'),
        cms.InputTag('muonMatchHLTCtfTrack'),
    )

)

## ==== Trigger Sequence ====
patTriggerMatching = cms.Sequence(
    patTrigger * 
    ( muonMatchHLTL1MuOpen   +
      muonMatchHLTL2Mu0      +
      muonMatchHLTL2Mu3      +
      muonMatchHLTMu3        +
      muonMatchHLTMu5        +
      # symmetric double muons
      muonMatchHLTL1DoubleMuOpen +
      muonMatchHLTL2DoubleMu0    +
      muonMatchHLTDoubleMu0      +
      muonMatchHLTDoubleMu3      +
      # mu + l1 mu
      muonMatchHLTMu0L1MuOpenL3 + 
      muonMatchHLTMu3L1MuOpenL3 + 
      muonMatchHLTMu5L1MuOpenL3 + 
      muonMatchHLTMu0L1MuOpenL1 + 
      muonMatchHLTMu3L1MuOpenL1 + 
      muonMatchHLTMu5L1MuOpenL1 + 
      # mu + l2 mu
      muonMatchHLTMu0L2Mu0L3 + 
      muonMatchHLTMu3L2Mu0L3 + 
      muonMatchHLTMu5L2Mu0L3 + 
      muonMatchHLTMu0L2Mu0L2 + 
      muonMatchHLTMu3L2Mu0L2 + 
      muonMatchHLTMu5L2Mu0L2 + 
      # mu + track
      muonMatchHLTMu0Tk0Mu + 
      muonMatchHLTMu3Tk0Mu + 
      muonMatchHLTMu5Tk0Mu + 
      #muonMatchHLTMu0Tk0Tk + 
      #muonMatchHLTMu3Tk0Tk + 
      #muonMatchHLTMu5Tk0Tk  
      muonMatchHLTCtfTrack
    ) *
    patMuonsWithTrigger
)

### ==== Apply some final selection (none by default) ====
patMuons = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag("patMuonsWithTrigger"),
    cut = cms.string(""), 
)

### ==== Sequence ====
patMuonSequence = cms.Sequence( 
    mergedMuons *
    muonMatch *
    patMuonsWithoutTrigger *
    patTriggerMatching *
    patMuons  
)

##    ____  _       __  __                       
##   |  _ \(_)     |  \/  |_   _  ___  _ __  ___ 
##   | | | | |_____| |\/| | | | |/ _ \| '_ \/ __|
##   | |_| | |_____| |  | | |_| | (_) | | | \__ \
##   |____/|_|     |_|  |_|\__,_|\___/|_| |_|___/
##                                               
##   
import HeavyFlavorAnalysis.Onia2MuMu.onia2MuMuPAT_cfi 

onia2MuMuPatGlbGlb = HeavyFlavorAnalysis.Onia2MuMu.onia2MuMuPAT_cfi.onia2MuMuPAT.clone()

onia2MuMuPatGlbTrk = HeavyFlavorAnalysis.Onia2MuMu.onia2MuMuPAT_cfi.onia2MuMuPAT.clone()
onia2MuMuPatGlbTrk.lowerPuritySelection  = cms.string("isGlobalMuon || isTrackerMuon")

onia2MuMuPatGlbCal = HeavyFlavorAnalysis.Onia2MuMu.onia2MuMuPAT_cfi.onia2MuMuPAT.clone()
onia2MuMuPatGlbCal.lowerPuritySelection  = cms.string("isGlobalMuon || isTrackerMuon || (track.isNonnull && isCaloMuon)")

onia2MuMuPatTrkTrk = HeavyFlavorAnalysis.Onia2MuMu.onia2MuMuPAT_cfi.onia2MuMuPAT.clone()
onia2MuMuPatTrkTrk.higherPuritySelection  = cms.string("isGlobalMuon || isTrackerMuon")
onia2MuMuPatTrkTrk.lowerPuritySelection   = cms.string("isGlobalMuon || isTrackerMuon")

onia2MuMuPatTrkCal = HeavyFlavorAnalysis.Onia2MuMu.onia2MuMuPAT_cfi.onia2MuMuPAT.clone()
onia2MuMuPatTrkCal.higherPuritySelection  = cms.string("isGlobalMuon || isTrackerMuon")
onia2MuMuPatTrkCal.lowerPuritySelection   = cms.string("isGlobalMuon || isTrackerMuon || (track.isNonnull && isCaloMuon)")

onia2MuMuPatArbTrkTrk = HeavyFlavorAnalysis.Onia2MuMu.onia2MuMuPAT_cfi.onia2MuMuPAT.clone()
onia2MuMuPatArbTrkTrk.higherPuritySelection  = cms.string("isGlobalMuon || (isTrackerMuon && muonID('TrackerMuonArbitrated'))")
onia2MuMuPatArbTrkTrk.lowerPuritySelection   = cms.string("isGlobalMuon || (isTrackerMuon && muonID('TrackerMuonArbitrated'))")

##    _____           _     
##   |_   _|__   ___ | |___ 
##     | |/ _ \ / _ \| / __|
##     | | (_) | (_) | \__ \
##     |_|\___/ \___/|_|___/
##                          
##   
def onia2MuMu_isNotMC(process):
    process.patMuonSequence.remove(muonMatch)
    process.patMuonsWithoutTrigger.addGenMatch = False
    process.onia2MuMuPatGlbGlb.addMCTruth = False
    process.onia2MuMuPatGlbCal.addMCTruth = False
    process.onia2MuMuPatGlbTrk.addMCTruth = False
    process.onia2MuMuPatTrkTrk.addMCTruth = False
    process.onia2MuMuPatTrkCal.addMCTruth = False
 #   process.onia2MuMuPatGlbGlb.addMuonlessPrimaryVertex = False
 #   process.onia2MuMuPatGlbCal.addMuonlessPrimaryVertex = False
 #   process.onia2MuMuPatGlbTrk.addMuonlessPrimaryVertex = False
 #   process.onia2MuMuPatTrkTrk.addMuonlessPrimaryVertex = False
 #   process.onia2MuMuPatTrkCal.addMuonlessPrimaryVertex = False
