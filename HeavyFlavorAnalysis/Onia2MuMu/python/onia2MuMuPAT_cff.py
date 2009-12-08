import FWCore.ParameterSet.Config as cms
import  HeavyFlavorAnalysis.Onia2MuMu.onia2MuMuPAT_cfi 

##    __  __  ____   _____           _   _     
##   |  \/  |/ ___| |_   _| __ _   _| |_| |__  
##   | |\/| | |       | || '__| | | | __| '_ \ 
##   | |  | | |___    | || |  | |_| | |_| | | |
##   |_|  |_|\____|   |_||_|   \__,_|\__|_| |_|
##                                             
##   
from PhysicsTools.PatAlgos.mcMatchLayer0.muonMatch_cfi import muonMatch


##    __  __       _          ____   _  _____   __  __                       
##   |  \/  | __ _| | _____  |  _ \ / \|_   _| |  \/  |_   _  ___  _ __  ___ 
##   | |\/| |/ _` | |/ / _ \ | |_) / _ \ | |   | |\/| | | | |/ _ \| '_ \/ __|
##   | |  | | (_| |   <  __/ |  __/ ___ \| |   | |  | | |_| | (_) | | | \__ \
##   |_|  |_|\__,_|_|\_\___| |_| /_/   \_\_|   |_|  |_|\__,_|\___/|_| |_|___/
##                                                                           
##   
### ==== Make PAT Muons ====
from PhysicsTools.PatAlgos.producersLayer1.muonProducer_cfi import allLayer1Muons
patMuonsWithoutTrigger = allLayer1Muons.clone(
    # embed the tracks, so we don't have to carry them around
    embedTrack          = True,
    embedCombinedMuon   = True,
    embedStandAloneMuon = True,
    # then switch off some features we don't need
    #addTeVRefits = False, ## <<--- this doesn't work. PAT bug ??
    embedPickyMuon = False,
    embedTpfmsMuon = False, 
    isolation = cms.PSet(),   # no extra isolation beyond what's in reco::Muon itself
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
from PhysicsTools.PatAlgos.triggerLayer1.triggerMatcher_cfi import muonTriggerMatchHLT1MuonIso
muonMatchHLTL1MuOpen  = muonTriggerMatchHLT1MuonIso.clone(src = 'patMuonsWithoutTrigger',pathNames = [ "HLT_L1MuOpen" ])
muonMatchHLTMu3       = muonTriggerMatchHLT1MuonIso.clone(src = 'patMuonsWithoutTrigger',pathNames = [ "HLT_Mu3" ])
muonMatchHLTMu5       = muonTriggerMatchHLT1MuonIso.clone(src = 'patMuonsWithoutTrigger',pathNames = [ "HLT_Mu5" ])
muonMatchHLTDoubleMu0 = muonTriggerMatchHLT1MuonIso.clone(src = 'patMuonsWithoutTrigger',pathNames = [ "HLT_DoubleMu0" ])
muonMatchHLTDoubleMu3 = muonTriggerMatchHLT1MuonIso.clone(src = 'patMuonsWithoutTrigger',pathNames = [ "HLT_DoubleMu3" ])
## ==== Embed ====
patMuonsWithTrigger = cms.EDProducer( "PATTriggerMatchMuonEmbedder",
    src     = cms.InputTag(  "patMuonsWithoutTrigger" ),
    matches = cms.VInputTag( 
        cms.InputTag('muonMatchHLTL1MuOpen'),
        cms.InputTag('muonMatchHLTMu3'),
        cms.InputTag('muonMatchHLTMu5'),
        cms.InputTag('muonMatchHLTDoubleMu0'),
        cms.InputTag('muonMatchHLTDoubleMu3'),
    )
)
patTriggerMatching = cms.Sequence(
    patTrigger * 
    ( muonMatchHLTL1MuOpen  +
      muonMatchHLTMu3       +
      muonMatchHLTMu5       +
      muonMatchHLTDoubleMu0 +
      muonMatchHLTDoubleMu3 ) *
    patMuonsWithTrigger
)

### ==== Apply some final selection ====
patMuons = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag("patMuonsWithTrigger"),
    cut = cms.string(""), 
)
### ==== Sequence ====
patMuonSequence = cms.Sequence( 
    muonMatch *
    patMuonsWithoutTrigger *
    patTriggerMatching *
    patMuons  
)


onia2MuMuPatGlbGlb = HeavyFlavorAnalysis.Onia2MuMu.onia2MuMuPAT_cfi.onia2MuMuPAT.clone()

onia2MuMuPatGlbTrk = HeavyFlavorAnalysis.Onia2MuMu.onia2MuMuPAT_cfi.onia2MuMuPAT.clone()
onia2MuMuPatGlbTrk.lowerPuritySelection  = cms.int32(2)

onia2MuMuPatTrkTrk = HeavyFlavorAnalysis.Onia2MuMu.onia2MuMuPAT_cfi.onia2MuMuPAT.clone()
onia2MuMuPatTrkTrk.higherPuritySelection  = cms.int32(2)
onia2MuMuPatTrkTrk.lowerPuritySelection   = cms.int32(2)


def onia2MuMu_isNotMC(process):
    process.patMuonSequence.remove(muonMatch)
    process.patMuonsWithoutTrigger.addGenMatch = False
    process.onia2MuMuPatGlbGlb.addMCTruth = False
    process.onia2MuMuPatGlbTrk.addMCTruth = False
    process.onia2MuMuPatTrkTrk.addMCTruth = False
