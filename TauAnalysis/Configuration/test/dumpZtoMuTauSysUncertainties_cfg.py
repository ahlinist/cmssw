import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# Print-out systematic uncertainties on Z --> tau+ tau- 
# signal acceptance and efficiency
#--------------------------------------------------------------------------------

process = cms.Process('dumpZtoMuTauSysUncertainties')

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

process.loadZtoMuTauSysUncertainties = cms.EDAnalyzer("DQMFileLoader",
    dump = cms.PSet(
        inputFileNames = cms.vstring(
            'rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_1_2/plots/ZtoMuTau/7TeV/plotsZtoMuTau_ZtautauGenTauLeptonPairAccSum.root'
        ),
        dqmDirectory_store = cms.string('')
    )
)

dqmDirectory_Ztautau = 'zMuTauAnalyzerOS/afterGenPhaseSpaceCut_beforeEvtSelTrigger'

process.dumpZtoMuTauAcceptance = cms.EDAnalyzer("DQMDumpBinningResults",
    binningService = cms.PSet(
        pluginType = cms.string("ModelBinningService"),
        dqmDirectories = cms.PSet(
            ZtautauGenTauLeptonPairAcc = cms.string(dqmDirectory_Ztautau + '/' + 'modelBinnerForMuTauGenTauLeptonPairAcc'),
            ZtautauWrtGenTauLeptonPairAcc = cms.string(dqmDirectory_Ztautau + '/' + 'modelBinnerForMuTauWrtGenTauLeptonPairAcc')
        )
    )
)

theoryUncertainty = cms.PSet(
    sysNames = cms.vstring(""),
    sysTitle = cms.string(""),
    sysCentralValue = cms.string("CENTRAL_VALUE"),
    pluginType = cms.string("ModelBinningService"),
    method = cms.string("simple")
)

process.dumpZtoMuTauAccUncertainties = cms.EDAnalyzer("DQMDumpSysUncertaintyBinningResults",
    config = cms.VPSet(
        theoryUncertainty.clone(
            sysNames = cms.vstring("sysPdfWeights(45)"),
            sysTitle = cms.string("PDF"),
            sysCentralValue = cms.string("sysPdfWeights(0)"),
            method = cms.string("pdf")
        ),
        theoryUncertainty.clone(
            sysNames = cms.vstring("sysIsrWeight"),
            sysTitle = cms.string("ISR")
        ),
        theoryUncertainty.clone(            
            sysNames = cms.vstring("sysFsrWeight"),
            sysTitle = cms.string("FSR")
        )
    ),
    resultTypes = cms.vstring("acceptance"),  
    dqmDirectories = cms.PSet(
        Ztautau = cms.string(dqmDirectory_Ztautau + '/' + 'sysUncertaintyBinningResults/modelBinnerForMuTauGenTauLeptonPairAcc')
    )
)

expUncertainty = cms.PSet(
    sysNames = cms.vstring(""),
    sysTitle = cms.string(""),
    sysCentralValue = cms.string("CENTRAL_VALUE"),
    pluginType = cms.string("ModelBinningService")
)

process.dumpZtoMuTauEffUncertainties = cms.EDAnalyzer("DQMDumpSysUncertaintyBinningResults",
    config = cms.VPSet(
        expUncertainty.clone(
            sysNames = cms.vstring(
                "sysMuonPtUp",
                "sysMuonPtDown"
            ),
            sysTitle = cms.string("Muon Momentum scale")
        ),
        expUncertainty.clone(
            sysNames = cms.vstring(
                "sysTauJetEnUp",
                "sysTauJetEnDown"
            ),
            sysTitle = cms.string("Tau-jet Energy scale")
        ),
	expUncertainty.clone(
            sysNames = cms.vstring(
                "sysJetEnUp",
                "sysJetEnDown"
            ),
            sysTitle = cms.string("Jet Energy scale")
        ),
        expUncertainty.clone(
            sysNames = cms.vstring(
                "sysZllRecoilCorrectionUp",
                "sysZllRecoilCorrectionDown"
            ),
            sysTitle = cms.string("MEt Z-recoil correction")
        )
    ),
    resultTypes = cms.vstring("acceptance"),                                                  
    dqmDirectories = cms.PSet(
        Ztautau = cms.string(dqmDirectory_Ztautau + '/' + 'sysUncertaintyBinningResults/modelBinnerForMuTauWrtGenTauLeptonPairAcc')
    )
) 

process.p = cms.Path(
    process.loadZtoMuTauSysUncertainties
   + process.dumpZtoMuTauAcceptance
   + process.dumpZtoMuTauAccUncertainties
   + process.dumpZtoMuTauEffUncertainties
)



