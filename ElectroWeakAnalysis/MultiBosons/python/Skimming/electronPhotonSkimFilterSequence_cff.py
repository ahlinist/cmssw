import FWCore.ParameterSet.Config as cms

goodElectrons = cms.EDFilter("CandViewRefSelector",
    src = cms.InputTag("gsfElectrons"),
    cut = cms.string("pt > 15"),
    filter = cms.bool(True)
    )

goodPhotons = cms.EDProducer("PhotonCleaner",
    finalCut = cms.string(''),
    src = cms.InputTag("photons"),
    checkOverlaps = cms.PSet(
        electrons = cms.PSet(
            src = cms.InputTag("gsfElectrons"),
            requireNoOverlaps = cms.bool(True),
            algorithm = cms.string('bySuperClusterSeed')
        )
    ),
    preselection = cms.string('pt > 10')
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
