import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------  
# produce collections of electron + muon pairs passing selection criteria
#--------------------------------------------------------------------------------

# require electron and muon to be separated in space
selectedElecMuPairsAntiOverlapVeto = cms.EDFilter("PATElecMuPairSelector",
     cut = cms.string('dR12 > 0.7'),
     filter = cms.bool(False)
)

# require muon and electron to form a zero-charge pair
selectedElecMuPairsZeroCharge = cms.EDFilter("PATElecMuPairSelector",
     cut = cms.string('charge = 0'),
     filter = cms.bool(False)
)

# require electron and muon to be back-to-back
#
# NOTE:
#  (1) this cut is introduced into the cut-flow as a place-holder only,
#      to be used as backup in case MET cannot be relied upon in the early data
#  (2) in case the cut on electron-muon acoplanarity is applied
#      there will be no events left for which the invariant e + tau mass
#      can be reonstructed by collinear approximation
selectedElecMuPairsAcoplanarity12 = cms.EDFilter("PATElecMuPairSelector",
    cut = cms.string('cos(dPhi12) > -1.01'),
    #cut = cms.string('cos(dPhi12) < -0.95'),                                            
    filter = cms.bool(False)
)

# require low transverse mass of electron and MET
selectedElecMuPairsMt1MET = cms.EDFilter("PATElecMuPairSelector",
    cut = cms.string('mt1MET < 50.'),
    filter = cms.bool(False)
)

# require low transverse mass of muon and MET
selectedElecMuPairsMt2MET = cms.EDFilter("PATElecMuPairSelector",    
    cut = cms.string('mt2MET < 50.'),
    filter = cms.bool(False)
)

# require missing transverse momentum to point
# in the direction of visible tau decay products
selectedElecMuPairsPzetaDiff = cms.EDFilter("PATElecMuPairSelector",
    cut = cms.string('(pZeta - 1.5*pZetaVis) > -20.'),
    filter = cms.bool(False)
)

# define additional collections of electron + muon candidates
# with loose track and ECAL isolation applied on electron leg
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of electron isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)

selectedElecMuPairsAntiOverlapVetoLooseElectronIsolation = copy.deepcopy(selectedElecMuPairsAntiOverlapVeto)

selectedElecMuPairsZeroChargeLooseElectronIsolation = copy.deepcopy(selectedElecMuPairsZeroCharge)

selectedElecMuPairsAcoplanarity12LooseElectronIsolation = copy.deepcopy(selectedElecMuPairsAcoplanarity12)

selectedElecMuPairsMt1METlooseElectronIsolation = copy.deepcopy(selectedElecMuPairsMt1MET)

selectedElecMuPairsMt2METlooseElectronIsolation = copy.deepcopy(selectedElecMuPairsMt2MET)

selectedElecMuPairsPzetaDiffLooseElectronIsolation = copy.deepcopy(selectedElecMuPairsPzetaDiff)
