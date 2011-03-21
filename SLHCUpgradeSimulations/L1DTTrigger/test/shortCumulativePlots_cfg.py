import FWCore.ParameterSet.Config as cms

################################################################################
maxEvNr = -1 #Max

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

rootFileNamesIn = cms.untracked.vstring( 
 'file:outputs/10GeVPtMuons_DTSeededTracklets_fromPixelDigis_tsTheta.root'
 ,'file:outputs/15GeVPtMuons_DTSeededTracklets_fromPixelDigis_tsTheta.root'
 #,'file:outputs/20GeVPtMuons_DTSeededTracklets_fromPixelDigis_tsTheta.root'
 #,'file:outputs/30GeVPtMuons_DTSeededTracklets_fromPixelDigis_tsTheta.root'
 #,'file:outputs/40GeVPtMuons_DTSeededTracklets_fromPixelDigis_tsTheta.root'
 #,'file:outputs/50GeVPtMuons_DTSeededTracklets_fromPixelDigis_tsTheta.root'
 #,'file:outputs/60GeVPtMuons_DTSeededTracklets_fromPixelDigis_tsTheta.root'
 #,'file:outputs/70GeVPtMuons_DTSeededTracklets_fromPixelDigis_tsTheta.root'
 #,'file:outputs/100GeVPtMuons_DTSeededTracklets_fromPixelDigis_tsTheta.root'
 #,'file:outputs/150GeVPtMuons_DTSeededTracklets_fromPixelDigis_tsTheta.root'
)
    
rootFileNamePlots = cms.untracked.string(
     outputDir + 'Plots_' + use_theta_label + '.root') 

msgFileNames = cms.untracked.vstring(
     outputDir + 'Plots.msg')

################################################################################

process = cms.Process("MuonsUpgradePlots")

process.load("SLHCUpgradeSimulations.L1DTTrigger.DTSeededTrackletsPlots_cfi")

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(maxEvNr))
process.MessageLogger.destinations = msgFileNames
process.source.fileNames = rootFileNamesIn
process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')

process.DTL1slhcPlots.rootFileNamePlots = rootFileNamePlots
process.DTL1slhcPlots.do_fit_stubstubDePhi = True
process.plots = cms.Path(process.DTL1slhcPlots)

#process.end = cms.EndPath(process.Out)

process.schedule = cms.Schedule(process.plots)
#process.schedule = cms.Schedule(process.plots, process.end)


