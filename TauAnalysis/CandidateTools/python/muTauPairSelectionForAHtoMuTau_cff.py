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

patMuTauPairSelConfiguratorForAHtoMuTau = objSelConfigurator(
    [ selectedMuTauPairsForAHtoMuTauAntiOverlapVeto,
      selectedMuTauPairsForAHtoMuTauZeroCharge,
      selectedMuTauPairsForAHtoMuTauMt1MET,
      selectedMuTauPairsForAHtoMuTauPzetaDiff,
      selectedMuTauPairsForAHtoMuTauNonBackToBack,
      selectedMuTauPairsForAHtoMuTauValidCollinearApprox ],
    src = "allMuTauPairs",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectMuTauPairsForAHtoMuTau = patMuTauPairSelConfiguratorForAHtoMuTau.configure(pyNameSpace = locals())

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

patMuTauPairSelConfiguratorForAHtoMuTauLooseMuonIsolation = objSelConfigurator(
    [ selectedMuTauPairsForAHtoMuTauAntiOverlapVetoLooseMuonIsolation,
      selectedMuTauPairsForAHtoMuTauZeroChargeLooseMuonIsolation,
      selectedMuTauPairsForAHtoMuTauMt1METlooseMuonIsolation,
      selectedMuTauPairsForAHtoMuTauPzetaDiffLooseMuonIsolation,
      selectedMuTauPairsForAHtoMuTauNonBackToBackLooseMuonIsolation,
      selectedMuTauPairsForAHtoMuTauValidCollinearApproxLooseMuonIsolation ],
    src = "allMuTauPairsLooseMuonIsolation",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectMuTauPairsForAHtoMuTauLooseMuonIsolation = \
  patMuTauPairSelConfiguratorForAHtoMuTauLooseMuonIsolation.configure(pyNameSpace = locals())
