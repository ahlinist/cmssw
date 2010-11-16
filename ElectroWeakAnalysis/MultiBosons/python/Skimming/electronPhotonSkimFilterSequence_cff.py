import FWCore.ParameterSet.Config as cms

goodElectrons = cms.EDFilter("GsfElectronSelector",
    src = cms.InputTag("gsfElectrons"),
    cut = cms.string("pt > 15"),
    filter = cms.bool(True)
    )

goodPhotons = cms.EDProducer("PhotonCleaner",
    src = cms.InputTag("photons"),
    preselection = cms.string("pt > 10"),
    checkOverlaps = cms.PSet(
        electrons = cms.PSet(
            src = cms.InputTag("goodElectrons"),
            requireNoOverlaps = cms.bool(True),
            algorithm = cms.string("bySuperClusterSeed")
        )
    ),
    finalCut = cms.string(""),
)

photonSkimFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("goodPhotons"),
    minNumber = cms.uint32(1)
    )

electronPhotonSkimFilterSequence = cms.Sequence(
  goodElectrons *
  goodPhotons *
  photonSkimFilter
  )
