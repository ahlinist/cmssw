import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# define sources of systematic uncertainties
# considered in Z --> tau+ tau- and W --> tau nu cross-section measurements
# (Note that the same values for systematic uncertainties are used for all channels)
#--------------------------------------------------------------------------------

electronSystematics = {
    "sysElectronEnUp"       : "smearedLayer1Electrons:sysElectronEnUp",
    "sysElectronEnDown"     : "smearedLayer1Electrons:sysElectronEnDown"
}

muonSystematics = {
    "sysMuonPtUp"           : "smearedLayer1Muons:sysMuonPtUp",
    "sysMuonPtDown"         : "smearedLayer1Muons:sysMuonPtDown" 
}

tauSystematics = {   
    "sysTauJetEnUp"         : "smearedLayer1Taus:sysTauJetEnUp",
    "sysTauJetEnDown"       : "smearedLayer1Taus:sysTauJetEnDown",    
    "sysTauJetThetaUp"      : "smearedLayer1Taus:sysTauJetThetaUp",
    "sysTauJetThetaDown"    : "smearedLayer1Taus:sysTauJetThetaDown",
    "sysTauJetPhiUp"        : "smearedLayer1Taus:sysTauJetPhiUp",
    "sysTauJetPhiDown"      : "smearedLayer1Taus:sysTauJetPhiDown",
    "sysTauLeadTrackPtUp"   : "smearedLayer1Taus:sysTauLeadTrackPtUp",
    "sysTauLeadTrackPtDown" : "smearedLayer1Taus:sysTauLeadTrackPtDown"
}

muTauPairSystematics = {
    "sysMuonPtUp"           : "smearedMuTauPairsSysMuonPtUp",
    "sysMuonPtDown"         : "smearedMuTauPairsSysMuonPtDown",
    "sysTauJetEnUp"         : "smearedMuTauPairsSysTauJetEnUp",
    "sysTauJetEnDown"       : "smearedMuTauPairsSysTauJetEnDown",
    "sysTauJetThetaUp"      : "smearedMuTauPairsSysTauJetThetaUp",
    "sysTauJetThetaDown"    : "smearedMuTauPairsSysTauJetThetaDown",
    "sysTauJetPhiUp"        : "smearedMuTauPairsSysTauJetPhiUp",
    "sysTauJetPhiDown"      : "smearedMuTauPairsSysTauJetPhiDown",
    "sysTauLeadTrackPtUp"   : "smearedMuTauPairsSysTauLeadTrackPtUp",
    "sysTauLeadTrackPtDown" : "smearedMuTauPairsSysTauLeadTrackPtDown"
}

theorySystematics = {
    "sysPdfWeights(41)"     : "pdfWeights:cteq65",
    "sysIsrWeight"          : "isrWeight",
    "sysFsrWeight"          : "fsrWeight"
}

def getSysUncertaintyNames(sysUncertaintyMaps):
    
    sysUncertaintyNames = []
    
    for sysUncertaintyMap in sysUncertaintyMaps:
        for sysUncertaintyName in sysUncertaintyMap.keys():
            sysUncertaintyNames.append(sysUncertaintyName)

    return sysUncertaintyNames

def getSysUncertaintyParameterSets(sysUncertaintyMaps):
    
    sysUncertaintyParameterSets = []
    
    for sysUncertaintyMap in sysUncertaintyMaps:
        for sysUncertaintyName, sysUncertaintyInputTag in sysUncertaintyMap.items():
            sysUncertaintyParameterSet = cms.PSet()

            setattr(sysUncertaintyParameterSet, "name", cms.string(sysUncertaintyName))
            setattr(sysUncertaintyParameterSet, "src", cms.InputTag(sysUncertaintyInputTag))

            sysUncertaintyParameterSets.append(sysUncertaintyParameterSet)

    return cms.VPSet(sysUncertaintyParameterSets)
