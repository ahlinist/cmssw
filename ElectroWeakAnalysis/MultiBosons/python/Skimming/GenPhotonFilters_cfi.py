import FWCore.ParameterSet.Config as cms

leadingPhoton = cms.EDFilter("LargestPtPatPhotonSelector",
    src = cms.InputTag("cleanPatPhotons"),
    maxNumber = cms.uint32(1)
)

## Event passes if src collection contains a candidate matching Pythia PS ISR
pythiaPartonShowerIsrFilter = cms.EDFilter("PATPhotonSelector",
    src = cms.InputTag("leadingPhoton"),
    ## userInt("photonGenMatch:motherPdgId") = 0 if no photon gen match found
    cut = cms.string("""
        userInt("photonGenMatch:motherPdgId") != 0 &
        (
            userInt("photonGenMatch:motherPdgId") = 21 |
            abs(userInt("photonGenMatch:motherPdgId")) < 7
        )"""),
    filter = cms.bool(True)
)

## Event passes if src collection contains a candidate matching Pythia PS FSR
pythiaPartonShowerFsrFilter = cms.EDFilter("PATPhotonSelector",
    src = cms.InputTag("leadingPhoton"),
    cut = cms.string("""
        userInt("photonGenMatch:grandMotherPdgId") = 23 |
        abs(userInt("photonGenMatch:grandMotherPdgId")) = 24
        """),
    filter = cms.bool(True)
)

## Event passes if src collection contains a candidate matching Pythia PS FSR
powhegPartonShowerFsrFilter = cms.EDFilter("PATPhotonSelector",
    src = cms.InputTag("leadingPhoton"),
    cut = cms.string("""
        abs(userInt("photonGenMatch:motherPdgId")) = 11 |
        abs(userInt("photonGenMatch:motherPdgId")) = 13
        """),
    filter = cms.bool(True)
)

## Events passes if src contains a candidate that is neither Pythia ISR nor FSR
pythiaPartonShowerPhotonVeto = cms.EDFilter("PATPhotonSelector",
    src = cms.InputTag("cleanPatPhotons"),
    ## userInt("photonGenMatch:motherPdgId") = 0 if no photon gen match found
    cut = cms.string("""
        userInt("photonGenMatch:motherPdgId") = 0 ||
        (
            abs(userInt("photonGenMatch:motherPdgId"))      >= 7 &
                userInt("photonGenMatch:motherPdgId")       != 21 &
            abs(userInt("photonGenMatch:grandMotherPdgId")) != 24 &
                userInt("photonGenMatch:grandMotherPdgId")  != 23
        )"""),
    filter = cms.bool(True)
)

## Events passes if src contains a candidate that is neither Pythia ISR nor FSR
powhegPartonShowerPhotonVeto = cms.EDFilter("PATPhotonSelector",
    src = cms.InputTag("cleanPatPhotons"),
    ## userInt("photonGenMatch:motherPdgId") = 0 if no photon gen match found
    cut = cms.string("""
        userInt("photonGenMatch:motherPdgId") = 0 ||
        (
            abs(userInt("photonGenMatch:motherPdgId")) >= 7  &
                userInt("photonGenMatch:motherPdgId")  != 21 &
            abs(userInt("photonGenMatch:motherPdgId")) != 11 &
            abs(userInt("photonGenMatch:motherPdgId")) != 13
        )"""),
    filter = cms.bool(True)
)

if __name__ == "__main__": import user
