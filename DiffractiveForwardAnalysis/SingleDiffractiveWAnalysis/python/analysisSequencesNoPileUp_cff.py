import FWCore.ParameterSet.Config as cms

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.wmunuSelFilter_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.wmunuAnalyzer_cfi import *
wmunuAnalyzerAfterFilter = wmunuAnalyzer.clone()

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.genParticlesCalo_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.edmDump_cff import *

wmunuAnalysis = cms.Path(wmunuAnalyzer)
wmunuAfterFilter = cms.Path(wmunuSelFilter*wmunuAnalyzerAfterFilter)

wmunuSelectionSequence = cms.Sequence(wmunuSelFilter)
edmDumpSequence = cms.Sequence(wmunuSelFilter*genParticlesCalo*edmDumpAllNoPileUp)
