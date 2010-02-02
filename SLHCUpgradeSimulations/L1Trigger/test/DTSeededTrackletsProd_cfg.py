import FWCore.ParameterSet.Config as cms

################################################################################
maxEvNr = 3000

PT = '50'

#Stubs = "fromSimHits"
Stubs = "fromPixelDigis"
#Stubs = "fromTrackTriggerHits"

USE_TSTHETA = True    ### True/False!

################################################################################
 
use_tstheta_label = ""
if USE_TSTHETA == True:
        use_tstheta_label = "_tsTheta"

inputDir = '/data/SLHCprod/long_barrel_2_2_6/'
outputDir = "../../../../outputs/"

rootFileNamesIn = cms.untracked.vstring( 
      'file:' + inputDir + 
       PT + 'GeVPtMuons_StackedTrackerlongbarrel_fromPixelDigis.root')
    
rootFileNameOut = cms.untracked.string(
     outputDir + PT+ 'GeVPtMuons_DTSeededTracklets_fromPixelDigis' 
     + use_tstheta_label + '.root') 
	 
rootFileNamePlots = cms.untracked.string(
     outputDir + 'Plots_' + PT + 'GeVMuons' + use_tstheta_label + '.root') 

asciiFileName = cms.untracked.string(
     outputDir + PT + 'GeVPtMuons_DTSeededTracklets_fromPixelDigis'
     + use_tstheta_label + '.outAscii')

msgFileNames = cms.untracked.vstring(
     outputDir + PT + 'GeVPtMuons_DTSeededTracklets_fromPixelDigis'
     + use_tstheta_label + '.msg')

################################################################################

process = cms.Process("MuonsUpgrade")

process.load("SLHCUpgradeSimulations.L1Trigger.stackedTracker_specific_cff")
process.load("SLHCUpgradeSimulations.L1Trigger.DTSeededTrackletsProd_cff")
process.load("SLHCUpgradeSimulations.L1Trigger.DTSeededTrackletsPlots_cfi")
process.load("SLHCUpgradeSimulations.L1Trigger.DTSeededTrackletsOutMod_cff")

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(maxEvNr))
process.MessageLogger.destinations = msgFileNames
process.source.fileNames = rootFileNamesIn

# to produce, in case, collection of L1MuDTTrack objects:    
process.dttfDigis = cms.Path(process.simDttfDigis)

if Stubs == "fromSimHits":
    process.stubs = cms.Path(process.stubs_fromSimHits)
elif Stubs == "fromPixelDigis":
    process.stubs = cms.Path(process.stubs_fromPixelDigis)
else:
    process.stubs = cms.Path(process.stubs_fromTrackTriggerHits)

if USE_TSTHETA == True:
	process.DTL1slhcProd.use_TSTheta = cms.untracked.bool(True)
process.DTL1slhcProd.rootFileNameOut = rootFileNameOut
process.DTL1slhcProd.asciiFileName   = asciiFileName
process.DTL1slhcProd.min_invRb = cms.untracked.double(0.000045)
process.DTL1slhcProd.max_invRb = cms.untracked.double(0.0035)
process.scms = cms.Path(process.DTL1slhcProd)

process.DTL1slhcPlots.rootFileNamePlots = rootFileNamePlots
#process.DTL1slhcPlots.GaugeSampleSize = cms.untracked.uint32(1500)
#process.DTL1slhcPlots.breath = cms.untracked.int32(5)
#process.DTL1slhcPlots.desert = cms.untracked.double(0.0005)
process.plots = cms.Path(process.DTL1slhcPlots)

process.Out.fileName = rootFileNameOut
process.end = cms.EndPath(process.Out)

#process.schedule = cms.Schedule(process.scms, process.end)
process.schedule = cms.Schedule(process.scms, process.plots, process.end)
#process.schedule = cms.Schedule(process.dttfDigis, process.scms, process.end)

