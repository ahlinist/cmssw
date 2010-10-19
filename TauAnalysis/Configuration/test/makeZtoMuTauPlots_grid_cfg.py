import FWCore.ParameterSet.Config as cms

INPUT_FILES_DIR = '/data1/veelken/CMSSW_3_6_x/plots/ZtoMuTau'
sample_mapper = lambda sample : "harvested_ZtoMuTau_%s_7TeV.root" % sample

process = cms.Process('makeZtoMuTauPlots')

import TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_7TeV_grid_cfi as samples

process.load("TauAnalysis.Configuration.dumpZtoMuTau_grid_cff")
process.load("TauAnalysis.Configuration.plotZtoMuTau_grid_cff")

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

# define directory from which .root files containing histograms 
# for individual processes get loaded
process.loadZtoMuTauSamples.inputFilePath = cms.string(INPUT_FILES_DIR)

# Update our input files
for sample in samples.FLATTENED_SAMPLES_TO_PLOT:
    getattr(process.loadZtoMuTauSamples, sample).inputFileNames = cms.vstring(
        sample_mapper(sample)
    )

# import utility function to enable factorization
from TauAnalysis.Configuration.tools.factorizationTools import enableFactorization_makeZtoMuTauPlots_grid

samplesToFactorize = [sample for sample in samples.FLATTENED_SAMPLES_TO_PLOT 
                      if samples.ALL_SAMPLES[sample].get('factorize', False)]

relevantMergedSamples = [sample for sample, sample_info in samples.MERGE_SAMPLES.iteritems() if 
                         [subsample for subsample in sample_info['samples'] 
                          if subsample in samplesToFactorize]]

print "Factorizing", samplesToFactorize
print "Updating", relevantMergedSamples

enableFactorization_makeZtoMuTauPlots_grid(
    process,
    samplesToFactorize = samplesToFactorize,
    relevantMergedSamples = relevantMergedSamples,
    mergedToRecoSampleDict = samples.MERGE_SAMPLES,
)

process.plotZtoMuTau.labels.mcNormScale.text = cms.vstring(
    '%0.1fpb^{-1}' % samples.TARGET_LUMI,
    '#sqrt{s}=7TeV'
)

process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

# define name and directory in which .root file containing all histograms gets saved
process.saveZtoMuTau.outputFileName = cms.string(
    "/data1/veelken/CMSSW_3_6_x/plots/ZtoMuTau/plotsZtoMuTau_all.root"
)

process.makeZtoMuTauPlots = cms.Sequence(
    process.loadZtoMuTau
   #+ process.dumpDQMStore
   + process.saveZtoMuTau
   + process.dumpZtoMuTau
   + process.plotZtoMuTau
)

process.p = cms.Path(process.makeZtoMuTauPlots)

# print-out all python configuration parameter information
#print process.dumpPython()
