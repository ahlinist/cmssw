import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# define sources of systematic uncertainties
# considered in Z --> tau+ tau- and W --> tau nu cross-section measurements
# (Note that the same values for systematic uncertainties are used for all channels)
#--------------------------------------------------------------------------------

electronSystematics = {
    "sysElectronEnUp"       : cms.InputTag('smearedElectronsEnScaleUp'),
    "sysElectronEnDown"     : cms.InputTag('smearedElectronsEnScaleDown')
}

muonSystematics = {
    "sysMuonPtUp"           : cms.InputTag('smearedMuonsEnScaleUp'),
    "sysMuonPtDown"         : cms.InputTag('smearedMuonsEnScaleDown')
}

tauSystematics = {   
    "sysTauJetEnUp"         : cms.InputTag('smearedTausEnScaleUp'),
    "sysTauJetEnDown"       : cms.InputTag('smearedTausEnScaleDown'),
    "sysTauJetThetaUp"      : cms.InputTag('smearedTausEtaShiftUp'),
    "sysTauJetThetaDown"    : cms.InputTag('smearedTausEtaShiftDown'),
    "sysTauJetPhiUp"        : cms.InputTag('smearedTausPhiShiftUp'),
    "sysTauJetPhiDown"      : cms.InputTag('smearedTausPhiShiftDown')
}

muTauPairSystematics = {
    "sysMuonPtUp"           : cms.InputTag('allMuTauPairsSysMuonPtUp'),
    "sysMuonPtDown"         : cms.InputTag('allMuTauPairsSysMuonPtDown'),
    "sysTauJetEnUp"         : cms.InputTag('allMuTauPairsSysTauJetEnUp'),
    "sysTauJetEnDown"       : cms.InputTag('allMuTauPairsSysTauJetEnDown'),
    "sysTauJetThetaUp"      : cms.InputTag('allMuTauPairsSysTauJetThetaUp'),
    "sysTauJetThetaDown"    : cms.InputTag('allMuTauPairsSysTauJetThetaDown'),
    "sysTauJetPhiUp"        : cms.InputTag('allMuTauPairsSysTauJetPhiUp'),
    "sysTauJetPhiDown"      : cms.InputTag('allMuTauPairsSysTauJetPhiDown'),
    "sysJetEnUp"            : cms.InputTag('allMuTauPairsSysJetEnUp'),
    "sysJetEnDown"          : cms.InputTag('allMuTauPairsSysJetEnDown')
}

metSystematicsForZtoMuTau = {
    "sysMuonPtUp"           : cms.InputTag('patMETsSysMuonPtUp'),
    "sysMuonPtDown"         : cms.InputTag('patMETsSysMuonPtDown'),
    "sysTauJetEnUp"         : cms.InputTag('patMETsSysTauJetEnUp'),
    "sysTauJetEnDown"       : cms.InputTag('patMETsSysTauJetEnDown'),
    "sysTauJetThetaUp"      : cms.InputTag('patMETsSysTauJetThetaUp'),
    "sysTauJetThetaDown"    : cms.InputTag('patMETsSysTauJetThetaDown'),
    "sysTauJetPhiUp"        : cms.InputTag('patMETsSysTauJetPhiUp'),
    "sysTauJetPhiDown"      : cms.InputTag('patMETsSysTauJetPhiDown')
}

metSystematicsForAHtoMuTau = {
    "sysMuonPtUp"           : cms.InputTag('patMETsSysMuonPtUp'),
    "sysMuonPtDown"         : cms.InputTag('patMETsSysMuonPtDown'),
    "sysTauJetEnUp"         : cms.InputTag('patMETsSysTauJetEnUp'),
    "sysTauJetEnDown"       : cms.InputTag('patMETsSysTauJetEnDown'),
    "sysTauJetThetaUp"      : cms.InputTag('patMETsSysTauJetThetaUp'),
    "sysTauJetThetaDown"    : cms.InputTag('patMETsSysTauJetThetaDown'),
    "sysTauJetPhiUp"        : cms.InputTag('patMETsSysTauJetPhiUp'),
    "sysTauJetPhiDown"      : cms.InputTag('patMETsSysTauJetPhiDown'),
    "sysJetEnUp"            : cms.InputTag('patMETsSysJetEnUp'),
    "sysJetEnDown"          : cms.InputTag('patMETsSysJetEnDown')
}

jetSystematics = {   
    "sysJetEnUp"            : cms.InputTag('smearedJetsEnScaleUp'),
    "sysJetEnDown"          : cms.InputTag('smearedJetsEnScaleDown')
}

theorySystematics = {
    ##"sysPdfWeights(41)"     : cms.InputTag('pdfWeights:cteq65'),
    "sysIsrWeight"          : cms.InputTag('isrWeight'),
    "sysFsrWeight"          : cms.InputTag('fsrWeight')
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
            setattr(sysUncertaintyParameterSet, "src", sysUncertaintyInputTag)

            sysUncertaintyParameterSets.append(sysUncertaintyParameterSet)

    return cms.VPSet(sysUncertaintyParameterSets)
