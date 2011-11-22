#!/usr/bin/env python

from TauAnalysis.TauIdEfficiency.tools.buildConfigFilesTauIdEffAnalysis import buildConfigFile_hadd
from TauAnalysis.RecoTools.tools.buildConfigFilesZllRecoilCorrectionAnalysis import *
from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_7TeV_grid_cfi import RECO_SAMPLES

import os

version = 'v4_3'

inputFilePath = '/data1/veelken/CMSSW_4_2_x/PATtuples/ZllRecoilCorrection/%s/' % version \
               + 'user/v/veelken/CMSSW_4_2_x/PATtuples/ZllRecoilCorrection/%s/' % version
outputFilePath = '/data1/veelken/tmp/ZllRecoilCorrection/%s_2' % version

samplesToAnalyze = {
    'Data_2011RunA' : {
        'samples' : [
            'Data_runs160329to163869',
            'Data_runs165071to167913',
            'Data_runs170053to172619',
            'Data_runs172620to175770'
        ],
        'isMC' : False
    },
    'Data_2011RunB' : {
        'samples' : [
            'Data_runs175832to180252'
        ],
        'isMC' : False
    },
    'simDYtoMuMu' : {
        'samples' : [
            'simDYtoMuMu'
        ],
        'allEvents_DBS' : RECO_SAMPLES['Zmumu_powheg']['events_processed'],
        'xSection' : RECO_SAMPLES['Zmumu_powheg']['x_sec'],
        'isMC' : True,
        'Type' : 'Signal',
        'applyRhoNeutralReweighting' : True,
        'scaleFactor' : 1. # 1.8/7.64
    },
    'simWW' : {
       'samples' : [
           'simWW'
       ],
       'allEvents_DBS' : RECO_SAMPLES['WW']['events_processed'],
       'xSection' : RECO_SAMPLES['WW']['x_sec'],
       'isMC' : True,
       'Type' : 'Background',
       'applyRhoNeutralReweighting' : False,
       'scaleFactor' : 1. # 1./334.4
    },
    'simWZ' : {
       'samples' : [
           'simWZ'
       ],
       'allEvents_DBS' : RECO_SAMPLES['WZ']['events_processed'],
       'xSection' : RECO_SAMPLES['WZ']['x_sec'],
       'isMC' : True,
       'Type' : 'Background',
       'applyRhoNeutralReweighting' : False,
        'scaleFactor' : 1. # 1./58.8
    },
    'simZZ' : {
       'samples' : [
           'simZZ'
       ],
       'allEvents_DBS' : RECO_SAMPLES['ZZ']['events_processed'],
       'xSection' : RECO_SAMPLES['ZZ']['x_sec'],
       'isMC' : True,
       'Type' : 'Background',
       'applyRhoNeutralReweighting' : False,
        'scaleFactor' : 1. # 1./29.7
    },
    'simTTplusJets' : {
        'samples' : [
            'simTTplusJets'
        ],
        'allEvents_DBS' : RECO_SAMPLES['TTplusJets_madgraph']['events_processed'],
        'xSection' : RECO_SAMPLES['TTplusJets_madgraph']['x_sec'],
        'isMC' : True,
        'Type' : 'Background',
        'applyRhoNeutralReweighting' : False,
        'scaleFactor' : 1. # 1.2/169.8
    },
    'simQCD' : {
        'samples' : [
            'simQCD'
        ],
        'allEvents_DBS' : RECO_SAMPLES['PPmuXptGt20Mu15']['events_processed'],
        'xSection' : RECO_SAMPLES['PPmuXptGt20Mu15']['x_sec'],
        'isMC' : True,
        'Type' : 'Background',
        'applyRhoNeutralReweighting' : False,
        'scaleFactor' : 1. # 1./7774.6
    }
}

runPeriod = '2011RunA'
#runPeriod = '2011RunB'

intLumiData = None
hltPaths = None
srcWeights = None
if runPeriod == '2011RunA':
    samplesToAnalyze['Data'] = samplesToAnalyze['Data_2011RunA']
    intLumiData = 1522.7 # runs 160431-173198
    hltPaths = {
        'Data' : [
            'HLT_DoubleMu7_v1',
            'HLT_DoubleMu7_v2',
            'HLT_Mu13_Mu8_v2',
            'HLT_Mu13_Mu8_v4',
            'HLT_Mu13_Mu8_v6',
            'HLT_Mu13_Mu8_v7'
        ],
        'smMC' : [
            'HLT_DoubleMu7_v1'
        ]
    }
    srcWeights = {
        'Data' : [],
        'smMC' : [ 'vertexMultiplicityReweight3dRunA' ]
    }
elif runPeriod == '2011RunB':
    samplesToAnalyze['Data'] = samplesToAnalyze['Data_2011RunB']
    intLumiData = 2027.9 # runs 175860-179411 
    hltPaths = {
        'Data' : [
            'HLT_Mu17_Mu8_v7',
            'HLT_Mu17_Mu8_v10',
            'HLT_Mu17_Mu8_v11'
        ],
        'smMC' : [
            'HLT_DoubleMu7_v1'
        ]
    }
    srcWeights = {
        'Data' : [],
        'smMC' : [ 'vertexMultiplicityReweight3dRunB' ]
    }
else:
    raise ValueError("Invalid runPeriod = %s !!" % runPeriod)

for sampleName in samplesToAnalyze.keys():
    if not samplesToAnalyze[sampleName]['isMC'] and sampleName != 'Data':
        samplesToAnalyze.pop(sampleName)
        
#print samplesToAnalyze

metOptions = {
    'pfMEt' : {
        'srcJets' : {
            'Data' : {
                'central'    : 'patJets'
            },
            'smMC' : {
                'central'    : 'patJets'
            }
        },
        'srcMEt' : {
            'Data' : {
                'central'    : 'patPFMet'
            },
            'smMC' : {
                'central'    : 'patPFMetNoSmearing'
            }
        }
    },
    'pfMEtTypeIcorrected' : {
        'srcJets' : {
            'Data' : {
                'central'    : 'patJets'
            },
            'smMC' : {
                'central'    : 'patJets'
            }
        },
        'srcMEt' : {
            'Data' : {
                'central'    : 'patType1CorrectedPFMet'
            },
            'smMC' : {
                'central'    : 'patType1CorrectedPFMetNoSmearing'
            }
        }
    },
    'pfMEtTypeIpIIcorrected' : {
        'srcJets' : {
            'Data' : {
                'central'    : 'patJets'
            },
            'smMC' : {
                'central'    : 'patJets'
            }
        },
        'srcMEt'  : {
            'Data' : {
                'central'    : 'patType1p2CorrectedPFMet'
            },
            'smMC' : {
                'central'    : 'patType1p2CorrectedPFMetNoSmearing'
            }
        }
    },
    'pfMEtSmeared' : {
        'srcJets' : {
            'Data' : {
                'central'    : 'patJets'
            },
            'smMC' : {
                'central'    : 'smearedPatJets',
                'jetEnUp'    : 'smearedPatJets',
                'jetEnDown'  : 'smearedPatJets',
                'jetResUp'   : 'smearedPatJets',
                'jetResDown' : 'smearedPatJets',
                ##'unclEnUp'   : 'smearedPatJets',
                ##'unclEnDown' : 'smearedPatJets'
            }
        },
        'srcMEt' : {
            'Data' : {
                'central'    : 'patPFMet'
            },
            'smMC' : {
                'central'    : 'patPFMet',
                'jetEnUp'    : 'patPFMetJetEnUp',
                'jetEnDown'  : 'patPFMetJetEnDown',
                'jetResUp'   : 'patPFMetJetResUp',
                'jetResDown' : 'patPFMetJetResDown',
                ##'unclEnUp'   : 'patPFMetUnclusteredEnUp',
                ##'unclEnDown' : 'patPFMetUnclusteredEnDown'
            }
        }
    },
    'pfMEtTypeIcorrectedSmeared' : {
        'srcJets' : {
            'Data' : {
                'central'    : 'patJets'
            },
            'smMC' : {
                'central'    : 'smearedPatJets',
                'jetEnUp'    : 'smearedPatJets',
                'jetEnDown'  : 'smearedPatJets',
                'jetResUp'   : 'smearedPatJets',
                'jetResDown' : 'smearedPatJets',
                ##'unclEnUp'   : 'smearedPatJets',
                ##'unclEnDown' : 'smearedPatJets'
            }
        },
        'srcMEt' : {
            'Data' : {
                'central'    : 'patType1CorrectedPFMet'
            },
            'smMC' : {
                'central'    : 'patType1CorrectedPFMet',
                'jetEnUp'    : 'patType1CorrectedPFMetJetEnUp',
                'jetEnDown'  : 'patType1CorrectedPFMetJetEnDown',
                'jetResUp'   : 'patType1CorrectedPFMetJetResUp',
                'jetResDown' : 'patType1CorrectedPFMetJetResDown',
                ##'unclEnUp'   : 'patType1CorrectedPFMetUnclusteredEnUp',
                ##'unclEnDown' : 'patType1CorrectedPFMetUnclusteredEnDown'
            }
        }
    },
    'pfMEtTypeIpIIcorrectedSmeared' : {
        'srcJets' : {
            'Data' : {
                'central'    : 'patJets'
            },                
            'smMC' : {
                'central'    : 'smearedPatJets',
                'jetEnUp'    : 'smearedPatJets',
                'jetEnDown'  : 'smearedPatJets',
                'jetResUp'   : 'smearedPatJets',
                'jetResDown' : 'smearedPatJets',
                ##'unclEnUp'   : 'smearedPatJets',
                ##'unclEnDown' : 'smearedPatJets'
            }
        },    
        'srcMEt' : {
            'Data' : {
                'central'    : 'patType1p2CorrectedPFMet'
            },
            'smMC' : {
                'central'    : 'patType1p2CorrectedPFMet',
                'jetEnUp'    : 'patType1p2CorrectedPFMetJetEnUp',
                'jetEnDown'  : 'patType1p2CorrectedPFMetJetEnDown',
                'jetResUp'   : 'patType1p2CorrectedPFMetJetResUp',
                'jetResDown' : 'patType1p2CorrectedPFMetJetResDown',
                ##'unclEnUp'   : 'patType1p2CorrectedPFMetUnclusteredEnUp',
                ##'unclEnDown' : 'patType1p2CorrectedPFMetUnclusteredEnDown'
            }
        }
    }
}

execDir = "%s/bin/%s/" % (os.environ['CMSSW_BASE'], os.environ['SCRAM_ARCH'])

executable_produceZllRecoilNtuples           = execDir + 'FWLiteZllRecoilCorrectionNtupleProducer'
executable_fitZllRecoilNtuples               = execDir + 'fitZllRecoilCorrection'
executable_FWLiteZllRecoilCorrectionAnalyzer = execDir + 'FWLiteZllRecoilCorrectionAnalyzer'
executable_hadd                              = 'hadd -f'
executable_makeZllRecoilCorrectionFinalPlots = execDir + 'makeZllRecoilCorrectionFinalPlots'
executable_shell                             = '/bin/csh'
executable_python                            = 'python'

nice = 'nice '

if not os.path.exists(outputFilePath):
    os.mkdir(outputFilePath)
    
outputFilePath = os.path.join(outputFilePath, runPeriod)
if not os.path.exists(outputFilePath):
    os.mkdir(outputFilePath)

#--------------------------------------------------------------------------------
#
# build config files for producing "plain" ROOT Ntuples
# needed for fitting Z-recoil correction parameters
#
fileNames_produceZllRecoilNtuples = {}
for metOptionName in metOptions.keys():
    fileNames_produceZllRecoilNtuples[metOptionName] = {}
    for sampleName in samplesToAnalyze.keys():
        if not (samplesToAnalyze[sampleName]['isMC'] and samplesToAnalyze[sampleName]['Type'] == 'Background'):
            processType = None
            if samplesToAnalyze[sampleName]['isMC']:
                processType = 'smMC'
            else:
                processType = 'Data'
            srcJets = metOptions[metOptionName]['srcJets'][processType]
            srcMEt = metOptions[metOptionName]['srcMEt'][processType]
            fileNames_produceZllRecoilNtuples[metOptionName][sampleName] = {}
            for central_or_shift in srcMEt.keys():
                retVal_produceZllRecoilNtuples = \
                  buildConfigFile_produceZllRecoilNtuples(
                    sampleName, metOptionName, inputFilePath, outputFilePath, samplesToAnalyze,
                    central_or_shift, srcMEt[central_or_shift], srcJets[central_or_shift], hltPaths[processType], srcWeights[processType])

                if retVal_produceZllRecoilNtuples is None:
                    continue

                fileNames_produceZllRecoilNtuples[metOptionName][sampleName][central_or_shift] = \
                  retVal_produceZllRecoilNtuples
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#
# build config files for fitting Z-recoil correction parameters 
#
fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp = {}
for metOptionName in metOptions.keys():
    fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName] = {}
    for sampleName in samplesToAnalyze.keys():
        if not (samplesToAnalyze[sampleName]['isMC'] and samplesToAnalyze[sampleName]['Type'] == 'Background'):
            processType = None
            if samplesToAnalyze[sampleName]['isMC']:
                processType = 'smMC'
            else:
                processType = 'Data'
            fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleName] = {}            
            for central_or_shift in metOptions[metOptionName]['srcMEt'][processType].keys():
                retVal_fitZllRecoilNtuples = \
                  buildConfigFile_fitZllRecoilNtuples(
                    sampleName, metOptionName,
                    fileNames_produceZllRecoilNtuples[metOptionName][sampleName][central_or_shift]['outputFileName'], outputFilePath,
                    samplesToAnalyze, central_or_shift, 
                    "qT", "uParl", "uPerp")

                if retVal_fitZllRecoilNtuples is None:
                    continue

                fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleName][central_or_shift] = \
                  retVal_fitZllRecoilNtuples
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#
# build config files for running FWLiteZllRecoilCorrectionAnalyzer 
#
fileNames_FWLiteZllRecoilCorrectionAnalyzer = {}
for metOptionName in metOptions.keys():
    fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName] = {}
    for sampleName in samplesToAnalyze.keys():
        processType = None
        if samplesToAnalyze[sampleName]['isMC']:
            processType = 'smMC'
        else:
            processType = 'Data'
        srcJets = metOptions[metOptionName]['srcJets'][processType]
        srcMEt = metOptions[metOptionName]['srcMEt'][processType]
        fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName] = {}
        for central_or_shift in srcMEt.keys():
            retVal_FWLiteZllRecoilCorrectionAnalyzer = \
              buildConfigFile_FWLiteZllRecoilCorrectionAnalyzer(
                sampleName, metOptionName, inputFilePath, outputFilePath, samplesToAnalyze,
                central_or_shift, srcMEt[central_or_shift], srcJets[central_or_shift], hltPaths[processType], srcWeights[processType],
                None, intLumiData)

            if retVal_FWLiteZllRecoilCorrectionAnalyzer is None:
                continue

            fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName][central_or_shift] = \
              retVal_FWLiteZllRecoilCorrectionAnalyzer
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#
# build shell script for running 'hadd' in order to "harvest" histograms
# produced by FWLiteZllRecoilCorrectionAnalyzer macro
#
fileNames_hadd = {}
for metOptionName in metOptions.keys():
    haddShellFileName  = os.path.join(outputFilePath, 'harvestZllRecoilCorrectionHistograms_%s.csh' % metOptionName)
    haddInputFileNames = []
    for sampleName in samplesToAnalyze.keys():
        if fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName].has_key(sampleName):
            for central_or_shift in metOptions[metOptionName]['srcJets']['smMC'].keys():
                if fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName].has_key(central_or_shift):
                    haddInputFileNames.append(
                      fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName][central_or_shift]['outputFileName'])
    haddOutputFileName = os.path.join(outputFilePath, 'analyzeZllRecoilCorrectionHistograms_all_%s.root' % metOptionName)

    retVal_hadd = \
      buildConfigFile_hadd(executable_hadd, haddShellFileName, haddInputFileNames, haddOutputFileName)

    fileNames_hadd[metOptionName] = {}
    fileNames_hadd[metOptionName]['shellFileName']  = haddShellFileName
    fileNames_hadd[metOptionName]['inputFileNames'] = haddInputFileNames
    fileNames_hadd[metOptionName]['outputFileName'] = haddOutputFileName
    fileNames_hadd[metOptionName]['logFileName']    = retVal_hadd['logFileName']
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#
# build config files for making final plots
#
sampleNameData           = None
sampleNameMC_signal      = None
sampleNameMCs_background = []
for sampleName in samplesToAnalyze.keys():
    if samplesToAnalyze[sampleName]['isMC']:
        if samplesToAnalyze[sampleName]['Type'] == 'Signal':
            if sampleNameMC_signal is not None:
                raise ValueError("'MC' sample must be defined only once !!")
            sampleNameMC_signal = sampleName
        elif samplesToAnalyze[sampleName]['Type'] == 'Background':
            sampleNameMCs_background.append(sampleName)
        else:
            raise ValueError("Invalid MC type = %s !!" % samplesToAnalyze[sampleName]['Type'])
    else:
        if sampleNameData is not None:
            raise ValueError("'Data' sample must be defined only once !!")
        sampleNameData = sampleName
        
corrLevelsMC = [
    'beforeGenPUreweight',
    'beforeAddPUreweight',
    'beforeZllRecoilCorr',
    'afterZllRecoilMCtoDataCorr',
    'afterZllRecoilAbsCalib',
]

fileNames_makeZllRecoilCorrectionFinalPlots = {}
for metOptionName in metOptions.keys():
    fileNames_makeZllRecoilCorrectionFinalPlots[metOptionName] = {}
    for corrLevelMC in corrLevelsMC:
        retVal_makeZllRecoilCorrectionFinalPlots = \
          buildConfigFile_makeZllRecoilCorrectionFinalPlots(
            sampleNameData, sampleNameMC_signal, sampleNameMCs_background, runPeriod, metOptionName,
            fileNames_hadd[metOptionName]['outputFileName'], outputFilePath, samplesToAnalyze, corrLevelMC,
            metOptions[metOptionName]['srcJets']['smMC'].keys())
            
        fileNames_makeZllRecoilCorrectionFinalPlots[metOptionName][corrLevelMC] = retVal_makeZllRecoilCorrectionFinalPlots
#--------------------------------------------------------------------------------

def make_MakeFile_vstring(list_of_strings):
    retVal = ""
    for i, string_i in enumerate(list_of_strings):
        if i > 0:
            retVal += " "
        retVal += string_i
    return retVal

# done building config files, now build Makefile...
makeFileName = "Makefile_ZllRecoilCorrectionAnalysis_%s_%s" % (version, runPeriod)
makeFile = open(makeFileName, "w")
makeFile.write("\n")
outputFileNames_makeZllRecoilCorrectionFinalPlots_and_Numbers = []
for metOptionName in metOptions.keys():
    for corrLevelMC in corrLevelsMC:
        outputFileNames_makeZllRecoilCorrectionFinalPlots_and_Numbers.append(
          fileNames_makeZllRecoilCorrectionFinalPlots[metOptionName][corrLevelMC]['outputFileName'])
    for sampleName in samplesToAnalyze.keys():
        if not (samplesToAnalyze[sampleName]['isMC'] and samplesToAnalyze[sampleName]['Type'] == 'Background'):
            if fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName].has_key(sampleName):
                for central_or_shift in metOptions[metOptionName]['srcMEt']['smMC'].keys():
                    if fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleName].has_key(central_or_shift):
                        outputFileNames_makeZllRecoilCorrectionFinalPlots_and_Numbers.append(
                          fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleName][central_or_shift]['outputFileName'])
makeFile.write("all: %s\n" % make_MakeFile_vstring(outputFileNames_makeZllRecoilCorrectionFinalPlots_and_Numbers))
makeFile.write("\techo 'Finished running ZllRecoilCorrectionAnalysis.'\n")
makeFile.write("\n")
for metOptionName in metOptions.keys():
    for sampleName in samplesToAnalyze.keys():
        if not (samplesToAnalyze[sampleName]['isMC'] and samplesToAnalyze[sampleName]['Type'] == 'Background'):
            if fileNames_produceZllRecoilNtuples[metOptionName].has_key(sampleName):
                for central_or_shift in metOptions[metOptionName]['srcJets']['smMC'].keys():
                    if fileNames_produceZllRecoilNtuples[metOptionName][sampleName].has_key(central_or_shift):
                        makeFile.write("%s: %s\n" %
                          (fileNames_produceZllRecoilNtuples[metOptionName][sampleName][central_or_shift]['outputFileName'],
                           #executable_produceZllRecoilNtuples,
                           ""))
                        makeFile.write("\t%s%s %s &> %s\n" %
                          (nice, executable_produceZllRecoilNtuples,
                           fileNames_produceZllRecoilNtuples[metOptionName][sampleName][central_or_shift]['configFileName'],
                           fileNames_produceZllRecoilNtuples[metOptionName][sampleName][central_or_shift]['logFileName']))
makeFile.write("\n")
for metOptionName in metOptions.keys():
    for sampleName in samplesToAnalyze.keys():
        if not (samplesToAnalyze[sampleName]['isMC'] and samplesToAnalyze[sampleName]['Type'] == 'Background'):
            if fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName].has_key(sampleName) and \
               fileNames_produceZllRecoilNtuples[metOptionName].has_key(sampleName):
                for central_or_shift in metOptions[metOptionName]['srcJets']['smMC'].keys():
                    if fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleName].has_key(central_or_shift) and \
                       fileNames_produceZllRecoilNtuples[metOptionName][sampleName].has_key(central_or_shift):
                        makeFile.write("%s: %s %s\n" %
                          (fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleName][central_or_shift]['outputFileName'],
                           fileNames_produceZllRecoilNtuples[metOptionName][sampleName][central_or_shift]['outputFileName'],
                           #executable_fitZllRecoilNtuples,
                           ""))
                        makeFile.write("\t%s%s %s &> %s\n" %
                          (nice, executable_fitZllRecoilNtuples,
                           fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleName][central_or_shift]['configFileName'],
                           fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleName][central_or_shift]['logFileName']))
makeFile.write("\n")
for metOptionName in metOptions.keys():
    for sampleName in samplesToAnalyze.keys():
        if fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName].has_key(sampleName) and \
           fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName].has_key(sampleName):
            for central_or_shift in metOptions[metOptionName]['srcJets']['smMC'].keys():
                if fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName].has_key(central_or_shift):
                    if central_or_shift == 'central':
                        processType = None
                        if samplesToAnalyze[sampleName]['isMC']:
                            processType = 'smMC'
                        else:
                            processType = 'Data'
                        srcJets = metOptions[metOptionName]['srcJets'][processType]
                        srcMEt = metOptions[metOptionName]['srcMEt'][processType]            
                        makeFile.write("%s: %s %s %s\n" %
                         (fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName][central_or_shift]['outputFileName'],
                          fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleNameData]['central']['outputFileName'],
                          fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleNameMC_signal][central_or_shift]['outputFileName'],
                          #executable_FWLiteZllRecoilCorrectionAnalyzer,
                          ""))
                        # rebuild config file to run FWLiteZllRecoilCorrectionAnalyzer macro
                        # using actual Z-recoil correction parameter values determind by fit
                        tmpConfig = \
"""
#!/usr/bin/env python

from TauAnalysis.RecoTools.tools.buildConfigFilesZllRecoilCorrectionAnalysis import *

samplesToAnalyze = %s

metOptions = %s

buildConfigFile_FWLiteZllRecoilCorrectionAnalyzer(
  '%s', '%s', '%s', '%s', samplesToAnalyze, '%s', '%s', '%s', %s, %s, { 'data' : '%s', 'mc' : '%s' }, %i)
""" % (str(samplesToAnalyze),
       str(metOptions),
       sampleName, metOptionName, inputFilePath, outputFilePath,
       central_or_shift, srcMEt[central_or_shift], srcJets[central_or_shift], hltPaths[processType], srcWeights[processType],
       fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleNameData]['central']['outputFileName'],
       fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleNameMC_signal][central_or_shift]['outputFileName'],
       intLumiData)      
                        tmpConfigFileName = "makeTMPconfigFile_%s_%s.py" % (sampleName, metOptionName)
                        tmpConfigFileName_full = os.path.join(outputFilePath, tmpConfigFileName)    
                        tmpConfigFile = open(tmpConfigFileName_full, "w")
                        tmpConfigFile.write(tmpConfig)
                        tmpConfigFile.close()            
                        makeFile.write("\t%s%s %s\n" %
                          (nice, executable_python, tmpConfigFileName_full))
                        makeFile.write("\t%s%s %s &> %s\n" %
                          (nice, executable_FWLiteZllRecoilCorrectionAnalyzer,
                           fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName][central_or_shift]['configFileName'],
                           fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName][central_or_shift]['logFileName']))
                    elif samplesToAnalyze[sampleName]['isMC'] and samplesToAnalyze[sampleName]['Type'] == 'Signal':
                        makeFile.write("%s: %s\n" %
                          (fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName][central_or_shift]['outputFileName'],
                           #executable_FWLiteZllRecoilCorrectionAnalyzer,
                           ""))
                        makeFile.write("\t%s%s %s &> %s\n" %
                          (nice, executable_FWLiteZllRecoilCorrectionAnalyzer,
                           fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName][central_or_shift]['configFileName'],
                           fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName][central_or_shift]['logFileName']))
        elif samplesToAnalyze[sampleName]['isMC'] and samplesToAnalyze[sampleName]['Type'] == 'Background':
            for central_or_shift in metOptions[metOptionName]['srcJets']['smMC'].keys():
                if fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName].has_key(central_or_shift):
                    makeFile.write("%s: %s\n" %
                      (fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName][central_or_shift]['outputFileName'],
                       #executable_FWLiteZllRecoilCorrectionAnalyzer,
                       ""))
                    makeFile.write("\t%s%s %s &> %s\n" %
                      (nice, executable_FWLiteZllRecoilCorrectionAnalyzer,
                       fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName][central_or_shift]['configFileName'],
                       fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName][central_or_shift]['logFileName']))
makeFile.write("\n")
for metOptionName in metOptions.keys():
    if len(fileNames_hadd[metOptionName]['inputFileNames']) > 0:
        makeFile.write("%s: %s\n" %
          (fileNames_hadd[metOptionName]['outputFileName'],
           make_MakeFile_vstring(fileNames_hadd[metOptionName]['inputFileNames'])))
        makeFile.write("\t%s%s %s &> %s\n" %
          (nice, executable_shell,
           fileNames_hadd[metOptionName]['shellFileName'],
           fileNames_hadd[metOptionName]['logFileName']))
makeFile.write("\n")
for metOptionName in metOptions.keys():
    for corrLevelMC in corrLevelsMC:
        makeFile.write("%s: %s %s\n" %
          (fileNames_makeZllRecoilCorrectionFinalPlots[metOptionName][corrLevelMC]['outputFileName'],
           fileNames_hadd[metOptionName]['outputFileName'],
           executable_makeZllRecoilCorrectionFinalPlots))
        makeFile.write("\t%s%s %s &> %s\n" %
          (nice, executable_makeZllRecoilCorrectionFinalPlots,
           fileNames_makeZllRecoilCorrectionFinalPlots[metOptionName][corrLevelMC]['configFileName'],
           fileNames_makeZllRecoilCorrectionFinalPlots[metOptionName][corrLevelMC]['logFileName']))
makeFile.write("\n")
makeFile.write(".PHONY: clean\n")
makeFile.write("clean:\n")
outputFileNames_produceZllRecoilNtuples = []
for metOptionName in metOptions.keys():
    for sampleName in samplesToAnalyze.keys():
        if not (samplesToAnalyze[sampleName]['isMC'] and samplesToAnalyze[sampleName]['Type'] == 'Background'):
            if fileNames_produceZllRecoilNtuples[metOptionName].has_key(sampleName):
                for central_or_shift in metOptions[metOptionName]['srcMEt']['smMC'].keys():
                    if fileNames_produceZllRecoilNtuples[metOptionName][sampleName].has_key(central_or_shift):
                        outputFileNames_produceZllRecoilNtuples.append(
                          fileNames_produceZllRecoilNtuples[metOptionName][sampleName][central_or_shift]['outputFileName'])
makeFile.write("\trm -f %s\n" % make_MakeFile_vstring(outputFileNames_produceZllRecoilNtuples))
outputFileNames_fitZllRecoilNtuples = []
for metOptionName in metOptions.keys():
    for sampleName in samplesToAnalyze.keys():
        if not (samplesToAnalyze[sampleName]['isMC'] and samplesToAnalyze[sampleName]['Type'] == 'Background'):
            if fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName].has_key(sampleName):
                for central_or_shift in metOptions[metOptionName]['srcMEt']['smMC'].keys():
                    if fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleName].has_key(central_or_shift):
                        outputFileNames_fitZllRecoilNtuples.append(
                          fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleName][central_or_shift]['outputFileName'])
makeFile.write("\trm -f %s\n" % make_MakeFile_vstring(outputFileNames_fitZllRecoilNtuples))
outputFileNames_FWLiteZllRecoilCorrectionAnalyzer = []
for metOptionName in metOptions.keys():
    for sampleName in samplesToAnalyze.keys():
        if fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName].has_key(sampleName):
            for central_or_shift in metOptions[metOptionName]['srcMEt']['smMC'].keys():
                if fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName].has_key(central_or_shift):
                    outputFileNames_FWLiteZllRecoilCorrectionAnalyzer.append(
                      fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName][central_or_shift]['outputFileName'])
makeFile.write("\trm -f %s\n" % make_MakeFile_vstring(outputFileNames_FWLiteZllRecoilCorrectionAnalyzer))
outputFileNames_hadd = []
for metOptionName in metOptions.keys():
    outputFileNames_hadd.append(
      fileNames_hadd[metOptionName]['outputFileName'])
makeFile.write("\trm -f %s\n" % make_MakeFile_vstring(outputFileNames_hadd))
outputFileNames_makeZllRecoilCorrectionFinalPlots = []
for metOptionName in metOptions.keys():
    for corrLevelMC in corrLevelsMC:
        outputFileNames_makeZllRecoilCorrectionFinalPlots.append(
          fileNames_makeZllRecoilCorrectionFinalPlots[metOptionName][corrLevelMC]['outputFileName'])
makeFile.write("\trm -f %s\n" % make_MakeFile_vstring(outputFileNames_makeZllRecoilCorrectionFinalPlots))
makeFile.write("\techo 'Finished deleting old files.'\n")
makeFile.write("\n")
makeFile.close()

print("Finished building Makefile. Now execute 'make -j 8 -f %s'." % makeFileName)
