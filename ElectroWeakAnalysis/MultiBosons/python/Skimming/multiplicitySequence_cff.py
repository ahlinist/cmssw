import FWCore.ParameterSet.Config as cms

from ElectroWeakAnalysis.MultiBosons.Skimming.trackMultiplicities_cfi import *
from ElectroWeakAnalysis.MultiBosons.Skimming.electronSeedMultiplicity_cfi import *

multiplicitySequence = cms.Sequence( gsfTrackMultiplicities +
                                     trackMultiplicities +
                                     electronSeedMultiplicity )
