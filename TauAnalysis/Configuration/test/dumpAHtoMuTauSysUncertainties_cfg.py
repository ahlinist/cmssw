import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# Print-out systematic uncertainties on MSSM Higgs A/H --> tau+ tau- 
# signal acceptance and efficiency
#--------------------------------------------------------------------------------

process = cms.Process('dumpZtoMuTauSysUncertainties')

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

process.loadAHtoMuTauSysUncertainties = cms.EDAnalyzer("DQMFileLoader",
    dump = cms.PSet(
        inputFileNames = cms.vstring(
            '/data2/friis/Run1PDF/plotsAHtoMuTau_all.bsmbb.root',
	    '/data2/friis/Run1PDF/plotsAHtoMuTau_all.bsmgg.root'
        ),
        dqmDirectory_store = cms.string('/')
    )
)

process.dumpAHtoMuTauSysUncertaintySequence = cms.Sequence(process.loadAHtoMuTauSysUncertainties)

genAccBinner = 'modelBinnerForMuTauGenTauLeptonPairAcc'
recEffBinner = {
    'ahMuTauAnalyzerOS_woBtag' : 'modelBinnerForMuTauCentralJetVetoWrtGenTauLeptonPairAcc',
    'ahMuTauAnalyzerOS_wBtag'  : 'modelBinnerForMuTauCentralJetBtagWrtGenTauLeptonPairAcc'
}

#higgsMassPoints = [ '90', '100', '130', '160', '200', '250', '350' ]
higgsMassPoints = {
  'A'   : [ '90', '100', '120', '160', '180', '200', '250', '300', '350' ],
  'bbA' : [ '130', '180', '200', '300' ]
}

theoryUncertainty = cms.PSet(
    sysNames = cms.vstring(""),
    sysTitle = cms.string(""),
    sysCentralValue = cms.string("CENTRAL_VALUE"),
    pluginType = cms.string("ModelBinningService"),
    method = cms.string("simple")
)

expUncertainty = cms.PSet(
    sysNames = cms.vstring(""),
    sysTitle = cms.string(""),
    sysCentralValue = cms.string("CENTRAL_VALUE"),
    pluginType = cms.string("ModelBinningService")
)

for higgsType in [ "A", "bbA" ]:
    for higgsMassPoint in higgsMassPoints[higgsType]:
	for analyzer in [ "ahMuTauAnalyzerOS_woBtag", "ahMuTauAnalyzerOS_wBtag" ]:
            dqmDirectory = "/harvested/%s%s/%s/afterGenPhaseSpaceCut_beforeEvtSelTrigger" % (higgsType, higgsMassPoint, analyzer)

  	    dumpAccModule = cms.EDAnalyzer("DQMDumpBinningResults",
	        binningService = cms.PSet(
                    pluginType = cms.string("ModelBinningService"),
                    dqmDirectories = cms.PSet(
                        genAcc = cms.string(dqmDirectory + '/' + genAccBinner),
                        recWrtGenAcc = cms.string(dqmDirectory + '/' + recEffBinner[analyzer])
                    )
                )
            )
	    dumpAccModuleName = "dumpAHtoMuTauAcc%s%s%s" % (analyzer, higgsType, higgsMassPoint)
	    setattr(process, dumpAccModuleName, dumpAccModule)

	    process.dumpAHtoMuTauSysUncertaintySequence += dumpAccModule

	    dumpAccUncertaintyModule = cms.EDAnalyzer("DQMDumpSysUncertaintyBinningResults",
                config = cms.VPSet(
                   theoryUncertainty.clone(
            	       sysNames = cms.vstring("sysPdfWeights(45)"),
            	       sysTitle = cms.string("PDF"),
            	       sysCentralValue = cms.string("sysPdfWeights(0)"),
            	       method = cms.string("pdf")
        	   )
                ),
    	        resultTypes = cms.vstring("acceptance"),  
    	        dqmDirectories = cms.PSet(
    	            genAccUncertainry = cms.string(
	                dqmDirectory + '/' + 'sysUncertaintyBinningResults' + '/' + genAccBinner
                    )
    	        )
            )
            dumpAccUncertaintyModuleName = "dumpAHtoMuTauAccUncertainty%s%s%s" % (analyzer, higgsType, higgsMassPoint)
	    setattr(process, dumpAccUncertaintyModuleName, dumpAccUncertaintyModule)

	    process.dumpAHtoMuTauSysUncertaintySequence += dumpAccUncertaintyModule

	    dumpEffUncertaintyModule = cms.EDAnalyzer("DQMDumpSysUncertaintyBinningResults",
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
                    effUncertainty = cms.string(
                        dqmDirectory + '/' + 'sysUncertaintyBinningResults' + '/' + recEffBinner[analyzer]
                    )
                )
            ) 
            dumpEffUncertaintyModuleName = "dumpAHtoMuTauEffUncertainty%s%s%s" % (analyzer, higgsType, higgsMassPoint)
	    setattr(process, dumpEffUncertaintyModuleName, dumpEffUncertaintyModule)

	    process.dumpAHtoMuTauSysUncertaintySequence += dumpEffUncertaintyModule

#--------------------------------------------------------------------------------
# done with configuring. Now run it...
#--------------------------------------------------------------------------------

process.p = cms.Path(process.dumpAHtoMuTauSysUncertaintySequence)

# print-out all python configuration parameter information
#print process.dumpPython()


