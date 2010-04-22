import FWCore.ParameterSet.Config as cms

from ElectroWeakAnalysis.MultiBosons.Skimming.MuMuGammas_cfi import *
from ElectroWeakAnalysis.MultiBosons.Skimming.MuMuGammasCountFilter_cfi import *

MuMuGammasSequence = cms.Sequence(
  MuMuGammas *
  MuMuGammasCountFilter
)

from ElectroWeakAnalysis.MultiBosons.Skimming.MuMuGammasOneTrack_cfi import *
from ElectroWeakAnalysis.MultiBosons.Skimming.MuMuGammasOneTrackCountFilter_cfi import *

MuMuGammasOneTrackSequence = cms.Sequence(
  MuMuGammasOneTrack *
  MuMuGammasOneTrackCountFilter
)