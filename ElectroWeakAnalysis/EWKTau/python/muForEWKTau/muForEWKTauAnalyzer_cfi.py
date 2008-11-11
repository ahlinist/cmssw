import FWCore.ParameterSet.Config as cms

analyzeMuon = cms.EDAnalyzer("MuonHistManager",
  muonSource = cms.InputTag('allLayer1MuSelForEWKTau'),

  #outputFileName = cms.string('ewkTauMuAnalyzer.root'),
  outputFileName = cms.string(''),                            
  outputDirectoryName = cms.string('MuonQuantities'),

  muonKineSelVar = cms.string('selKineMuForEWKTau'),
  muonHLTmatchSelVar = cms.string('selHLTmatchMuForEWKTau'),                    
  muonTrkIsoSelVar = cms.string('selTrkIsoMuForEWKTau'), 
  muonEcalIsoSelVar = cms.string('selEcalIsoMuForEWKTau'),
  muonHcalIsoSelVar = cms.string('selHcalIsoMuForEWKTau'),                  
  muonIdSelVar = cms.string('selIdMuForEWKTau'), 
  muonTrkIpSelVar = cms.string('selTrkIpMuForEWKTau'),

  #requireGenMuonMatch = cms.bool(True)
  requireGenMuonMatch = cms.bool(False)
)
