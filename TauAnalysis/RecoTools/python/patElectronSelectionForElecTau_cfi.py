import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.RecoTools.patElectronSelection_cfi import *
from TauAnalysis.RecoTools.patLeptonPFIsolationSelector_cfi import patElectronPFIsolationSelector

#--------------------------------------------------------------------------------
# define additional collections of electron candidates 
# to be used specifically for the electron + tau-jet analysis
#--------------------------------------------------------------------------------

# require electron candidate to pass the tight electron id. criteria
selectedPatElectronsForElecTauTightId = copy.deepcopy(selectedPatElectronsTightId)

# require electron candidate to pass the loose electron id. criteria
selectedPatElectronsForElecTauLooseId = copy.deepcopy(selectedPatElectronsLooseId)

# require electron candidate to pass the electron id. criteria 
selectedPatElectronsForElecTauId = copy.deepcopy(selectedPatElectronsLooseId)

# require electron candidate to not be within eta-crack
# between Barrel and Encap ECAL calorimeter
selectedPatElectronsForElecTauAntiCrackCut = copy.deepcopy(selectedPatElectronsAntiCrackCut)

# require electron candidate to be within geometric acceptance of electron trigger
selectedPatElectronsForElecTauEta = copy.deepcopy(selectedPatElectronsEta21)

# require electron candidate to have transverse momentum above threshold
selectedPatElectronsForElecTauPt = copy.deepcopy(selectedPatElectronsPt15)

# require electron candidate to be isolated
# with respect to particle-flow candidates
selectedPatElectronsForElecTauIso = cms.EDFilter("PATElectronPFIsolationSelector",
	patElectronPFIsolationSelector,
	filter = cms.bool(False)
)

# require track of electron candidate to have small transverse impact parameter
# (in order to veto electrons resulting from b-quark decays)
selectedPatElectronsForElecTauTrkIP = copy.deepcopy(selectedPatElectronsTrkIP)


#
# make collections with loose electron isolation
#
selectedPatElectronsForElecTauIsoLooseIsolation = selectedPatElectronsForElecTauIso.clone(
	sumPtMax = cms.double(0.30)
)

selectedPatElectronsForElecTauTrkIPlooseIsolation = copy.deepcopy(selectedPatElectronsForElecTauTrkIP)


# require electrons not to originate from photon conversions
#
selectedPatElectronsForElecTauConversionVeto = cms.EDFilter("NPATElectronConversionFinder",
    maxMissingInnerHits = cms.int32(0),
    minMissingInnerHits = cms.int32(0),
    minRxy = cms.double(2.0),
    minFitProb = cms.double(1e-6),
    maxHitsBeforeVertex = cms.int32(0),
    invertConversionVeto = cms.bool(False)
)

# loosen conversion rejection as part of loose isolation
selectedPatElectronsForElecTauConversionVetoLooseIsolation = selectedPatElectronsForElecTauConversionVeto.clone(
    maxMissingInnerHits = cms.int32(2),
    invertConversionVeto = cms.bool(True)
)

