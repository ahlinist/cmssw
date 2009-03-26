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
     src = cms.InputTag("allMuTauPairs"),
     cut = cms.string('dR12 > 0.7'),
     filter = cms.bool(False)
)

# require muon and tau to form a zero-charge pair
selectedMuTauPairsZeroChargeIndividual = cms.EDFilter("PATMuTauPairSelector",
     src = selectedMuTauPairsAntiOverlapVeto.src,
     cut = cms.string('charge = 0'),
     #cut = cms.string('(leg1.charge + leg2.leadTrack.charge) = 0'), # NOTE: to be used for background studies only !!   
     filter = cms.bool(False)
)

selectedMuTauPairsZeroChargeCumulative = copy.deepcopy(selectedMuTauPairsZeroChargeIndividual)
selectedMuTauPairsZeroChargeCumulative.src = cms.InputTag("selectedMuTauPairsAntiOverlapVeto")

#require cut transverse mass of muon and MET
selectedMuTauPairsMt1METIndividual = cms.EDFilter("PATMuTauPairSelector",
     src = selectedMuTauPairsAntiOverlapVeto.src,
     cut = cms.string('mt1MET < 60.'),
     filter = cms.bool(False)
)

selectedMuTauPairsMt1METCumulative = copy.deepcopy(selectedMuTauPairsMt1METIndividual)
selectedMuTauPairsMt1METCumulative.src = cms.InputTag("selectedMuTauPairsZeroChargeCumulative")

selectMuTauPairs = cms.Sequence( selectedMuTauPairsAntiOverlapVeto
                                *selectedMuTauPairsZeroChargeIndividual
                                *selectedMuTauPairsZeroChargeCumulative
                                *selectedMuTauPairsMt1METIndividual
                                *selectedMuTauPairsMt1METCumulative )

# define additional collections of muon candidates
# with loose track and ECAL isolation applied
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of muon isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)

selectedMuTauPairsAntiOverlapVetoLooseMuonIsolation = copy.deepcopy(selectedMuTauPairsAntiOverlapVeto)
selectedMuTauPairsAntiOverlapVetoLooseMuonIsolation.src = cms.InputTag("allMuTauPairsLooseMuonIsolation")

selectedMuTauPairsZeroChargeIndividualLooseMuonIsolation = copy.deepcopy(selectedMuTauPairsZeroChargeIndividual)
selectedMuTauPairsZeroChargeIndividualLooseMuonIsolation.src = selectedMuTauPairsAntiOverlapVetoLooseMuonIsolation.src

selectedMuTauPairsZeroChargeCumulativeLooseMuonIsolation = copy.deepcopy(selectedMuTauPairsZeroChargeIndividualLooseMuonIsolation)
selectedMuTauPairsZeroChargeCumulativeLooseMuonIsolation.src = cms.InputTag("selectedMuTauPairsAntiOverlapVetoLooseMuonIsolation")

selectedMuTauPairsMt1METIndividualLooseMuonIsolation = copy.deepcopy(selectedMuTauPairsMt1METIndividual)
selectedMuTauPairsMt1METIndividualLooseMuonIsolation.src = selectedMuTauPairsAntiOverlapVetoLooseMuonIsolation.src

selectedMuTauPairsMt1METCumulativeLooseMuonIsolation = copy.deepcopy(selectedMuTauPairsMt1METIndividualLooseMuonIsolation)
selectedMuTauPairsMt1METCumulativeLooseMuonIsolation.src = cms.InputTag("selectedMuTauPairsZeroChargeCumulativeLooseMuonIsolation")

selectMuTauPairsLooseMuonIsolation = cms.Sequence( selectedMuTauPairsAntiOverlapVetoLooseMuonIsolation
                                                  *selectedMuTauPairsZeroChargeIndividualLooseMuonIsolation
                                                  *selectedMuTauPairsZeroChargeCumulativeLooseMuonIsolation
                                                  *selectedMuTauPairsMt1METIndividualLooseMuonIsolation
                                                  *selectedMuTauPairsMt1METCumulativeLooseMuonIsolation )

selectMuTauPairsAll = cms.Sequence( selectMuTauPairs * selectMuTauPairsLooseMuonIsolation )
