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

# define additional collections of electron + muon candidates
# with loose track and ECAL isolation applied on electron leg
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of electron isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)

selectedElecMuPairsAntiOverlapVetoLooseElectronIsolation = copy.deepcopy(selectedElecMuPairsAntiOverlapVeto)

selectedElecMuPairsZeroChargeLooseElectronIsolation = copy.deepcopy(selectedElecMuPairsZeroCharge)

selectedElecMuPairsMt1METlooseElectronIsolation = copy.deepcopy(selectedElecMuPairsMt1MET)

selectedElecMuPairsMt2METlooseElectronIsolation = copy.deepcopy(selectedElecMuPairsMt2MET)
