import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------  
# produce collections of electron + tau-jet pairs passing selection criteria
#--------------------------------------------------------------------------------

# require electron and tau-jet to be separated in eta-phi,
# in order to ensure that both do not refer to one and the same physical particle
selectedElecTauPairsAntiOverlapVeto = cms.EDFilter("PATElecTauPairSelector",
    cut = cms.string('dR12 > 0.7'),
    filter = cms.bool(False)
)

# require low transverse mass of electron and MET
selectedElecTauPairsMt1MET = cms.EDFilter("PATElecTauPairSelector",
    cut = cms.string('mt1MET < 50.'),
    filter = cms.bool(False)
)

# require missing transverse momentum to point
# in the direction of visible tau decay products
selectedElecTauPairsPzetaDiff = cms.EDFilter("PATElecTauPairSelector",
    cut = cms.string('(pZeta - 1.5*pZetaVis) > -20.'),
    filter = cms.bool(False)
)

# require electron and tau to form a zero-charge pair
selectedElecTauPairsZeroCharge = cms.EDFilter("PATElecTauPairSelector",
    cut = cms.string('charge = 0'),
    filter = cms.bool(False)
)

# require electron and tau to form a non-zero-charge pair
selectedElecTauPairsNonZeroCharge = cms.EDFilter("PATElecTauPairSelector",
    cut = cms.string('charge != 0'),
    filter = cms.bool(False)
)

# require electron and tau to be back-to-back
#
# NOTE:
#  (1) this cut is introduced into the cut-flow as a place-holder only,
#      to be used as backup in case MET cannot be relied upon in the early data
#  (2) in case the cut on electron-tau-jet acoplanarity is applied
#      there will be no events left for which the invariant e + tau mass
#      can be reonstructed by collinear approximation
#selectedElecTauPairsAcoplanarity12 = cms.EDFilter("PATElecTauPairSelector",
#    cut = cms.string('cos(dPhi12) > -1.01'),
#    #cut = cms.string('cos(dPhi12) < -0.95'),                                            
#    filter = cms.bool(False)
#)

# define additional collections of electron + tau-jet candidates
# with loose track and ECAL isolation applied on electron leg
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of electron isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)

selectedElecTauPairsAntiOverlapVetoLooseElectronIsolation = copy.deepcopy(selectedElecTauPairsAntiOverlapVeto)

#selectedElecTauPairsAcoplanarity12LooseElectronIsolation = copy.deepcopy(selectedElecTauPairsAcoplanarity12)

selectedElecTauPairsMt1METlooseElectronIsolation = copy.deepcopy(selectedElecTauPairsMt1MET)

selectedElecTauPairsPzetaDiffLooseElectronIsolation = copy.deepcopy(selectedElecTauPairsPzetaDiff)

selectedElecTauPairsZeroChargeLooseElectronIsolation = copy.deepcopy(selectedElecTauPairsZeroCharge)

selectedElecTauPairsNonZeroChargeLooseElectronIsolation = copy.deepcopy(selectedElecTauPairsZeroCharge)
