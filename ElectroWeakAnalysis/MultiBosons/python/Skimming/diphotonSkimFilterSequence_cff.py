import FWCore.ParameterSet.Config as cms

photonsCountFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("photons"),
    minNumber = cms.uint32(2)
    )

goodLoosePhotons = cms.EDFilter("PhotonViewRefSelector",
    src = cms.InputTag("photons"),
    ## See https://twiki.cern.ch/twiki/bin/view/CMS/Higgs2GAnalyzed#Very_Loose_Event_Preselection
    cut = cms.string("""
        superCluster.energy*sin(superCluster.position.theta) > 10 &
        hadronicOverEm < 0.5 &
        abs(superCluster.eta) < 2.5 &
        trkSumPtHollowConeDR03 < 2 * (3.5 + 0.001 * pt) &
        ecalRecHitSumEtConeDR03 < 2 * (4.2 + 0.006 * pt) &
        hcalTowerSumEtConeDR03 < 2 * (2.2 + 0.0025 * pt)
        """),
    filter = cms.bool(True)
    )

goodLoosePhotonsCountFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("goodLoosePhotons"),
    minNumber = cms.uint32(2)
    )

goodTightPhotons = goodLoosePhotons.clone(
    src = "goodLoosePhotons",
    cut = "pt > 20"
    )

goodDiphotons = cms.EDProducer("CandViewShallowCloneCombiner",
    decay = cms.string("goodTightPhotons goodLoosePhotons"),
    checkCharge = cms.bool(False),
    cut = cms.string("30 < mass")
    )

goodDiphotonsCountFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("goodDiphotons"),
    minNumber = cms.uint32(1)
    )

diphotonSkimFilterSequence = cms.Sequence(
    photonsCountFilter *
    goodLoosePhotons *
    goodLoosePhotonsCountFilter *
    goodTightPhotons *
    goodDiphotons *
    goodDiphotonsCountFilter
    )
