import FWCore.ParameterSet.Config as cms

from TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi import *

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

higgsMassPoints = [ '90', '100', '130', '160', '200', '250', '350' ]

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
#outputFilePath = "/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/export_AHtoMuTau_TaNCmedium_wSys"
outputFilePath = "/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/export_AHtoMuTau_TaNCloose_woSys"

channels = {
    'AHtoMuTau_woBtag' : {
        'shortName'                : "ma",
        'dqmDirectoryTemplate'     : '/export/harvested/%s/ahMuTauAnalyzerOS_woBtag/afterEvtSelDiTauCandidateForAHtoMuTauZeroCharge/',
        'dqmDirectoryFilterStat'   : '/export/harvested/%s/ahMuTauAnalyzerOS_woBtag/FilterStatistics/',
        'meNameTemplate'           : 'DiTauCandidateSVfitQuantities/psKine_MEt_ptBalance/Mass',
        'meNameNumEventsProcessed' : 'genPhaseSpaceCut/passed_cumulative_numWeighted#a1#s1',
        'meNameNumEventsPassed'    : 'evtSelDiTauCandidateForAHtoMuTauZeroCharge/passed_cumulative_numWeighted#a1#s1',
        ##'meNameNumEventsPassed'    : 'binContent_region1#a1#s1',
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
            #'/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_TaNCmedium_wSys_skimmed_part01.root',
            #'/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_TaNCmedium_wSys_skimmed_part02.root'
            '/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_TaNCloose_woSys_skimmed_part01.root',
            '/data1/veelken/CMSSW_3_8_x/plots/MSSM_Higgs_combined/plotsAHtoMuTau_TaNCloose_woSys_skimmed_part02.root'
        ]
    },
    'AHtoMuTau_wBtag' : {
        'shortName'                : "mab",
        'dqmDirectoryTemplate'     : '/export/harvested/%s/ahMuTauAnalyzerOS_wBtag/afterEvtSelDiTauCandidateForAHtoMuTauZeroCharge/',
        'dqmDirectoryFilterStat'   : '/export/harvested/%s/ahMuTauAnalyzerOS_wBtag/FilterStatistics/',
        'meNameTemplate'           : 'DiTauCandidateSVfitQuantities/psKine_MEt_ptBalance/MassL',
        'meNameNumEventsProcessed' : 'genPhaseSpaceCut/passed_cumulative_numWeighted#a1#s1',
        'meNameNumEventsPassed'    : 'evtSelDiTauCandidateForAHtoMuTauZeroCharge/passed_cumulative_numWeighted#a1#s1',
        ##'meNameNumEventsPassed'    : 'binContent_region1#a1#s1',
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
            'TTplusJets' : 'TpsetProcessesTplusJets',
            'data'       : 'data'
        },
        'inputFileNames' : [
            # CV: no files need to be loaded, as wBtag histograms are contained in same .root file as woBtag histograms
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
    'Ztautau' : {
        'xSection'       : ZtoMuTau.RECO_SAMPLES['ZtautauPU156bx']['x_sec']
                          + ZtoMuTau.RECO_SAMPLES['qqZtautauPU156bx']['x_sec'],
        'outputFileName' : "ztt_#CHANNEL_OUTPUTFILENAME#.hst"        
    },
    'Zmumu' : {
        'xSection'       : ZtoMuTau.RECO_SAMPLES['Zmumu']['x_sec'],
        'outputFileName' : "zmm_#CHANNEL_OUTPUTFILENAME#.hst"
    },
    'Zee' : {
        'xSection'       : ZtoMuTau.RECO_SAMPLES['Zee']['x_sec'],
        'outputFileName' : "zee_#CHANNEL_OUTPUTFILENAME#.hst"
    },
    'QCD' : {
        'outputFileName' : "qcd_#CHANNEL_OUTPUTFILENAME#.hst"
    },
    'gammaPlusJets' : {
        'outputFileName' : "gammajets_#CHANNEL_OUTPUTFILENAME#.hst"
    },
    'WplusJets' : {
        'xSection'       : ##ZtoMuTau.RECO_SAMPLES['Wenu']['x_sec']
                           ZtoMuTau.RECO_SAMPLES['Wmunu']['x_sec']
                          + ZtoMuTau.RECO_SAMPLES['Wtaunu']['x_sec'],
        'outputFileName' : "wjets_#CHANNEL_OUTPUTFILENAME#.hst"
    },
    'TTplusJets' : {
        'xSection'       : ZtoMuTau.RECO_SAMPLES['TTplusJets']['x_sec'],
        'outputFileName' : "ttbar_#CHANNEL_OUTPUTFILENAME#.hst"
    },
    'data' : {
        'outputFileName' : "data_#CHANNEL_OUTPUTFILENAME#.hst"
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
        pset = cms.PSet(
            meName_input = cms.string(
                (channels['AHtoMuTau_woBtag']['dqmDirectoryFilterStat']
               + channels['AHtoMuTau_woBtag']['meNameNumEventsPassed']) % ("A%sSum" % higgsMassPoint)
            ),
            meName_output = cms.string(
                (channels['AHtoElecTau']['dqmDirectoryFilterStat']
               + channels['AHtoElecTau']['meNameNumEventsPassed']) % ("A%sSum" % higgsMassPoint)
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
        pset = cms.PSet(
            meName_input = cms.string(
                (channels['AHtoMuTau_woBtag']['dqmDirectoryFilterStat']
               + channels['AHtoMuTau_woBtag']['meNameNumEventsPassed']) % ("A%sSum" % higgsMassPoint)
            ),
            meName_output = cms.string(
                (channels['AHtoElecTau']['dqmDirectoryFilterStat']
               + channels['AHtoElecTau']['meNameNumEventsPassed']) % ("A%sSum" % higgsMassPoint)
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
# set configuration parameters of DQMExportAnalysisResults module
#--------------------------------------------------------------------------------

psetsChannels = []
for channelName in channels.keys():
    if channelName in channelsToExport:
        pset = cms.PSet(
            name = cms.string(channelName),
            shortName = cms.string(channels[channelName]['shortName']),
            binning = cms.string(
                (channels[channelName]['dqmDirectoryTemplate']
               + channels[channelName]['meNameTemplate']) % (channels[channelName]['processes']['Ztautau'])
            ),
            dataIntLumi = cms.double(channels[channelName]['dataIntLumi'])
        )
        psetsChannels.append(pset)

psetProcesses = cms.PSet()
for processName in processesToExport:
    psetProcess = cms.PSet(
        distributions = cms.PSet(),
        outputFilePath = cms.string(""),
        outputFileName = cms.string(processes[processName]['outputFileName'])
    )
    if processes[processName].get('xSection') is not None:
        setattr(psetProcess, "xSection", cms.double(processes[processName]['xSection']))
    for channelName in channels.keys():
        if channelName in channelsToExport:
            if channels[channelName]['processes'].get(processName) is not None:
                psetChannel = cms.PSet(
                    template = cms.string(
                        (channels[channelName]['dqmDirectoryTemplate']
                       ##+ 'sysUncertaintyHistManagerResults/#SYSTEMATICSDIR#/'
                       + channels[channelName]['meNameTemplate']) % (channels[channelName]['processes'][processName])
                    ),
                    normalization = cms.PSet(
                        numEventsProcessed = cms.string(
                            (channels[channelName]['dqmDirectoryFilterStat'] 
                           + channels[channelName]['meNameNumEventsProcessed']) % (channels[channelName]['processes'][processName])
                        ),
                        numEventsPassed = cms.string(
                            (channels[channelName]['dqmDirectoryFilterStat']
                           + channels[channelName]['meNameNumEventsPassed']) % (channels[channelName]['processes'][processName])
                           ## (channels[channelName]['dqmDirectoryTemplate']
                           ##+ '/sysUncertaintyBinningResults/dataBinningResults/#SYSTEMATICSDIR#/'
                           ##+ channels[channelName]['meNameNumEventsPassed']) % (channels[channelName]['processes'][processName])
                        )
                    )
                )
                if channels[channelName]['doExportSystematics']:
                    setattr(psetChannel, "systematics", cms.vstring(channels[channelName]['systematics']))                
                setattr(psetProcess.distributions, channelName, psetChannel)
    setattr(psetProcesses, processName, psetProcess)
 
# add template shapes for Higgs mass-points
# CV: take all Higgs templates from AHtoMuTau channel for now;
#     adjust normalization by AHtoElecTau/AHtoMuTau (AHtoElecMu/AHtoMuTau) Ztautau efficiency ratio
for higgsMassPoint in higgsMassPoints:
    psetHiggsMassPoint = cms.PSet(
        distributions = cms.PSet(),
        outputFilePath = cms.string("m%s" % higgsMassPoint),
        outputFileName = cms.string("A_#CHANNEL_OUTPUTFILENAME#.hst")
    )
    xSection_gg = AHtoMuTauSpecific_RECO_SAMPLES['A%s' % higgsMassPoint]['x_sec']
    xSection_bb = AHtoMuTauSpecific_RECO_SAMPLES['bbA%s' % higgsMassPoint]['x_sec']
    setattr(psetHiggsMassPoint, "xSection", cms.double(xSection_gg + xSection_bb))
    for channelName in channels.keys():
        if channelName in channelsToExport:
            psetChannel = cms.PSet(
                template = cms.string(
                    (channels['AHtoMuTau_woBtag']['dqmDirectoryTemplate']
                   ##+ 'sysUncertaintyHistManagerResults/#SYSTEMATICSDIR#/'
                   + channels['AHtoMuTau_woBtag']['meNameTemplate']) % ('A%sSum' % higgsMassPoint)
                ),
                normalization = cms.PSet(
                    numEventsProcessed = cms.string(
                         (channels[channelName]['dqmDirectoryFilterStat'] 
                        + channels[channelName]['meNameNumEventsProcessed']) % ('A%sSum' % higgsMassPoint)
                    ),
                    numEventsPassed = cms.string(
                         (channels[channelName]['dqmDirectoryFilterStat']
                        + channels[channelName]['meNameNumEventsPassed']) % ('A%sSum' % higgsMassPoint)
                        ## (channels[channelName]['dqmDirectoryTemplate']
                        ##+ '/sysUncertaintyBinningResults/dataBinningResults/#SYSTEMATICSDIR#/'
                        ##+ channels[channelName]['meNameNumEventsPassed']) % ('A%sSum' % higgsMassPoint)
                    )
                )
            )
            if channels[channelName]['doExportSystematics']:
                setattr(psetChannel, "systematics", cms.vstring(channels[channelName]['systematics']))                
            setattr(psetHiggsMassPoint.distributions, channelName, psetChannel)
    setattr(psetProcesses, "A%s" % higgsMassPoint, psetHiggsMassPoint)
    
process.exportAnalysisResults = cms.EDAnalyzer("DQMExportAnalysisResults",

    channels = cms.VPSet(psetsChannels),

    outputFilePath = cms.string(outputFilePath),

    processes = psetProcesses,

    systematics = cms.PSet(
    	elecEnUp = cms.PSet(
    	      dqmDirectory = cms.string('sysElecEnUp'),
     	      outputFilePath = cms.string("mu_pt+1")
        ),
    	elecEnDown = cms.PSet(
    	      dqmDirectory = cms.string('sysElecEnDown'),
    	      outputFilePath = cms.string("mu_pt-1")
        ),
    	muonPtUp = cms.PSet(
    	      dqmDirectory = cms.string('sysMuonPtUp'),
     	      outputFilePath = cms.string("mu_pt+1")
        ),
    	muonPtDown = cms.PSet(
    	      dqmDirectory = cms.string('sysMuonPtDown'),
    	      outputFilePath = cms.string("mu_pt-1")
        ),                                                  
    	tauJetEnUp = cms.PSet(
    	      dqmDirectory = cms.string('sysTauJetEnUp'),
     	      outputFilePath = cms.string("tau_es+1")
        ),
    	tauJetEnDown = cms.PSet(
    	      dqmDirectory = cms.string('sysTauJetEnDown'),
    	      outputFilePath = cms.string("tau_es-1")
        ),
    	jetEnUp = cms.PSet(
    	      dqmDirectory = cms.string('sysJetEnUp'),
     	      outputFilePath = cms.string("jet_es+1")
        ),
    	jetEnDown = cms.PSet(
    	      dqmDirectory = cms.string('sysJetEnDown'),
    	      outputFilePath = cms.string("jet_es-1")
        ),
        ZllRecoilCorrectionUp = cms.PSet(
     	      dqmDirectory = cms.string('sysZllRecoilCorrectionUp'),
    	      outputFilePath = cms.string("met_recoil+1")
        ),
        ZllRecoilCorrectionDown = cms.PSet(
     	      dqmDirectory = cms.string('sysZllRecoilCorrectionDown'),
    	      outputFilePath = cms.string("met_recoil-1")
        )
    )
)

delattr(process.exportAnalysisResults, "systematics")

process.exportAnalysisSequence += process.exportAnalysisResults

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
