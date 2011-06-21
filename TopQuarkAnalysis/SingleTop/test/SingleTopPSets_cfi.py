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

tWChanMu = cms.PSet(
        crossSection = cms.untracked.double(10.6),
            channel = cms.untracked.string("tWChan"),
            originalEvents = cms.untracked.double(494961),
            finalLumi = cms.untracked.double(191.091),
            MTWCut = cms.untracked.double(40.0),#Default 50.0 GeV
            RelIsoCut = cms.untracked.double(0.05),
            )

tWChanEle = cms.PSet(
        crossSection = cms.untracked.double(10.6),
            channel = cms.untracked.string("tWChan"),
            finalLumi = cms.untracked.double(191.091),
            originalEvents = cms.untracked.double(494961),
            )

sChanMu = cms.PSet(
            crossSection = cms.untracked.double(1.533),
                        channel = cms.untracked.string("sChan"),
                        originalEvents = cms.untracked.double(494967),
                        finalLumi = cms.untracked.double(191.091),
                        MTWCut = cms.untracked.double(40.0),#Default 50.0 GeV
                        RelIsoCut = cms.untracked.double(0.05),
                        )

sChanEle = cms.PSet(
            crossSection = cms.untracked.double(1.533),
                        channel = cms.untracked.string("sChan"),
                        finalLumi = cms.untracked.double(191.091),
                        originalEvents = cms.untracked.double(494967),
                        )
  

ZJetsMu = cms.PSet(
    crossSection = cms.untracked.double(2321),
    channel = cms.untracked.string("ZJets"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(2595097),
    MTWCut = cms.untracked.double(40.0),#Default 50.0 GeV
    RelIsoCut = cms.untracked.double(0.05),
)


ZJetsEle = cms.PSet(
    crossSection = cms.untracked.double(2321),
    channel = cms.untracked.string("ZJets"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(2595097),
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
    crossSection = cms.untracked.double(24170),
    channel = cms.untracked.string("WJets_wlight"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(14800000),
    MTWCut = cms.untracked.double(40.0),#Default 50.0 GeV
    RelIsoCut = cms.untracked.double(0.05),
    )


WJets_wlightEle = cms.PSet(
    crossSection = cms.untracked.double(24170),
    channel = cms.untracked.string("WJets_wlight"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(14800000),
    )
  


WJets_wccMu = cms.PSet(
    crossSection = cms.untracked.double(24170),
    channel = cms.untracked.string("WJets_wcc"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(14800000),
    MTWCut = cms.untracked.double(40.0),#Default 50.0 GeV
    RelIsoCut = cms.untracked.double(0.05),
    )


WJets_wccEle = cms.PSet(
    crossSection = cms.untracked.double(24170),
    channel = cms.untracked.string("WJets_wcc"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(14800000),
    )

WJets_wbbMu = cms.PSet(
    crossSection = cms.untracked.double(24170),
    channel = cms.untracked.string("WJets_wbb"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(14800000),
    MTWCut = cms.untracked.double(40.0),#Default 50.0 GeV
    RelIsoCut = cms.untracked.double(0.05),
    )
WJets_wbbEle = cms.PSet(
    crossSection = cms.untracked.double(24170),
    channel = cms.untracked.string("WJets_wbb"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(14800000),
    )
  


#Z


ZJets_wlightMu = cms.PSet(
    crossSection = cms.untracked.double(2321),
    channel = cms.untracked.string("ZJets_wlight"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(14800000),
    MTWCut = cms.untracked.double(40.0),#Default 50.0 GeV
    RelIsoCut = cms.untracked.double(0.05),
    )


ZJets_wlightEle = cms.PSet(
    crossSection = cms.untracked.double(2321),
    channel = cms.untracked.string("ZJets_wlight"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(14800000),
    )
  


ZJets_wccMu = cms.PSet(
    crossSection = cms.untracked.double(2321),
    channel = cms.untracked.string("ZJets_wcc"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(14800000),
    MTWCut = cms.untracked.double(40.0),#Default 50.0 GeV
    RelIsoCut = cms.untracked.double(0.05),
    )


ZJets_wccEle = cms.PSet(
    crossSection = cms.untracked.double(2321),
    channel = cms.untracked.string("ZJets_wcc"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(14800000),
    )

ZJets_wbbMu = cms.PSet(
    crossSection = cms.untracked.double(2321),
    channel = cms.untracked.string("ZJets_wbb"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(14800000),
    MTWCut = cms.untracked.double(40.0),#Default 50.0 GeV
    RelIsoCut = cms.untracked.double(0.05),
    )
ZJets_wbbEle = cms.PSet(
    crossSection = cms.untracked.double(2321),
    channel = cms.untracked.string("ZJets_wbb"),
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
  

VVMu = cms.PSet(
    crossSection = cms.untracked.double(4.8),
    channel = cms.untracked.string("VV"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(963356),
    MTWCut = cms.untracked.double(40.0),#Default 50.0 GeV
    RelIsoCut = cms.untracked.double(0.05),
    )

VVEle = cms.PSet(
    crossSection = cms.untracked.double(4.8),
    channel = cms.untracked.string("VV"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(963356),
    MTWCut = cms.untracked.double(50.0),#Default 50.0 GeV
    RelIsoCut = cms.untracked.double(0.1)
    )
  

TTBarMu = cms.PSet(
    crossSection = cms.untracked.double(150.),
    channel = cms.untracked.string("TTBar"),
#    originalEvents = cms.untracked.double(1100000),
    originalEvents = cms.untracked.double(1014208),
    finalLumi = cms.untracked.double(191.091),
    MTWCut = cms.untracked.double(40.0),#Default 50.0 GeV
    RelIsoCut = cms.untracked.double(0.05),
    )

TTBarEle = cms.PSet(
    crossSection = cms.untracked.double(150.),
    channel = cms.untracked.string("TTBar"),
    finalLumi = cms.untracked.double(191.091),
    originalEvents = cms.untracked.double(1014208),
#    originalEvents = cms.untracked.double(1100000),
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

