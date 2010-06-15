import FWCore.ParameterSet.Config as cms

process = cms.Process('makeZtoElecTauPlots')

process.load("TauAnalysis.Configuration.dumpZtoElecTau_cff")
process.load("TauAnalysis.Configuration.plotZtoElecTau_cff")

##### set cm energy here #####
cmsEnergy = cms.string('7TeV')
##############################

process.plotZtoElecTau.indOutputFileName = cms.string('plotZtoElecTau_' + cmsEnergy.value() + '_#PLOT#.png')
# need this to get intLumiZtoElecTau_Data
from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecTau_cfi import *
process.plotZtoElecTau.labels.mcNormScale.text = cms.vstring('sim. ' + str(intLumiZtoElecTau_Data) + 'pb^{-1}','#sqrt{s}=' + cmsEnergy.value())
process.saveZtoElecTau.outputFileName = 'plotsZtoElecTau_' + cmsEnergy.value() + '_all.root'
if cmsEnergy.value() == '10TeV':
	process.loadZtoElecTau = process.loadZtoElecTau_10TeV
if cmsEnergy.value() == '7TeV':
	process.loadZtoElecTau = process.loadZtoElecTau_7TeV

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

# define directory from which .root files containing the histograms get loaded
process.loadZtoElecTau.inputFilePath = cms.string("rfio:/castor/cern.ch/user/j/jkolb/elecTauAnalysis/summer09/hists/")

# import utility function to enable factorization
from TauAnalysis.Configuration.tools.factorizationTools import enableFactorization_makeZtoElecTauPlots
enableFactorization_makeZtoElecTauPlots(process)

# import function for modifying parameters of individual plots
from TauAnalysis.Configuration.tools.modifyZtoElecTauPlots import modifyPlots
modifyPlots(process)

# load only signal and gamma+jets background
#from TauAnalysis.Configuration.plotZtoElecTau_extras_cfi import loadSignalAndBgd
#loadSignalAndBgd(process)

process.makeZtoElecTauPlots = cms.Sequence(
		process.loadZtoElecTau
		+ process.addZtoElecTau
		+ process.saveZtoElecTau 
		+ process.dumpZtoElecTau
		+ process.plotZtoElecTau
)

process.p = cms.Path(process.makeZtoElecTauPlots)

# print-out all python configuration parameter information
#print process.dumpPython()
