import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------  
# produce collections of electron + muons pairs passing selection criteria
#--------------------------------------------------------------------------------

# require missing transverse momentum to point either in direction of the electron
# or in direction of the muon in the transverse plane
selectedElecMuPairsAcoplanarity = cms.EDFilter("PATElecMuPairSelector",
     src = cms.InputTag("allElecMuPairs"),
     cut = cms.string('cos(dPhi1MET) > 0.5 | cos(dPhi2MET) > 0.5'),
     filter = cms.bool(False)
)

# require muon and tau to form a zero-charge pair
selectedElecMuPairsZeroChargeIndividual = cms.EDFilter("PATElecMuPairSelector",
     src = selectedElecMuPairsAcoplanarity.src,
     cut = cms.string('charge = 0'),
     filter = cms.bool(False)
)

selectedElecMuPairsZeroChargeCumulative = copy.deepcopy(selectedElecMuPairsZeroChargeIndividual)
selectedElecMuPairsZeroChargeCumulative.src = cms.InputTag("selectedElecMuPairsAcoplanarity")

selectElecMuPairs = cms.Sequence( selectedElecMuPairsAcoplanarity
                                 *selectedElecMuPairsZeroChargeIndividual
                                 *selectedElecMuPairsZeroChargeCumulative )
