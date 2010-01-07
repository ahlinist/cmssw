import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# produce all objects which get stored in PATTuple specific to data-driven background estimation methods
#--------------------------------------------------------------------------------

from TauAnalysis.BgEstimationTools.patMuonSelectionForBgEst_cff import *
from TauAnalysis.BgEstimationTools.patPFTauSelectionForBgEst_cff import *

from TauAnalysis.BgEstimationTools.muTauPairProductionForBgEst_cff import *

from TauAnalysis.BgEstimationTools.patJetSelectionForBgEst_cff import *
from TauAnalysis.BgEstimationTools.patJetSelectionForZtoMuTauBgEst_cff import *

producePatTupleZtoMuTauSpecificForBgEst = cms.Sequence(
    selectMuonsForBgEst
   * selectTausForBgEst
   * produceMuTauPairsForBgEst
   * selectJetsForBgEst * selectJetsForZtoMuTauBgEst
)
