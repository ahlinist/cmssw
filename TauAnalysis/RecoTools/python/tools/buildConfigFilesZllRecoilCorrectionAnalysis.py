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

    processType = None
    if samplesToAnalyze[sampleName]['isMC']:
        processType = 'smMC'
    else:
        processType = 'Data'

    srcMEt = metOptions[metOptionName][key]
    hltPaths_string = make_inputFileNames_vstring(hltPaths[processType])
    srcWeights_string = make_inputFileNames_vstring(srcWeights[processType])

    addPUreweight_string = ""
# CV: do not apply rho_neutral reweighting to Monte Carlo samples other than Zmumu 
#     until difference in rho_neutral distribution between Zmummu, 
#     TTbar and WW/WZ/ZZ is understood and corrected for
    if samplesToAnalyze[sampleName]['isMC'] and samplesToAnalyze[sampleName]['applyRhoNeutralReweighting']:
        addPUreweight_string = \
"""
    addPUreweight = cms.PSet(
        inputFileName = cms.FileInPath('TauAnalysis/RecoTools/data/vertexMultiplicityVsRhoPFNeutralReweight.root'),
        meName = cms.string('histoReweight_fitted'),
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
    srcJets = cms.InputTag('patJets'),
    srcUnclPFCands = cms.InputTag('pfCandsNotInJet'),

    srcTrigger = cms.InputTag('TriggerResults::HLT'),
    hltPaths = cms.vstring(%s),

    srcWeights = cms.VInputTag(%s),

    srcVertices = cms.InputTag('selectedPrimaryVertexPosition'),
    srcRhoNeutral = cms.InputTag('kt6PFNeutralJetsForVtxMultReweighting', 'rho'),
%s
)
""" % (inputFileNames_string, outputFileName_full, directory, 
       srcMEt, hltPaths_string, srcWeights_string, addPUreweight_string)

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

def buildConfigFile_fitZllRecoilNtuples(sampleName, metOptionName, inputFileName, outputFilePath, samplesToAnalyze,
                                        refBranchName = "qT", projParlBranchName = "uParl", projPerpBranchName = "uPerp"):

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

    outputFileName = "fittedZllRecoilCorrectionParameters_%s_%s_%s_cfi.py" % (sampleName, metOptionName, refBranchName)
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

    type = cms.string('%s'),

    refBranchName = cms.string('%s'),
    projParlBranchName = cms.string('%s'),
    projPerpBranchName = cms.string('%s'),

    outputFileName = cms.string('%s')
)
""" % (inputFileName, directory, 
       processType, refBranchName, projParlBranchName, projPerpBranchName, outputFileName_full)

    configFileName = "fitZllRecoilCorrectionNtuple_%s_%s_%s_cfg.py" % (sampleName, metOptionName, refBranchName)
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
                                                      samplesToAnalyze, metOptions, ZllRecoilCorrectionParameterFileNames, intLumiData):

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
        if samplesToAnalyze[sampleName]['Type'] == 'Signal':
            processType = 'MC_signal'
        elif samplesToAnalyze[sampleName]['Type'] == 'Background':
            processType = 'MC_background'
        else:
            raise ValueError("Invalid MC type = %s !!" % samplesToAnalyze[sampleName]['Type'])
    else:
        processType = 'Data'

    recoZllRecoilCorrectionParameters_string = ""
    if ZllRecoilCorrectionParameterFileNames is not None:
        recoZllRecoilCorrectionParameters_string = "    algorithm = cms.PSet(\n"
        recoZllRecoilCorrectionParameters_string += "        parameter = cms.PSet(\n"
        for parameterSetName in [ 'data', 'mc' ]: 
            ZllRecoilCorrectionParameterFile = open(ZllRecoilCorrectionParameterFileNames[parameterSetName], 'r')
            lines = ZllRecoilCorrectionParameterFile.readlines()
            for lineNumber, line in enumerate(lines):
                #print "%i: %s" % (lineNumber, line)
                if lineNumber == 0:
                    recoZllRecoilCorrectionParameters_string += "            %s = cms.PSet(\n" % parameterSetName
                elif lineNumber < (len(lines) - 1):
                    recoZllRecoilCorrectionParameters_string += "            %s" % line
            recoZllRecoilCorrectionParameters_string += "            ),\n"
            ZllRecoilCorrectionParameterFile.close()
        recoZllRecoilCorrectionParameters_string += "        )\n"
        recoZllRecoilCorrectionParameters_string += "    ),\n"

    srcJets = metOptions[metOptionName]['srcJets']
    if processType = 'Data':
        srcJets = 'patJets'

    processType = None
    if samplesToAnalyze[sampleName]['isMC']:
        processType = 'smMC'
    else:
        processType = 'Data'

    srcMEt = metOptions[metOptionName][key]
    hltPaths_string = make_inputFileNames_vstring(hltPaths[processType])
    srcWeights_string = make_inputFileNames_vstring(srcWeights[processType])
    
    allEvents_DBS = 0
    xSection = 0.
    if samplesToAnalyze[sampleName]['isMC']:
        allEvents_DBS = samplesToAnalyze[sampleName]['allEvents_DBS']
        xSection = samplesToAnalyze[sampleName]['xSection']

    addPUreweight_string = ""
# CV: do not apply rho_neutral reweighting to Monte Carlo samples other than Zmumu
#     until difference in rho_neutral distribution between Zmummu,
#     TTbar and WW/WZ/ZZ is understood and corrected for
    if samplesToAnalyze[sampleName]['isMC'] and samplesToAnalyze[sampleName]['applyRhoNeutralReweighting']:
        addPUreweight_string = \
"""
    addPUreweight = cms.PSet(
        inputFileName = cms.FileInPath('TauAnalysis/RecoTools/data/vertexMultiplicityVsRhoPFNeutralReweight.root'),
        meName = cms.string('histoReweight_fitted'),
        minPUreweight = cms.double(1.e-1),
        maxPUreweight = cms.double(1.e+1)
    ),
"""

    selEventsFileName = 'selEvents_%s_%s.txt' % (sampleName, metOptionName)
        
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

    type = cms.string('%s'),

%s

    srcZllCandidates = cms.InputTag('goldenZmumuCandidatesGe1IsoMuons'),
    srcMEt = cms.InputTag('%s'),

    srcTrigger = cms.InputTag('TriggerResults::HLT'),
    hltPaths = cms.vstring(%s),

    srcWeights = cms.VInputTag(%s),

    srcVertices = cms.InputTag('selectedPrimaryVertexPosition'),
    srcRhoNeutral = cms.InputTag('kt6PFNeutralJetsForVtxMultReweighting', 'rho'),
%s

    selEventsFileName = cms.string('%s'),

    # CV: 'srcEventCounter' is defined in TauAnalysis/Skimming/test/skimTauIdEffSample_cfg.py
    srcEventCounter = cms.InputTag('totalEventsProcessed'),
    allEvents_DBS = cms.int32(%i),
    
    xSection = cms.double(%f),
    
    intLumiData = cms.double(%f)
)
""" % (inputFileNames_string, outputFileName_full, directory,
       processType, recoZllRecoilCorrectionParameters_string,
       srcMEt, hltPaths_string, srcWeights_string, addPUreweight_string,
       os.path.join(outputFilePath, selEventsFileName), allEvents_DBS, xSection, intLumiData)

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

def buildConfigFile_makeZllRecoilCorrectionFinalPlots(sampleNameData, sampleNameMC_signal, sampleNameMCs_background,
                                                      metOptionName, inputFileName, outputFilePath, corrLevelMC):

    """Build cfg.py file to run makeZllRecoilCorrectionFinalPlots macro
       and make final control plots of MET in Data compared to Monte Carlo simulation with Z-recoil corrections applied"""

    print "<buildConfigFile_makeZllRecoilCorrectionFinalPlots>:"
    print " processing combination of sampleData %s, sampleMC (signal) %s" % (sampleNameData, sampleNameMC_signal)

    print(" building config file...")

    #corrLevelData = "beforeGenPUreweight"
    corrLevelData = corrLevelMC
    
    directoryData           =   "/".join([ sampleNameData,      corrLevelData ])
    directoryMC_signal      =   "/".join([ sampleNameMC_signal, corrLevelMC   ])
    directoryMCs_background = [ "/".join([ sampleNameMC_bgr,    corrLevelData ]) for sampleNameMC_bgr in sampleNameMCs_background ]

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

    directoryData           = cms.string('%s'),
    directoryMC_signal      = cms.string('%s'),
    directoryMCs_background = cms.vstring(%s),

    variables = cms.VPSet(
        cms.PSet(
            meName = cms.string('lPlusPt'),
            xAxisTitle = cms.string('P_{T}^{#mu+} / GeV')
        ),
        cms.PSet(
            meName = cms.string('lMinusPt'),
            xAxisTitle = cms.string('P_{T}^{#mu-} / GeV')
        ),
        cms.PSet(
            meName = cms.string('ZllCandMass'),
            xAxisTitle = cms.string('M_{#mu #mu}GeV')
        ),
        cms.PSet(
            meName = cms.string('metS'),
            xAxisTitle = cms.string('E_{T}^{miss} / GeV')
        ),
        cms.PSet(
            meName = cms.string('metL'),
            xAxisTitle = cms.string('E_{T}^{miss} / GeV')
        ),
        cms.PSet(
            meName = cms.string('metProjParlZ'),
            xAxisTitle = cms.string('u_{1}^{miss} / GeV')
        ),
        cms.PSet(
            meName = cms.string('metProjPerpZ'),
            xAxisTitle = cms.string('u_{2}^{miss} / GeV')
        ),
        cms.PSet(
            meName = cms.string('numVertices'),
            xAxisTitle = cms.string('rec. Vertex Multiplicity')
        ),
        cms.PSet(
            meName = cms.string('rhoNeutral'),
            xAxisTitle = cms.string('#rho_{h0} / GeV')
        )
    ),

    outputFileName = cms.string('%s')
)
""" % (inputFileName, directoryData, directoryMC_signal, 
       make_inputFileNames_vstring(directoryMCs_background), outputFileName_full)

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


