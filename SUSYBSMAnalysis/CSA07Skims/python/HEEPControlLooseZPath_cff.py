import FWCore.ParameterSet.Config as cms

#
# HEEP skim, control sample
#
# reuse the EWK loose Z chain, but with tighter mass cut
from ElectroWeakAnalysis.ZReco.zToEESequences_cff import *
from SUSYBSMAnalysis.CSA07Skims.HEEPControlLooseZSequence_cff import *
electronRecoForZToEEPath = cms.Path(electronRecoForZToEE)
HEEPControlLooseZTrack = cms.Path(HEEPControlZOneTrackReco)
HEEPControlLooseZCluster = cms.Path(HEEPControlZOneSuperClusterReco)

