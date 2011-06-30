import FWCore.ParameterSet.Config as cms


#muon skim part
looseMuons = cms.EDFilter("PATMuonSelector",
  src = cms.InputTag("selectedPatMuons"),
  cut = cms.string('pt > 10 & abs(eta) < 2.1 & isGlobalMuon'),
  filter = cms.bool(False)                                
)

#electron skim part
looseElectrons = cms.EDFilter("PATElectronSelector",
  src = cms.InputTag("selectedPatElectrons"),
  cut = cms.string('pt >  20 & abs(eta) < 2.4'),
)

#electrons for z veto part
zVetoElectrons = cms.EDFilter("PATElectronSelector",
  src = cms.InputTag("selectedPatElectrons"),
  cut = cms.string('pt >  20 & abs(eta) < 2.4'),
)

preselectedJets = cms.EDFilter("PATJetSelector",
  src = cms.InputTag("selectedPatJets"),
  cut = cms.string('pt >  30 & abs(eta) < 5.0'),
  filter = cms.bool(False)                                
)

#UnclusteredMET
UnclusteredMETPF = cms.EDProducer("SingleTopUnclusteredMETProducer",
                                  metSource = cms.InputTag("patMETs"),
                                  jetsSource = cms.InputTag("selectedPatJets"),
                                  electronsSource = cms.InputTag("selectedPatElectrons"),
                                  muonsSource = cms.InputTag("selectedPatMuons"),
                                  )

#PDF Info
PDFInfo = cms.EDProducer("PDFInfoDumper",
                         )

topJetsPF = cms.EDFilter("PATJetSelector",
                         preselection = cms.string(''),
                         src = cms.InputTag("selectedPatJets"),
                         cut = cms.string('pt >  20 & abs(eta) < 5.'),
                         checkOverlaps = cms.PSet(),
                           )

tightMuons = cms.EDFilter("PATMuonSelector",
                        preselection = cms.string(''),
                        src = cms.InputTag("selectedPatMuons"),
                        cut = cms.string(''),
                        checkOverlaps = cms.PSet(),
                        )

tightElectrons = cms.EDFilter("PATElectronSelector",
                        preselection = cms.string(''),
                        src = cms.InputTag("selectedPatElectrons"),
                        cut = cms.string(''),
                        checkOverlaps = cms.PSet(),
                         )

#Met skim part
preselectedMETs = cms.EDFilter("PATMETSelector",
  src = cms.InputTag("patMETs"),
  cut = cms.string('pt >   0'),
  filter = cms.bool(False)                                
)


#Part of MC Truth particles production
MCTruthParticles = cms.EDProducer("SingleTopTChannelMCProducer",
                                          genParticlesSource = cms.InputTag("genParticles")
                                          )

