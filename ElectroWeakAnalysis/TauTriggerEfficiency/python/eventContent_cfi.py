import FWCore.ParameterSet.Config as cms

OutputCommands = cms.untracked.vstring(
    "drop *",
    "keep *_*_*_HLT",
    "keep *_*_*_sampleProducer",
    "keep recoCaloTaus_*_*_RECO",
    "keep recoCaloTauTagInfos_*_*_RECO",
    "keep recoCaloTauDiscriminatorByIsolation_*_*_RECO",
    "keep recoPFTaus_*_*_RECO",
    "keep recoPFTauTagInfos_*_*_RECO",
    "keep recoPFTauDiscriminatorByIsolation_*_*_RECO",
    "keep recoTracks_*_*_RECO",
    "keep recoVertexs_*_*_RECO"
)

