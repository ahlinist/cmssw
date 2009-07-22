import FWCore.ParameterSet.Config as cms

from TauAnalysis.CandidateTools.elecMuPairSelection_cfi import *
from TauAnalysis.CandidateTools.elecTauPairSelection_cfi import *
from TauAnalysis.CandidateTools.muTauPairSelection_cfi import *
from TauAnalysis.CandidateTools.diTauPairSelection_cfi import *

from TauAnalysis.RecoTools.objSelConfigurator import *

#--------------------------------------------------------------------------------
# define selection criteria for e + mu pairs
# (settings made here overwrite values defined in elecMuPairSelector_cfi)
#--------------------------------------------------------------------------------

selectedElecMuPairsAntiOverlapVeto.cut = cms.string('dR12 > 1.57')
selectedElecMuPairsZeroCharge.cut = cms.string('charge = 0')
selectedElecMuPairsMt1MET.cut = cms.string('mt1MET < 50.')
selectedElecMuPairsMt2MET.cut = cms.string('mt2MET < 50.')

patElecMuPairSelConfigurator = objSelConfigurator(
    [ selectedElecMuPairsAntiOverlapVeto,
      selectedElecMuPairsZeroCharge,
      selectedElecMuPairsMt1MET,
      selectedElecMuPairsMt2MET ],
    src = "allElecMuPairs",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectElecMuPairs = patElecMuPairSelConfigurator.configure(namespace = locals())

selectedElecMuPairsAntiOverlapVetoLooseElectronIsolation.cut = selectedElecMuPairsAntiOverlapVeto.cut
selectedElecMuPairsZeroChargeLooseElectronIsolation.cut = selectedElecMuPairsZeroCharge.cut
selectedElecMuPairsMt1METlooseElectronIsolation.cut = selectedElecMuPairsMt1MET.cut

patElecMuPairSelConfiguratorLooseElectronIsolation = objSelConfigurator(
    [ selectedElecMuPairsAntiOverlapVetoLooseElectronIsolation,
      selectedElecMuPairsZeroChargeLooseElectronIsolation,
      selectedElecMuPairsMt1METlooseElectronIsolation,
      selectedElecMuPairsMt2METlooseElectronIsolation ],
    src = "allElecMuPairsLooseElectronIsolation",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectElecMuPairsLooseElectronIsolation = patElecMuPairSelConfiguratorLooseElectronIsolation.configure(namespace = locals())

#--------------------------------------------------------------------------------
# define selection criteria for e + tau-jet pairs
# (settings made here overwrite values defined in elecTauPairSelector_cfi)
#--------------------------------------------------------------------------------

selectedElecTauPairsAntiOverlapVeto.cut = cms.string('dR12 > 0.7')
selectedElecTauPairsZeroCharge.cut = cms.string('charge = 0')
selectedElecTauPairsMt1MET.cut = cms.string('mt1MET < 60.')

patElecTauPairSelConfigurator = objSelConfigurator(
    [ selectedElecTauPairsAntiOverlapVeto,
      selectedElecTauPairsZeroCharge,
      selectedElecTauPairsMt1MET ],
    src = "allElecTauPairs",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectElecTauPairs = patElecTauPairSelConfigurator.configure(namespace = locals())

selectedElecTauPairsAntiOverlapVetoLooseElectronIsolation.cut = selectedElecTauPairsAntiOverlapVeto.cut
selectedElecTauPairsZeroChargeLooseElectronIsolation.cut = selectedElecTauPairsZeroCharge.cut
selectedElecTauPairsMt1METlooseElectronIsolation.cut = selectedElecTauPairsMt1MET.cut

patElecTauPairSelConfiguratorLooseElectronIsolation = objSelConfigurator(
    [ selectedElecTauPairsAntiOverlapVetoLooseElectronIsolation,
      selectedElecTauPairsZeroChargeLooseElectronIsolation,
      selectedElecTauPairsMt1METlooseElectronIsolation ],
    src = "allElecTauPairsLooseElectronIsolation",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectElecTauPairsLooseElectronIsolation = patElecTauPairSelConfiguratorLooseElectronIsolation.configure(namespace = locals())

#--------------------------------------------------------------------------------
# define selection criteria for mu + tau-jet pairs
# (settings made here overwrite values defined in muTauPairSelector_cfi)
#--------------------------------------------------------------------------------

selectedMuTauPairsAntiOverlapVeto.cut = cms.string('dR12 > 0.7')
selectedMuTauPairsZeroCharge.cut = cms.string('charge = 0')
selectedMuTauPairsMt1MET.cut = cms.string('mt1MET < 60.')

patMuTauPairSelConfigurator = objSelConfigurator(
    [ selectedMuTauPairsAntiOverlapVeto,
      selectedMuTauPairsZeroCharge,
      selectedMuTauPairsMt1MET ],
    src = "allMuTauPairs",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectMuTauPairs = patMuTauPairSelConfigurator.configure(namespace = locals())

selectedMuTauPairsAntiOverlapVetoLooseMuonIsolation.cut = selectedMuTauPairsAntiOverlapVeto.cut
selectedMuTauPairsZeroChargeLooseMuonIsolation.cut = selectedMuTauPairsZeroCharge.cut
selectedMuTauPairsMt1METlooseMuonIsolation.cut = selectedMuTauPairsMt1MET.cut

patMuTauPairSelConfiguratorLooseMuonIsolation = objSelConfigurator(
    [ selectedMuTauPairsAntiOverlapVetoLooseMuonIsolation,
      selectedMuTauPairsZeroChargeLooseMuonIsolation,
      selectedMuTauPairsMt1METlooseMuonIsolation ],
    src = "allMuTauPairsLooseMuonIsolation",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectMuTauPairsLooseMuonIsolation = patMuTauPairSelConfiguratorLooseMuonIsolation.configure(namespace = locals())

#--------------------------------------------------------------------------------
# define selection criteria for tau-jet + tau-jet pairs
# (settings made here overwrite values defined in muTauPairSelector_cfi)
#--------------------------------------------------------------------------------

selectedDiTauPairsAntiOverlapVeto.cut = cms.string('dR12 > 0.7')
selectedDiTauPairsAcoplanarity.cut = cms.string('(dPhi1MET < 2.4) | (dPhi2MET < 2.4)')
selectedDiTauPairsZeroCharge.cut = cms.string('charge = 0')

patDiTauPairSelConfigurator = objSelConfigurator(
    [ selectedDiTauPairsAntiOverlapVeto,
      selectedDiTauPairsAcoplanarity,
      selectedDiTauPairsZeroCharge ],
    src = "selectedDiTauPairs2ndTauProngCumulative",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectDiTauPairs = patDiTauPairSelConfigurator.configure(namespace = locals())

selectDiTauPairsAllKinds = cms.Sequence( selectElecMuPairs + selectElecMuPairsLooseElectronIsolation
                                        +selectElecTauPairs + selectElecTauPairsLooseElectronIsolation
                                        +selectMuTauPairs + selectMuTauPairsLooseMuonIsolation
                                        +selectDiTauPairs )
