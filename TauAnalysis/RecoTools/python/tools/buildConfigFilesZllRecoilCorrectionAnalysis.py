import FWCore.ParameterSet.Config as cms

from TauAnalysis.TauIdEfficiency.tools.buildConfigFilesTauIdEffAnalysis import make_inputFileNames_vstring

import os
import re

#--------------------------------------------------------------------------------
#
# define auxiliary functions
#
def getPATtupleFileNames(sampleNames, inputFilePath):
    
    inputFileNames = os.listdir(inputFilePath)
    #print(inputFileNames)

    # check if inputFile is PAT-tuple and
    # matches sample to be analyzed
    inputFileNames_matched = []
    for inputFileName in inputFileNames:
        isMatched = False
        for sampleName in sampleNames:
            inputFile_regex = \
              r"ZllRecoilCorrectionPATtuple_%s_[a-zA-Z0-9_]*.root" % sampleName
            inputFile_matcher = re.compile(inputFile_regex)
            if inputFile_matcher.match(inputFileName):
                isMatched = True
        if isMatched:
            inputFileNames_matched.append(os.path.join(inputFilePath, inputFileName))

    print " found %i input files." % len(inputFileNames_matched)

    return inputFileNames_matched
#--------------------------------------------------------------------------------

def buildConfigFile_produceZllRecoilNtuples(sampleName, metOptionName, inputFilePath, outputFilePath, samplesToAnalyze, metOptions):

    """Build cfg.py file to run FWLiteZllRecoilCorrectionNtupleProducer macro on PAT-tuples,
       and produce 'plain' ROOT Ntuple needed for fitting Z-recoil correction parameters"""

    print "<buildConfigFile_produceZllRecoilNtuples>:"
    print " processing sample %s" % sampleName

    inputFileNames = getPATtupleFileNames(samplesToAnalyze[sampleName]['samples'], inputFilePath)
    if len(inputFileNames) == 0:
        print("Sample %s has no input files --> skipping !!" % sampleName)
        return

    inputFileNames_string = make_inputFileNames_vstring(inputFileNames)

    print(" building config file...")

    outputFileName = 'ZllRecoilCorrectionNtuple_%s_%s.root' % (sampleName, metOptionName)
    outputFileName_full = os.path.join(outputFilePath, outputFileName)
    
    directory = sampleName

    srcMEt = metOptions[metOptionName]['srcMEt']
    
    srcWeights = []
    if samplesToAnalyze[sampleName]['isMC']:
        srcWeights.extend([ 'vertexMultiplicityReweight' ])

    addPUreweight_string = ""
    if samplesToAnalyze[sampleName]['isMC']:
        addPUreweight_string = \
"""
    addPUreweight = cms.PSet(
        inputFileName = cms.FileInPath('TauAnalysis/RecoTools/data/vertexMultiplicityVsRhoPFNeutralReweight.root'),
        meName = cms.string('histoReweight'),
        minPUreweight = cms.double(1.e-1),
        maxPUreweight = cms.double(1.e+1)
    )
"""    

    config = \
"""
import FWCore.ParameterSet.Config as cms

process = cms.PSet()

process.fwliteInput = cms.PSet(
    fileNames = cms.vstring(%s),
    
    maxEvents = cms.int32(-1),
    
    outputEvery = cms.uint32(1000)
)
    
process.fwliteOutput = cms.PSet(
    fileName = cms.string('%s')
)

process.ZllRecoilCorrectionNtupleProducer = cms.PSet(

    directory = cms.string('%s'),

    srcZllCandidates = cms.InputTag('goldenZmumuCandidatesGe1IsoMuons'),
    srcMEt = cms.InputTag('%s'),

    srcWeights = cms.VInputTag('%s'),

    srcVertices = cms.InputTag('selectedPrimaryVertexPosition'),
    srcPFNeutralRho = cms.InputTag('kt6PFNeutralJets', 'rho'),
%s    
)
""" % (inputFileNames_string, outputFileName_full,
       directory, srcMEt, srcWeights, addPUreweight_string)

    configFileName = "produceZllRecoilCorrectionNtuple_%s_%s_cfg.py" % (sampleName, metOptionName)
    configFileName_full = os.path.join(outputFilePath, configFileName)    
    configFile = open(configFileName_full, "w")
    configFile.write(config)
    configFile.close()

    logFileName = configFileName.replace('_cfg.py', '.log')
    logFileName_full = os.path.join(outputFilePath, logFileName)

    retVal = {}
    retVal['configFileName'] = configFileName_full
    retVal['outputFileName'] = outputFileName_full
    retVal['logFileName']    = logFileName_full

    return retVal

def buildConfigFile_fitZllRecoilNtuples(sampleName, metOptionName, inputFileName, outputFilePath, samplesToAnalyze):

    """Build cfg.py file to run fitZllRecoilCorrection macro to run on 'plain' ROOT Ntuples
       and fit Z-recoil correction parameters"""

    print "<buildConfigFile_fitZllRecoilNtuples>:"
    print " processing sample %s" % sampleName


    print(" building config file...")

    directory = sampleName
    
    processType = None
    if samplesToAnalyze[sampleName]['isMC']:
        processType = 'MC'
    else:
        processType = 'Data'

    outputFileName = "fittedZllRecoilCorrectionParameters_%s_%s_cfi.py" % (sampleName, metOptionName)
    outputFileName_full = os.path.join(outputFilePath, outputFileName)

    config = \
"""
import FWCore.ParameterSet.Config as cms

process = cms.PSet()

process.fwliteInput = cms.PSet(
    fileNames = cms.vstring('%s'),
    
    maxEvents = cms.int32(-1),
    
    outputEvery = cms.uint32(1000)
)

process.fitZllRecoilCorrection = cms.PSet(

    directory = cms.string('%s'),

    processType = cms.string('%s'),

    outputFileName = cms.string('%s')
)
""" % (inputFileName,
       directory, processType, outputFileName_full)

    configFileName = "fitZllRecoilCorrectionNtuple_%s_%s_cfg.py" % (sampleName, metOptionName)
    configFileName_full = os.path.join(outputFilePath, configFileName)    
    configFile = open(configFileName_full, "w")
    configFile.write(config)
    configFile.close()

    logFileName = configFileName.replace('_cfg.py', '.log')
    logFileName_full = os.path.join(outputFilePath, logFileName)

    retVal = {}
    retVal['configFileName'] = configFileName_full
    retVal['outputFileName'] = outputFileName_full
    retVal['logFileName']    = logFileName_full

    return retVal

def buildConfigFile_FWLiteZllRecoilCorrectionAnalyzer(sampleName, metOptionName, inputFilePath, outputFilePath,
                                                      samplesToAnalyze, metOptions, ZllRecoilCorrectionParameterFileName, intLumiData):

    """Build cfg.py file to run FWLiteZllRecoilCorrectionAnalyzer macro on PAT-tuples,
       and fill control plots of MET in Data compared to Monte Carlo simulation with Z-recoil corrections applied"""

    print "<buildConfigFile_FWLiteZllRecoilCorrectionAnalyzer>:"
    print " processing sample %s" % sampleName

    inputFileNames = getPATtupleFileNames(samplesToAnalyze[sampleName]['samples'], inputFilePath)
    if len(inputFileNames) == 0:
        print("Sample %s has no input files --> skipping !!" % sampleName)
        return

    inputFileNames_string = make_inputFileNames_vstring(inputFileNames)

    print(" building config file...")

    outputFileName = 'analyzeZllRecoilCorrectionHistograms_%s_%s.root' % (sampleName, metOptionName)
    outputFileName_full = os.path.join(outputFilePath, outputFileName)
    
    directory = sampleName

    processType = None
    if samplesToAnalyze[sampleName]['isMC']:
        processType = 'MC'
    else:
        processType = 'Data'

    recoZllRecoilCorrectionParameters_string = ""
    if ZllRecoilCorrectionParameterFileName is not None:
        ZllRecoilCorrectionParameterFile = open(ZllRecoilCorrectionParameterFileName, 'r')
        recoZllRecoilCorrectionParameters_string = ZllRecoilCorrectionParameterFile.read()
        ZllRecoilCorrectionParameterFile.close()

    srcMEt = metOptions[metOptionName]['srcMEt']
    
    srcWeights = []
    if samplesToAnalyze[sampleName]['isMC']:
        srcWeights.extend([ 'vertexMultiplicityReweight' ])

    addPUreweight_string = ""
    if samplesToAnalyze[sampleName]['isMC']:
        addPUreweight_string = \
"""
    addPUreweight = cms.PSet(
        inputFileName = cms.FileInPath('TauAnalysis/RecoTools/data/vertexMultiplicityVsRhoPFNeutralReweight.root'),
        meName = cms.string('histoReweight'),
        minPUreweight = cms.double(1.e-1),
        maxPUreweight = cms.double(1.e+1)
    ),
"""
        
    config = \
"""
import FWCore.ParameterSet.Config as cms

process = cms.PSet()

process.fwliteInput = cms.PSet(
    fileNames = cms.vstring(%s),
    
    maxEvents = cms.int32(-1),
    
    outputEvery = cms.uint32(1000)
)
    
process.fwliteOutput = cms.PSet(
    fileName = cms.string('%s')
)

process.ZllRecoilCorrectionAnalyzer = cms.PSet(

    directory = cms.string('%s'),

    processType = cms.string('%s'),

%s,

    srcZllCandidates = cms.InputTag('goldenZmumuCandidatesGe1IsoMuons'),
    srcMEt = cms.InputTag('%s'),

    srcWeights = cms.VInputTag('%s'),

    srcVertices = cms.InputTag('selectedPrimaryVertexPosition'),
    srcPFNeutralRho = cms.InputTag('kt6PFNeutralJets', 'rho'),    
%s

    # CV: 'srcEventCounter' is defined in TauAnalysis/Skimming/test/skimTauIdEffSample_cfg.py
    srcEventCounter = cms.InputTag('totalEventsProcessed'),
    allEvents_DBS = cms.int32(29743564), # for sample /DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia/Summer11-PU_S4_START42_V11-v1/AODSIM
    
    xSection = cms.double(1666.), # Z --> mu+ mu- cross-section computed @ NNLO in units of pb
    
    intLumiData = cms.double(%f)
)
""" % (inputFileNames_string, outputFileName_full,
       directory, processType, recoZllRecoilCorrectionParameters_string, srcMEt, srcWeights, addPUreweight_string, intLumiData)

    configFileName = "analyzeZllRecoilCorrectionPATtuple_%s_%s_cfg.py" % (sampleName, metOptionName)
    configFileName_full = os.path.join(outputFilePath, configFileName)    
    configFile = open(configFileName_full, "w")
    configFile.write(config)
    configFile.close()

    logFileName = configFileName.replace('_cfg.py', '.log')
    logFileName_full = os.path.join(outputFilePath, logFileName)

    retVal = {}
    retVal['configFileName'] = configFileName_full
    retVal['outputFileName'] = outputFileName_full
    retVal['logFileName']    = logFileName_full

    return retVal

def buildConfigFile_makeZllRecoilCorrectionFinalPlots(sampleNameData, sampleNameMC, metOptionName, inputFileName, outputFilePath,
                                                      corrLevelMC):

    """Build cfg.py file to run makeZllRecoilCorrectionFinalPlots macro
       and make final control plots of MET in Data compared to Monte Carlo simulation with Z-recoil corrections applied"""

    print "<buildConfigFile_makeZllRecoilCorrectionFinalPlots>:"
    print " processing combination of sampleData %s, sampleMC %s" % (sampleNameData, sampleNameMC)

    print(" building config file...")

    corrLevelData = "beforeGenPUreweight"
    
    directoryData = "/".join([ sampleNameData, corrLevelData ])
    directoryMC   = "/".join([ sampleNameData, corrLevelMC   ])

    outputFileName = "plotZllRecoilCorrection_%s_%s.png" % (metOptionName, corrLevelMC)
    outputFilePath_plots = os.path.join(outputFilePath, "plots")
    if not os.path.exists(outputFilePath_plots):
        os.mkdir(outputFilePath_plots)    
    outputFileName_full = os.path.join(outputFilePath_plots, outputFileName)

    config = \
"""
import FWCore.ParameterSet.Config as cms

process = cms.PSet()

process.fwliteInput = cms.PSet(
    fileNames = cms.vstring('%s'),
    
    maxEvents = cms.int32(-1),
    
    outputEvery = cms.uint32(1000)
)

process.makeZllRecoilCorrectionFinalPlots = cms.PSet(

    directoryData = cms.string('%s'),
    directoryMC   = cms.string('%s'),

    variables = cms.VPSet(
        cms.PSet(
            meName = cms.string('lPlusPt'),
            xAxisTitle = cms.string('GeV')
        ),
        cms.PSet(
            meName = cms.string('lMinusPt'),
            xAxisTitle = cms.string('GeV')
        ),
        cms.PSet(
            meName = cms.string('ZllCandMass'),
            xAxisTitle = cms.string('GeV')
        ),
        cms.PSet(
            meName = cms.string('met'),
            xAxisTitle = cms.string('GeV')
        ),
        cms.PSet(
            meName = cms.string('metProjParlZ'),
            xAxisTitle = cms.string('GeV')
        ),
        cms.PSet(
            meName = cms.string('metProjPerpZ'),
            xAxisTitle = cms.string('GeV')
        ),
        cms.PSet(
            meName = cms.string('numVertices'),
            xAxisTitle = cms.string('')
        ),
        cms.PSet(
            meName = cms.string('rho'),
            xAxisTitle = cms.string('GeV')
        )
    ),

    outputFileName = cms.string('%s')
)
""" % (inputFileName,
       directoryData, directoryMC, outputFileName_full)

    configFileName = "makeZllRecoilCorrectionFinalPlots_%s_%s_cfg.py" % (metOptionName, corrLevelMC)
    configFileName_full = os.path.join(outputFilePath, configFileName)    
    configFile = open(configFileName_full, "w")
    configFile.write(config)
    configFile.close()

    logFileName = configFileName.replace('_cfg.py', '.log')
    logFileName_full = os.path.join(outputFilePath, logFileName)

    retVal = {}
    retVal['configFileName'] = configFileName_full
    retVal['outputFileName'] = outputFileName_full
    retVal['logFileName']    = logFileName_full

    return retVal


