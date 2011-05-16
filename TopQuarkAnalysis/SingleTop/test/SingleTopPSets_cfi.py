import FWCore.ParameterSet.Config as cms


TChannelMu = cms.PSet(
    crossSection = cms.untracked.double(20.93),
    channel = cms.untracked.string("TChannel"),
    originalEvents = cms.untracked.double(480000),
    finalLumi = cms.untracked.double(191.091),
    MTWCut = cms.untracked.double(50.0),#Default 50.0 GeV
    RelIsoCut = cms.untracked.double(0.05),
    )


TChannelEle = cms.PSet(
    crossSection = cms.untracked.double(20.93),
    channel = cms.untracked.string("TChannel"),
    originalEvents = cms.untracked.double(480000),
    finalLumi = cms.untracked.double(191.091),
    MTWCut = cms.untracked.double(40.0),#Default 50.0 GeV
    )


  

WJetsMu = cms.PSet(
    crossSection = cms.untracked.double(31314),
    channel = cms.untracked.string("WJets"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(14800000),
    MTWCut = cms.untracked.double(40.0),#Default 50.0 GeV
    RelIsoCut = cms.untracked.double(0.05),
)


WJetsEle = cms.PSet(
    crossSection = cms.untracked.double(31314),
    channel = cms.untracked.string("WJets"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(14800000),
    )
  


WJets_wlightMu = cms.PSet(
    crossSection = cms.untracked.double(31314),
    channel = cms.untracked.string("WJets_wlight"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(14800000),
    MTWCut = cms.untracked.double(40.0),#Default 50.0 GeV
    RelIsoCut = cms.untracked.double(0.05),
    )


WJets_wlightEle = cms.PSet(
    crossSection = cms.untracked.double(31314),
    channel = cms.untracked.string("WJets_wlight"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(14800000),
    )
  


WJets_wccMu = cms.PSet(
    crossSection = cms.untracked.double(31314),
    channel = cms.untracked.string("WJets_wcc"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(14800000),
    MTWCut = cms.untracked.double(40.0),#Default 50.0 GeV
    RelIsoCut = cms.untracked.double(0.05),
    )


WJets_wccEle = cms.PSet(
    crossSection = cms.untracked.double(31314),
    channel = cms.untracked.string("WJets_wcc"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(14800000),
    )
  


WJets_wbbMu = cms.PSet(
    crossSection = cms.untracked.double(31314),
    channel = cms.untracked.string("WJets_wbb"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(14800000),
    MTWCut = cms.untracked.double(40.0),#Default 50.0 GeV
    RelIsoCut = cms.untracked.double(0.05),
    )


WJets_wbbEle = cms.PSet(
    crossSection = cms.untracked.double(31314),
    channel = cms.untracked.string("WJets_wbb"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(14800000),
    )
  


Vqq_wbbMu = cms.PSet(
    crossSection = cms.untracked.double(36),
    channel = cms.untracked.string("Vqq_wbb"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(740488),
    MTWCut = cms.untracked.double(40.0),#Default 50.0 GeV
    RelIsoCut = cms.untracked.double(0.05),
    )


Vqq_wbbEle = cms.PSet(
    crossSection = cms.untracked.double(36),
    channel = cms.untracked.string("Vqq_wbb"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(740488),
    MTWCut = cms.untracked.double(50.0),#Default 50.0 GeV
    RelIsoCut = cms.untracked.double(0.1)
    )
  

Vqq_wccMu = cms.PSet(
    crossSection = cms.untracked.double(36),
    channel = cms.untracked.string("Vqq_wcc"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(740488),
    MTWCut = cms.untracked.double(40.0),#Default 50.0 GeV
    RelIsoCut = cms.untracked.double(0.05),
    )


Vqq_wccEle = cms.PSet(
    crossSection = cms.untracked.double(36),
    channel = cms.untracked.string("Vqq_wcc"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(740488),
    MTWCut = cms.untracked.double(50.0),#Default 50.0 GeV
    RelIsoCut = cms.untracked.double(0.1)
    )
  


Wc_wcMu = cms.PSet(
    crossSection = cms.untracked.double(606),
    channel = cms.untracked.string("Wc_wc"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(2792637),
    MTWCut = cms.untracked.double(40.0),#Default 50.0 GeV
    RelIsoCut = cms.untracked.double(0.05),
    )

Wc_wcEle = cms.PSet(
    crossSection = cms.untracked.double(606),
    channel = cms.untracked.string("Wc_wc"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(2792637),
    MTWCut = cms.untracked.double(50.0),#Default 50.0 GeV
    RelIsoCut = cms.untracked.double(0.1)
    )
  


TTBarMu = cms.PSet(
    crossSection = cms.untracked.double(150.),
    channel = cms.untracked.string("TTBar"),
    originalEvents = cms.untracked.double(1100000),
    finalLumi = cms.untracked.double(191.091),
    MTWCut = cms.untracked.double(40.0),#Default 50.0 GeV
    RelIsoCut = cms.untracked.double(0.05),
    )

TTBarEle = cms.PSet(
    crossSection = cms.untracked.double(150.),
    channel = cms.untracked.string("TTBar"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(1100000),
    )
    

DataMu = cms.PSet(
    crossSection = cms.untracked.double(-1),
    channel = cms.untracked.string("Data"),
    originalEvents = cms.untracked.double(-1),
    finalLumi = cms.untracked.double(-1),
    MTWCut = cms.untracked.double(40.0),#Default 50.0 GeV
    RelIsoCut = cms.untracked.double(0.05),
    )

DataEle = cms.PSet(
    crossSection = cms.untracked.double(-1),
    channel = cms.untracked.string("Data"),
    originalEvents = cms.untracked.double(-1),
    finalLumi = cms.untracked.double(-1),
    )

