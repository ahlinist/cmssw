import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# Export analysis results into ASCII files,
# in the format used by the CDF collaboration
#--------------------------------------------------------------------------------

process = cms.Process('exportAnalysisResults')

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

process.loadAnalysisResults = cms.EDAnalyzer("DQMFileLoader",
    all = cms.PSet(
        inputFileNames = cms.vstring(
            '/data1/veelken/CMSSW_3_8_x/plots/AHtoMuTau/export/plotsAHtoMuTau.root'
        ),
        dqmDirectory_store = cms.string('harvested/AH120')
    )
)

process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

process.exportAnalysisResults_woBtag = cms.EDAnalyzer("DQMExportAnalysisResults",

    dqmDirectory = cms.string('harvested/#PROCESSDIR#/ahMuTauAnalyzer_woBtag'),
    outputFilePath = cms.string("/data1/veelken/CMSSW_3_8_x/plots/AHtoMuTau/export"),                 

    processes = cms.PSet(
	AH120 = cms.PSet(
            dqmDirectory = cms.string('AH120'),
	    outputFilePath = cms.string("m120"),
	    outputFileName = cms.string("gga_#CHANNEL_OUTPUTFILENAME#.hst"),
	    numEvents = cms.uint32(2057446), # number of events in sample before skimming
            hasSysUncertainties = cms.bool(True)
        )
    ),

    channels = cms.PSet(
	AHtoMuTau = cms.PSet(
	    meName = cms.string(
	       'afterEvtSelNonCentralJetEt20bTag/#SYSTEMATICSDIR#/DiTauCandidateSVfitQuantities/psKine_MEt_ptBalance/Mass'
            ),
            outputFileName = cms.string("ma")
        )
    ),

    systematics = cms.PSet(
	tauJetEnUp = cms.PSet(
	    dqmDirectory = cms.string('sysUncertaintyHistManagerResults/sysTauJetEnUp'),
	    outputFilePath = cms.string("tes+1")
        ),
	tauJetEnDown = cms.PSet(
	    dqmDirectory = cms.string('sysUncertaintyHistManagerResults/sysTauJetEnDown'),
	    outputFilePath = cms.string("tes-1")
        )
    )
)
 
process.p = cms.Path(
    process.loadAnalysisResults
  #+ process.dumpDQMStore 
   + process.exportAnalysisResults_woBtag
)
