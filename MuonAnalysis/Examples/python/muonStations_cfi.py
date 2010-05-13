import FWCore.ParameterSet.Config as cms

muonStations = cms.EDProducer("MuonStationCounter",
    src = cms.InputTag("muons"),
    useGlobalTrack = cms.bool(False) ## Use the standAlone track
)

## Helper function to add this info into a pat::Muon
def addUserData(patMuonProducer, label="muonStations"):
    patMuonProducer.userData.userInts.src += [
        cms.InputTag(label,""),
        cms.InputTag(label,"any"),
        cms.InputTag(label,"dt"),
        cms.InputTag(label,"dtAny"),
        cms.InputTag(label,"csc"),
        cms.InputTag(label,"cscAny"),
        cms.InputTag(label,"rpc"),
        cms.InputTag(label,"rpcAny"),
    ]

