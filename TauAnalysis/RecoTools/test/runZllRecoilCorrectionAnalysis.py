#!/usr/bin/env python

from TauAnalysis.TauIdEfficiency.tools.buildConfigFilesTauIdEffAnalysis import buildConfigFile_hadd
from TauAnalysis.RecoTools.tools.buildConfigFilesZllRecoilCorrectionAnalysis import *

import os

version = 'v2'

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
        'isMC' : True
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
    }
}

intLumiData = 1100 # pb^-1 for runs 160404 - 167913 ("golden" quality EPS dataset)

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
fileNames_fitZllRecoilNtuples = {}
for metOptionName in metOptions.keys():
    fileNames_fitZllRecoilNtuples[metOptionName] = {}
    for sampleName in samplesToAnalyze.keys():
        retVal_fitZllRecoilNtuples = \
          buildConfigFile_fitZllRecoilNtuples(
            sampleName, metOptionName,
            fileNames_produceZllRecoilNtuples[metOptionName][sampleName]['outputFileName'], outputFilePath, samplesToAnalyze)

        if retVal_fitZllRecoilNtuples is None:
            continue

        fileNames_fitZllRecoilNtuples[metOptionName][sampleName] = retVal_fitZllRecoilNtuples
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
sampleNameData = None
sampleNameMC   = None
for sampleName in samplesToAnalyze.keys():
    if samplesToAnalyze[sampleName]['isMC']:
        if sampleNameMC is not None:
            raise ValueError("'MC' sample must be defined only once !!")
        sampleNameMC = sampleName
    else:
        if sampleNameData is not None:
            raise ValueError("'Data' sample must be defined only once !!")
        sampleNameData = sampleName
        
corrLevelsMC = [
    'beforeGenPUreweight',
    'beforeAddPUreweight',
    'beforeZllRecoilCorr',
    'afterZllRecoilCorr'
]

fileNames_makeZllRecoilCorrectionFinalPlots = {}
for metOptionName in metOptions.keys():
    fileNames_makeZllRecoilCorrectionFinalPlots[metOptionName] = {}
    for corrLevelMC in corrLevelsMC:
        retVal_makeZllRecoilCorrectionFinalPlots = \
          buildConfigFile_makeZllRecoilCorrectionFinalPlots(
            sampleNameData, sampleNameMC, metOptionName,
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
        if fileNames_fitZllRecoilNtuples[metOptionName].has_key(sampleName) and \
           fileNames_produceZllRecoilNtuples[metOptionName].has_key(sampleName):
            makeFile.write("%s: %s %s\n" %
              (fileNames_fitZllRecoilNtuples[metOptionName][sampleName]['outputFileName'],
               fileNames_produceZllRecoilNtuples[metOptionName][sampleName]['outputFileName'],
               executable_fitZllRecoilNtuples))
            makeFile.write("\t%s %s &> %s\n" %
              (executable_fitZllRecoilNtuples,
               fileNames_fitZllRecoilNtuples[metOptionName][sampleName]['configFileName'],
               fileNames_fitZllRecoilNtuples[metOptionName][sampleName]['logFileName']))
makeFile.write("\n")
for metOptionName in metOptions.keys():
    for sampleName in samplesToAnalyze.keys():
        if fileNames_fitZllRecoilNtuples[metOptionName].has_key(sampleName) and \
           fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName].has_key(sampleName):
            makeFile.write("%s: %s %s %s\n" %
              (fileNames_FWLiteZllRecoilCorrectionAnalyzer[metOptionName][sampleName]['outputFileName'],
               fileNames_fitZllRecoilNtuples[metOptionName][sampleNameData]['outputFileName'],
               fileNames_fitZllRecoilNtuples[metOptionName][sampleNameMC]['outputFileName'],
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
       fileNames_fitZllRecoilNtuples[metOptionName][sampleNameData]['outputFileName'],
       fileNames_fitZllRecoilNtuples[metOptionName][sampleNameMC]['outputFileName'],
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
        if fileNames_produceZllRecoilNtuples[metOptionName].has_key(sampleName):
            outputFileNames_produceZllRecoilNtuples.append(
              fileNames_produceZllRecoilNtuples[metOptionName][sampleName]['outputFileName'])
makeFile.write("\trm -f %s\n" % make_MakeFile_vstring(outputFileNames_produceZllRecoilNtuples))
outputFileNames_fitZllRecoilNtuples = []
for metOptionName in metOptions.keys():
    for sampleName in samplesToAnalyze.keys():
        if fileNames_fitZllRecoilNtuples[metOptionName].has_key(sampleName):
            outputFileNames_fitZllRecoilNtuples.append(
              fileNames_fitZllRecoilNtuples[metOptionName][sampleName]['outputFileName'])
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
