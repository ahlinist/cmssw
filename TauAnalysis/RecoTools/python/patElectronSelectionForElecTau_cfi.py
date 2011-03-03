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
# NOTE: EG POG method only pairs OS tracks, hence nConvPairMax is typically 0
#       the old method( usePogMethod = False ) partners all tracks, therefore nConvPairMax is typically 1 
#
selectedPatElectronsForElecTauConversionVeto = cms.EDFilter("PATElectronConversionFinder",
    trackSource = cms.InputTag('generalTracks'),
    conversionSource = cms.InputTag('conversions'),
	photonSource = cms.InputTag('photons'),
	dcsTag = cms.InputTag('scalersRawToDigi'),
	usePogMethod = cms.bool(False),
    cotThetaMax = cms.double(0.05),
	docaElecTrackMax = cms.double(0.1),
    dRElecTrackMax = cms.double(0.1),
    doPixCut = cms.bool(True),
	doMissingHitsCut = cms.bool(False),
    useConversionColl = cms.bool(True),
    nConvPairMax = cms.double(1),
	nConvPairMin = cms.double(0),
	useOnlyOSPairs = cms.bool(False),
	isData = cms.bool(False)
)

# loosen conversion rejection as part of loose isolation
selectedPatElectronsForElecTauConversionVetoLooseIsolation = selectedPatElectronsForElecTauConversionVeto.clone(
    nConvPairMax = cms.double(5),
    nConvPairMin = cms.double(0)
)

