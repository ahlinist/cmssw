import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# produce all objects which get stored in PATTuple specific to data-driven background estimation methods
#--------------------------------------------------------------------------------

#from TauAnalysis.BgEstimationTools.patElectronSelectionForBgEst_cff import *
from TauAnalysis.BgEstimationTools.patMuonSelectionForBgEst_cff import *
from TauAnalysis.BgEstimationTools.patPFTauSelectionForBgEst_cff import *
#from TauAnalysis.RecoTools.patLeptonSelection_cff import *

from TauAnalysis.BgEstimationTools.muTauPairProductionForBgEst_cff import *

from TauAnalysis.BgEstimationTools.patJetSelectionForBgEst_cff import *
from TauAnalysis.BgEstimationTools.patJetSelectionForZtoMuTauBgEst_cff import *

producePatTupleForBgEst = cms.Sequence(
#    selectLayer1ElectronsLooseIsolation * selectElectronsForBgEst 
    selectMuonsForBgEst
   * selectTausForBgEst
   * produceMuTauPairsForBgEst
   * selectJetsForBgEst * selectJetsForZtoMuTauBgEst
)
