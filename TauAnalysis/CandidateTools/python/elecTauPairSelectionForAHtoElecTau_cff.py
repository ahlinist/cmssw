import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.elecTauPairSelection_cfi import *

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *


#--------------------------------------------------------------------------------  
# produce collections of electron + tau-jet pairs passing selection criteria
#--------------------------------------------------------------------------------

selectedElecTauPairsForAHtoElecTauAntiOverlapVeto = copy.deepcopy(selectedElecTauPairsAntiOverlapVeto)

selectedElecTauPairsForAHtoElecTauZeroCharge = copy.deepcopy(selectedElecTauPairsZeroCharge)

selectedElecTauPairsForAHtoElecTauMt1MET= copy.deepcopy(selectedElecTauPairsMt1MET)

selectedElecTauPairsForAHtoElecTauPzetaDiff = copy.deepcopy(selectedElecTauPairsPzetaDiff)

patElecTauPairSelConfiguratorForAHtoElecTau = objSelConfigurator(
    [ selectedElecTauPairsForAHtoElecTauAntiOverlapVeto,
      selectedElecTauPairsForAHtoElecTauZeroCharge,
      selectedElecTauPairsForAHtoElecTauMt1MET,
      selectedElecTauPairsForAHtoElecTauPzetaDiff ],
    src = "allElecTauPairs",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectElecTauPairsForAHtoElecTau = patElecTauPairSelConfiguratorForAHtoElecTau.configure(pyNameSpace = locals())

# define additional collections of electron + tau-jet candidates
# with loose track and ECAL isolation applied on electron leg
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of electron isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)

selectedElecTauPairsForAHtoElecTauAntiOverlapVetoLooseElectronIsolation = copy.deepcopy(selectedElecTauPairsAntiOverlapVeto)

selectedElecTauPairsForAHtoElecTauZeroChargeLooseElectronIsolation = copy.deepcopy(selectedElecTauPairsZeroCharge)

selectedElecTauPairsForAHtoElecTauMt1METlooseElectronIsolation = copy.deepcopy(selectedElecTauPairsMt1MET)

selectedElecTauPairsForAHtoElecTauPzetaDiffLooseElectronIsolation = copy.deepcopy(selectedElecTauPairsPzetaDiff)

patElecTauPairSelConfiguratorForAHtoElecTauLooseElectronIsolation = objSelConfigurator(
    [ selectedElecTauPairsForAHtoElecTauAntiOverlapVetoLooseElectronIsolation,
      selectedElecTauPairsForAHtoElecTauZeroChargeLooseElectronIsolation,
      selectedElecTauPairsForAHtoElecTauMt1METlooseElectronIsolation,
      selectedElecTauPairsForAHtoElecTauPzetaDiffLooseElectronIsolation ],
    src = "allElecTauPairsLooseElectronIsolation",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectElecTauPairsForAHtoElecTauLooseElectronIsolation = patElecTauPairSelConfiguratorForAHtoElecTauLooseElectronIsolation.configure(pyNameSpace = locals())


