import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# define resolutions shared by different analysis channels
#--------------------------------------------------------------------------------

pfMEtResolutionPx = cms.string("0.70*sqrt(x)")
pfMEtResolutionPy = pfMEtResolutionPx

