import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------  
# produce collections of tau-jet + tau-jet pairs passing selection criteria
#--------------------------------------------------------------------------------

# require the two tau-jets to be separated in eta-phi,
# in order to ensure that both do not refer to one and the same physical particle
# (NOTE: cut is already applied during skimming,
#        so should not reject any events)
selectedDiTauPairsAntiOverlapVeto = cms.EDFilter("PATDiTauPairSelector",
    cut = cms.string('dR12 > 0.7'),
    filter = cms.bool(False)
)

# require muon and tau to form a zero-charge pair
selectedDiTauPairsZeroCharge = cms.EDFilter("PATDiTauPairSelector",
    cut = cms.string('charge = 0'),
    # CV: the following definition of the zero-charge cut is to be used for background studies only !!
    #cut = cms.string('(leg1.leadPFChargedHadrCand.charge + leg2.leadPFChargedHadrCand.charge) = 0'),
    filter = cms.bool(False)
)

# require the two tau-jets not to be back-to-back
selectedDiTauPairsAcoplanarity = cms.EDFilter("PATDiTauPairSelector",
    #cut = cms.string('(dPhi1MET < 2.4) | (dPhi2MET < 2.4)'),
    cut = cms.string('(dPhi1MET < 3.2) | (dPhi2MET < 3.2)'), # CV: cut disabled for now...
    filter = cms.bool(False)
)

# require missing transverse momentum to point
# in the direction of visible tau decay products
selectedDiTauPairsPzetaDiff = cms.EDFilter("PATDiTauPairSelector",
    cut = cms.string('(pZeta - 1.5*pZetaVis) > -20.'),
    filter = cms.bool(False)
)

# define additional collections of tau-jet + tau-jet candidates
# with loose lead. track Pt, track isolation and ECAL isolation applied on second leg
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of tau id. criteria from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)

selectedDiTauPairsAntiOverlapVetoLoose2ndTau = copy.deepcopy(selectedDiTauPairsAntiOverlapVeto)

selectedDiTauPairsZeroChargeLoose2ndTau = selectedDiTauPairsZeroCharge.clone(
    cut = cms.string('(leg1.charge + leg2.leadPFChargedHadrCand.charge) = 0')
)    

selectedDiTauPairsAcoplanarityLoose2ndTau = copy.deepcopy(selectedDiTauPairsAcoplanarity)

selectedDiTauPairsPzetaDiffLoose2ndTau = copy.deepcopy(selectedDiTauPairsPzetaDiff)
