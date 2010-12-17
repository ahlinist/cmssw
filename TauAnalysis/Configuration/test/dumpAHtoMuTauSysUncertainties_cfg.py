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
            'rfio:/data2/friis/Run35SYS/plotsAHtoMuTau_all.root'
        ),
        dqmDirectory_store = cms.string('/')
    )
)

process.dumpAHtoMuTauSysUncertaintySequence = cms.Sequence(process.loadAHtoMuTauSysUncertainties)

higgsMassPoints = [ '90', '100', '130', '160', '200', '250', '350' ]

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

for higgsMassPoint in higgsMassPoints:
    for higgsType in [ "A", "bbA" ]:
	for analyzer in [ "ahMuTauAnalyzerOS_woBtag", "ahMuTauAnalyzerOS_wBtag" ]:
            dqmDirectory = "/harvested/%s%s/%s/afterGenPhaseSpaceCut_beforeEvtSelTrigger" % (higgsType, higgsMassPoint, analyzer)

  	    dumpAccModule = cms.EDAnalyzer("DQMDumpBinningResults",
	        binningService = cms.PSet(
                    pluginType = cms.string("ModelBinningService"),
                    dqmDirectories = cms.PSet(
                        genAcc = cms.string(dqmDirectory + '/' + 'modelBinnerForMuTauGenTauLeptonPairAcc'),
                        recWrtGenAcc = cms.string(dqmDirectory + '/' + 'modelBinnerForMuTauWrtGenTauLeptonPairAcc')
                    )
                )
            )
	    dumpAccModuleName = "dumpAHtoMuTauAcc%s%s" % (higgsType, higgsMassPoint)
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
	                dqmDirectory + '/' + 'sysUncertaintyBinningResults/modelBinnerForMuTauGenTauLeptonPairAcc'
                    )
    	        )
            )
            dumpAccUncertaintyModuleName = "dumpAHtoMuTauAccUncertainty%s%s" % (higgsType, higgsMassPoint)
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
                        dqmDirectory + '/' + 'sysUncertaintyBinningResults/modelBinnerForMuTauWrtGenTauLeptonPairAcc'
                    )
                )
            ) 
            dumpEffUncertaintyModuleName = "dumpAHtoMuTauEffUncertainty%s%s" % (higgsType, higgsMassPoint)
	    setattr(process, dumpEffUncertaintyModuleName, dumpEffUncertaintyModule)

	    process.dumpAHtoMuTauSysUncertaintySequence += dumpEffUncertaintyModule

#--------------------------------------------------------------------------------
# done with configuring. Now run it...
#--------------------------------------------------------------------------------

process.p = cms.Path(process.dumpAHtoMuTauSysUncertaintySequence)

# print-out all python configuration parameter information
#print process.dumpPython()


