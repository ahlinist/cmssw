import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.muTauPairSelection_cfi import *

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *

#--------------------------------------------------------------------------------  
# produce collections of muon + tau-jet pairs passing selection criteria
# specific to MSSM Higgs A/H --> tau+ tau- --> muon + tau-jet analysis
#--------------------------------------------------------------------------------

selectedMuTauPairsForAHtoMuTauAntiOverlapVeto = copy.deepcopy(selectedMuTauPairsAntiOverlapVeto)

selectedMuTauPairsForAHtoMuTauZeroCharge = copy.deepcopy(selectedMuTauPairsZeroCharge)

selectedMuTauPairsForAHtoMuTauMt1MET = copy.deepcopy(selectedMuTauPairsMt1MET)

selectedMuTauPairsForAHtoMuTauPzetaDiff = copy.deepcopy(selectedMuTauPairsPzetaDiff)

# require muon and tau-jet not to be back-to-back
# (dPhi(muon-tau) < 160 degrees, such that collinear approximation provides good mass resolution)
selectedMuTauPairsForAHtoMuTauNonBackToBack = cms.EDFilter("PATMuTauPairSelector",
    cut = cms.string('dPhi12 < 2.79'),
    filter = cms.bool(False)
)

# require missing transverse momentum to point
# in the direction of visible tau decay products
selectedMuTauPairsForAHtoMuTauValidCollinearApprox = cms.EDFilter("PATMuTauPairSelector",
    cut = cms.string('collinearApproxIsValid()'),
    filter = cms.bool(False)
)

patMuTauPairSelConfiguratorForAHtoMuTau_path01 = objSelConfigurator(
    [ selectedMuTauPairsForAHtoMuTauAntiOverlapVeto,
      selectedMuTauPairsForAHtoMuTauZeroCharge,
      selectedMuTauPairsForAHtoMuTauMt1MET,
      selectedMuTauPairsForAHtoMuTauPzetaDiff ],
    src = "allMuTauPairs",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectMuTauPairsForAHtoMuTau_path01 = patMuTauPairSelConfiguratorForAHtoMuTau_path01.configure(pyNameSpace = locals())

patMuTauPairSelConfiguratorForAHtoMuTau_path02 = objSelConfigurator(
    [ selectedMuTauPairsForAHtoMuTauNonBackToBack,
      selectedMuTauPairsForAHtoMuTauValidCollinearApprox ],
    src = "selectedMuTauPairsForAHtoMuTauMt1METcumulative",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectMuTauPairsForAHtoMuTau_path02 = patMuTauPairSelConfiguratorForAHtoMuTau_path02.configure(pyNameSpace = locals())

selectMuTauPairsForAHtoMuTau = cms.Sequence(
    selectMuTauPairsForAHtoMuTau_path01 * selectMuTauPairsForAHtoMuTau_path02
)

# define additional collections of muon + tau-jet candidates
# with loose track and ECAL isolation applied on muon leg
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of muon isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)

selectedMuTauPairsForAHtoMuTauAntiOverlapVetoLooseMuonIsolation = copy.deepcopy(selectedMuTauPairsForAHtoMuTauAntiOverlapVeto)

selectedMuTauPairsForAHtoMuTauZeroChargeLooseMuonIsolation = copy.deepcopy(selectedMuTauPairsForAHtoMuTauZeroCharge)

selectedMuTauPairsForAHtoMuTauMt1METlooseMuonIsolation = copy.deepcopy(selectedMuTauPairsForAHtoMuTauMt1MET)

selectedMuTauPairsForAHtoMuTauPzetaDiffLooseMuonIsolation = copy.deepcopy(selectedMuTauPairsForAHtoMuTauPzetaDiff)

selectedMuTauPairsForAHtoMuTauNonBackToBackLooseMuonIsolation = copy.deepcopy(selectedMuTauPairsForAHtoMuTauNonBackToBack)

selectedMuTauPairsForAHtoMuTauValidCollinearApproxLooseMuonIsolation = copy.deepcopy(selectedMuTauPairsForAHtoMuTauValidCollinearApprox)

patMuTauPairSelConfiguratorForAHtoMuTauLooseMuonIsolation_path01 = objSelConfigurator(
    [ selectedMuTauPairsForAHtoMuTauAntiOverlapVetoLooseMuonIsolation,
      selectedMuTauPairsForAHtoMuTauZeroChargeLooseMuonIsolation,
      selectedMuTauPairsForAHtoMuTauMt1METlooseMuonIsolation,
      selectedMuTauPairsForAHtoMuTauPzetaDiffLooseMuonIsolation ],
    src = "allMuTauPairsLooseMuonIsolation",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectMuTauPairsForAHtoMuTauLooseMuonIsolation_path01 = \
  patMuTauPairSelConfiguratorForAHtoMuTauLooseMuonIsolation_path01.configure(pyNameSpace = locals())

patMuTauPairSelConfiguratorForAHtoMuTauLooseMuonIsolation_path02 = objSelConfigurator(
    [ selectedMuTauPairsForAHtoMuTauNonBackToBackLooseMuonIsolation,
      selectedMuTauPairsForAHtoMuTauValidCollinearApproxLooseMuonIsolation ],
    src = "selectedMuTauPairsForAHtoMuTauMt1METlooseMuonIsolationCumulative",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectMuTauPairsForAHtoMuTauLooseMuonIsolation_path02 = \
  patMuTauPairSelConfiguratorForAHtoMuTauLooseMuonIsolation_path02.configure(pyNameSpace = locals())

selectMuTauPairsForAHtoMuTauLooseMuonIsolation = cms.Sequence(
    selectMuTauPairsForAHtoMuTauLooseMuonIsolation_path01 * selectMuTauPairsForAHtoMuTauLooseMuonIsolation_path02
)    
