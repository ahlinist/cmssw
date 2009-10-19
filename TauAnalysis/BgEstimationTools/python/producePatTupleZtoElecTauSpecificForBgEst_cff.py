import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# produce all objects which get stored in PATTuple specific to data-driven background estimation methods
#--------------------------------------------------------------------------------

from TauAnalysis.BgEstimationTools.patElectronSelectionForBgEst_cff import *
#from TauAnalysis.BgEstimationTools.patMuonSelectionForBgEst_cff import *
from TauAnalysis.BgEstimationTools.patPFTauSelectionForBgEst_cff import *
#from TauAnalysis.RecoTools.patLeptonSelection_cff import *

from TauAnalysis.BgEstimationTools.elecTauPairProductionForBgEst_cff import *
from TauAnalysis.BgEstimationTools.elecTauPairZeeHypothesisForBgEst_cff import *

from TauAnalysis.BgEstimationTools.patJetSelectionForBgEst_cff import *
from TauAnalysis.BgEstimationTools.patJetSelectionForZtoElecTauBgEst_cff import *

producePatTupleZtoElecTauSpecificForBgEst = cms.Sequence(
    selectElectronsForBgEst
#   * selectLayer1MuonsLooseIsolation * selectMuonsForBgEst
   * selectTausForBgEst
   * produceElecTauPairsForBgEst
   * produceElecTauPairZeeHypothesesForBgEst
   * selectJetsForBgEst * selectJetsForZtoElecTauBgEst
)
