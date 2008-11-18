import FWCore.ParameterSet.Config as cms

compEffMuon = cms.EDAnalyzer("EWKTauEffHistogramProducer",
  plots = cms.PSet(
    muonHLTmatchEff = cms.PSet(
      numerator = cms.string('/DQMData/MuonQuantities/MuonHLTmatchSel#PAR#'),
      denominator = cms.string('/DQMData/MuonQuantities/MuonKineSel#PAR#'),
      efficiency = cms.string('MuonHLTmatchEff#PAR#'),
      parameter = cms.vstring('Pt', 'Eta', 'Phi')
    ),
    muonTrkIsoEff = cms.PSet(
      numerator = cms.string('/DQMData/MuonQuantities/MuonTrkIsoSel#PAR#'),
      denominator = cms.string('/DQMData/MuonQuantities/MuonKineSel#PAR#'),
      efficiency = cms.string('MuonTrkIsoEff#PAR#'),
      parameter = cms.vstring('Pt', 'Eta', 'Phi')
    ),
    muonEcalIsoEff = cms.PSet(
      numerator = cms.string('/DQMData/MuonQuantities/MuonEcalIsoSel#PAR#'),
      denominator = cms.string('/DQMData/MuonQuantities/MuonKineSel#PAR#'),
      efficiency = cms.string('MuonEcalIsoEff#PAR#'),
      parameter = cms.vstring('Pt', 'Eta', 'Phi')
    ),
    muonHcalIsoEff = cms.PSet(
      numerator = cms.string('/DQMData/MuonQuantities/MuonHcalIsoSel#PAR#'),
      denominator = cms.string('/DQMData/MuonQuantities/MuonKineSel#PAR#'),
      efficiency = cms.string('MuonHcalIsoEff#PAR#'),
      parameter = cms.vstring('Pt', 'Eta', 'Phi')
    ),
    muonIdEff = cms.PSet(
      numerator = cms.string('/DQMData/MuonQuantities/MuonIdSel#PAR#'),
      denominator = cms.string('/DQMData/MuonQuantities/MuonKineSel#PAR#'),
      efficiency = cms.string('MuonIdEff#PAR#'),
      parameter = cms.vstring('Pt', 'Eta', 'Phi')
    )
  )
)
