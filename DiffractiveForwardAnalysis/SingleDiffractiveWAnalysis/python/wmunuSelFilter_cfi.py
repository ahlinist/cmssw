import FWCore.ParameterSet.Config as cms

wmunuSelFilter = cms.EDFilter("WMuNuSelector",
    # Defaults follow...
    #
    # Input collections ->
    MuonTag = cms.InputTag("muons"),
    METTag = cms.InputTag("met"),
    JetTag = cms.InputTag("iterativeCone5CaloJets"),
    VertexTag = cms.InputTag("offlinePrimaryVertices"),
    UseOnlyGlobalMuons = cms.bool(True),
    AssociateMuonWithPV = cms.bool(False),
    #
    # Main cuts ->
    PtCut = cms.double(25.0),
    EtaCut = cms.double(2.0),
    IsRelativeIso = cms.bool(True),
    IsoCut03 = cms.double(0.09),
    MassTMin = cms.double(50.0),
    MassTMax = cms.double(200.0),
    #
    # To suppress Zmm ->
    PtThrForZCount = cms.double(20.0),
    #
    # To suppress ttbar ->
    AcopCut = cms.double(999999.),
    EJetMin = cms.double(999999.),
    NJetMax = cms.int32(999999)
)
