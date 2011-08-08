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


selectedTauNuPairsMt40 = cms.EDFilter("PATTauNuPairSelector",
                                      cut = cms.string('mt() > 40'),
                                      filter = cms.bool(False)
                                      )

patTauNuPairSelConfigurator = objSelConfigurator(
    [
    #selectedTauNuPairsDPhiMetTau
    selectedTauNuPairsMt40
      ],
    src = "allTauNuPairs",
    pyModuleName = __name__,
    doSelIndividual = False
)

selectTauNuPairs = patTauNuPairSelConfigurator.configure(pyNameSpace = locals())
