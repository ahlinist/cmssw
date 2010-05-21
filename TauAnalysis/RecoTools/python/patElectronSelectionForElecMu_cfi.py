import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.RecoTools.patElectronSelection_cfi import *

# require electron candidate not to overlap with selected muons
# (in order to avoid double-counting one and the same physical particle
#  as muon and as electron candidate)
selectedPatElectronsForElecMuAntiOverlapWithMuonsVeto = cms.EDFilter("PATElectronAntiOverlapSelector",
    srcNotToBeFiltered = cms.VInputTag("selectedPatMuonsGlobalIndividual"),
    dRmin = cms.double(0.3),
    filter = cms.bool(False)                                           
)

# require electron candidate to pass the tight electron id. criteria
selectedPatElectronsForElecMuTightId = copy.deepcopy(selectedPatElectronsTightId)

# require electron candidate to not be within eta-crack
# between Barrel and Encap ECAL calorimeter
selectedPatElectronsForElecMuAntiCrackCut = copy.deepcopy(selectedPatElectronsAntiCrackCut)

# require electron candidate to be within geometric acceptance of electron trigger
selectedPatElectronsForElecMuEta21 = copy.deepcopy(selectedPatElectronsEta21)

# require electron candidate to have transverse momentum above threshold
selectedPatElectronsForElecMuPt15 = copy.deepcopy(selectedPatElectronsPt15)

# require electron candidate to be isolated
# with respect to tracks (of Pt >~ 0.3 GeV)
selectedPatElectronsForElecMuTrkIso = copy.deepcopy(selectedPatElectronsTrkIso)

# require electron candidate to be isolated
# with respect to energy deposits in ECAL
# (not associated to electron candidate)
selectedPatElectronsForElecMuEcalIso = copy.deepcopy(selectedPatElectronsEcalIso)

# require electron candidate to be linked to (GSF) track
selectedPatElectronsForElecMuTrk = copy.deepcopy(selectedPatElectronsTrk)

# require track of electron candidate to have small transverse impact parameter
# (in order to veto electrons resulting from b-quark decays)
selectedPatElectronsForElecMuTrkIP = copy.deepcopy(selectedPatElectronsTrkIP)

#--------------------------------------------------------------------------------
# define additional collections of electron candidates
# with loose track and ECAL isolation applied
#
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of electron isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)
#--------------------------------------------------------------------------------

selectedPatElectronsForElecMuTrkIsoLooseIsolation = copy.deepcopy(selectedPatElectronsForElecMuTrkIso)
selectedPatElectronsForElecMuTrkIsoLooseIsolation.cut = cms.string('trackIso < 8.')

selectedPatElectronsForElecMuEcalIsoLooseIsolation = copy.deepcopy(selectedPatElectronsForElecMuEcalIso)
selectedPatElectronsForElecMuEcalIsoLooseIsolation.cut = cms.string('ecalIso < 8.')

selectedPatElectronsForElecMuTrkLooseIsolation = copy.deepcopy(selectedPatElectronsForElecMuTrk)

selectedPatElectronsForElecMuTrkIPlooseIsolation = copy.deepcopy(selectedPatElectronsForElecMuTrkIP)

