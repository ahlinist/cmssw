import FWCore.ParameterSet.Config as cms

process = cms.Process('makeZtoElecTauPlots')

process.load("TauAnalysis.Configuration.dumpZtoElecTau_cff")
process.load("TauAnalysis.Configuration.plotZtoElecTau_cff")

# need this to get intLumiZtoElecTau_Data
from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecTau_cfi import *
process.plotZtoElecTau.labels.mcNormScale.text = cms.vstring(str(intLumiZtoElecTau_Data) + 'pb^{-1}','#sqrt{s}=7TeV')

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

# define directory from which .root files containing the histograms get loaded
process.loadZtoElecTau.inputFilePath = cms.string("rfio:/castor/cern.ch/user/j/jkolb/elecTauAnalysis/fall10/current/")
#process.loadZtoElecTau.inputFilePath = cms.string("file:/data/ndpc2/b/jkolb/ZtoElecTauAnalysis/fall10/analysis/harvested/")

# import utility function to enable factorization
from TauAnalysis.Configuration.tools.factorizationTools import enableFactorization_makeZtoElecTauPlots
enableFactorization_makeZtoElecTauPlots(process)

# import function for modifying parameters of individual plots
from TauAnalysis.Configuration.tools.modifyZtoElecTauPlots import modifyPlots
modifyPlots(process)

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
