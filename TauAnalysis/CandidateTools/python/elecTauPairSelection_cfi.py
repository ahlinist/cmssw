import FWCore.ParameterSet.Config as cms
import copy
from TauAnalysis.CandidateTools.tools.objSelConfigurator import *

#--------------------------------------------------------------------------------  
# produce collections of electron + tau-jet pairs passing selection criteria
# for SM Higgs seach
#--------------------------------------------------------------------------------

# require electron and tau-jet to be separated in eta-phi,
# in order to ensure that both do not refer to one and the same physical particle
selectedElecTauPairsAntiOverlapVeto = cms.EDFilter("PATElecTauPairSelector",
    cut = cms.string('dR12 > 0.3'),
    filter = cms.bool(False)
)

# require low transverse mass of electron and MET
selectedElecTauPairsMt1MET = cms.EDFilter("PATElecTauPairSelector",
    cut = cms.string('mt1MET < 40.'),
    filter = cms.bool(False)
)

# require missing transverse momentum to point
# in the direction of visible tau decay products
selectedElecTauPairsPzetaDiff = cms.EDFilter("PATElecTauPairSelector",
    cut = cms.string('(pZeta - 1.5*pZetaVis) > -2000.'),
    filter = cms.bool(False)
)

# require electron and tau to form a zero-charge pair
selectedElecTauPairsZeroCharge = cms.EDFilter("PATElecTauPairSelector",
    cut = cms.string('charge = 0'),
    filter = cms.bool(False)
)

# require electron and tau to form a non-zero-charge pair
selectedElecTauPairsNonZeroCharge = cms.EDFilter("PATElecTauPairSelector",
    cut = cms.string('charge != 0'),
    filter = cms.bool(False)
)

# configure production sequence
patElecTauPairSelConfiguratorOS = objSelConfigurator(
    [ selectedElecTauPairsAntiOverlapVeto,
      selectedElecTauPairsMt1MET,
      selectedElecTauPairsPzetaDiff,
      selectedElecTauPairsZeroCharge ],
    src = "allElecTauPairs",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectElecTauPairsOS = patElecTauPairSelConfiguratorOS.configure(pyNameSpace = locals())

patElecTauPairSelConfiguratorSS = objSelConfigurator(
    [ selectedElecTauPairsNonZeroCharge ],
    src = "selectedElecTauPairsPzetaDiffCumulative",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectElecTauPairsSS = patElecTauPairSelConfiguratorSS.configure(pyNameSpace = locals())

selectElecTauPairs = cms.Sequence( selectElecTauPairsOS * selectElecTauPairsSS)



# define additional collections of electron + tau-jet candidates
# with loose track and ECAL isolation applied on electron leg
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of electron isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)

selectedElecTauPairsAntiOverlapVetoLooseElectronIsolation = copy.deepcopy(selectedElecTauPairsAntiOverlapVeto)

selectedElecTauPairsMt1METlooseElectronIsolation = copy.deepcopy(selectedElecTauPairsMt1MET)

selectedElecTauPairsPzetaDiffLooseElectronIsolation = copy.deepcopy(selectedElecTauPairsPzetaDiff)

selectedElecTauPairsZeroChargeLooseElectronIsolation = copy.deepcopy(selectedElecTauPairsZeroCharge)

selectedElecTauPairsNonZeroChargeLooseElectronIsolation = copy.deepcopy(selectedElecTauPairsZeroCharge)

patElecTauPairSelConfiguratorLooseElectronIsolationOS = objSelConfigurator(
    [ selectedElecTauPairsAntiOverlapVetoLooseElectronIsolation,
      selectedElecTauPairsMt1METlooseElectronIsolation,
      selectedElecTauPairsPzetaDiffLooseElectronIsolation,
      selectedElecTauPairsZeroChargeLooseElectronIsolation ],
    src = "allElecTauPairsLooseElectronIsolation",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectElecTauPairsLooseElectronIsolationOS = patElecTauPairSelConfiguratorLooseElectronIsolationOS.configure(pyNameSpace = locals())

patElecTauPairSelConfiguratorLooseElectronIsolationSS = objSelConfigurator(
    [ selectedElecTauPairsNonZeroChargeLooseElectronIsolation ],
    src = "selectedElecTauPairsPzetaDiffLooseElectronIsolationCumulative",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectElecTauPairsLooseElectronIsolationSS = patElecTauPairSelConfiguratorLooseElectronIsolationSS.configure(pyNameSpace = locals())

selectElecTauPairsLooseElectronIsolation = cms.Sequence( selectElecTauPairsLooseElectronIsolationOS * selectElecTauPairsLooseElectronIsolationSS)

