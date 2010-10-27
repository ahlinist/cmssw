import FWCore.ParameterSet.Config as cms

from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi import *

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
            ##'/data1/veelken/CMSSW_3_8_x/plots/AHtoMuTau/plotsAHtoMuTau_all.root'
            '/data1/friis/Run11.bak/Run11/plots_all.root'
        ),
        dqmDirectory_store = cms.string('')
    )
)

process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

process.exportAnalysisResults_woBtag = cms.EDAnalyzer("DQMExportAnalysisResults",

    dqmDirectory = cms.string('harvested/#PROCESSDIR#/ahMuTauAnalyzer_woBtag'),
    outputFilePath = cms.string("/data1/veelken/CMSSW_3_8_x/plots/AHtoMuTau/export"),                 

    processes = cms.PSet(
	##A120 = cms.PSet(
        ##    dqmDirectory = cms.string('A120'),
	##    outputFilePath = cms.string("m120"),
	##    outputFileName = cms.string("ggA_#CHANNEL_OUTPUTFILENAME#.hst"),
        ##    hasSysUncertainties = cms.bool(True)
        ##),
        ##bbA120 = cms.PSet(
        ##    dqmDirectory = cms.string('bbA120'),
	##    outputFilePath = cms.string("m120"),
	##    outputFileName = cms.string("bbA_#CHANNEL_OUTPUTFILENAME#.hst"),
        ##    hasSysUncertainties = cms.bool(True)
        ##),
        A120 = cms.PSet(
            dqmDirectory = cms.string('A120Sum'),
	    outputFilePath = cms.string("m120"),
	    outputFileName = cms.string("A_#CHANNEL_OUTPUTFILENAME#.hst"),
            hasSysUncertainties = cms.bool(True)
        ),
        ##A130 = cms.PSet(
        ##    dqmDirectory = cms.string('A130'),
	##    outputFilePath = cms.string("m130"),
	##    outputFileName = cms.string("ggA_#CHANNEL_OUTPUTFILENAME#.hst"),
        ##    hasSysUncertainties = cms.bool(True)
        ##),
        ##bbA130 = cms.PSet(
        ##    dqmDirectory = cms.string('bbA130'),
	##    outputFilePath = cms.string("m130"),
	##    outputFileName = cms.string("bbA_#CHANNEL_OUTPUTFILENAME#.hst"),
        ##    hasSysUncertainties = cms.bool(True)
        ##),
        A130 = cms.PSet(
            dqmDirectory = cms.string('A130Sum'),
	    outputFilePath = cms.string("m130"),
	    outputFileName = cms.string("A_#CHANNEL_OUTPUTFILENAME#.hst"),
            hasSysUncertainties = cms.bool(True)
        ),
        ##A200 = cms.PSet(
        ##    dqmDirectory = cms.string('A200'),
	##    outputFilePath = cms.string("m200"),
	##    outputFileName = cms.string("ggA_#CHANNEL_OUTPUTFILENAME#.hst"),
        ##    hasSysUncertainties = cms.bool(True)
        ##),
        ##bbA200 = cms.PSet(
        ##    dqmDirectory = cms.string('bbA200'),
	##    outputFilePath = cms.string("m200"),
	##    outputFileName = cms.string("bbA_#CHANNEL_OUTPUTFILENAME#.hst"),
        ##    hasSysUncertainties = cms.bool(True)
        ##),
        A200 = cms.PSet(
            dqmDirectory = cms.string('A200Sum'),
	    outputFilePath = cms.string("m200"),
	    outputFileName = cms.string("A_#CHANNEL_OUTPUTFILENAME#.hst"),
            hasSysUncertainties = cms.bool(True)
        ),
        Ztautau = cms.PSet(
            dqmDirectory = cms.string('Ztautau'),
	    outputFilePath = cms.string(""),
	    outputFileName = cms.string("ztt_#CHANNEL_OUTPUTFILENAME#.hst"),
            hasSysUncertainties = cms.bool(True)
        ),
        Zmumu = cms.PSet(
            dqmDirectory = cms.string('Zmumu'),
	    outputFilePath = cms.string(""),
	    outputFileName = cms.string("zmm_#CHANNEL_OUTPUTFILENAME#.hst"),
            hasSysUncertainties = cms.bool(False)
        ),
        QCD = cms.PSet(
            dqmDirectory = cms.string('Zmumu'),
	    outputFilePath = cms.string(""),
	    outputFileName = cms.string("qcd_#CHANNEL_OUTPUTFILENAME#.hst"),
            hasSysUncertainties = cms.bool(False)
        ),
        WplusJets = cms.PSet(
            dqmDirectory = cms.string('WplusJets'),
	    outputFilePath = cms.string(""),
	    outputFileName = cms.string("wjets_#CHANNEL_OUTPUTFILENAME#.hst"),
            hasSysUncertainties = cms.bool(False)
        ),
        TTplusJets = cms.PSet(
            dqmDirectory = cms.string('TTplusJets'),
	    outputFilePath = cms.string(""),
	    outputFileName = cms.string("ttbar_#CHANNEL_OUTPUTFILENAME#.hst"),
	    numEvents = cms.double(
                TARGET_LUMI*RECO_SAMPLES['TTplusJets']['x_sec']
            ),
            hasSysUncertainties = cms.bool(False)
        ),
        data = cms.PSet(
            dqmDirectory = cms.string('data'),
	    outputFilePath = cms.string(""),
	    outputFileName = cms.string("data_#CHANNEL_OUTPUTFILENAME#.hst"),
            hasSysUncertainties = cms.bool(False)
        )
    ),

    channels = cms.PSet(
	AHtoMuTau = cms.PSet(
	    template = cms.string(
	        'afterEvtSelNonCentralJetEt20bTag/#SYSTEMATICSDIR#/DiTauCandidateSVfitQuantities/psKine_MEt_ptBalance/Mass'
            ),
            normalization = cms.PSet(
                numEventsProcessed = cms.string(
                    'FilterStatistics/#SYSTEMATICSDIR#/genPhaseSpaceCut/passed_cumulative_numWeighted#a1#s1'
                ),
                numEventsPassed = cms.string(
                    'FilterStatistics/#SYSTEMATICSDIR#/evtSelNonCentralJetEt20bTag/passed_cumulative_numWeighted#a1#s1'
                )
            ),
            outputFileName = cms.string("ma")
        )
    ),

    ##systematics = cms.PSet(
    ##	  tauJetEnUp = cms.PSet(
    ##	      dqmDirectory = cms.string('sysUncertaintyHistManagerResults/sysTauJetEnUp'),
    ## 	      outputFilePath = cms.string("tau_es+1")
    ##    ),
    ##	  tauJetEnDown = cms.PSet(
    ##	      dqmDirectory = cms.string('sysUncertaintyHistManagerResults/sysTauJetEnDown'),
    ##	      outputFilePath = cms.string("tau_es-1")
    ##    ),
    ##	  muonPtUp = cms.PSet(
    ##	      dqmDirectory = cms.string('sysUncertaintyHistManagerResults/sysTauJetEnUp'),
    ## 	      outputFilePath = cms.string("mu_pt+1")
    ##    ),
    ##	  muonPtDown = cms.PSet(
    ##	      dqmDirectory = cms.string('sysUncertaintyHistManagerResults/sysTauJetEnDown'),
    ##	      outputFilePath = cms.string("mu_pt-1")
    ##    ),
    ##	  jetEnUp = cms.PSet(
    ##	      dqmDirectory = cms.string('sysUncertaintyHistManagerResults/sysTauJetEnUp'),
    ## 	      outputFilePath = cms.string("jet_es+1")
    ##    ),
    ##	  jetEnDown = cms.PSet(
    ##	      dqmDirectory = cms.string('sysUncertaintyHistManagerResults/sysTauJetEnDown'),
    ##	      outputFilePath = cms.string("jet_es-1")
    ##    ),                                                  
    ##)
)
 
process.p = cms.Path(
    process.loadAnalysisResults
  #+ process.dumpDQMStore 
   + process.exportAnalysisResults_woBtag
)
