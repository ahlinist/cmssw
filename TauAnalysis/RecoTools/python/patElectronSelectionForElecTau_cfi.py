import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.RecoTools.patElectronSelection_cfi import *
from TauAnalysis.RecoTools.patLeptonPFIsolationSelector_cfi import patElectronPFIsolationSelector

#--------------------------------------------------------------------------------
# define additional collections of electron candidates 
# to be used specifically for the electron + tau-jet analysis
#--------------------------------------------------------------------------------

# require electron candidate to pass pre-selection for ID MVA
selectedPatElectronsForElecTauPreId = copy.deepcopy(selectedPatElectronsTightId)

# require electron candidate to pass MVA-based ID
selectedPatElectronsForElecTauId = cms.EDFilter("PATElectronIdSelector",
    srcVertex = cms.InputTag("selectedPrimaryVertexHighestPtTrackSum"),
    cut = cms.string("tight"),                                              
    filter = cms.bool(False)
)                                                

# tmp for testing
##selectedPatElectronsForElecTauId = cms.EDFilter("PATElectronSelector",
##    cut = cms.string(''),
##    filter = cms.bool(False)
##)

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

# require electrons not to originate from photon conversions
selectedPatElectronsForElecTauConversionVeto = cms.EDFilter("NPATElectronConversionFinder",
    maxMissingInnerHits = cms.int32(0),
    minMissingInnerHits = cms.int32(0),
    minRxy = cms.double(2.0),
    minFitProb = cms.double(1e-6),
    maxHitsBeforeVertex = cms.int32(0),
    invertConversionVeto = cms.bool(False)
)

#
# make collections with loose electron isolation
#
selectedPatElectronsForElecTauIsoLooseIsolation = selectedPatElectronsForElecTauIso.clone(
    sumPtMax = cms.double(0.30)
)

selectedPatElectronsForElecTauTrkIPlooseIsolation = copy.deepcopy(selectedPatElectronsForElecTauTrkIP)

# loosen conversion rejection as part of loose isolation
selectedPatElectronsForElecTauConversionVetoLooseIsolation = selectedPatElectronsForElecTauConversionVeto.clone(
    maxMissingInnerHits = cms.int32(2),
    invertConversionVeto = cms.bool(True)
)

