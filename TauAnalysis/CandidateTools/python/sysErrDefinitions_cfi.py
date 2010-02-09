import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# define sources of systematic uncertainties
# considered in Z --> tau+ tau- and W --> tau nu cross-section measurements
# (Note that the same values for systematic uncertainties are used for all channels)
#--------------------------------------------------------------------------------

electronSystematics = {
    "sysElectronEnUp"       : "smearedElectronsEnScaleUp",
    "sysElectronEnDown"     : "smearedElectronsEnScaleDown"
}

muonSystematics = {
    "sysMuonPtUp"           : "smearedMuonsEnScaleUp",
    "sysMuonPtDown"         : "smearedMuonsEnScaleDown"
}

tauSystematics = {   
    "sysTauJetEnUp"         : "smearedTausEnScaleUp",
    "sysTauJetEnDown"       : "smearedTausEnScaleDown",
    "sysTauJetThetaUp"      : "smearedTausEtaShiftUp",
    "sysTauJetThetaDown"    : "smearedTausEtaShiftDown",
    "sysTauJetPhiUp"        : "smearedTausPhiShiftUp",
    "sysTauJetPhiDown"      : "smearedTausPhiShiftDown"
}

muTauPairSystematics = {
    "sysMuonPtUp"           : "allMuTauPairsSysMuonPtUp",
    "sysMuonPtDown"         : "allMuTauPairsSysMuonPtDown",
    "sysTauJetEnUp"         : "allMuTauPairsSysTauJetEnUp",
    "sysTauJetEnDown"       : "allMuTauPairsSysTauJetEnDown",
    "sysTauJetThetaUp"      : "allMuTauPairsSysTauJetThetaUp",
    "sysTauJetThetaDown"    : "allMuTauPairsSysTauJetThetaDown",
    "sysTauJetPhiUp"        : "allMuTauPairsSysTauJetPhiUp",
    "sysTauJetPhiDown"      : "allMuTauPairsSysTauJetPhiDown"
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
