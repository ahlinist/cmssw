import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------  
# produce collections of electron + muons pairs passing selection criteria
#--------------------------------------------------------------------------------

# require missing transverse momentum to point either in direction of the electron
# or in direction of the muon in the transverse plane
selectedElecMuPairsAcoplanarity = cms.PSet(
    pluginName = cms.string("selectedElecMuPairsAcoplanarity"),
    pluginType = cms.string("PATElecMuPairSelector"),
    cut = cms.string('cos(dPhi1MET) > 0.5 | cos(dPhi2MET) > 0.5'),
    filter = cms.bool(False)
)

# require muon and tau to form a zero-charge pair
selectedElecMuPairsZeroCharge = cms.PSet(
    pluginName = cms.string("selectedElecMuPairsZeroCharge"),
    pluginType = cms.string("PATElecMuPairSelector"),              
    cut = cms.string('charge = 0'),
    filter = cms.bool(False)
)
