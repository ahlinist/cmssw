import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------  
# produce collections of pat::(PF)Taus for data-driven background estimation methods
#--------------------------------------------------------------------------------

tausTrkIsoLooseIsolationForBgEst = cms.EDFilter("PATTauSelector",
    src = cms.InputTag('selectedLayer1TausLeadTrkPtCumulative'),                                        
    cut = cms.string('tauID("trackIsolation") > 0.5 | chargedHadronIso < 8.'),
    filter = cms.bool(False)
)

tausEcalIsoLooseIsolationForBgEst = cms.EDFilter("PATTauSelector",
    src = cms.InputTag('tausTrkIsoLooseIsolationForBgEst'),                                        
    cut = cms.string('tauID("ecalIsolation") > 0.5 | photonIso < 8.'),
    filter = cms.bool(False)
)

tausAntiOverlapWithMuonsVetoLooseIsolationForBgEst = cms.EDFilter("PATTauAntiOverlapSelector",
    src = cms.InputTag('tausEcalIsoLooseIsolationForBgEst'),                                              
    srcNotToBeFiltered = cms.VInputTag("selectedLayer1MuonsGlobalIndividual"),
    dRmin = cms.double(0.7),
    filter = cms.bool(False)                                           
)

tausAntiOverlapWithElectronsVetoLooseIsolationForBgEst = cms.EDFilter("PATTauAntiOverlapSelector",
    src = cms.InputTag('tausEcalIsoLooseIsolationForBgEst'),                                              
    srcNotToBeFiltered = cms.VInputTag("selectedLayer1ElectronsTightIdIndividual"),
    dRmin = cms.double(0.7),
    filter = cms.bool(False)                                           
)

tausNoIsolationForBgEst = cms.EDFilter("PATTauSelector",
    src = cms.InputTag('selectedLayer1TausForMuTauPt20Cumulative'),                                        
    cut = cms.string('tauID("againstMuon") > 0.5'),
    filter = cms.bool(False)
)

selectTausForBgEst = cms.Sequence(
    tausTrkIsoLooseIsolationForBgEst * tausEcalIsoLooseIsolationForBgEst
   * tausAntiOverlapWithMuonsVetoLooseIsolationForBgEst * tausAntiOverlapWithElectronsVetoLooseIsolationForBgEst
   * tausNoIsolationForBgEst
)
