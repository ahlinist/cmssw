import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# preselection of events considered in data-driven background estimation methods
#--------------------------------------------------------------------------------

electronsTrkLooseIsolationForBgEst = cms.EDFilter("PATElectronSelector",
    src = cms.InputTag('selectedLayer1ElectronsEcalIsoLooseIsolationCumulative'),                                        
    cut = cms.string('gsfTrack.isNonnull'),
    filter = cms.bool(False)
)

electronsTrkIsoForBgEst = cms.EDFilter("PATElectronSelector",
    src = cms.InputTag('selectedLayer1ElectronsPt15Cumulative'),
    cut = cms.string('trackIso < 2.'),
    filter = cms.bool(False)
)

electronsEcalIsoForBgEst = cms.EDFilter("PATElectronSelector",
    src = cms.InputTag('electronsTrkIsoForBgEst'),                          
    cut = cms.string('ecalIso < 2.'),
    filter = cms.bool(False)
)

electronsTrkTightIsolationForBgEst = cms.EDFilter("PATElectronSelector",
    src = cms.InputTag('electronsEcalIsoForBgEst'),                                        
    cut = cms.string('gsfTrack.isNonnull'),
    filter = cms.bool(False)
)

selectElectronsForBgEst = cms.Sequence(
    electronsTrkLooseIsolationForBgEst
   * electronsTrkIsoForBgEst * electronsEcalIsoForBgEst * electronsTrkTightIsolationForBgEst
)

