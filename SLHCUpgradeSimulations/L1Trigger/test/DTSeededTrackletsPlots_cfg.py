import FWCore.ParameterSet.Config as cms

################################################################################
maxEvNr = -1 #300

PT = '50'

BREATH = cms.untracked.int32(5)
DESERT = cms.untracked.double(0.001)

#Stubs = "fromSimHits"
Stubs = "fromPixelDigis"
#Stubs = "fromTrackTriggerHits"

USE_TSTHETA = True    ### True/False!

################################################################################

use_tstheta_label = ""
if USE_TSTHETA == True:
        use_tstheta_label = "_tsTheta"

inputDir  = "../../../../outputs/"
outputDir = "../../../../outputs/"

rootFileNamesIn = cms.untracked.vstring( 
      'file:' + inputDir + 
       PT + 'GeVPtMuons_DTSeededTracklets_fromPixelDigis'
       + use_tstheta_label + '.root')
    
rootFileNamePlots = cms.untracked.string(
     outputDir + 'Plots_' + PT + 'GeVMuons' + use_tstheta_label + '.root') 

msgFileNames = cms.untracked.vstring(
     outputDir + 'Plots_' + PT + 'GeVMuons.msg')

################################################################################

process = cms.Process("MuonsUpgradePlots")

process.load("SLHCUpgradeSimulations.L1Trigger.DTSeededTrackletsPlots_cfi")

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(maxEvNr))
process.MessageLogger.destinations = msgFileNames
process.source.fileNames = rootFileNamesIn

process.DTL1slhcPlots.rootFileNamePlots = rootFileNamePlots
#process.DTL1slhcPlots.GaugeSampleSize = cms.untracked.uint32(1500)
process.DTL1slhcPlots.breath = BREATH  #cms.untracked.int32(5)
process.DTL1slhcPlots.desert = DESERT  #cms.untracked.double(0.0005)
process.plots = cms.Path(process.DTL1slhcPlots)

#process.end = cms.EndPath(process.Out)

process.schedule = cms.Schedule(process.plots)
#process.schedule = cms.Schedule(process.plots, process.end)


