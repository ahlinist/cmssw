import os

import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.Types as CfgTypes
import FWCore.ParameterSet.VarParsing as VarParsing
import FWCore.PythonUtilities.LumiList as LumiList

## setup 'analysis'  options
options = VarParsing.VarParsing ('analysis')

## register customized options
options.register("jsonFile",
  "", # default value
  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
  VarParsing.VarParsing.varType.string,          # string, int, or float
  "JSON file to be applied."
)

## Your data goes here:
inputDir = "/mnt/tier2/store/user/veverka/Mu/Run2010A-Dec22ReReco_v1-DimuonVGammaSkim_v4/f81f6ec801d687f509099b4fa4a3c90c"
options.inputFiles = [
    #"file:VGammaPAT_DimuonSkim_testWinter10.root"
    #os.path.join("file:",
                  #"/uscmst1b_scratch/lpc1/3DayLifetime/veverka/mu",
                  #"VGammaSkim_LyonSyncTest_DimuonSkim_testWinter10.root"
                  #)
    #os.path.join(
        #"file:",
        #"/uscmst1b_scratch/lpc1/3DayLifetime/veverka/mu",
        #file
    #) for file in [
        #"VGammaSkim_LyonSyncTest_Dec22ReReco_v2_DimuonSkim_1_of_4.root",
        #"VGammaSkim_LyonSyncTest_Dec22ReReco_v2_DimuonSkim_2_of_4.root",
        #"VGammaSkim_LyonSyncTest_Dec22ReReco_v2_DimuonSkim_3_of_4.root",
        #"VGammaSkim_LyonSyncTest_Dec22ReReco_v2_DimuonSkim_4_of_4.root",
    #]
    "file:" + os.path.join(inputDir, f) 
        for f in os.popen("ls " + inputDir).read().split()
]

options.jsonFile = "Cert_136033-149442_7TeV_Dec22ReReco_Collisions10_JSON_v3.txt"

## get and parse the command line arguments
options.parseArguments()

process = cms.Process("ANALYZE")

process.inputs = cms.PSet (
    fileNames = cms.vstring() + options.inputFiles[:]
)

if options.jsonFile != "":
    myLumis = LumiList.LumiList(
        filename = options.jsonFile
        ).getCMSSWString().split(',')
    process.inputs.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
    process.inputs.lumisToProcess.extend(myLumis)

process.outputs = cms.PSet (
    outputName = cms.string('basicExample.root')
)

process.analysis = cms.PSet(
    verbosity = cms.int32(1),
    muonSrc = cms.InputTag("cleanPatMuonsTriggerMatch"),
    photonSrc = cms.InputTag("cleanPatPhotonsTriggerMatch"),
    beamSpotSrc = cms.InputTag("offlineBeamSpot"),
    ipDefinition = cms.string("RECO_globalTrackDxy_BS"),
    ipCondition = cms.string("LTGT")
)

if __name__ == "__main__": import user
