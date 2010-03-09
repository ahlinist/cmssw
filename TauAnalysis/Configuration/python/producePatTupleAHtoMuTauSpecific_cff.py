import FWCore.ParameterSet.Config as cms

from TauAnalysis.Configuration.producePatTupleZtoMuTauSpecific_cff import *

#--------------------------------------------------------------------------------
# produce all objects specific to MSSM Higgs A/H --> muon + tau-jet channel
# which do **not** get stored in PATTuple
#--------------------------------------------------------------------------------
#
# produce collections of muon + tau-jet pairs
# passing different selection criteria
#
from TauAnalysis.CandidateTools.muTauPairSelectionForAHtoMuTau_cff import *
#
# produce data-formats providing information about compatibility
# of an muon + tau-jet pair with the hypothesis
# of being an muon pair resulting from a Z --> mu+ mu- decay
#
from TauAnalysis.RecoTools.muTauPairZmumuHypothesisForAHtoMuTau_cff import *
#
# produce collections of central jets
# passing b-tagging discriminator
#
from TauAnalysis.RecoTools.patJetSelectionForAHtoMuTau_cff import *

producePatTupleAHtoMuTauSpecific = cms.Sequence(
    producePatTupleZtoMuTauSpecific
   + selectMuTauPairsForAHtoMuTau + selectMuTauPairsForAHtoMuTauLooseMuonIsolation
   + produceMuTauPairZmumuHypothesesForAHtoMuTau + produceMuTauPairZmumuHypothesesForAHtoMuTauLooseMuonIsolation
   + selectLayer1JetsForAHtoMuTau + selectLayer1JetsForAHtoMuTauLooseMuonIsolation
)
