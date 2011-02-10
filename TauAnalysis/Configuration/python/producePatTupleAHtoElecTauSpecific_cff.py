import FWCore.ParameterSet.Config as cms

from TauAnalysis.Configuration.producePatTupleZtoElecTauSpecific_cff import *

#--------------------------------------------------------------------------------
# produce all objects specific to MSSM Higgs A/H --> electron + tau-jet channel
# which do **not** get stored in PATTuple
#--------------------------------------------------------------------------------
#
# produce collections of electron + tau-jet pairs
# passing different selection criteria
#
from TauAnalysis.CandidateTools.elecTauPairSelectionForAHtoElecTau_cff import *
#
# produce data-formats providing information about compatibility
# of an electron + tau-jet pair with the hypothesis
# of being an electron pair resulting from a Z --> e+ e- decay
#
from TauAnalysis.RecoTools.elecTauPairZeeHypothesisForAHtoElecTau_cff import *
#
# produce collections of central jets
# passing b-tagging discriminator
#
from TauAnalysis.RecoTools.patJetSelectionForAHtoElecTau_cff import *

producePatTupleAHtoElecTauSpecific = cms.Sequence(
	producePatTupleZtoElecTauSpecific
	+ selectElecTauPairsForAHtoElecTau + selectElecTauPairsForAHtoElecTauLooseElectronIsolation
	+ produceElecTauPairZeeHypothesesForAHtoElecTau + produceElecTauPairZeeHypothesesForAHtoElecTauLooseElectronIsolation
	+ selectPatJetsForAHtoElecTau + selectPatJetsForAHtoElecTauLooseElectronIsolation
)

