import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# produce objects that are needed for TauAnalysis specific PAT layer 1 customizations
# (e.g. non-standard electron and muon isolations)
#--------------------------------------------------------------------------------

from TauAnalysis.RecoTools.recoElectronIdentification_cfi import *
from TauAnalysis.RecoTools.recoElectronIsolation_cfi import *

producePrePat = cms.Sequence( electronIdCutBased + electronIsoDeposits )

