import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *

#--------------------------------------------------------------------------------  
# produce collections of tau-jet + MET pairs passing selection criteria
#--------------------------------------------------------------------------------
 
selectedTauNuPairsDPhiMetTau = cms.EDFilter("PATTauNuPairSelector",
     cut = cms.string('dPhi() > 0.3'),
     filter = cms.bool(False)
)


patTauNuPairSelConfigurator = objSelConfigurator(
    [ selectedTauNuPairsDPhiMetTau
      ],
    src = "allTauNuPairs",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectTauNuPairs = patTauNuPairSelConfigurator.configure(pyNameSpace = locals())

selectedTauNuPairsDPhiMetTauLooseIsolation = cms.EDFilter("PATTauNuPairSelector",
      cut = cms.string('dPhi() > 0.3'),
      filter = cms.bool(False)
)

patTauNuPairSelConfiguratorLooseIsolation = objSelConfigurator(
    [ selectedTauNuPairsDPhiMetTauLooseIsolation
      ],
    src = "allTauNuPairsLooseIsolation",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectTauNuPairsLooseIsolation = patTauNuPairSelConfiguratorLooseIsolation.configure(pyNameSpace = locals())
