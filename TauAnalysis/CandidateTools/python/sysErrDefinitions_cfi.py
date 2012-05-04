import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# define sources of systematic uncertainties
# considered in Z --> tau+ tau- and W --> tau nu cross-section measurements
# (Note that the same values for systematic uncertainties are used for all channels)
#--------------------------------------------------------------------------------

electronSystematics = {
    "sysElectronEnUp"            : cms.InputTag('smearedElectronsEnScaleUp'),
    "sysElectronEnDown"          : cms.InputTag('smearedElectronsEnScaleDown')
}

muonSystematics = {
    # CV: MuScleFit muon momentum corrections do not work in CMSSW_5_2_x (May 4th 2012)
    ##"sysMuonPtUp"                : cms.InputTag('patMuonsMuScleFitCorrectedMomentumShiftUp'),
    ##"sysMuonPtDown"              : cms.InputTag('patMuonsMuScleFitCorrectedMomentumShiftDown')
}

tauSystematics = {   
    "sysTauJetEnUp"              : cms.InputTag('patTausJECshiftUp'),
    "sysTauJetEnDown"            : cms.InputTag('patTausJECshiftDown')
}

muTauPairSystematics = {
    # CV: MuScleFit muon momentum corrections do not work in CMSSW_5_2_x (May 4th 2012)
    ##"sysMuonPtUp"                : cms.InputTag('allMuTauPairsSysMuonPtUp'),
    ##"sysMuonPtDown"              : cms.InputTag('allMuTauPairsSysMuonPtDown'),
    "sysTauJetEnUp"              : cms.InputTag('allMuTauPairsSysTauJetEnUp'),
    "sysTauJetEnDown"            : cms.InputTag('allMuTauPairsSysTauJetEnDown'),
    "sysJetEnUp"                 : cms.InputTag('allMuTauPairsSysJetEnUp'),
    "sysJetEnDown"               : cms.InputTag('allMuTauPairsSysJetEnDown'),
    "sysZllRecoilCorrectionUp"   : cms.InputTag('allMuTauPairsZllRecoilCorrectedSysUp'),
    "sysZllRecoilCorrectionDown" : cms.InputTag('allMuTauPairsZllRecoilCorrectedSysDown')
}

##muTauPairVBFEventSystematics = {
##    "sysMuonPtUp"                : cms.InputTag('allVBFEventHypothesesForAHtoMuTauSysMuonPtUp'),
##    "sysMuonPtDown"              : cms.InputTag('allVBFEventHypothesesForAHtoMuTauSysMuonPtDown'),
##    "sysTauJetEnUp"              : cms.InputTag('allVBFEventHypothesesForAHtoMuTauSysTauJetEnUp'),
##    "sysTauJetEnDown"            : cms.InputTag('allVBFEventHypothesesForAHtoMuTauSysTauJetEnDown'),
##    "sysJetEnUp"                 : cms.InputTag('allVBFEventHypothesesForAHtoMuTauSysJetEnUp'),
##    "sysJetEnDown"               : cms.InputTag('allVBFEventHypothesesForAHtoMuTauSysJetEnDown'),
##    "sysZllRecoilCorrectionUp"   : cms.InputTag('allVBFEventHypothesesForAHtoMuTauZllRecoilCorrectedSysUp'),
##    "sysZllRecoilCorrectionDown" : cms.InputTag('allVBFEventHypothesesForAHtoMuTauZllRecoilCorrectedSysDown')
##}

muTauPairSystematicsLooseMuonIsolation = {
    # CV: MuScleFit muon momentum corrections do not work in CMSSW_5_2_x (May 4th 2012)
    ##"sysMuonPtUp"                : cms.InputTag('allMuTauPairsLooseMuonIsolationSysMuonPtUp'),
    ##"sysMuonPtDown"              : cms.InputTag('allMuTauPairsLooseMuonIsolationSysMuonPtDown'),
    "sysTauJetEnUp"              : cms.InputTag('allMuTauPairsLooseMuonIsolationSysTauJetEnUp'),
    "sysTauJetEnDown"            : cms.InputTag('allMuTauPairsLooseMuonIsolationSysTauJetEnDown'),
    "sysJetEnUp"                 : cms.InputTag('allMuTauPairsLooseMuonIsolationSysJetEnUp'),
    "sysJetEnDown"               : cms.InputTag('allMuTauPairsLooseMuonIsolationSysJetEnDown'),
    "sysZllRecoilCorrectionUp"   : cms.InputTag('allMuTauPairsLooseMuonIsolationZllRecoilCorrectedSysUp'),
    "sysZllRecoilCorrectionDown" : cms.InputTag('allMuTauPairsLooseMuonIsolationZllRecoilCorrectedSysDown')
}

muTauPairVBFEventSystematicsLooseMuonIsolation = {
    # CV: MuScleFit muon momentum corrections do not work in CMSSW_5_2_x (May 4th 2012)
    ##"sysMuonPtUp"                : cms.InputTag('allVBFEventHypothesesForAHtoMuTauLooseMuonIsolationSysMuonPtUp'),
    ##"sysMuonPtDown"              : cms.InputTag('allVBFEventHypothesesForAHtoMuTauLooseMuonIsolationSysMuonPtDown'),
    "sysTauJetEnUp"              : cms.InputTag('allVBFEventHypothesesForAHtoMuTauLooseMuonIsolationSysTauJetEnUp'),
    "sysTauJetEnDown"            : cms.InputTag('allVBFEventHypothesesForAHtoMuTauLooseMuonIsolationSysTauJetEnDown'),
    "sysJetEnUp"                 : cms.InputTag('allVBFEventHypothesesForAHtoMuTauLooseMuonIsolationSysJetEnUp'),
    "sysJetEnDown"               : cms.InputTag('allVBFEventHypothesesForAHtoMuTauLooseMuonIsolationSysJetEnDown'),
    "sysZllRecoilCorrectionUp"   : cms.InputTag('allVBFEventHypothesesForAHtoMuTauLooseMuonIsolationZllRecoilCorrectedSysUp'),
    "sysZllRecoilCorrectionDown" : cms.InputTag('allVBFEventHypothesesForAHtoMuTauLooseMuonIsolationZllRecoilCorrectedSysDown')
}

elecTauPairSystematics = {
    "sysElectronEnUp"            : cms.InputTag('allElecTauPairsSysElectronEnUp'),
    "sysElectronEnDown"          : cms.InputTag('allElecTauPairsSysElectronEnDown'),
    "sysTauJetEnUp"              : cms.InputTag('allElecTauPairsSysTauJetEnUp'),
    "sysTauJetEnDown"            : cms.InputTag('allElecTauPairsSysTauJetEnDown'),
    "sysJetEnUp"                 : cms.InputTag('allElecTauPairsSysJetEnUp'),
    "sysJetEnDown"               : cms.InputTag('allElecTauPairsSysJetEnDown')
}

elecTauPairVBFEventSystematics = {
    "sysElectronEnUp"            : cms.InputTag('allVBFEventHypothesesForAHtoElecTauSysElectronEnUp'),
    "sysElectronEnDown"          : cms.InputTag('allVBFEventHypothesesForAHtoElecTauSysElectronEnDown'),
    "sysTauJetEnUp"              : cms.InputTag('allVBFEventHypothesesForAHtoElecTauSysTauJetEnUp'),
    "sysTauJetEnDown"            : cms.InputTag('allVBFEventHypothesesForAHtoElecTauSysTauJetEnDown'),
    "sysJetEnUp"                 : cms.InputTag('allVBFEventHypothesesForAHtoElecTauSysJetEnUp'),
    "sysJetEnDown"               : cms.InputTag('allVBFEventHypothesesForAHtoElecTauSysJetEnDown'),
    ##"sysZllRecoilCorrectionUp"   : cms.InputTag('allVBFEventHypothesesForAHtoElecTauZllRecoilCorrectedSysUp'),
    ##"sysZllRecoilCorrectionDown" : cms.InputTag('allVBFEventHypothesesForAHtoElecTauZllRecoilCorrectedSysDown')
}

elecTauPairSystematicsLooseElectronIsolation = {
    "sysElectronEnUp"            : cms.InputTag('allElecTauPairsLooseElectronIsolationSysElectronEnUp'),
    "sysElectronEnDown"          : cms.InputTag('allElecTauPairsLooseElectronIsolationSysElectronEnDown'),
    "sysTauJetEnUp"              : cms.InputTag('allElecTauPairsLooseElectronIsolationSysTauJetEnUp'),
    "sysTauJetEnDown"            : cms.InputTag('allElecTauPairsLooseElectronIsolationSysTauJetEnDown'),
    "sysJetEnUp"                 : cms.InputTag('allElecTauPairsLooseElectronIsolationSysJetEnUp'),
    "sysJetEnDown"               : cms.InputTag('allElecTauPairsLooseElectronIsolationSysJetEnDown')
}

tauNuPairSystematics = {
    "sysTauJetEnUp"        : cms.InputTag('allTauNuPairsSysTauJetEnUp'),
    "sysTauJetEnDown"      : cms.InputTag('allTauNuPairsSysTauJetEnDown'),
    "sysJetEnUp"           : cms.InputTag('allTauNuPairsSysJetEnUp'),
    "sysJetEnDown"         : cms.InputTag('allTauNuPairsSysJetEnDown')
}
htRatioSystematics = {
    "sysTauJetEnUp"       : cms.InputTag('htRatiosSysTauJetEnUp'),
    "sysTauJetEnDown"     : cms.InputTag('htRatiosSysTauJetEnDown'),
    "sysJetEnUp"          : cms.InputTag('htRatiosSysJetEnUp'),
    "sysJetEnDown"        : cms.InputTag('htRatiosSysJetEnDown')
}
metTopologySystematics = {
    "sysTauJetEnUp"      : cms.InputTag('metTopologiesSysTauJetEnUp'),
    "sysTauJetEnDown"    : cms.InputTag('metTopologiesSysTauJetEnDown'),
    "sysJetEnUp"         : cms.InputTag('metTopologiesSysJetEnUp'),
    "sysJetEnDown"       : cms.InputTag('metTopologiesSysJetEnDown')
}

metSystematicsForZtoMuTau = {
    # CV: MuScleFit muon momentum corrections do not work in CMSSW_5_2_x (May 4th 2012)
    ##"sysMuonPtUp"                : cms.InputTag('patMETsSysMuonPtUp'),
    ##"sysMuonPtDown"              : cms.InputTag('patMETsSysMuonPtDown'),
    "sysTauJetEnUp"              : cms.InputTag('patMETsSysTauJetEnUp'),
    "sysTauJetEnDown"            : cms.InputTag('patMETsSysTauJetEnDown'),
    "sysJetEnUp"                 : cms.InputTag('patMETsSysJetEnUp'),
    "sysJetEnDown"               : cms.InputTag('patMETsSysJetEnDown')
}

metSystematicsForZtoElecTau = {
    "sysElectronEnUp"            : cms.InputTag('patMETsSysElectronEnUp'),
    "sysElectronEnDown"          : cms.InputTag('patMETsSysElectronEnDown'),
    "sysTauJetEnUp"              : cms.InputTag('patMETsSysTauJetEnUp'),
    "sysTauJetEnDown"            : cms.InputTag('patMETsSysTauJetEnDown'),
}

metSystematicsForAHtoMuTau = {
    # CV: MuScleFit muon momentum corrections do not work in CMSSW_5_2_x (May 4th 2012)
    ##"sysMuonPtUp"                : cms.InputTag('patMETsSysMuonPtUp'),
    ##"sysMuonPtDown"              : cms.InputTag('patMETsSysMuonPtDown'),
    "sysTauJetEnUp"              : cms.InputTag('patMETsSysTauJetEnUp'),
    "sysTauJetEnDown"            : cms.InputTag('patMETsSysTauJetEnDown'),
    "sysJetEnUp"                 : cms.InputTag('patMETsSysJetEnUp'),
    "sysJetEnDown"               : cms.InputTag('patMETsSysJetEnDown')
}

metSystematicsForAHtoElecTau = {
    "sysElectronPtUp"            : cms.InputTag('patMETsSysElectronPtUp'),
    "sysElectronPtDown"          : cms.InputTag('patMETsSysElectronPtDown'),
    "sysTauJetEnUp"              : cms.InputTag('patMETsSysTauJetEnUp'),
    "sysTauJetEnDown"            : cms.InputTag('patMETsSysTauJetEnDown'),
    "sysJetEnUp"                 : cms.InputTag('patMETsSysJetEnUp'),
    "sysJetEnDown"               : cms.InputTag('patMETsSysJetEnDown')
}

metSystematicsForWtoTauNu = {
    "sysTauJetEnUp"        : cms.InputTag('smearedMETsysTauJetEnUp'),
    "sysTauJetEnDown"      : cms.InputTag('smearedMETsysTauJetEnUp'),
    "sysJetEnUp"           : cms.InputTag('smearedMETsysJetEnUp'),
    "sysJetEnDown"         : cms.InputTag('smearedMETsysJetEnDown')
}

jetSystematics = {   
    "sysJetEnUp"                 : cms.InputTag('patJetsJECshiftUp'),
    "sysJetEnDown"               : cms.InputTag('patJetsJECshiftDown')
}     

theorySystematics = {
    # CV: do not run theory systematics for now,
    #     as in particular the module for estimating PDF uncertainties
    #     significantly increases the overall run-time/memory consumption of the cmsRun job !!
    ##"sysPdfWeightsCTEQ66(45)"          : cms.InputTag('pdfWeights:cteq66'),
    ##"sysPdfWeightsMSTW2008nlo68cl(41)" : cms.InputTag('pdfWeights:MSTW2008nlo68cl'),
    ##"sysPdfWeightsNNPDF20(100)"        : cms.InputTag('pdfWeights:NNPDF20'),
    ##"sysIsrWeight"                     : cms.InputTag('isrWeight'),
    ##"sysFsrWeight"                     : cms.InputTag('fsrWeight')
}

def getSysUncertaintyNames(sysUncertaintyMaps):
    
    sysUncertaintyNames = set()
    
    for sysUncertaintyMap in sysUncertaintyMaps:
        for sysUncertaintyName in sysUncertaintyMap.keys():
            sysUncertaintyNames.add(sysUncertaintyName)

    return list(sysUncertaintyNames)

def getSysUncertaintyParameterSets(sysUncertaintyMaps):
    
    sysUncertaintyParameterSets = []
    
    for sysUncertaintyMap in sysUncertaintyMaps:
        for sysUncertaintyName, sysUncertaintyInputTag in sysUncertaintyMap.items():
            sysUncertaintyParameterSet = cms.PSet()

            setattr(sysUncertaintyParameterSet, "name", cms.string(sysUncertaintyName))
            setattr(sysUncertaintyParameterSet, "src", sysUncertaintyInputTag)

            sysUncertaintyParameterSets.append(sysUncertaintyParameterSet)

    return cms.VPSet(sysUncertaintyParameterSets)
