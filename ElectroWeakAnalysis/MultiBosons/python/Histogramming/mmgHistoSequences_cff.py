import FWCore.ParameterSet.Config as cms

from ElectroWeakAnalysis.MultiBosons.Histogramming.muonHistos_cfi import *
from ElectroWeakAnalysis.MultiBosons.Histogramming.photonHistos_cfi import *
from ElectroWeakAnalysis.MultiBosons.Histogramming.mmgHistos_cfi import *

mmgHistoSequence = cms.Sequence(muonHistos + photonHistos + mmgHistos)