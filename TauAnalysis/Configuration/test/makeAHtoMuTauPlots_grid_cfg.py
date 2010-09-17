import FWCore.ParameterSet.Config as cms

INPUT_FILES_DIR = '/data1/friis'
sample_mapper = lambda sample : "harvested_AHtoMuTau_%s_Run7.root" % sample

process = cms.Process('makeAHtoMuTauPlots')

import TauAnalysis.Configuration.recoSampleDefinitionsAHtoMuTau_7TeV_grid_cfi as samples

process.load("TauAnalysis.Configuration.dumpAHtoMuTau_grid_cff")
process.load("TauAnalysis.Configuration.plotAHtoMuTau_grid_cff")

process.DQMStore = cms.Service("DQMStore")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(0)         
)

process.source = cms.Source("EmptySource")

# define directory from which .root files containing histograms 
# for individual processes get loaded
process.loadAHtoMuTauSamples.inputFilePath = cms.string(INPUT_FILES_DIR)

# Update our input files
for sample in samples.FLATTENED_SAMPLES_TO_PLOT:
    getattr(process.loadAHtoMuTauSamples, sample).inputFileNames = cms.vstring(
        sample_mapper(sample)
    )

# import utility function to enable factorization
from TauAnalysis.Configuration.tools.factorizationTools import enableFactorization_makeAHtoMuTauPlots_grid

samplesToFactorize = [sample for sample in samples.FLATTENED_SAMPLES_TO_PLOT 
                      if samples.ALL_SAMPLES[sample].get('factorize', False)]

relevantMergedSamples = [sample for sample, sample_info in samples.MERGE_SAMPLES.iteritems() if 
                         [subsample for subsample in sample_info['samples'] 
                          if subsample in samplesToFactorize]]

print "Factorizing", samplesToFactorize
print "Updating", relevantMergedSamples

enableFactorization_makeAHtoMuTauPlots_grid(
    process,
    samplesToFactorize = samplesToFactorize,
    relevantMergedSamples = relevantMergedSamples,
    mergedToRecoSampleDict = samples.MERGE_SAMPLES,
)

process.dumpDQMStore = cms.EDAnalyzer("DQMStoreDump")

# define name and directory in which .root file containing all histograms gets saved
process.saveAHtoMuTau.outputFileName = cms.string(
    "/data1/friis/Run7/plotsAHtoMuTau_all.root"
)

process.makeAHtoMuTauPlots = cms.Sequence(
    process.loadAHtoMuTau
   #+ process.dumpDQMStore
   + process.saveAHtoMuTau
   + process.dumpAHtoMuTau
   + process.plotAHtoMuTau
)

process.p = cms.Path(process.makeAHtoMuTauPlots)

# print-out all python configuration parameter information
#print process.dumpPython()
