import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# produce all objects specific to Z --> electron + muon channel
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
# produce collections of electron + muon pairs
# passing different selection criteria
#
from TauAnalysis.CandidateTools.elecMuPairProduction_cff import *
from TauAnalysis.CandidateTools.diTauPairSelectionAllKinds_cff import *
#
# produce data-formats providing information about compatibility
# of an electron + muon pair with the hypothesis
# of being an electron pair/muon pair resulting from a Z --> e+ e-/Z --> mu+ mu- decay
#
from TauAnalysis.RecoTools.elecMuPairZmumuHypothesis_cff import *

producePatTupleZtoElecMuSpecific = cms.Sequence(
    selectLayer1Electrons + selectLayer1ElectronsLooseIsolation
   + selectLayer1ElectronsForElecMu + selectLayer1ElectronsForElecMuLooseIsolation
   + selectLayer1Muons + selectLayer1MuonsLooseIsolation
   + produceElecMuPairs + produceElecMuPairsLooseElectronIsolation
   + selectElecMuPairs + selectElecMuPairsLooseElectronIsolation
   + produceElecMuPairZmumuHypotheses
   + selectLayer1Jets
)
