import FWCore.ParameterSet.Config as cms

import TauAnalysis.Configuration.tools.castor as castor

import os
import re

process = cms.PSet()

process.fwliteInput = cms.PSet(
    fileNames = cms.vstring()
)

#--------------------------------------------------------------------------------
inputFilePath = '/data1/veelken/CMSSW_4_2_x/Ntuples/user/v/veelken/CMSSW_4_2_x/Ntuples/neuralMtautauTraining/v1_4'
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
    fileName = cms.string('trainNeuralMtautau.root')
)

mvaType_MLP = "MLP"
mvaName_MLP = "trainNeuralMtautau_MLP"
mvaTrainingOptions_MLP = [
    "!H",
    "!V",
    "VarTransform=Norm",
    "NeuronType=tanh",
    "NCycles=20000",
    #"HiddenLayers=N+20",
    "TestRate=6",
    "TrainingMethod=BFGS",
    "Sampling=0.3",
    "SamplingEpoch=0.8",
    "ConvergenceImprove=1e-6",
    "ConvergenceTests=15",
    "!UseRegulator"
]
mvaTrainingOptions_MLP_string = ":".join(mvaTrainingOptions_MLP)
#print "mvaTrainingOptions_MLP_string = %s" % mvaTrainingOptions_MLP_string

mvaType_kNN = "kNN"
mvaName_kNN = "trainNeuralMtautau_kNN"
mvaTrainingOptions_kNN = [
    "nkNN=20",
    "ScaleFrac=0.8",
    "SigmaFact=1.0",
    "Kernel=Gaus",
    "UseKernel=F",
    "UseWeight=T",
    "!Trim"
]
mvaTrainingOptions_kNN_string = ":".join(mvaTrainingOptions_kNN)
#print "mvaTrainingOptions_kNN_string = %s" % mvaTrainingOptions_kNN_string

process.trainNeuralMtautau = cms.PSet(
    mvaType = cms.string(mvaType_kNN),
    mvaName = cms.string(mvaName_kNN),
    mvaTrainingOptions = cms.string(mvaTrainingOptions_kNN_string),
    treeName = cms.string("neuralMtautauNtupleProducer/neuralMtautauNtuple"),
    #treeName = cms.string("tree"),
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
        "TMath::Min(recMEtSigmaX, 1.e+2)",
        "TMath::Min(recMEtSigmaY, 1.e+2)",
        "recMEtCorrXY",
        ##"recDPhi12",
        ##"recDAlpha12",
        ##"recSVfitMtautau",
        ##"recSigmaSVfit",
        ##"recVisMass"
    ),
    targetBranchName = cms.string(
        "genMtautau"
        #"genM"
    )
)
