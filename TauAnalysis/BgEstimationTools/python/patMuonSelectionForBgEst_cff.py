import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------  
# produce collections of pat::Muons for data-driven background estimation methods
#--------------------------------------------------------------------------------

muonsPionVetoLooseIsolationForBgEst = cms.EDFilter("PATMuonAntiPionSelector",
    src = cms.InputTag('selectedPatMuonsEcalIsoLooseIsolationCumulative'),                                  
    CaloCompCoefficient = cms.double(0.8),
    SegmCompCoefficient = cms.double(1.2),
    AntiPionCut = cms.double(1.0),
    filter = cms.bool(False)
)
# disable cut on muon calo. + segment compatibility
# (check that muon calo. compatibility is not affected by pile-up before re-enabling this cut)
muonsPionVetoLooseIsolationForBgEst.AntiPionCut = cms.double(-1000.)

muonsTrkLooseIsolationForBgEst = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag('muonsPionVetoLooseIsolationForBgEst'),                                        
    cut = cms.string('innerTrack.isNonnull'),
    filter = cms.bool(False)
)

#--------------------------------------------------------------------------------

muonsPionVetoNoIsolationForBgEst = cms.EDFilter("PATMuonAntiPionSelector",
    src = cms.InputTag('selectedPatMuonsPt10Cumulative'),                                  
    CaloCompCoefficient = cms.double(0.8),
    SegmCompCoefficient = cms.double(1.2),
    AntiPionCut = cms.double(1.0),
    filter = cms.bool(False)
)
# disable cut on muon calo. + segment compatibility
# (check that muon calo. compatibility is not affected by pile-up before re-enabling this cut)
muonsPionVetoNoIsolationForBgEst.AntiPionCut = cms.double(-1000.)

muonsTrkNoIsolationForBgEst = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag('muonsPionVetoNoIsolationForBgEst'),                                        
    cut = cms.string('innerTrack.isNonnull'),
    filter = cms.bool(False)
)

#--------------------------------------------------------------------------------

muonsTrkIsoForBgEst = cms.EDFilter("PATMuonIsoDepositSelector",
    src = cms.InputTag('selectedPatMuonsPt10Cumulative'),                                
    type = cms.string('tracker'),
    #vetos = cms.vstring("0.01", "Threshold(0.9)"),
    vetos = cms.vstring("0.01"),                          
    dRisoCone = cms.double(0.6),
    #numMax = cms.int32(0),
    sumPtMax = cms.double(2.),
    filter = cms.bool(False)
)

muonsEcalIsoForBgEst = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag('muonsTrkIsoForBgEst'),                          
    cut = cms.string('ecalIso < 2.'),
    filter = cms.bool(False)
)

muonsPionVetoTightIsolationForBgEst = cms.EDFilter("PATMuonAntiPionSelector",
    src = cms.InputTag('muonsEcalIsoForBgEst'),                                        
    CaloCompCoefficient = cms.double(0.8),
    SegmCompCoefficient = cms.double(1.2),
    AntiPionCut = cms.double(1.0),
    filter = cms.bool(False)
)
# disable cut on muon calo. + segment compatibility
# (check that muon calo. compatibility is not affected by pile-up before re-enabling this cut)
muonsPionVetoTightIsolationForBgEst.AntiPionCut = cms.double(-1000.)

muonsTrkTightIsolationForBgEst = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag('muonsPionVetoTightIsolationForBgEst'),                                        
    cut = cms.string('innerTrack.isNonnull'),
    filter = cms.bool(False)
)

selectMuonsForBgEst = cms.Sequence(
    muonsPionVetoLooseIsolationForBgEst * muonsTrkLooseIsolationForBgEst
   * muonsPionVetoNoIsolationForBgEst * muonsTrkNoIsolationForBgEst
   * muonsTrkIsoForBgEst * muonsEcalIsoForBgEst * muonsPionVetoTightIsolationForBgEst * muonsTrkTightIsolationForBgEst
)


