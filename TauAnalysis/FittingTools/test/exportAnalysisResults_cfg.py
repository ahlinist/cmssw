import FWCore.ParameterSet.Config as cms

from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi import *
from TauAnalysis.FittingTools.tools.configureDQMExportAnalysisResults import configureDQMExportAnalysisResults

#--------------------------------------------------------------------------------
# Export analysis results into ASCII files,
# in the format used by the CDF collaboration
#--------------------------------------------------------------------------------

process = cms.Process('exportAnalysisResults')

process.DQMStore = cms.Service("DQMStore")

process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

higgsMassPoints = [ 90, 100, 130, 160, 200, 250, 350 ]
#higgsMassPoints = [ 90, 100, 120, 130, 140, 160, 180, 200, 250, 300, 350 ]

channelsToExport = [
    'AHtoMuTau_woBtag',
    'AHtoMuTau_wBtag'
]

processesToExport = [
    'Ztautau',
    'Zmumu',
    'QCD',
    ##'gammaPlusJets',
    'WplusJets',
    'TTplusJets',
    'data'
]

#outputFilePath = "/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/export_AHtoMuTau_AHtoElecTau_AHtoElecMu"
#outputFilePath = "/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/export_AHtoMuTau_TaNCmedium_woSys"
outputFilePath = "/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/export_AHtoMuTau_TaNCmedium_wSys"
#outputFilePath = "/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/export_AHtoMuTau_TaNCloose_woSys"

channels = {
    'AHtoMuTau_woBtag' : {
        'shortName'                : "ma",
        'dqmDirectoryTemplate'     : '/export/harvested/%s/ahMuTauAnalyzerOS_woBtag/afterEvtSelDiTauCandidateForAHtoMuTauZeroCharge/',
        'dqmDirectoryFilterStat'   : '/export/harvested/%s/ahMuTauAnalyzerOS_woBtag/FilterStatistics/',
        'meNameNumEventsProcessed' : 'genPhaseSpaceCut/passed_cumulative_numWeighted#a1#s1',
        # wo. systematics
        'meNameTemplate'           : 'DiTauCandidateSVfitQuantities/psKine_MEt_ptBalance/MassL',
        'meNameNumEventsPassed'    : 'evtSelDiTauCandidateForAHtoMuTauZeroCharge/passed_cumulative_numWeighted#a1#s1',
        # w. systematics
	'meNameTemplateSys'        : 'sysUncertaintyHistManagerResults/#SYSTEMATICSDIR#/' \
                                    + 'DiTauCandidateSVfitQuantities/psKine_MEt_ptBalance/MassL',
        'meNameNumEventsPassedSys' : 'sysUncertaintyBinningResults/dataBinningResults/#SYSTEMATICSDIR#/binContent_region1#a1#s1',
        'dataIntLumi' : ZtoMuTau.TARGET_LUMI,
        'systematics' : [
            'muonPtUp',
            'muonPtDown',
            'tauJetEnUp',
            'tauJetEnDown',
            'jetEnUp',
            'jetEnDown',
            'ZllRecoilCorrectionUp',
            'ZllRecoilCorrectionDown'
        ],
        'doExportSystematics' : True,
        'processes' : {
            'Ztautau'    : 'ZtautauSum',
            'Zmumu'      : 'Zmumu',
            'QCD'        : 'qcdSum',
            'WplusJets'  : 'WplusJetsSum',
            'TTplusJets' : 'TTplusJets',
            'data'       : 'data'
        },
        'inputFileNames' : [
            '/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_TaNCmedium_wSys_skimmed_part01.root',
            '/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_TaNCmedium_wSys_skimmed_part02.root'
            #'/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_TaNCloose_woSys_skimmed_part01.root',
            #'/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_TaNCloose_woSys_skimmed_part02.root'
        ]
    },
    'AHtoMuTau_wBtag' : {
        'shortName'                : "mab",
	'dqmDirectoryTemplate'     : '/export/harvested/%s/ahMuTauAnalyzerOS_wBtag/afterEvtSelDiTauCandidateForAHtoMuTauZeroCharge/',
        'dqmDirectoryFilterStat'   : '/export/harvested/%s/ahMuTauAnalyzerOS_wBtag/FilterStatistics/',
        'meNameNumEventsProcessed' : 'genPhaseSpaceCut/passed_cumulative_numWeighted#a1#s1',
        # wo. systematics
        'meNameTemplate'           : 'DiTauCandidateSVfitQuantities/psKine_MEt_ptBalance/MassL',
        'meNameNumEventsPassed'    : 'evtSelDiTauCandidateForAHtoMuTauZeroCharge/passed_cumulative_numWeighted#a1#s1',
        # w. systematics
	'meNameTemplateSys'        : 'sysUncertaintyHistManagerResults/#SYSTEMATICSDIR#/' \
                                    + 'DiTauCandidateSVfitQuantities/psKine_MEt_ptBalance/MassL',
        'meNameNumEventsPassedSys' : 'sysUncertaintyBinningResults/dataBinningResults/#SYSTEMATICSDIR#/binContent_region1#a1#s1',
        'dataIntLumi' : ZtoMuTau.TARGET_LUMI,
        'systematics' : [
            'muonPtUp',
            'muonPtDown',
            'tauJetEnUp',
            'tauJetEnDown',
            'jetEnUp',
            'jetEnDown',
            'ZllRecoilCorrectionUp',
            'ZllRecoilCorrectionDown'
        ],
        'doExportSystematics' : True,
        'processes' : {
            'Ztautau'    : 'ZtautauSum',
            'Zmumu'      : 'Zmumu',
            'QCD'        : 'qcdSum',
            'WplusJets'  : 'WplusJetsSum',
            'TTplusJets' : 'TTplusJets',
            'data'       : 'data'
        },
        'inputFileNames' : [
            # CV: no files need to be loaded, as wBtag histograms are contained in same .root file as woBtag histograms
        ]
    },
    'AHtoMuTau_inclusive' : {
        'shortName'                : "ma",
        'dqmDirectoryTemplate'     : '/export/harvested/%s/ahMuTauAnalyzerOS_inclusive/afterEvtSelDiTauCandidateForAHtoMuTauZeroCharge/',
        'dqmDirectoryFilterStat'   : '/export/harvested/%s/ahMuTauAnalyzerOS_inclusive/FilterStatistics/',
        'meNameNumEventsProcessed' : 'genPhaseSpaceCut/passed_cumulative_numWeighted#a1#s1',
        # wo. systematics
        'meNameTemplate'           : 'DiTauCandidateSVfitQuantities/psKine_MEt_ptBalance/MassL',
        'meNameNumEventsPassed'    : 'evtSelDiTauCandidateForAHtoMuTauZeroCharge/passed_cumulative_numWeighted#a1#s1',
        # w. systematics
	'meNameTemplateSys'        : 'sysUncertaintyHistManagerResults/#SYSTEMATICSDIR#/' \
                                    + 'DiTauCandidateSVfitQuantities/psKine_MEt_ptBalance/MassL',
        'meNameNumEventsPassedSys' : 'sysUncertaintyBinningResults/dataBinningResults/#SYSTEMATICSDIR#/binContent_region1#a1#s1',
        'dataIntLumi' : ZtoMuTau.TARGET_LUMI,
        'systematics' : [
            'muonPtUp',
            'muonPtDown',
            'tauJetEnUp',
            'tauJetEnDown',
            'jetEnUp',
            'jetEnDown',
            'ZllRecoilCorrectionUp',
            'ZllRecoilCorrectionDown'
        ],
        'doExportSystematics' : True,
        'processes' : {
            'Ztautau'    : 'ZtautauSum',
            'Zmumu'      : 'Zmumu',
            'QCD'        : 'qcdSum',
            'WplusJets'  : 'WplusJetsSum',
            'TTplusJets' : 'TTplusJets',
            'data'       : 'data'
        },
        'inputFileNames' : [
            # CV: no files need to be loaded, as inclusive histograms are computed "on-the-fly" from wBtag and woBtag histograms
        ]
    },	    

    'AHtoElecTau' : {
        'shortName'                : "ea",
        'dqmDirectoryTemplate'     : '/export/summed/harvested/%s/zElecTauAnalyzer/afterEvtSelElecTauPairZeeHypothesisVeto/',
        'dqmDirectoryFilterStat'   : '/export/summed/harvested/%s/zElecTauAnalyzer/FilterStatistics/',
        'meNameTemplate'           : 'DiTauCandidateSVfitQuantities/psKine_MEt_ptBalance/MassL',
        'meNameNumEventsProcessed' : 'genPhaseSpaceCut/passed_cumulative_numWeighted#a1#s1',
        'meNameNumEventsPassed'    : 'evtSelElecTauPairZeeHypothesisVeto/passed_cumulative_numWeighted#a1#s1',
        'dataIntLumi' : 32.0,
        'systematics' : [
            'elecEnUp',
            'elecEnDown',
            'tauJetEnUp',
            'tauJetEnDown',
            'jetEnUp',
            'jetEnDown',
            'ZllRecoilCorrectionUp',
            'ZllRecoilCorrectionDown'
        ],
        'doExportSystematics' : False,
        'processes' : {
            'Ztautau'       : 'Ztautau',
            'Zee'           : 'Zee',
            'QCD'           : 'qcdSum',
            'gammaPlusJets' : 'gammaPlusJetsSum',
            'WplusJets'     : 'WplusJetsSum',
            'TTplusJets'    : 'TTplusJets',
            'data'          : 'Data'
        },
        'inputFileNames' : [
            '/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsZtoElecTau_skimmed.root'
        ]
    },
    
    'AHtoElecMu' : {
        'shortName'                : "em",
        'dqmDirectoryTemplate'     : '/export/harvested/%s/zElecMuAnalyzer/afterEvtSelDiMuZMass/',
        'dqmDirectoryFilterStat'   : '/export/harvested/%s/zElecMuAnalyzer/FilterStatistics/',
        'meNameTemplate'           : 'DiTauCandidateSVfitQuantities/psKine_MEt_ptBalance/Mass',
        'meNameNumEventsProcessed' : 'genPhaseSpaceCut/passed_cumulative_numWeighted#a1#s1',
        'meNameNumEventsPassed'    : 'evtSelDiMuZMass/passed_cumulative_numWeighted#a1#s1',
        'dataIntLumi' : 35.0,
        'systematics' : [
            'elecEnUp',
            'elecEnDown',
            'muonPtUp',
            'muonPtDown',
            'jetEnUp',
            'jetEnDown',
            'ZllRecoilCorrectionUp',
            'ZllRecoilCorrectionDown'
        ],
        'doExportSystematics' : False,
        'processes' : {
            'Ztautau'    : 'Ztautau',
            'Zee'        : 'Zee',
            'Zmumu'      : 'Zmumu',
            'QCD'        : 'qcdSum',
            'WplusJets'  : 'WplusJetsSum',
            'TTplusJets' : 'TTplusJets',
            'Data'       : 'Data'
        },
        'inputFileNames' : [
            '/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsZtoElecMu_skimmed.root'
        ]
    }
}

processes = {
    'A' : {
        'outputFileName' : "gga_#CHANNEL_OUTPUTFILENAME#.hst",
        'hasSystematics' : True        
    },
    'bbA' : {
        'outputFileName' : "qqa_#CHANNEL_OUTPUTFILENAME#.hst",
        'hasSystematics' : True                
    },
    'Ztautau' : {
        'xSection'       : ZtoMuTau.RECO_SAMPLES['ZtautauPU156bx']['x_sec']
                          + ZtoMuTau.RECO_SAMPLES['qqZtautauPU156bx']['x_sec'],
        'outputFileName' : "ztt_#CHANNEL_OUTPUTFILENAME#.hst",
        'hasSystematics' : True                
    },
    'Zmumu' : {
        'xSection'       : ZtoMuTau.RECO_SAMPLES['Zmumu']['x_sec'],
        'outputFileName' : "zmm_#CHANNEL_OUTPUTFILENAME#.hst",
        'hasSystematics' : True        
    },
    'Zee' : {
        'xSection'       : ZtoMuTau.RECO_SAMPLES['Zee']['x_sec'],
        'outputFileName' : "zee_#CHANNEL_OUTPUTFILENAME#.hst",
        'hasSystematics' : True        
    },
    'QCD' : {
        'outputFileName' : "qcd_#CHANNEL_OUTPUTFILENAME#.hst",
        'hasSystematics' : True        
    },
    'gammaPlusJets' : {
        'outputFileName' : "gammajets_#CHANNEL_OUTPUTFILENAME#.hst",
        'hasSystematics' : True        
    },
    'WplusJets' : {
        'xSection'       : ZtoMuTau.RECO_SAMPLES['Wenu']['x_sec']
                          + ZtoMuTau.RECO_SAMPLES['Wmunu']['x_sec']
                          + ZtoMuTau.RECO_SAMPLES['Wtaunu']['x_sec'],
        'outputFileName' : "wjets_#CHANNEL_OUTPUTFILENAME#.hst",
        'hasSystematics' : True        
    },
    'TTplusJets' : {
        'xSection'       : ZtoMuTau.RECO_SAMPLES['TTplusJets']['x_sec'],
        'outputFileName' : "ttbar_#CHANNEL_OUTPUTFILENAME#.hst",
        'hasSystematics' : True        
    },
    'data' : {
        'outputFileName' : "data_#CHANNEL_OUTPUTFILENAME#.hst",
        'hasSystematics' : False        
    }
}

#--------------------------------------------------------------------------------
# set configuration parameters of DQMFileLoader module
#--------------------------------------------------------------------------------

inputFileNames = []
for channelName in channels.keys():
    if channelName in channelsToExport:
        inputFileNames.extend(channels[channelName]['inputFileNames'])

process.loadAnalysisResults = cms.EDAnalyzer("DQMFileLoader",
    all = cms.PSet(
        inputFileNames = cms.vstring(inputFileNames),
        dqmDirectory_store = cms.string('/export')
    )
)

process.exportAnalysisSequence = cms.Sequence(process.loadAnalysisResults)

#--------------------------------------------------------------------------------
# CV: No Higgs templates available for AHtoElecTau channel yet
#    --> assume Higgs template shape to be the same in AHtoElecTau and AHtoMuTau channels
#    --> assume ratio of Ztautau/Higgs efficiencies to be constants,
#        so that normalization of Higgs template for AHtoElecTau channel can be obtained
#        by scaling template for AHtoMuTau by AHtoElecTau/AHtoMuTau Ztautau efficiency ratio
#--------------------------------------------------------------------------------

if 'AHtoElecTau' in channelsToExport:
    process.sumAHtoElecTau = cms.EDAnalyzer("DQMHistAdder",
        qcdSum = cms.PSet(
            dqmDirectories_input = cms.vstring(
                '/export/summed/harvested/qcdBCtoESum/zElecTauAnalyzer/',
                '/export/summed/harvested/qcdEMenrichedSum/zElecTauAnalyzer/'
            ),
            dqmDirectory_output = cms.string('/export/summed/harvested/qcdSum/zElecTauAnalyzer/')
        ),
        WplusJetsSum = cms.PSet(
            dqmDirectories_input = cms.vstring(
                '/export/summed/harvested/WtoENu/zElecTauAnalyzer/',
                '/export/summed/harvested/WtoTauNu/zElecTauAnalyzer/'
            ),
            dqmDirectory_output = cms.string('/export/summed/harvested/WplusJetsSum/zElecTauAnalyzer/')
        )                                   
    )

    process.exportAnalysisSequence += process.sumAHtoElecTau

    psetsAHtoElecTauPrediction = []
    for higgsMassPoint in higgsMassPoints:
	for higgsType in [ "A", "bbA" ]:
	    pset = cms.PSet(
                meName_input = cms.string(
                    (channels['AHtoMuTau_woBtag']['dqmDirectoryFilterStat']
                   + channels['AHtoMuTau_woBtag']['meNameNumEventsPassed']) % ("%s%d" % (higgsType, higgsMassPoint))
                ),
                meName_output = cms.string(
                    (channels['AHtoElecTau']['dqmDirectoryFilterStat']
                   + channels['AHtoElecTau']['meNameNumEventsPassed']) % ("%s%d" % (higgsType, higgsMassPoint))
                ),
                meType = cms.string("real"),
                scaleFactor = cms.double(0.613) # CV: scale-factor set to ratio of Ztautau efficiencies
            )
            psetsAHtoElecTauPrediction.append(pset)

    process.compAHtoElecTauPrediction = cms.EDAnalyzer("DQMHistScaler",
        config = cms.VPSet(psetsAHtoElecTauPrediction)
    )

    process.exportAnalysisSequence += process.compAHtoElecTauPrediction

#--------------------------------------------------------------------------------
# CV: No Higgs templates available for AHtoElecMu channel yet
#    --> assume Higgs template shape to be the same in AHtoElecMu and AHtoMuTau channels
#    --> assume ratio of Ztautau/Higgs efficiencies to be constants,
#        so that normalization of Higgs template for AHtoElecMu channel can be obtained
#        by scaling template for AHtoMuTau by AHtoElecMu/AHtoMuTau Ztautau efficiency ratio
#--------------------------------------------------------------------------------

if 'AHtoElecMu' in channelsToExport:
    process.sumAHtoElecMu = cms.EDAnalyzer("DQMHistAdder",
        WplusJetsSum = cms.PSet(
            dqmDirectories_input = cms.vstring(
                '/export/harvested/Wenu/zElecMuAnalyzer/',
                '/export/harvested/Wmunu/zElecMuAnalyzer/',
                '/export/harvested/Wtaunu/zElecMuAnalyzer/'
            ),
            dqmDirectory_output = cms.string('/export/harvested/WplusJetsSum/zElecMuAnalyzer/')
        )                                   
    )

    process.exportAnalysisSequence += process.sumAHtoElecMu

    psetsAHtoElecMuPrediction = []
    for higgsMassPoint in higgsMassPoints:
	for higgsType in [ "A", "bbA" ]:
	    pset = cms.PSet(
                meName_input = cms.string(
                    (channels['AHtoMuTau_woBtag']['dqmDirectoryFilterStat']
                   + channels['AHtoMuTau_woBtag']['meNameNumEventsPassed']) % ("%s%d" % (higgsType, higgsMassPoint))
                ),
                meName_output = cms.string(
                    (channels['AHtoElecMu']['dqmDirectoryFilterStat']
                   + channels['AHtoElecMu']['meNameNumEventsPassed']) % ("%s%d" % (higgsType, higgsMassPoint))
                ),
                meType = cms.string("real"),
                scaleFactor = cms.double(0.433) # CV: scale-factor set to ratio of Ztautau efficiencies
            )
            psetsAHtoElecMuPrediction.append(pset)

    process.compAHtoElecMuPrediction = cms.EDAnalyzer("DQMHistScaler",
        config = cms.VPSet(psetsAHtoElecMuPrediction)
    )
    
    process.exportAnalysisSequence += process.compAHtoElecMuPrediction

#--------------------------------------------------------------------------------
# add b-tag and no-b-tag channels for "inclusive" analysis
#--------------------------------------------------------------------------------

process.compInclusiveResultsAHtoMuTau = cms.EDAnalyzer("DQMHistAdder")
for processName in processesToExport:
    pset = cms.PSet(
       dqmDirectories_input = cms.vstring(
           '/export/harvested/%s/ahMuTauAnalyzerOS_woBtag/' % channels['AHtoMuTau_woBtag']['processes'][processName],
           '/export/harvested/%s/ahMuTauAnalyzerOS_wBtag/' % channels['AHtoMuTau_woBtag']['processes'][processName]
       ),
       dqmDirectory_output = cms.string(
           '/export/harvested/%s/ahMuTauAnalyzerOS_inclusive/' % channels['AHtoMuTau_woBtag']['processes'][processName]
       )
    )
    setattr(process.compInclusiveResultsAHtoMuTau, processName, pset)

for higgsMassPoint in higgsMassPoints:
    for higgsType in [ "A", "bbA" ]:
        pset = cms.PSet(
           dqmDirectories_input = cms.vstring(
               '/export/harvested/%s%d/ahMuTauAnalyzerOS_woBtag/' % (higgsType, higgsMassPoint),
               '/export/harvested/%s%d/ahMuTauAnalyzerOS_wBtag/' % (higgsType, higgsMassPoint)
           ),
           dqmDirectory_output = cms.string(
               '/export/harvested/%s%d/ahMuTauAnalyzerOS_inclusive/' % (higgsType, higgsMassPoint)
           )
        )
        setattr(process.compInclusiveResultsAHtoMuTau, "%s%d" % (higgsType, higgsMassPoint), pset)

process.exportAnalysisSequence += process.compInclusiveResultsAHtoMuTau

#--------------------------------------------------------------------------------
# set configuration parameters of DQMExportAnalysisResults module
#--------------------------------------------------------------------------------
    
process.exportAnalysisResults = \
  configureDQMExportAnalysisResults(channels, channelsToExport, processes, processesToExport, higgsMassPoints, outputFilePath)

process.exportAnalysisSequence += process.exportAnalysisResults

# CV: this will add number of events passing genPhaseSpaceCut in woBtag and wBtag channels
#    --> acceptance*efficiency estimate for inclusive channel will be factor 2 too to low !!
#    --> need to correct for this by increasing effective cross-section for inclusive channel by corresponding factor 2
process.exportAnalysisResults_inclusive = \
  configureDQMExportAnalysisResults(channels, "AHtoMuTau_inclusive", processes, processesToExport, higgsMassPoints, 
	                            "/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/export_AHtoMuTau_TaNCmedium_wSys_inclusive")

process.exportAnalysisSequence += process.exportAnalysisResults_inclusive

#--------------------------------------------------------------------------------
# compute effective cross-sections for merged samples
#--------------------------------------------------------------------------------

if 'AHtoMuTau_woBtag' in channelsToExport:
    print("computing AHtoMuTau effectice cross-sections...")

    processEntryAHtoMuTau = cms.PSet(
        efficiency = cms.PSet(
            numerator = cms.string(channels['AHtoMuTau_woBtag']['meNameNumEventsPassed']),
            denominator = cms.string(channels['AHtoMuTau_woBtag']['meNameNumEventsProcessed'])
        ),
        numEventsPassed = cms.string(channels['AHtoMuTau_woBtag']['meNameNumEventsPassed']),
        dqmDirectory = cms.string('')
    )

    process.compDQMEffXsecAHtoMuTau = cms.EDAnalyzer("DQMEffXsecCalculator",
        dataIntLumi = cms.double(channels['AHtoMuTau_woBtag']['dataIntLumi']),
        channels = cms.PSet(
            QCD = processEntryAHtoMuTau.clone(
                dqmDirectory = cms.string(channels['AHtoMuTau_woBtag']['dqmDirectoryFilterStat']
                                          % channels['AHtoMuTau_woBtag']['processes']['QCD'])
            )
        )
    )
    
    process.exportAnalysisSequence += process.compDQMEffXsecAHtoMuTau
    
if 'AHtoElecTau' in channelsToExport:
    print("computing AHtoElecTau effectice cross-sections...")

    processEntryAHtoElecTau = cms.PSet(
        efficiency = cms.PSet(
            numerator = cms.string(channels['AHtoElecTau']['meNameNumEventsPassed']),
            denominator = cms.string(channels['AHtoElecTau']['meNameNumEventsProcessed'])
        ),
        numEventsPassed = cms.string(channels['AHtoElecTau']['meNameNumEventsPassed']),
        dqmDirectory = cms.string('')
    )

    process.compDQMEffXsecAHtoElecTau = cms.EDAnalyzer("DQMEffXsecCalculator",
        dataIntLumi = cms.double(channels['AHtoElecTau']['dataIntLumi']),
        channels = cms.PSet(
            QCD = processEntryAHtoElecTau.clone(
                dqmDirectory = cms.string(channels['AHtoElecTau']['dqmDirectoryFilterStat']
                                          % channels['AHtoElecTau']['processes']['QCD'])
            ),
            gammaPlusJets = processEntryAHtoElecTau.clone(
                dqmDirectory = cms.string(channels['AHtoElecTau']['dqmDirectoryFilterStat']
                                          % channels['AHtoElecTau']['processes']['gammaPlusJets'])
            )                                               
        )
    )
    
    process.exportAnalysisSequence += process.compDQMEffXsecAHtoElecTau

if 'AHtoElecMu' in channelsToExport:
    print("computing AHtoElecMu effectice cross-sections...")

    processEntryAHtoElecMu = cms.PSet(
        efficiency = cms.PSet(
            numerator = cms.string(channels['AHtoElecMu']['meNameNumEventsPassed']),
            denominator = cms.string(channels['AHtoElecMu']['meNameNumEventsProcessed'])
        ),
        numEventsPassed = cms.string(channels['AHtoElecMu']['meNameNumEventsPassed']),
        dqmDirectory = cms.string('')
    )

    process.compDQMEffXsecAHtoElecMu = cms.EDAnalyzer("DQMEffXsecCalculator",
        dataIntLumi = cms.double(channels['AHtoElecMu']['dataIntLumi']),
        channels = cms.PSet(
            QCD = processEntryAHtoElecMu.clone(
                dqmDirectory = cms.string(channels['AHtoElecMu']['dqmDirectoryFilterStat']
                                          % channels['AHtoElecMu']['processes']['QCD'])
            )
        )
    )
    
    process.exportAnalysisSequence += process.compDQMEffXsecAHtoElecMu

#--------------------------------------------------------------------------------
# done with configuring. Now run it...
#--------------------------------------------------------------------------------

process.p = cms.Path(process.exportAnalysisSequence)

# print-out all python configuration parameter information
#print process.dumpPython()
