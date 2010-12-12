import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# produce all objects specific to Z --> muon + tau-jet channel
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
# produce collections of muon + tau-jet pairs
# passing different selection criteria
#
from TauAnalysis.CandidateTools.muTauPairProduction_cff import *
from TauAnalysis.CandidateTools.diTauPairSelectionAllKinds_cff import *
#
# produce data-formats providing information about compatibility
# of an muon + tau-jet pair with the hypothesis
# of being an muon pair resulting from a Z --> mu+ mu- decay
#
from TauAnalysis.RecoTools.muTauPairZmumuHypothesis_cff import *
#
# produce combinations of muon + muon pairs,
# the hypothesis being that the pair of muons results from a Z --> mu+ mu- decay
#
from TauAnalysis.RecoTools.diMuPairZmumuHypothesis_cff import *

producePatTupleZtoMuTauSpecific = cms.Sequence(
    patMuonsMuScleFitCorrectedMomentum + selectPatMuons + selectPatMuonsLooseIsolation
   + selectPatElectrons
   + selectPatTaus + selectPatTausForMuTau
   + selectPatJets 
   + produceMuTauPairsAll
   + selectMuTauPairs + selectMuTauPairsLooseMuonIsolation
   + produceMuTauPairZmumuHypotheses
   + produceDiMuPairs
)
