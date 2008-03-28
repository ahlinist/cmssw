import FWCore.ParameterSet.Config as cms

#
# HEEP skim, control sample
#
# reuse the EWK loose Z chain, but with tighter mass cut
# 
from SUSYBSMAnalysis.CSA07Skims.HEEPControlTrigger_cff import *
#
# Redo EWK sequence "electron + track with tighter cut"
#
from SUSYBSMAnalysis.CSA07Skims.HEEPControlZOneTrackEWKConf_cff import *
from SUSYBSMAnalysis.CSA07Skims.HEEPControlZOneSuperClusterEWKConf_cff import *
HEEPControlZOneTrackReco = cms.Sequence(HEEPControlTrigger+HEEPControlZOneTrack+HEEPControlZOneTrackGenParticlesMatch+HEEPControlZOneTrackFilter)
HEEPControlZOneSuperClusterReco = cms.Sequence(HEEPControlTrigger+HEEPControlZOneSuperCluster+HEEPControlZOneSuperClusterGenParticlesMatch+HEEPControlZOneSuperClusterFilter)

