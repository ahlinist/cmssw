import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------  
# produce collections of electron + tau-jet pairs passing selection criteria
#--------------------------------------------------------------------------------

# require electron and tau-jet to be separated in eta-phi,
# in order to ensure that both do not refer to one and the same physical particle
# (NOTE: cut is already applied during skimming,
#        so should not reject any events)
selectedElecTauPairsAntiOverlapVeto = cms.EDFilter("PATElecTauPairSelector",
     src = cms.InputTag("allElecTauPairs"),
     cut = cms.string('dR12 > 0.7'),
     filter = cms.bool(False)
)

# require electron and tau to form a zero-charge pair
selectedElecTauPairsZeroChargeIndividual = cms.EDFilter("PATElecTauPairSelector",
     src = selectedElecTauPairsAntiOverlapVeto.src,
     cut = cms.string('charge = 0'),
     #cut = cms.string('(leg1.charge + leg2.leadTrack.charge) = 0'), # NOTE: to be used for background studies only !!                    
     filter = cms.bool(False)
)

selectedElecTauPairsZeroChargeCumulative = copy.deepcopy(selectedElecTauPairsZeroChargeIndividual)
selectedElecTauPairsZeroChargeCumulative.src = cms.InputTag("selectedElecTauPairsAntiOverlapVeto")

#require cut transverse mass of electron and MET
selectedElecTauPairsMt1METIndividual = cms.EDFilter("PATElecTauPairSelector",
     src = selectedElecTauPairsAntiOverlapVeto.src,
     cut = cms.string('mt1MET < 60.'),
     filter = cms.bool(False)
)

selectedElecTauPairsMt1METCumulative = copy.deepcopy(selectedElecTauPairsMt1METIndividual)
selectedElecTauPairsMt1METCumulative.src = cms.InputTag("selectedElecTauPairsZeroChargeCumulative")

selectElecTauPairs = cms.Sequence( selectedElecTauPairsAntiOverlapVeto
                                  *selectedElecTauPairsZeroChargeIndividual
                                  *selectedElecTauPairsZeroChargeCumulative
                                  *selectedElecTauPairsMt1METIndividual
                                  *selectedElecTauPairsMt1METCumulative )

# define additional collections of electron candidates
# with loose track and ECAL isolation applied
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of electron isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)

selectedElecTauPairsAntiOverlapVetoLooseElectronIsolation = copy.deepcopy(selectedElecTauPairsAntiOverlapVeto)
selectedElecTauPairsAntiOverlapVetoLooseElectronIsolation.src = cms.InputTag("allElecTauPairsLooseElectronIsolation")

selectedElecTauPairsZeroChargeIndividualLooseElectronIsolation = copy.deepcopy(selectedElecTauPairsZeroChargeIndividual)
selectedElecTauPairsZeroChargeIndividualLooseElectronIsolation.src = selectedElecTauPairsAntiOverlapVetoLooseElectronIsolation.src

selectedElecTauPairsZeroChargeCumulativeLooseElectronIsolation = copy.deepcopy(selectedElecTauPairsZeroChargeIndividualLooseElectronIsolation)
selectedElecTauPairsZeroChargeCumulativeLooseElectronIsolation.src = cms.InputTag("selectedElecTauPairsAcoplanarityCumulativeLooseElectronIsolation")

selectedElecTauPairsMt1METIndividualLooseElectronIsolation = copy.deepcopy(selectedElecTauPairsMt1METIndividual)
selectedElecTauPairsMt1METIndividualLooseElectronIsolation.src = selectedElecTauPairsAntiOverlapVetoLooseElectronIsolation.src

selectedElecTauPairsMt1METCumulativeLooseElectronIsolation = copy.deepcopy(selectedElecTauPairsMt1METIndividualLooseElectronIsolation)
selectedElecTauPairsMt1METCumulativeLooseElectronIsolation.src = cms.InputTag("selectedElecTauPairsZeroChargeCumulativeLooseElectronIsolation")

selectElecTauPairsLooseElectronIsolation = cms.Sequence( selectedElecTauPairsAntiOverlapVetoLooseElectronIsolation
                                                        *selectedElecTauPairsZeroChargeIndividualLooseElectronIsolation
                                                        *selectedElecTauPairsZeroChargeCumulativeLooseElectronIsolation
                                                        *selectedElecTauPairsMt1METIndividualLooseElectronIsolation
                                                        *selectedElecTauPairsMt1METCumulativeLooseElectronIsolation )
