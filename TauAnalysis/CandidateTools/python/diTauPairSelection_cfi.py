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
     src = cms.InputTag("selectedDiTauPairs2ndTauProngCumulative"),
     cut = cms.string('dR12 > 0.7'),
     filter = cms.bool(False)
)

# require muon and tau not to be back-to-back
selectedDiTauPairsAcoplanarityIndividual = cms.EDFilter("PATDiTauPairSelector",
     src = selectedDiTauPairsAntiOverlapVeto.src,
     cut = cms.string('(dPhi1MET < 2.4) | (dPhi2MET < 2.4)'),
     filter = cms.bool(False)
)

selectedDiTauPairsAcoplanarityCumulative = copy.deepcopy(selectedDiTauPairsAcoplanarityIndividual)
selectedDiTauPairsAcoplanarityCumulative.src = cms.InputTag("selectedDiTauPairsAntiOverlapVeto")

# require muon and tau to form a zero-charge pair
selectedDiTauPairsZeroChargeIndividual = cms.EDFilter("PATDiTauPairSelector",
     src = selectedDiTauPairsAntiOverlapVeto.src,
     cut = cms.string('charge = 0'),
     #cut = cms.string('(leg1.leadTrack.charge + leg2.leadTrack.charge) = 0'), # NOTE: to be used for background studies only !!
     filter = cms.bool(False)
)

selectedDiTauPairsZeroChargeCumulative = copy.deepcopy(selectedDiTauPairsZeroChargeIndividual)
selectedDiTauPairsZeroChargeCumulative.src = cms.InputTag("selectedDiTauPairsAcoplanarityCumulative")

selectDiTauPairs = cms.Sequence( selectedDiTauPairsAntiOverlapVeto
                                *selectedDiTauPairsAcoplanarityIndividual
                                *selectedDiTauPairsAcoplanarityCumulative
                                *selectedDiTauPairsZeroChargeIndividual
                                *selectedDiTauPairsZeroChargeCumulative )


