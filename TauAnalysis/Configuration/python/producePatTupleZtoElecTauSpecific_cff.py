import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# produce all objects specific to Z --> electron + tau-jet channel
# which do **not** get stored in PATTuple
#--------------------------------------------------------------------------------

#
# produce collections of pat::Electrons, pat::Muons and
# pat::(PF)Taus passing different selection criteria
#
from TauAnalysis.RecoTools.patLeptonSelection_cff import *
#
# produce collection of pat::Jets passing Et threshold and
# Eta acceptance cuts and not overlapping with any object
# passing selection criteria for pat::Electron, pat::Muon or pat::(PF)Tau
# (pat::Jet collection to be considered for central-jet veto)
#
from TauAnalysis.RecoTools.patJetSelection_cff import *
#
# produce collections of pat::(Calo)MET objects
# passing different selection criteria
#
from TauAnalysis.RecoTools.patMetSelection_cff import *
#
# produce collections of electron + tau-jet pairs
# passing different selection criteria
#
from TauAnalysis.CandidateTools.elecTauPairProduction_cff import *
from TauAnalysis.CandidateTools.diTauPairSelectionAllKinds_cff import *
#
# produce data-formats providing information about compatibility
# of an electron + tau-jet pair with the hypothesis
# of being an electron pair resulting from a Z --> e+ e- decay
#
from TauAnalysis.RecoTools.elecTauPairZeeHypothesis_cff import *
#
# produce PF charged hadrons with IP requirement
# and produce electron pairs for alternate Z->ee veto 
from TauAnalysis.RecoTools.diElecPairZeeHypothesis_cff import *

producePatTupleZtoElecTauSpecific = cms.Sequence(
		#patMuonsMuScleFitCorrectedMomentum +
	selectedPfCandidatesIpCut +
	selectPatMuons  +
    selectPatElectrons 
   + selectPatElectronsLooseIsolation
   + selectPatElectronsForElecTau + selectPatElectronsForElecTauLooseIsolation
   + selectPatTaus 
   + selectPatTausForElecTau
   + produceElecTauPairsAll
   + selectElecTauPairs + selectElecTauPairsLooseElectronIsolation
   + produceElecTauPairZeeHypotheses
   + selectPatJets
   + produceDiElecPairs
)
