import FWCore.ParameterSet.Config as cms

import TauAnalysis.Configuration.tools.castor as castor

import os
import re

process = cms.PSet()

process.fwliteInput = cms.PSet(
    fileNames = cms.vstring(),

    maxEvents = cms.int32(-1),
    
    outputEvery = cms.uint32(1000)
)

#--------------------------------------------------------------------------------
inputFilePath = '/data1/veelken/CMSSW_4_2_x/Ntuples/user/v/veelken/CMSSW_4_2_x/Ntuples/neuralMtautauTraining/v1_5'
inputFileNames = []
if inputFilePath.find('/castor/') != -1:
    inputFileNames = [ 'rfio:%s' % file_info['path'] for file_info in castor.nslsl(inputFilePath) ]
else:
    inputFileNames = [ 'file:%s' % os.path.join(inputFilePath, file_name) for file_name in os.listdir(inputFilePath) ]

inputFile_regex = \
  r"[a-zA-Z0-9_/:.]*neuralMtautauNtuple_(?P<sample>[a-zA-Z0-9_]+)_(?P<gridJob>\d*)(_(?P<gridTry>\d*))*_(?P<hash>[a-zA-Z0-9]*).root"
inputFile_matcher = re.compile(inputFile_regex)

inputFileNames_matched = []
for inputFileName in inputFileNames:
    if inputFile_matcher.match(inputFileName):
	inputFileNames_matched.append(inputFileName)

#print "inputFileNames_matched = %s" % inputFileNames_matched

setattr(process.fwliteInput, "fileNames", cms.vstring(inputFileNames_matched))
#--------------------------------------------------------------------------------

##process.fwliteInput.fileNames = cms.vstring(
##    '/afs/cern.ch/user/v/veelken/scratch0/CMSSW_4_2_4_patch1/src/TauAnalysis/CandidateTools/macros/neuralMtautauDummyNtuple.root'
##) 
    
process.fwliteOutput = cms.PSet(
    fileName = cms.string('testNeuralMtautau.root')
)

process.testNeuralMtautau = cms.PSet(
    #NNconfigFileName = cms.string('/data1/veelken/tmp/neuralMtautau/weights/trainNeuralMtautau_trainNeuralMtautau_MLP.weights.xml'),
    NNconfigFileName = cms.string('/data1/veelken/tmp/neuralMtautau/weights/trainNeuralMtautau_trainNeuralMtautau_kNN.weights.xml'),
    ##NNconfigFileName = cms.string('/afs/cern.ch/user/v/veelken/scratch0/CMSSW_4_2_4_patch1/src/TauAnalysis/CandidateTools/test/weights/trainNeuralMtautau_trainNeuralMtautau_MLP.weights.xml'),
    treeName = cms.string("neuralMtautauNtupleProducer/neuralMtautauNtuple"),
    ##treeName = cms.string("tree"),
    inputBranchNames = cms.vstring(
        "recLeg1Px",
        "recLeg1Py",
        "recLeg1Pz",
        "recLeg2Px",
        "recLeg2Py",
        "recLeg2Pz",
        "recLeg2M",
        "recMEtPx",
        "recMEtPy",
        "TMath::Min(recMEtSigmaX,    1.e+2)",
        "TMath::Min(recMEtSigmaY,    1.e+2)",
        "recMEtCorrXY",
        "recDPhi12",
        "recDAlpha12",
        "TMath::Min(recSVfitMtautau, 1.e+3)",
        "TMath::Min(recSigmaSVfit,   1.e+2)",
        "TMath::Min(recVisMass,      1.e+3)",
        ##"recPx1",
        ##"recPy1",
        ##"recPx2",
        ##"recPy2"
    ),
    targetBranchName = cms.string(
        "genMtautau"
        ##"genM"
    ),
    #runSVfit = cms.bool(True)
    runSVfit = cms.bool(False)
)
