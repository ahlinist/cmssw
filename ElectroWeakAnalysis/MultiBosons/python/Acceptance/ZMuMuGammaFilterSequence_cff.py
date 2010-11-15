import FWCore.ParameterSet.Config as cms

isMuon     = "abs(pdgId)=13"
isPhoton   = "pdgId=22"
isPrompt   = "mother(0).mother(0).status=3"
isStable   = "status=1"

looseMuons = cms.EDFilter("GenParticleSelector",
    filter = cms.bool(True),
    src = cms.InputTag("genParticles"),
    cut = cms.string("""
        %s & %s & %s &
        pt > 20 &
        abs(eta) < 2.4
        """ % (isMuon, isPrompt, isStable)
    )
)

tightMuons = cms.EDFilter("GenParticleSelector",
    filter = cms.bool(True),
    src = cms.InputTag("looseMuons"),
    cut = cms.string("abs(eta) < 2.1")
)

dimuons = cms.EDProducer("CandViewShallowCloneCombiner",
    decay = cms.string("looseMuons@+ tightMuons@-"),
    checkCharge = cms.bool(True),
    roles = cms.vstring("lepton1", "lepton2"),
    cut = cms.string("mass > 50"),
)

dimuonsFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("dimuons"),
    minNumber = cms.uint32(1)
)

goodPhotons = cms.EDFilter("GenParticleSelector",
    filter = cms.bool(True),
    src = cms.InputTag("genParticles"),
    cut = cms.string("""
        %s & %s & %s &
        pt > 10 &
        abs(eta) < 2.4
        """ % (isPhoton, isPrompt, isStable)
    )
)

ZGammaCands  = cms.EDProducer("CandViewShallowCloneCombiner",
    decay = cms.string("dimuons goodPhotons"),
    checkCharge = cms.bool(False),
    roles = cms.vstring("dilepton", "photon"),
    cut = cms.string("""
        min(
            deltaR(
              daughter("dilepton").daughter("lepton1").eta,
              daughter("dilepton").daughter("lepton1").phi,
              daughter("photon").eta,
              daughter("photon").phi
            ),
            deltaR(
              daughter("dilepton").daughter("lepton2").eta,
              daughter("dilepton").daughter("lepton2").phi,
              daughter("photon").eta,
              daughter("photon").phi
            )
        ) > 0.7
    """),
)

ZGammaCandsFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("ZGammaCands"),
    minNumber = cms.uint32(1)
)

acceptanceFilterSequence = cms.Sequence(
    looseMuons *
    tightMuons *
    dimuons    *
    dimuonsFilter *
    goodPhotons *
    ZGammaCands *
    ZGammaCandsFilter
)
