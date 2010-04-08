import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.RecoTools.patElectronSelection_cfi import *

#--------------------------------------------------------------------------------
# define additional collections of electron candidates 
# to be used specifically for the electron + tau-jet analysis
#--------------------------------------------------------------------------------

# require electron candidate to pass the tight electron id. criteria
selectedLayer1ElectronsForElecTauTightId = copy.deepcopy(selectedLayer1ElectronsTightId)

# require electron candidate to pass the loose electron id. criteria
selectedLayer1ElectronsForElecTauLooseId = copy.deepcopy(selectedLayer1ElectronsLooseId)

# require electron candidate to not be within eta-crack
# between Barrel and Encap ECAL calorimeter
selectedLayer1ElectronsForElecTauAntiCrackCut = copy.deepcopy(selectedLayer1ElectronsAntiCrackCut)

# require electron candidate to be within geometric acceptance of electron trigger
selectedLayer1ElectronsForElecTauEta21 = copy.deepcopy(selectedLayer1ElectronsEta21)

# require electron candidate to have transverse momentum above threshold
selectedLayer1ElectronsForElecTauPt15 = copy.deepcopy(selectedLayer1ElectronsPt15)

# require electron candidate to be isolated
# with respect to tracks (of Pt >~ 0.3 GeV)
selectedLayer1ElectronsForElecTauTrkIso = copy.deepcopy(selectedLayer1ElectronsTrkIso)

# require electron candidate to be isolated
# with respect to energy deposits in ECAL
# (not associated to electron candidate)
selectedLayer1ElectronsForElecTauEcalIso = copy.deepcopy(selectedLayer1ElectronsEcalIso)

# require electron candidate to be linked to (GSF) track
selectedLayer1ElectronsForElecTauTrk = copy.deepcopy(selectedLayer1ElectronsTrk)

# require track of electron candidate to have small transverse impact parameter
# (in order to veto electrons resulting from b-quark decays)
selectedLayer1ElectronsForElecTauTrkIP = copy.deepcopy(selectedLayer1ElectronsTrkIP)

selectedLayer1ElectronsForElecTauTrkIsoLooseIsolation = copy.deepcopy(selectedLayer1ElectronsForElecTauTrkIso)
selectedLayer1ElectronsForElecTauTrkIsoLooseIsolation.cut = cms.string('trackIso < 8.')

selectedLayer1ElectronsForElecTauEcalIsoLooseIsolation = copy.deepcopy(selectedLayer1ElectronsForElecTauEcalIso)
selectedLayer1ElectronsForElecTauEcalIsoLooseIsolation.cut = cms.string('ecalIso < 8.')

selectedLayer1ElectronsForElecTauTrkLooseIsolation = copy.deepcopy(selectedLayer1ElectronsForElecTauTrk)

selectedLayer1ElectronsForElecTauTrkIPlooseIsolation = copy.deepcopy(selectedLayer1ElectronsForElecTauTrkIP)


# require electrons not to originate from photon conversions
selectedLayer1ElectronsForElecTauConversionVeto = cms.EDFilter("PATElectronConversionFinder",
    trackSource = cms.InputTag('generalTracks'),
    conversionSource = cms.InputTag('conversions'),
	photonSource = cms.InputTag('photons'),
    cotThetaCut = cms.double(0.05),
    docaElecTrack = cms.double(0),
    dRElecTrack = cms.double(0.1),
    doPixCut = cms.bool(True),
    useInnerParsForElec = cms.bool(True),
    useInnerParsForTrks = cms.bool(True),
    useConversionColl = cms.bool(True),
    nTrkMax = cms.double(1),
    doHists = cms.bool(False)
)

selectedLayer1ElectronsForElecTauConversionVetoLooseIsolation = copy.deepcopy(selectedLayer1ElectronsForElecTauConversionVeto)

