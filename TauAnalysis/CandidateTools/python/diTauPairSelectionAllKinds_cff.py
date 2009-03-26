import FWCore.ParameterSet.Config as cms

from TauAnalysis.CandidateTools.elecTauPairSelection_cfi import *
from TauAnalysis.CandidateTools.muTauPairSelection_cfi import *
from TauAnalysis.CandidateTools.diTauPairSelection_cfi import *

#--------------------------------------------------------------------------------
# define selection criteria for e + tau-jet pairs
# (settings made here overwrite values defined in elecTauPairSelector_cfi)
#--------------------------------------------------------------------------------

selectedElecTauPairsAntiOverlapVeto.cut = cms.string('dR12 > 0.7')
selectedElecTauPairsZeroChargeIndividual.cut = cms.string('charge = 0')
selectedElecTauPairsMt1METIndividual.cut = cms.string('mt1MET < 60.')

selectedElecTauPairsZeroChargeCumulative.cut = selectedElecTauPairsZeroChargeIndividual.cut
selectedElecTauPairsMt1METCumulative.cut = selectedElecTauPairsMt1METIndividual.cut

selectedElecTauPairsAntiOverlapVetoLooseElectronIsolation.cut = selectedElecTauPairsAntiOverlapVeto.cut
selectedElecTauPairsZeroChargeIndividualLooseElectronIsolation.cut = selectedElecTauPairsZeroChargeIndividual.cut
selectedElecTauPairsMt1METIndividualLooseElectronIsolation.cut = selectedElecTauPairsMt1METIndividual.cut

selectedElecTauPairsZeroChargeCumulativeLooseElectronIsolation.cut = selectedElecTauPairsZeroChargeIndividualLooseElectronIsolation.cut
selectedElecTauPairsMt1METCumulativeLooseElectronIsolation.cut = selectedElecTauPairsMt1METIndividualLooseElectronIsolation.cut

#--------------------------------------------------------------------------------
# define selection criteria for mu + tau-jet pairs
# (settings made here overwrite values defined in muTauPairSelector_cfi)
#--------------------------------------------------------------------------------

selectedMuTauPairsAntiOverlapVeto.cut = cms.string('dR12 > 0.7')
selectedMuTauPairsZeroChargeIndividual.cut = cms.string('charge = 0')
selectedMuTauPairsMt1METIndividual.cut = cms.string('mt1MET < 60.')

selectedMuTauPairsZeroChargeCumulative.cut = selectedMuTauPairsZeroChargeIndividual.cut
selectedMuTauPairsMt1METCumulative.cut = selectedMuTauPairsMt1METIndividual.cut

selectedMuTauPairsAntiOverlapVetoLooseMuonIsolation.cut = selectedMuTauPairsAntiOverlapVeto.cut
selectedMuTauPairsZeroChargeIndividualLooseMuonIsolation.cut = selectedMuTauPairsZeroChargeIndividual.cut
selectedMuTauPairsMt1METIndividualLooseMuonIsolation.cut = selectedMuTauPairsMt1METIndividual.cut

selectedMuTauPairsZeroChargeCumulativeLooseMuonIsolation.cut = selectedMuTauPairsZeroChargeIndividualLooseMuonIsolation.cut
selectedMuTauPairsMt1METCumulativeLooseMuonIsolation.cut = selectedMuTauPairsMt1METIndividualLooseMuonIsolation.cut

#--------------------------------------------------------------------------------
# define selection criteria for tau-jet + tau-jet pairs
# (settings made here overwrite values defined in muTauPairSelector_cfi)
#--------------------------------------------------------------------------------

selectedDiTauPairsAntiOverlapVeto.cut = cms.string('dR12 > 0.7')
selectedDiTauPairsAcoplanarityIndividual.cut = cms.string('(dPhi1MET < 2.4) | (dPhi2MET < 2.4)')
selectedDiTauPairsZeroChargeIndividual.cut = cms.string('charge = 0')

selectedDiTauPairsAcoplanarityCumulative.cut = selectedDiTauPairsAcoplanarityIndividual.cut
selectedDiTauPairsZeroChargeCumulative.cut = selectedDiTauPairsZeroChargeIndividual.cut

selectDiTauPairsAllKinds = cms.Sequence( selectElecTauPairs
                                        +selectMuTauPairsAll
                                        +selectDiTauPairs )
