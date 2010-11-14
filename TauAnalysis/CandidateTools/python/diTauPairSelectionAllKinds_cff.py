import FWCore.ParameterSet.Config as cms

from TauAnalysis.CandidateTools.elecMuPairSelection_cfi import *
from TauAnalysis.CandidateTools.elecTauPairSelection_cfi import *
from TauAnalysis.CandidateTools.muTauPairSelection_cfi import *
from TauAnalysis.CandidateTools.diTauPairSelection_cfi import *

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *

#--------------------------------------------------------------------------------
# define selection criteria for e + mu pairs
# (settings made here overwrite values defined in elecMuPairSelector_cfi)
#--------------------------------------------------------------------------------

selectedElecMuPairsAntiOverlapVeto.cut = cms.string('dR12 > 1.57')
selectedElecMuPairsZeroCharge.cut = cms.string('charge = 0')
selectedElecMuPairsAcoplanarity12.cut = cms.string('cos(dPhi12) > -1.01')
selectedElecMuPairsMt1MET.cut = cms.string('mt1MET < 50.')
selectedElecMuPairsMt2MET.cut = cms.string('mt2MET < 50.')
selectedElecMuPairsPzetaDiff.cut = cms.string('(pZeta - 1.5*pZetaVis) > -20.')

patElecMuPairSelConfigurator = objSelConfigurator(
    [ selectedElecMuPairsAntiOverlapVeto,
      selectedElecMuPairsZeroCharge,
      selectedElecMuPairsAcoplanarity12,
      selectedElecMuPairsMt1MET,
      selectedElecMuPairsMt2MET,
      selectedElecMuPairsPzetaDiff ],
    src = "allElecMuPairs",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectElecMuPairs = patElecMuPairSelConfigurator.configure(pyNameSpace = locals())

selectedElecMuPairsAntiOverlapVetoLooseElectronIsolation.cut = selectedElecMuPairsAntiOverlapVeto.cut
selectedElecMuPairsZeroChargeLooseElectronIsolation.cut = selectedElecMuPairsZeroCharge.cut
selectedElecMuPairsAcoplanarity12LooseElectronIsolation.cut = selectedElecMuPairsAcoplanarity12.cut
selectedElecMuPairsMt1METlooseElectronIsolation.cut = selectedElecMuPairsMt1MET.cut
selectedElecMuPairsMt2METlooseElectronIsolation.cut = selectedElecMuPairsMt2MET.cut
selectedElecMuPairsPzetaDiffLooseElectronIsolation.cut = selectedElecMuPairsPzetaDiff.cut

patElecMuPairSelConfiguratorLooseElectronIsolation = objSelConfigurator(
    [ selectedElecMuPairsAntiOverlapVetoLooseElectronIsolation,
      selectedElecMuPairsZeroChargeLooseElectronIsolation,
      selectedElecMuPairsAcoplanarity12LooseElectronIsolation,
      selectedElecMuPairsMt1METlooseElectronIsolation,
      selectedElecMuPairsMt2METlooseElectronIsolation,
      selectedElecMuPairsPzetaDiffLooseElectronIsolation ],
    src = "allElecMuPairsLooseElectronIsolation",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectElecMuPairsLooseElectronIsolation = patElecMuPairSelConfiguratorLooseElectronIsolation.configure(pyNameSpace = locals())

#--------------------------------------------------------------------------------
# define selection criteria for e + tau-jet pairs
# (settings made here overwrite values defined in elecTauPairSelector_cfi)
#--------------------------------------------------------------------------------

selectedElecTauPairsAntiOverlapVeto.cut = cms.string('dR12 > 0.7')
selectedElecTauPairsZeroCharge.cut = cms.string('charge = 0')
selectedElecTauPairsAcoplanarity12.cut = cms.string('cos(dPhi12) > -1.01')
selectedElecTauPairsMt1MET.cut = cms.string('mt1MET < 50.')
selectedElecTauPairsPzetaDiff.cut = cms.string('(pZeta - 1.5*pZetaVis) > -20.')

patElecTauPairSelConfigurator = objSelConfigurator(
    [ selectedElecTauPairsAntiOverlapVeto,
      selectedElecTauPairsZeroCharge,
      selectedElecTauPairsAcoplanarity12,
      selectedElecTauPairsMt1MET,
      selectedElecTauPairsPzetaDiff ],
    src = "allElecTauPairs",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectElecTauPairs = patElecTauPairSelConfigurator.configure(pyNameSpace = locals())

selectedElecTauPairsAntiOverlapVetoLooseElectronIsolation.cut = selectedElecTauPairsAntiOverlapVeto.cut
selectedElecTauPairsZeroChargeLooseElectronIsolation.cut = selectedElecTauPairsZeroCharge.cut
selectedElecTauPairsAcoplanarity12LooseElectronIsolation.cut = selectedElecTauPairsAcoplanarity12.cut
selectedElecTauPairsMt1METlooseElectronIsolation.cut = selectedElecTauPairsMt1MET.cut
selectedElecTauPairsPzetaDiffLooseElectronIsolation.cut = selectedElecTauPairsPzetaDiff.cut

patElecTauPairSelConfiguratorLooseElectronIsolation = objSelConfigurator(
    [ selectedElecTauPairsAntiOverlapVetoLooseElectronIsolation,
      selectedElecTauPairsZeroChargeLooseElectronIsolation,
      selectedElecTauPairsAcoplanarity12LooseElectronIsolation,
      selectedElecTauPairsMt1METlooseElectronIsolation,
      selectedElecTauPairsPzetaDiffLooseElectronIsolation ],
    src = "allElecTauPairsLooseElectronIsolation",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectElecTauPairsLooseElectronIsolation = patElecTauPairSelConfiguratorLooseElectronIsolation.configure(pyNameSpace = locals())

#--------------------------------------------------------------------------------
# define selection criteria for mu + tau-jet pairs
# (settings made here overwrite values defined in muTauPairSelector_cfi)
#--------------------------------------------------------------------------------

selectedMuTauPairsAntiOverlapVeto.cut = cms.string('dR12 > 0.7')
selectedMuTauPairsZeroCharge.cut = cms.string('charge = 0')
selectedMuTauPairsAcoplanarity12.cut = cms.string('cos(dPhi12) > -1.01')
selectedMuTauPairsMt1MET.cut = cms.string('mt1MET < 50.')
selectedMuTauPairsPzetaDiff.cut = cms.string('(pZeta - 1.5*pZetaVis) > -20.')

patMuTauPairSelConfigurator = objSelConfigurator(
    [ selectedMuTauPairsAntiOverlapVeto,
      selectedMuTauPairsZeroCharge,
      selectedMuTauPairsAcoplanarity12,
      selectedMuTauPairsMt1MET,
      selectedMuTauPairsPzetaDiff ],
    src = "allMuTauPairs",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectedMuTauPairsCollinearApproxFailedCumulative = cms.EDFilter("PATMuTauPairSelector",
    src = cms.InputTag('selectedMuTauPairsAntiBackToBackCumulative'),
    cut = cms.string('!collinearApproxIsValid'),                                                       
    filter = cms.bool(False)
)

selectedMuTauPairsCollinearApproxFailedIndividual = cms.EDFilter("PATMuTauPairSelector",
    src = cms.InputTag('allMuTauPairs'),
    cut = cms.string('!collinearApproxIsValid'),                                                       
    filter = cms.bool(False)
)


selectMuTauPairs = patMuTauPairSelConfigurator.configure(pyNameSpace = locals())

selectedMuTauPairsAntiOverlapVetoLooseMuonIsolation.cut = selectedMuTauPairsAntiOverlapVeto.cut
selectedMuTauPairsZeroChargeLooseMuonIsolation.cut = selectedMuTauPairsZeroCharge.cut
selectedMuTauPairsAcoplanarity12LooseMuonIsolation.cut = selectedMuTauPairsAcoplanarity12.cut
selectedMuTauPairsMt1METlooseMuonIsolation.cut = selectedMuTauPairsMt1MET.cut
selectedMuTauPairsPzetaDiffLooseMuonIsolation.cut = selectedMuTauPairsPzetaDiff.cut

patMuTauPairSelConfiguratorLooseMuonIsolation = objSelConfigurator(
    [ selectedMuTauPairsAntiOverlapVetoLooseMuonIsolation,
      selectedMuTauPairsZeroChargeLooseMuonIsolation,
      selectedMuTauPairsAcoplanarity12LooseMuonIsolation,
      selectedMuTauPairsMt1METlooseMuonIsolation,
      selectedMuTauPairsPzetaDiffLooseMuonIsolation ],
    src = "allMuTauPairsLooseMuonIsolation",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectMuTauPairsLooseMuonIsolation = patMuTauPairSelConfiguratorLooseMuonIsolation.configure(pyNameSpace = locals())

#--------------------------------------------------------------------------------
# define selection criteria for tau-jet + tau-jet pairs
# (settings made here overwrite values defined in muTauPairSelector_cfi)
#--------------------------------------------------------------------------------

selectedDiTauPairsAntiOverlapVeto.cut = cms.string('dR12 > 0.7')
selectedDiTauPairsZeroCharge.cut = cms.string('charge = 0')
#selectedDiTauPairsAcoplanarity.cut = cms.string('(dPhi1MET < 2.4) | (dPhi2MET < 2.4)')
selectedDiTauPairsAcoplanarity.cut = cms.string('(dPhi1MET < 3.2) | (dPhi2MET < 3.2)') # CV: cut disabled for now...
selectedDiTauPairsPzetaDiff.cut = cms.string('(pZeta - 1.5*pZetaVis) > -20.')

patDiTauPairSelConfigurator = objSelConfigurator(
    [ selectedDiTauPairsAntiOverlapVeto,
      selectedDiTauPairsZeroCharge, 
      selectedDiTauPairsAcoplanarity,
      selectedDiTauPairsPzetaDiff ],
    src = "selectedDiTauPairs2ndTauElectronVetoCumulative",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectDiTauPairs = patDiTauPairSelConfigurator.configure(pyNameSpace = locals())

# define additional collections of tau-jet + tau-jet candidates
# with loose lead. track Pt, track isolation and ECAL isolation applied on second leg
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of tau id. criteria from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)

selectedDiTauPairsAntiOverlapVetoLoose2ndTau.cut = cms.string('dR12 > 0.7')
selectedDiTauPairsZeroChargeLoose2ndTau.cut = cms.string('(leg1.charge + leg2.leadPFChargedHadrCand.charge) = 0')
#selectedDiTauPairsAcoplanarityLoose2ndTau.cut = cms.string('(dPhi1MET < 2.4) | (dPhi2MET < 2.4)')
selectedDiTauPairsAcoplanarityLoose2ndTau.cut = cms.string('(dPhi1MET < 3.2) | (dPhi2MET < 3.2)') # CV: cut disabled for now...
selectedDiTauPairsPzetaDiffLoose2ndTau.cut = cms.string('(pZeta - 1.5*pZetaVis) > -20.')

patDiTauPairSelConfiguratorLoose2ndTau = objSelConfigurator(
    [ selectedDiTauPairsAntiOverlapVetoLoose2ndTau,
      selectedDiTauPairsZeroChargeLoose2ndTau, 
      selectedDiTauPairsAcoplanarityLoose2ndTau,
      selectedDiTauPairsPzetaDiffLoose2ndTau ],
    src = "selectedDiTauPairs2ndTauElectronVetoLooseCumulative",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectDiTauPairsLoose2ndTau = patDiTauPairSelConfiguratorLoose2ndTau.configure(pyNameSpace = locals())

selectDiTauPairsAllKinds = cms.Sequence(
    selectElecMuPairs + selectElecMuPairsLooseElectronIsolation
   + selectElecTauPairs + selectElecTauPairsLooseElectronIsolation
   + selectMuTauPairs + selectMuTauPairsLooseMuonIsolation
   + selectDiTauPairs + selectDiTauPairsLoose2ndTau
)
