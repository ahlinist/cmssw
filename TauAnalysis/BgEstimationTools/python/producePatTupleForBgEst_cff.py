import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# produce all objects which get stored in PATTuple specific to data-driven background estimation methods
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.producePatTuple_cff import *

from TauAnalysis.BgEstimationTools.patElectronSelectionForBgEst_cff import *
from TauAnalysis.BgEstimationTools.patMuonSelectionForBgEst_cff import *
from TauAnalysis.BgEstimationTools.patPFTauSelectionForBgEst_cff import *

#from TauAnalysis.BgEstimationTools.elecMuPairProductionForBgEst_cff import *
from TauAnalysis.BgEstimationTools.elecTauPairProductionForBgEst_cff import *
from TauAnalysis.BgEstimationTools.muTauPairProductionForBgEst_cff import *

from TauAnalysis.BgEstimationTools.patJetSelectionForBgEst_cff import *

producePatTupleForBgEst = cms.Sequence(
    producePatTuple
   * selectElectronsForBgEst * selectMuonsForBgEst * selectTausForBgEst
   * produceElecTauPairsForBgEst
   * produceMuTauPairsForBgEst
   #* produceElecMuPairsForBgEst
   * selectJetsForBgEst
)
