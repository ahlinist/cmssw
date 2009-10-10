import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.RecoTools.patElectronSelection_cfi import *

# require electron candidate not to overlap with selected muons
# (in order to avoid double-counting one and the same physical particle
#  as muon and as electron candidate)
selectedLayer1ElectronsForElecMuAntiOverlapWithMuonsVeto = cms.EDFilter("PATElectronAntiOverlapSelector",
    srcNotToBeFiltered = cms.VInputTag("selectedLayer1MuonsGlobalIndividual"),
    dRmin = cms.double(0.3),
    filter = cms.bool(False)                                           
)

# require electron candidate to pass the tight electron id. criteria
selectedLayer1ElectronsForElecMuTightId = copy.deepcopy(selectedLayer1ElectronsTightId)

# require electron candidate to not be within eta-crack
# between Barrel and Encap ECAL calorimeter
selectedLayer1ElectronsForElecMuAntiCrackCut = copy.deepcopy(selectedLayer1ElectronsAntiCrackCut)

# require electron candidate to be within geometric acceptance of electron trigger
selectedLayer1ElectronsForElecMuEta21 = copy.deepcopy(selectedLayer1ElectronsEta21)

# require electron candidate to have transverse momentum above threshold
selectedLayer1ElectronsForElecMuPt15 = copy.deepcopy(selectedLayer1ElectronsPt15)

# require electron candidate to be isolated
# with respect to tracks (of Pt >~ 0.3 GeV)
selectedLayer1ElectronsForElecMuTrkIso = copy.deepcopy(selectedLayer1ElectronsTrkIso)

# require electron candidate to be isolated
# with respect to energy deposits in ECAL
# (not associated to electron candidate)
selectedLayer1ElectronsForElecMuEcalIso = copy.deepcopy(selectedLayer1ElectronsEcalIso)

# require electron candidate to be linked to (GSF) track
selectedLayer1ElectronsForElecMuTrk = copy.deepcopy(selectedLayer1ElectronsTrk)

# require track of electron candidate to have small transverse impact parameter
# (in order to veto electrons resulting from b-quark decays)
selectedLayer1ElectronsForElecMuTrkIP = copy.deepcopy(selectedLayer1ElectronsTrkIP)

#--------------------------------------------------------------------------------
# define additional collections of electron candidates
# with loose track and ECAL isolation applied
#
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of electron isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)
#--------------------------------------------------------------------------------

selectedLayer1ElectronsForElecMuTrkIsoLooseIsolation = copy.deepcopy(selectedLayer1ElectronsForElecMuTrkIso)
selectedLayer1ElectronsForElecMuTrkIsoLooseIsolation.cut = cms.string('trackIso < 8.')

selectedLayer1ElectronsForElecMuEcalIsoLooseIsolation = copy.deepcopy(selectedLayer1ElectronsForElecMuEcalIso)
selectedLayer1ElectronsForElecMuEcalIsoLooseIsolation.cut = cms.string('ecalIso < 8.')

selectedLayer1ElectronsForElecMuTrkLooseIsolation = copy.deepcopy(selectedLayer1ElectronsForElecMuTrk)

selectedLayer1ElectronsForElecMuTrkIPlooseIsolation = copy.deepcopy(selectedLayer1ElectronsForElecMuTrkIP)

