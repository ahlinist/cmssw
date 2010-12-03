import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------  
# produce collections of muon + tau-jet pairs passing selection criteria
#--------------------------------------------------------------------------------

# require muon and tau-jet to be separated in eta-phi,
# in order to ensure that both do not refer to one and the same physical particle
# (NOTE: cut is already applied during skimming,
#        so should not reject any events)
selectedMuTauPairsAntiOverlapVeto = cms.EDFilter("PATMuTauPairSelector",
    cut = cms.string('dR12 > 0.7'),
    filter = cms.bool(False)
)

# require low transverse mass of muon and MET
selectedMuTauPairsMt1MET = cms.EDFilter("PATMuTauPairSelector",
    cut = cms.string('mt1MET < 40.'),
    filter = cms.bool(False)
)

# require missing transverse momentum to point
# in the direction of visible tau decay products
selectedMuTauPairsPzetaDiff = cms.EDFilter("PATMuTauPairSelector",
    cut = cms.string('(pZeta - 1.5*pZetaVis) > -20.'),
    filter = cms.bool(False)
)

# require muon and tau to form a zero-charge pair
# (selection applied in signal region)
selectedMuTauPairsZeroCharge = cms.EDFilter("PATMuTauPairSelector",
    cut = cms.string('charge = 0'),
    filter = cms.bool(False)
)

# require muon and tau to form a non-zero-charge pair
# (selection applied in background dominated control region)
selectedMuTauPairsNonZeroCharge = cms.EDFilter("PATMuTauPairSelector",
    cut = cms.string('charge != 0'),
    filter = cms.bool(False)
)

# define additional collections of muon + tau-jet candidates
# with loose track and ECAL isolation applied on muon leg
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of muon isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)

selectedMuTauPairsAntiOverlapVetoLooseMuonIsolation = copy.deepcopy(selectedMuTauPairsAntiOverlapVeto)

selectedMuTauPairsMt1METlooseMuonIsolation = copy.deepcopy(selectedMuTauPairsMt1MET)

selectedMuTauPairsPzetaDiffLooseMuonIsolation = copy.deepcopy(selectedMuTauPairsPzetaDiff)

selectedMuTauPairsZeroChargeLooseMuonIsolation = copy.deepcopy(selectedMuTauPairsZeroCharge)

selectedMuTauPairsNonZeroChargeLooseMuonIsolation = copy.deepcopy(selectedMuTauPairsNonZeroCharge)
