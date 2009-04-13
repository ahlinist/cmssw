import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------  
# produce collections of muon + tau-jet pairs passing selection criteria
#--------------------------------------------------------------------------------

# require muon and tau-jet to be separated in eta-phi,
# in order to ensure that both do not refer to one and the same physical particle
# (NOTE: cut is already applied during skimming,
#        so should not reject any events)
selectedMuTauPairsAntiOverlapVeto = cms.PSet(
    pluginName = cms.string("selectedMuTauPairsAntiOverlapVeto"),
    pluginType = cms.string("PATMuTauPairSelector"),                                              
    cut = cms.string('dR12 > 0.7'),
    filter = cms.bool(False)
)

# require muon and tau to form a zero-charge pair
selectedMuTauPairsZeroCharge = cms.PSet(
    pluginName = cms.string("selectedMuTauPairsZeroCharge"),
    pluginType = cms.string("PATMuTauPairSelector"),                                                  
    cut = cms.string('charge = 0'),
    #cut = cms.string('(leg1.charge + leg2.leadTrack.charge) = 0'), # NOTE: to be used for background studies only !!   
    filter = cms.bool(False)
)

#require cut transverse mass of muon and MET
selectedMuTauPairsMt1MET = cms.PSet(
    pluginName = cms.string("selectedMuTauPairsMt1MET"),
    pluginType = cms.string("PATMuTauPairSelector"),                                               
    cut = cms.string('mt1MET < 60.'),
    filter = cms.bool(False)
)

# define additional collections of muon candidates
# with loose track and ECAL isolation applied
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of muon isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)

selectedMuTauPairsAntiOverlapVetoLooseMuonIsolation = copy.deepcopy(selectedMuTauPairsAntiOverlapVeto)
selectedMuTauPairsAntiOverlapVetoLooseMuonIsolation.pluginName = cms.string("selectedMuTauPairsAntiOverlapVetoLooseMuonIsolation")

selectedMuTauPairsZeroChargeLooseMuonIsolation = copy.deepcopy(selectedMuTauPairsZeroCharge)
selectedMuTauPairsZeroChargeLooseMuonIsolation.pluginName = cms.string("selectedMuTauPairsZeroChargeLooseMuonIsolation")

selectedMuTauPairsMt1METlooseMuonIsolation = copy.deepcopy(selectedMuTauPairsMt1MET)
selectedMuTauPairsMt1METlooseMuonIsolation.pluginName = cms.string("selectedMuTauPairsMt1METlooseMuonIsolation")

