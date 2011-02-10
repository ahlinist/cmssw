import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.elecTauPairSelection_cfi import *

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *


#---------------------------------------------------------------------------------------------------
# produce opposite- and same-sign collections of electron + tau-jet pairs passing selection criteria
#---------------------------------------------------------------------------------------------------

selectedElecTauPairsForAHtoElecTauAntiOverlapVeto = copy.deepcopy(selectedElecTauPairsAntiOverlapVeto)

selectedElecTauPairsForAHtoElecTauMt1MET= copy.deepcopy(selectedElecTauPairsMt1MET)

selectedElecTauPairsForAHtoElecTauPzetaDiff = copy.deepcopy(selectedElecTauPairsPzetaDiff)

selectedElecTauPairsForAHtoElecTauZeroCharge = copy.deepcopy(selectedElecTauPairsZeroCharge)

patElecTauPairSelConfiguratorForAHtoElecTauOS = objSelConfigurator(
    [ selectedElecTauPairsForAHtoElecTauAntiOverlapVeto,
      selectedElecTauPairsForAHtoElecTauMt1MET,
      selectedElecTauPairsForAHtoElecTauPzetaDiff,
      selectedElecTauPairsForAHtoElecTauZeroCharge ],
    src = "allElecTauPairs",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectElecTauPairsForAHtoElecTauOS = patElecTauPairSelConfiguratorForAHtoElecTauOS.configure(pyNameSpace = locals())

selectedElecTauPairsForAHtoElecTauNonZeroCharge = copy.deepcopy(selectedElecTauPairsNonZeroCharge)

patElecTauPairSelConfiguratorForAHtoElecTauSS = objSelConfigurator(
	[ selectedElecTauPairsForAHtoElecTauNonZeroCharge ],
    src = "selectedElecTauPairsForAHtoElecTauPzetaDiffCumulative",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectElecTauPairsForAHtoElecTauSS = patElecTauPairSelConfiguratorForAHtoElecTauSS.configure(pyNameSpace = locals())

selectElecTauPairsForAHtoElecTau = cms.Sequence(selectElecTauPairsForAHtoElecTauOS * selectElecTauPairsForAHtoElecTauSS)

# define additional collections of electron + tau-jet candidates
# with loose track and ECAL isolation applied on electron leg
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of electron isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)

selectedElecTauPairsForAHtoElecTauAntiOverlapVetoLooseElectronIsolation = copy.deepcopy(selectedElecTauPairsAntiOverlapVeto)

selectedElecTauPairsForAHtoElecTauMt1METlooseElectronIsolation = copy.deepcopy(selectedElecTauPairsMt1MET)

selectedElecTauPairsForAHtoElecTauPzetaDiffLooseElectronIsolation = copy.deepcopy(selectedElecTauPairsPzetaDiff)

selectedElecTauPairsForAHtoElecTauZeroChargeLooseElectronIsolation = copy.deepcopy(selectedElecTauPairsZeroCharge)

patElecTauPairSelConfiguratorForAHtoElecTauLooseElectronIsolationOS = objSelConfigurator(
    [ selectedElecTauPairsForAHtoElecTauAntiOverlapVetoLooseElectronIsolation,
      selectedElecTauPairsForAHtoElecTauMt1METlooseElectronIsolation,
      selectedElecTauPairsForAHtoElecTauPzetaDiffLooseElectronIsolation,
      selectedElecTauPairsForAHtoElecTauZeroChargeLooseElectronIsolation ],
    src = "allElecTauPairsLooseElectronIsolation",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectElecTauPairsForAHtoElecTauLooseElectronIsolationOS = patElecTauPairSelConfiguratorForAHtoElecTauLooseElectronIsolationOS.configure(pyNameSpace = locals())

selectedElecTauPairsForAHtoElecTauNonZeroChargeLooseElectronIsolation = copy.deepcopy(selectedElecTauPairsNonZeroCharge)

patElecTauPairSelConfiguratorForAHtoElecTauLooseElectronIsolationSS = objSelConfigurator(
    [ selectedElecTauPairsForAHtoElecTauNonZeroChargeLooseElectronIsolation ],
    src = "selectedElecTauPairsForAHtoElecTauPzetaDiffLooseElectronIsolationCumulative",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectElecTauPairsForAHtoElecTauLooseElectronIsolationSS = \
		patElecTauPairSelConfiguratorForAHtoElecTauLooseElectronIsolationSS.configure(pyNameSpace = locals())

selectElecTauPairsForAHtoElecTauLooseElectronIsolation = cms.Sequence(
		selectElecTauPairsForAHtoElecTauLooseElectronIsolationOS * selectElecTauPairsForAHtoElecTauLooseElectronIsolationSS
)


