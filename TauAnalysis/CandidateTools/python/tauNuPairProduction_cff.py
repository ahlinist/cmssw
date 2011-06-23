import FWCore.ParameterSet.Config as cms

from TauAnalysis.CandidateTools.tools.objProdConfigurator import *

#--------------------------------------------------------------------------------
# produce combinations of tau-jet + missing transverse momentum 
#--------------------------------------------------------------------------------

allTauNuPairs = cms.EDProducer("PATTauNuPairProducer",
    srcVisDecayProducts = cms.InputTag('selectedPatTausForWTauNuEcalCrackVetoCumulative'),
    srcMET = cms.InputTag('patMETs'),
    verbosity = cms.untracked.int32(0)
)

tauNuPairProdConfigurator = objProdConfigurator(
    allTauNuPairs,
    pyModuleName = __name__
)

produceTauNuPairs = tauNuPairProdConfigurator.configure(pyNameSpace = locals())
