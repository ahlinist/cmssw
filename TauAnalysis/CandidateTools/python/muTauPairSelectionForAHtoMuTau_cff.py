import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.muTauPairSelection_cfi import *

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *

#--------------------------------------------------------------------------------  
# produce collections of muon + tau-jet pairs passing selection criteria
# specific to MSSM Higgs A/H --> tau+ tau- --> muon + tau-jet analysis
#--------------------------------------------------------------------------------

selectedMuTauPairsForAHtoMuTauAntiOverlapVeto = copy.deepcopy(selectedMuTauPairsAntiOverlapVeto)

selectedMuTauPairsForAHtoMuTauMt1MET = copy.deepcopy(selectedMuTauPairsMt1MET)

selectedMuTauPairsForAHtoMuTauPzetaDiff = copy.deepcopy(selectedMuTauPairsPzetaDiff)

selectedMuTauPairsForAHtoMuTauZeroCharge = copy.deepcopy(selectedMuTauPairsZeroCharge)

patMuTauPairSelConfiguratorForAHtoMuTauOS = objSelConfigurator(
    [ selectedMuTauPairsForAHtoMuTauAntiOverlapVeto,
      selectedMuTauPairsForAHtoMuTauMt1MET,
      selectedMuTauPairsForAHtoMuTauPzetaDiff,
      selectedMuTauPairsForAHtoMuTauZeroCharge ],
    src = "allMuTauPairs",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectMuTauPairsForAHtoMuTauOS = patMuTauPairSelConfiguratorForAHtoMuTauOS.configure(pyNameSpace = locals())

selectedMuTauPairsForAHtoMuTauNonZeroCharge = copy.deepcopy(selectedMuTauPairsNonZeroCharge)

patMuTauPairSelConfiguratorForAHtoMuTauSS = objSelConfigurator(
    [ selectedMuTauPairsForAHtoMuTauNonZeroCharge ],
    src = "selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectMuTauPairsForAHtoMuTauSS = patMuTauPairSelConfiguratorForAHtoMuTauSS.configure(pyNameSpace = locals())

selectMuTauPairsForAHtoMuTau = cms.Sequence(selectMuTauPairsForAHtoMuTauOS * selectMuTauPairsForAHtoMuTauSS)

# define additional collections of muon + tau-jet candidates
# with loose track and ECAL isolation applied on muon leg
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of muon isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)

selectedMuTauPairsForAHtoMuTauAntiOverlapVetoLooseMuonIsolation = copy.deepcopy(selectedMuTauPairsForAHtoMuTauAntiOverlapVeto)

selectedMuTauPairsForAHtoMuTauMt1METlooseMuonIsolation = copy.deepcopy(selectedMuTauPairsForAHtoMuTauMt1MET)

selectedMuTauPairsForAHtoMuTauPzetaDiffLooseMuonIsolation = copy.deepcopy(selectedMuTauPairsForAHtoMuTauPzetaDiff)

selectedMuTauPairsForAHtoMuTauZeroChargeLooseMuonIsolation = copy.deepcopy(selectedMuTauPairsForAHtoMuTauZeroCharge)

patMuTauPairSelConfiguratorForAHtoMuTauLooseMuonIsolationOS = objSelConfigurator(
    [ selectedMuTauPairsForAHtoMuTauAntiOverlapVetoLooseMuonIsolation,
      selectedMuTauPairsForAHtoMuTauMt1METlooseMuonIsolation,
      selectedMuTauPairsForAHtoMuTauPzetaDiffLooseMuonIsolation,
      selectedMuTauPairsForAHtoMuTauZeroChargeLooseMuonIsolation ],
    src = "allMuTauPairsLooseMuonIsolation",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectMuTauPairsForAHtoMuTauLooseMuonIsolationOS = \
  patMuTauPairSelConfiguratorForAHtoMuTauLooseMuonIsolationOS.configure(pyNameSpace = locals())

selectedMuTauPairsForAHtoMuTauNonZeroChargeLooseMuonIsolation = copy.deepcopy(selectedMuTauPairsForAHtoMuTauNonZeroCharge)

patMuTauPairSelConfiguratorForAHtoMuTauLooseMuonIsolationSS = objSelConfigurator(
    [ selectedMuTauPairsForAHtoMuTauNonZeroChargeLooseMuonIsolation ],
    src = "selectedMuTauPairsForAHtoMuTauPzetaDiffLooseMuonIsolationCumulative",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectMuTauPairsForAHtoMuTauLooseMuonIsolationSS = \
  patMuTauPairSelConfiguratorForAHtoMuTauLooseMuonIsolationSS.configure(pyNameSpace = locals())

selectMuTauPairsForAHtoMuTauLooseMuonIsolation = cms.Sequence(
  selectMuTauPairsForAHtoMuTauLooseMuonIsolationOS * selectMuTauPairsForAHtoMuTauLooseMuonIsolationSS
)
