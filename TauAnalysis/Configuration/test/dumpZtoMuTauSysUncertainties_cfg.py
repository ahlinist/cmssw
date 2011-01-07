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
            '/data2/friis/Run2PDF/harvested_ZtoMuTau_ZtautauPU156bx_Run2PDF.root'
            #'/data2/friis/Run2PDF/harvested_ZtoMuTau_Ztautau_powheg_Run2PDF.root'
        ),
        dqmDirectory_store = cms.string('/')
    )
)

dqmDirectory_Ztautau = '/zMuTauAnalyzerOS/afterGenPhaseSpaceCut_beforeEvtSelTrigger'

genAccBinner = 'modelBinnerForMuTauGenTauLeptonPairAcc'
recEffBinner = 'modelBinnerForMuTauWrtGenTauLeptonPairAcc'

process.dumpZtoMuTauAcceptance = cms.EDAnalyzer("DQMDumpBinningResults",
    binningService = cms.PSet(
        pluginType = cms.string("ModelBinningService"),
        dqmDirectories = cms.PSet(
            genAcc = cms.string(dqmDirectory_Ztautau + '/' + genAccBinner),
            recEff = cms.string(dqmDirectory_Ztautau + '/' + recEffBinner)
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
        Ztautau = cms.string(dqmDirectory_Ztautau + '/' + 'sysUncertaintyBinningResults' + '/' + genAccBinner)
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
        Ztautau = cms.string(dqmDirectory_Ztautau + '/' + 'sysUncertaintyBinningResults' + '/' + recEffBinner)
    )
) 

process.p = cms.Path(
    process.loadZtoMuTauSysUncertainties
   + process.dumpZtoMuTauAcceptance
   + process.dumpZtoMuTauAccUncertainties
   + process.dumpZtoMuTauEffUncertainties
)



