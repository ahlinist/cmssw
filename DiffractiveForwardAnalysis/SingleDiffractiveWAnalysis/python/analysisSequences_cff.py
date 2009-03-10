import FWCore.ParameterSet.Config as cms

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.wmunuSelFilter_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.wmunuAnalyzer_cfi import *
wmunuAnalyzerAfterFilter = wmunuAnalyzer.clone()

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpNumberFilter_cfi import *
filter0PU = pileUpNumberFilter.clone(NumberOfPileUpEvents = 0)
filter1PU = pileUpNumberFilter.clone(NumberOfPileUpEvents = 1)
filter2PU = pileUpNumberFilter.clone(NumberOfPileUpEvents = 2)
filter3PU = pileUpNumberFilter.clone(NumberOfPileUpEvents = 3)
filter4PU = pileUpNumberFilter.clone(NumberOfPileUpEvents = 4)

wmunuAnalyzer0PU = wmunuAnalyzer.clone()
wmunuAnalyzer1PU = wmunuAnalyzer.clone()
wmunuAnalyzer2PU = wmunuAnalyzer.clone()
wmunuAnalyzer3PU = wmunuAnalyzer.clone()
wmunuAnalyzer4PU = wmunuAnalyzer.clone()

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.genParticlesCalo_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.edmDump_cff import *

wmunu0PU = cms.Sequence(filter0PU*wmunuAnalyzer0PU)
wmunu1PU = cms.Sequence(filter1PU*wmunuAnalyzer1PU)
wmunu2PU = cms.Sequence(filter2PU*wmunuAnalyzer2PU)
wmunu3PU = cms.Sequence(filter3PU*wmunuAnalyzer3PU)
wmunu4PU = cms.Sequence(filter4PU*wmunuAnalyzer4PU)
wmunuAfterFilter = cms.Sequence(wmunuSelFilter*wmunuAnalyzerAfterFilter)
wmunuAnalysisSequence = cms.Sequence(wmunuAnalyzer + 
                                     wmunu0PU + 
                                     wmunu1PU +
                                     wmunu2PU +
                                     wmunu3PU +
                                     wmunu4PU +
                                     wmunuAfterFilter)
wmunuSelectionSequence = cms.Sequence(wmunuSelFilter)
edmDumpSequence = cms.Sequence(wmunuSelFilter*genParticlesCalo*edmDumpAllNoPileUp)
