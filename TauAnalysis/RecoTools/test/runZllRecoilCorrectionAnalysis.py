#!/usr/bin/env python

from TauAnalysis.TauIdEfficiency.tools.buildConfigFilesTauIdEffAnalysis import buildConfigFile_hadd
from TauAnalysis.RecoTools.tools.buildConfigFilesZllRecoilCorrectionAnalysis import *
from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_7TeV_grid_cfi import RECO_SAMPLES

import os

version = 'v3_6'

inputFilePath = '/data2/veelken/CMSSW_4_2_x/PATtuples/ZllRecoilCorrection/%s/' % version \
               + 'user/v/veelken/CMSSW_4_2_x/PATtuples/ZllRecoilCorrection/%s/' % version
outputFilePath = '/data1/veelken/tmp/ZllRecoilCorrection/%s' % version

samplesToAnalyze = {
    'Data' : {
        'samples' : [
            'Data_runs160329to163869',
            'Data_runs165071to167913'
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
        'applyRhoNeutralReweighting' : True
    },
    'simWW' : {
       'samples' : [
           'simWW'
       ],
       'allEvents_DBS' : RECO_SAMPLES['WW']['events_processed'],
       'xSection' : RECO_SAMPLES['WW']['x_sec'],
       'isMC' : True,
       'Type' : 'Background',
       'applyRhoNeutralReweighting' : False
    },
    'simWZ' : {
       'samples' : [
           'simWZ'
       ],
       'allEvents_DBS' : RECO_SAMPLES['WZ']['events_processed'],
       'xSection' : RECO_SAMPLES['WZ']['x_sec'],
       'isMC' : True,
       'Type' : 'Background',
       'applyRhoNeutralReweighting' : False
    },
    'simZZ' : {
       'samples' : [
           'simZZ'
       ],
       'allEvents_DBS' : RECO_SAMPLES['ZZ']['events_processed'],
       'xSection' : RECO_SAMPLES['ZZ']['x_sec'],
       'isMC' : True,
       'Type' : 'Background',
       'applyRhoNeutralReweighting' : False
    },
    'simTTplusJets' : {
        'samples' : [
            'simTTplusJets'
        ],
        'allEvents_DBS' : RECO_SAMPLES['TTplusJets_madgraph']['events_processed'],
        'xSection' : RECO_SAMPLES['TTplusJets_madgraph']['x_sec'],
        'isMC' : True,
        'Type' : 'Background',
        'applyRhoNeutralReweighting' : False
    },
    'simQCD' : {
        'samples' : [
            'simQCD'
        ],
        'allEvents_DBS' : RECO_SAMPLES['PPmuXptGt20Mu15']['events_processed'],
        'xSection' : RECO_SAMPLES['PPmuXptGt20Mu15']['x_sec'],
        'isMC' : True,
        'Type' : 'Background',
        'applyRhoNeutralReweighting' : False
    }
}

metOptions = {
    'pfMEt' : {
        'srcMEt' : 'patPFMETs'
    },
    'pfMEtTypeIcorrected' : {
        'srcMEt' : 'patPFMETsTypeIcorrected'
    },
    'pfMEtTypeIpIIcorrected' : {
        'srcMEt' : 'patPFMETsTypeIpIIcorrected'
    },
    'pfMEtSmeared' : {
        'srcMEt' : 'smearedPatPFMETs'
    },
    'pfMEtTypeIcorrectedSmeared' : {
        'srcMEt' : 'smearedPatPFMETsTypeIcorrected'
    },
    'pfMEtTypeIpIIcorrectedSmeared' : {
        'srcMEt' : 'smearedPatPFMETsTypeIpIIcorrected'
    }
}

intLumiData = 1130 # pb^-1 for runs 160404 - 167913 ("golden" quality EPS dataset)

execDir = "%s/bin/%s/" % (os.environ['CMSSW_BASE'], os.environ['SCRAM_ARCH'])

executable_produceZllRecoilNtuples           = execDir + 'FWLiteZllRecoilCorrectionNtupleProducer'
executable_fitZllRecoilNtuples               = execDir + 'fitZllRecoilCorrection'
executable_FWLiteZllRecoilCorrectionAnalyzer = execDir + 'FWLiteZllRecoilCorrectionAnalyzer'
executable_hadd                              = 'hadd -f'
executable_makeZllRecoilCorrectionFinalPlots = execDir + 'makeZllRecoilCorrectionFinalPlots'
executable_shell                             = '/bin/csh'
executable_python                            = 'python'

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
            retVal_produceZllRecoilNtuples = \
              buildConfigFile_produceZllRecoilNtuples(
                sampleName, metOptionName, inputFilePath, outputFilePath, samplesToAnalyze, metOptions)

            if retVal_produceZllRecoilNtuples is None:
                continue

            fileNames_produceZllRecoilNtuples[metOptionName][sampleName] = retVal_produceZllRecoilNtuples
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#
# build config files for fitting Z-recoil correction parameters 
#
fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp    = {}
for metOptionName in metOptions.keys():
    fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName] = {}
    for sampleName in samplesToAnalyze.keys():
        if not (samplesToAnalyze[sampleName]['isMC'] and samplesToAnalyze[sampleName]['Type'] == 'Background'):
            retVal_fitZllRecoilNtuples = \
              buildConfigFile_fitZllRecoilNtuples(
               sampleName, metOptionName,
                fileNames_produceZllRecoilNtuples[metOptionName][sampleName]['outputFileName'], outputFilePath, samplesToAnalyze,
                "qT", "uParl", "uPerp")

            if retVal_fitZllRecoilNtuples is None:
                continue

            fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleName] = retVal_fitZllRecoilNtuples

fileNames_fitZllRecoilNtuples_rT_vs_vParl_vPerp_ii = {}
for metOptionName in metOptions.keys():
    fileNames_fitZllRecoilNtuples_rT_vs_vParl_vPerp_ii[metOptionName] = {}
    for sampleName in samplesToAnalyze.keys():
        if not (samplesToAnalyze[sampleName]['isMC'] and samplesToAnalyze[sampleName]['Type'] == 'Background'):
            retVal_fitZllRecoilNtuples = \
              buildConfigFile_fitZllRecoilNtuples(
                sampleName, metOptionName,
                fileNames_produceZllRecoilNtuples[metOptionName][sampleName]['outputFileName'], outputFilePath, samplesToAnalyze,
                "rT_ii", "vParl_ii", "vPerp_ii")

            if retVal_fitZllRecoilNtuples is None:
                continue

            fileNames_fitZllRecoilNtuples_rT_vs_vParl_vPerp_ii[metOptionName][sampleName] = retVal_fitZllRecoilNtuples
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#
# build config files for running FWLiteZllRecoilCorrectionAnalyzer 
#
fileNames_FWLiteZllRecoilCorrectionAnalyzer = {}
for metOptionName in metOptions.keys():
    fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName] = {}
    for sampleName in samplesToAnalyze.keys():
        retVal_FWLiteZllRecoilCorrectionAnalyzer = \
          buildConfigFile_FWLiteZllRecoilCorrectionAnalyzer(
            sampleName, metOptionName, inputFilePath, outputFilePath, samplesToAnalyze, metOptions,
            None, intLumiData)

        if retVal_FWLiteZllRecoilCorrectionAnalyzer is None:
            continue

        fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName] = retVal_FWLiteZllRecoilCorrectionAnalyzer
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
            haddInputFileNames.append(fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName]['outputFileName'])
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
            sampleNameData, sampleNameMC_signal, sampleNameMCs_background, metOptionName,
            fileNames_hadd[metOptionName]['outputFileName'], outputFilePath, corrLevelMC)
            
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
makeFileName = "Makefile_ZllRecoilCorrectionAnalysis_%s" % version
makeFile = open(makeFileName, "w")
makeFile.write("\n")
outputFileNames_makeZllRecoilCorrectionFinalPlots = []
for metOptionName in metOptions.keys():
    for corrLevelMC in corrLevelsMC:
        outputFileNames_makeZllRecoilCorrectionFinalPlots.append(
          fileNames_makeZllRecoilCorrectionFinalPlots[metOptionName][corrLevelMC]['outputFileName'])
makeFile.write("all: %s\n" % make_MakeFile_vstring(outputFileNames_makeZllRecoilCorrectionFinalPlots))
makeFile.write("\techo 'Finished running ZllRecoilCorrectionAnalysis.'\n")
makeFile.write("\n")
for metOptionName in metOptions.keys():
    for sampleName in samplesToAnalyze.keys():
        if not (samplesToAnalyze[sampleName]['isMC'] and samplesToAnalyze[sampleName]['Type'] == 'Background'):
            if fileNames_produceZllRecoilNtuples[metOptionName].has_key(sampleName):
                makeFile.write("%s: %s\n" %
                  (fileNames_produceZllRecoilNtuples[metOptionName][sampleName]['outputFileName'],
                   executable_produceZllRecoilNtuples))
                makeFile.write("\t%s %s &> %s\n" %
                  (executable_produceZllRecoilNtuples,
                   fileNames_produceZllRecoilNtuples[metOptionName][sampleName]['configFileName'],
                   fileNames_produceZllRecoilNtuples[metOptionName][sampleName]['logFileName']))
makeFile.write("\n")
for metOptionName in metOptions.keys():
    for sampleName in samplesToAnalyze.keys():
        if not (samplesToAnalyze[sampleName]['isMC'] and samplesToAnalyze[sampleName]['Type'] == 'Background'):
            if fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName].has_key(sampleName) and \
               fileNames_produceZllRecoilNtuples[metOptionName].has_key(sampleName):
                makeFile.write("%s: %s %s\n" %
                  (fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleName]['outputFileName'],
                   fileNames_produceZllRecoilNtuples[metOptionName][sampleName]['outputFileName'],
                   executable_fitZllRecoilNtuples))
                makeFile.write("\t%s %s &> %s\n" %
                  (executable_fitZllRecoilNtuples,
                   fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleName]['configFileName'],
                   fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleName]['logFileName']))
            if fileNames_fitZllRecoilNtuples_rT_vs_vParl_vPerp_ii[metOptionName].has_key(sampleName) and \
               fileNames_produceZllRecoilNtuples[metOptionName].has_key(sampleName):
                makeFile.write("%s: %s %s\n" %
                  (fileNames_fitZllRecoilNtuples_rT_vs_vParl_vPerp_ii[metOptionName][sampleName]['outputFileName'],
                   fileNames_produceZllRecoilNtuples[metOptionName][sampleName]['outputFileName'],
                   executable_fitZllRecoilNtuples))
                makeFile.write("\t%s %s &> %s\n" %
                  (executable_fitZllRecoilNtuples,
                   fileNames_fitZllRecoilNtuples_rT_vs_vParl_vPerp_ii[metOptionName][sampleName]['configFileName'],
                   fileNames_fitZllRecoilNtuples_rT_vs_vParl_vPerp_ii[metOptionName][sampleName]['logFileName']))
makeFile.write("\n")
for metOptionName in metOptions.keys():
    for sampleName in samplesToAnalyze.keys():
        if fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName].has_key(sampleName) and \
           fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName].has_key(sampleName) and \
           fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName].has_key(sampleName):
            makeFile.write("%s: %s %s %s\n" %
              (fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName]['outputFileName'],
               fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleNameData]['outputFileName'],
               fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleNameMC_signal]['outputFileName'],
               executable_FWLiteZllRecoilCorrectionAnalyzer))
            # rebuild config file to run FWLiteZllRecoilCorrectionAnalyzer macro
            # using actual Z-recoil correction parameter values determind by fit
            tmpConfig = \
"""
#!/usr/bin/env python

from TauAnalysis.RecoTools.tools.buildConfigFilesZllRecoilCorrectionAnalysis import *

samplesToAnalyze = %s

metOptions = %s

buildConfigFile_FWLiteZllRecoilCorrectionAnalyzer(
  '%s', '%s', '%s', '%s', samplesToAnalyze, metOptions, { 'data' : '%s', 'mc' : '%s' }, %i)
""" % (str(samplesToAnalyze),
       str(metOptions),
       sampleName, metOptionName, inputFilePath, outputFilePath,
       fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleNameData]['outputFileName'],
       fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleNameMC_signal]['outputFileName'],
       intLumiData)      
            tmpConfigFileName = "makeTMPconfigFile_%s_%s.py" % (sampleName, metOptionName)
            tmpConfigFileName_full = os.path.join(outputFilePath, tmpConfigFileName)    
            tmpConfigFile = open(tmpConfigFileName_full, "w")
            tmpConfigFile.write(tmpConfig)
            tmpConfigFile.close()            
            makeFile.write("\t%s %s\n" % (executable_python, tmpConfigFileName_full))
            makeFile.write("\t%s %s &> %s\n" %
              (executable_FWLiteZllRecoilCorrectionAnalyzer,
               fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName]['configFileName'],
               fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName]['logFileName']))
        elif samplesToAnalyze[sampleName]['isMC'] and samplesToAnalyze[sampleName]['Type'] == 'Background':
             makeFile.write("%s: %s\n" %
              (fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName]['outputFileName'],
               executable_FWLiteZllRecoilCorrectionAnalyzer))
             makeFile.write("\t%s %s &> %s\n" %
              (executable_FWLiteZllRecoilCorrectionAnalyzer,
               fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName]['configFileName'],
               fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName]['logFileName']))
makeFile.write("\n")
for metOptionName in metOptions.keys():
    if len(fileNames_hadd[metOptionName]['inputFileNames']) > 0:
        makeFile.write("%s: %s\n" %
          (fileNames_hadd[metOptionName]['outputFileName'],
           make_MakeFile_vstring(fileNames_hadd[metOptionName]['inputFileNames'])))
        makeFile.write("\t%s %s &> %s\n" %
          (executable_shell,
           fileNames_hadd[metOptionName]['shellFileName'],
           fileNames_hadd[metOptionName]['logFileName']))
makeFile.write("\n")
for metOptionName in metOptions.keys():
    for corrLevelMC in corrLevelsMC:
        makeFile.write("%s: %s %s\n" %
          (fileNames_makeZllRecoilCorrectionFinalPlots[metOptionName][corrLevelMC]['outputFileName'],
           fileNames_hadd[metOptionName]['outputFileName'],
           executable_makeZllRecoilCorrectionFinalPlots))
        makeFile.write("\t%s %s &> %s\n" %
          (executable_makeZllRecoilCorrectionFinalPlots,
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
                outputFileNames_produceZllRecoilNtuples.append(
                  fileNames_produceZllRecoilNtuples[metOptionName][sampleName]['outputFileName'])
makeFile.write("\trm -f %s\n" % make_MakeFile_vstring(outputFileNames_produceZllRecoilNtuples))
outputFileNames_fitZllRecoilNtuples = []
for metOptionName in metOptions.keys():
    for sampleName in samplesToAnalyze.keys():
        if not (samplesToAnalyze[sampleName]['isMC'] and samplesToAnalyze[sampleName]['Type'] == 'Background'):
            if fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName].has_key(sampleName):
                outputFileNames_fitZllRecoilNtuples.append(
                  fileNames_fitZllRecoilNtuples_qT_vs_uParl_uPerp[metOptionName][sampleName]['outputFileName'])
            if fileNames_fitZllRecoilNtuples_rT_vs_vParl_vPerp_ii[metOptionName].has_key(sampleName):
                outputFileNames_fitZllRecoilNtuples.append(
                  fileNames_fitZllRecoilNtuples_rT_vs_vParl_vPerp_ii[metOptionName][sampleName]['outputFileName'])    
makeFile.write("\trm -f %s\n" % make_MakeFile_vstring(outputFileNames_fitZllRecoilNtuples))
outputFileNames_FWLiteZllRecoilCorrectionAnalyzer = []
for metOptionName in metOptions.keys():
    for sampleName in samplesToAnalyze.keys():
        if fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName].has_key(sampleName):
            outputFileNames_FWLiteZllRecoilCorrectionAnalyzer.append(
              fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName]['outputFileName'])
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
