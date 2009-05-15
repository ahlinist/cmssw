import FWCore.ParameterSet.Config as cms
import sys

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.wmunuSelFilter_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.wmunuAnalyzer_cfi import *
wmunuAnalyzerAfterFilter = wmunuAnalyzer.clone()

wmunuAnalysis = cms.Path(wmunuAnalyzer)
wmunuAfterFilter = cms.Path(wmunuSelFilter*wmunuAnalyzerAfterFilter)

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpNumberFilter_cfi import *
"""
for i in range(5):
    filter = pileUpNumberFilter.clone(NumberOfPileUpEvents = i)
    globals()['filter%dPU'%i] = filter
    analyzer = wmunuAnalyzer.clone()
    globals()['wmunuAnalyzer%dPU'%i] = analyzer
    globals()['wmunu%dPU'%i] = cms.Path(filter*analyzer)
"""

glob = sys.modules[__name__]
for i in range(5):
    filter = 'filter%dPU'%i
    setattr(glob,filter,pileUpNumberFilter.clone(NumberOfPileUpEvents = i))
    analyzer = 'wmunuAnalyzer%dPU'%i
    setattr(glob,analyzer,wmunuAnalyzer.clone()) 
    setattr(glob,'wmunu%dPU'%i,cms.Path(getattr(glob,filter)*getattr(glob,analyzer)))

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.genParticlesCalo_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.edmDump_cff import *

wmunuSelectionSequence = cms.Sequence(wmunuSelFilter)
edmDumpSequence = cms.Sequence(wmunuSelFilter*genParticlesCalo*edmDumpAll)
