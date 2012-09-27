import FWCore.ParameterSet.Config as cms

################################################################################
maxEvNr = Max

RCSIZE = cms.untracked.uint32(1500)
BREATH = cms.untracked.int32(5)
DESERT = cms.untracked.double(0.01)

#Stubs = "fromSimHits"
Stubs = "fromPixelDigis"
#Stubs = "fromTrackTriggerHits"

USE_TSTHETA = True    ### True/False!
USE_roughTHETA = False

################################################################################

use_tstheta_label = ""
if USE_TSTHETA == True:
        use_theta_label = "_tsTheta"
if USE_roughTHETA == True:
	use_theta_label = "_roughTheta"

inputDir  = "../../../../outputs/"
outputDir = "../../../../outputs/"

singleMuonPt = PT

rootFileNamesIn = cms.untracked.vstring( 
      'file:' + inputDir + 'PT' + 
       'GeVPtMuons_DTSeededTracklets_fromPixelDigis'
       + use_theta_label + '.root'
      #,
      #'file:' + inputDir +
      # '50GeVPtMuons_DTSeededTracklets_fromPixelDigis'
      # + use_tstheta_label + '.root'
)
    
rootFileNamePlots = cms.untracked.string(
     outputDir + 'Plots_' + 'PT' + 'GeVMuons' + use_theta_label + '.root') 

msgFileNames = cms.untracked.vstring(
     outputDir + 'Plots_' + 'PT' + 'GeVMuons.msg')

################################################################################

process = cms.Process("MuonsUpgradePlots")

process.load("SLHCUpgradeSimulations.L1DTTrigger.DTSeededTrackletsPlots_cfi")

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(maxEvNr))
process.MessageLogger.destinations = msgFileNames
process.source.fileNames = rootFileNamesIn

process.DTL1slhcPlots.rootFileNamePlots = rootFileNamePlots
process.DTL1slhcPlots.RangeCalibrationSampleSize = RCSIZE
process.DTL1slhcPlots.breath = BREATH  #cms.untracked.int32(5)
process.DTL1slhcPlots.desert = DESERT  #cms.untracked.double(0.0005)
process.DTL1slhcPlots.singleMuonPt = singleMuonPt
process.DTL1slhcPlots.optimize_plot = True
process.plots = cms.Path(process.DTL1slhcPlots)

#process.end = cms.EndPath(process.Out)

process.schedule = cms.Schedule(process.plots)
#process.schedule = cms.Schedule(process.plots, process.end)


