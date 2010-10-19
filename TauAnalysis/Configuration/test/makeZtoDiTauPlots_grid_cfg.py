import FWCore.ParameterSet.Config as cms

INPUT_FILES_DIR = '/data1/veelken/CMSSW_3_6_x/plots/ZtoDiTau'
sample_mapper = lambda sample : "harvested_ZtoDiTau_%s_7TeV.root" % sample

process = cms.Process('makeZtoDiTauPlots')

import TauAnalysis.Configuration.recoSampleDefinitionsZtoDiTau_7TeV_grid_cfi as samples

process.load("TauAnalysis.Configuration.dumpZtoDiTau_grid_cff")
process.load("TauAnalysis.Configuration.plotZtoDiTau_grid_cff")

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

# define directory from which .root files containing histograms 
# for individual processes get loaded
process.loadZtoDiTauSamples.inputFilePath = cms.string(INPUT_FILES_DIR)

# Update our input files
for sample in samples.FLATTENED_SAMPLES_TO_PLOT:
    getattr(process.loadZtoDiTauSamples, sample).inputFileNames = cms.vstring(
        sample_mapper(sample)
    )

# import utility function to enable factorization
from TauAnalysis.Configuration.tools.factorizationTools import enableFactorization_makeZtoDiTauPlots_grid

samplesToFactorize = [sample for sample in samples.FLATTENED_SAMPLES_TO_PLOT 
                      if samples.ALL_SAMPLES[sample].get('factorize', False)]

relevantMergedSamples = [sample for sample, sample_info in samples.MERGE_SAMPLES.iteritems() if 
                         [subsample for subsample in sample_info['samples'] 
                          if subsample in samplesToFactorize]]

print "Factorizing", samplesToFactorize
print "Updating", relevantMergedSamples

enableFactorization_makeZtoDiTauPlots_grid(
    process,
    samplesToFactorize = samplesToFactorize,
    relevantMergedSamples = relevantMergedSamples,
    mergedToRecoSampleDict = samples.MERGE_SAMPLES,
)

process.plotZtoDiTau.labels.mcNormScale.text = cms.vstring(
    '%0.1fpb^{-1}' % samples.TARGET_LUMI,
    '#sqrt{s}=7TeV'
)

process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

# define name and directory in which .root file containing all histograms gets saved
process.saveZtoDiTau.outputFileName = cms.string(
    "/data1/veelken/CMSSW_3_6_x/plots/ZtoDiTau/plotsZtoDiTau_all.root"
)

process.makeZtoDiTauPlots = cms.Sequence(
    process.loadZtoDiTau
   #+ process.dumpDQMStore
   + process.saveZtoDiTau
   + process.dumpZtoDiTau
   + process.plotZtoDiTau
)

process.p = cms.Path(process.makeZtoDiTauPlots)

# print-out all python configuration parameter information
#print process.dumpPython()
