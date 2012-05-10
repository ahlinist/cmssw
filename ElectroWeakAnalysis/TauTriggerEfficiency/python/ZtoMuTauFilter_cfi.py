import FWCore.ParameterSet.Config as cms

hpsTauSelection =  "pt() > 15 && abs(eta()) < 2.5"
hpsTauSelection += " && tauID('decayModeFinding') > 0.5"
hpsTauSelection += " && tauID('byLooseIsolation') > 0.5"
hpsTauSelection += " && tauID('againstElectronLoose') > 0.5"
hpsTauSelection += " && tauID('againstMuonLoose') > 0.5"

muonSelection =  "isGlobalMuon() && isTrackerMuon()"
muonSelection += "&& pt() > 15 & abs(eta()) < 2.5 "
muonSelection += "&& innerTrack().numberOfValidHits() > 10"
muonSelection += "&& innerTrack().hitPattern().pixelLayersWithMeasurement() >= 1"
muonSelection += "&& numberOfMatches() > 1"
muonSelection += "&& globalTrack().normalizedChi2() < 10.0"
muonSelection += "&& globalTrack().hitPattern().numberOfValidMuonHits() > 0"

muTauPairs = cms.EDProducer("DeltaRMinCandCombiner",
    decay = cms.string('selectedPatMuons@+ selectedPatTausHpsPFTau@-'),
    checkCharge = cms.bool(False),
    cut = cms.string(''),
    name = cms.string('muTauCandidates'),
    deltaRMin = cms.double(0.7)
)

def addMuTauSelection(process):
    process.selectedPatTausHpsPFTau.cut = hpsTauSelection
    process.selectedPatMuons.cut = muonSelection

    process.selectedTauFilter = cms.EDFilter("CandViewCountFilter",
        src = cms.InputTag("selectedPatTausHpsPFTau"),
        minNumber = cms.uint32(1),
    )
    process.selectedMuonFilter = cms.EDFilter("CandViewCountFilter",
        src = cms.InputTag("selectedPatMuons"),
        minNumber = cms.uint32(1),
    )

    process.muTauPairs = muTauPairs.clone()
    process.muTauPairsFilter = cms.EDFilter("CandViewCountFilter",
        src = cms.InputTag('muTauPairs'),
        minNumber = cms.uint32(1),
    )

    return cms.Sequence(
        process.selectedTauFilter +
        process.selectedMuonFilter +
        process.muTauPairs +
        process.muTauPairsFilter
    )
    
def addTauSelection(process):
    process.selectedPatTausHpsPFTau.cut = hpsTauSelection
    
    process.selectedTauFilter = cms.EDFilter("CandViewCountFilter",
        src = cms.InputTag("selectedPatTausHpsPFTau"),
        minNumber = cms.uint32(1),
    )
     
    return cms.Sequence(
        process.selectedTauFilter
    )
