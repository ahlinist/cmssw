import FWCore.ParameterSet.Config as cms

from TauAnalysis.CandidateTools.tools.objProdConfigurator import *
from TauAnalysis.CandidateTools.tools.objSelConfigurator import *

#--------------------------------------------------------------------------------
# produce data-formats providing information 
# about distribution of energy deposits in the event 
# with respect to direction of missing Et vector
#--------------------------------------------------------------------------------

htRatios = cms.EDProducer("HtRatioProducer",
    srcJet = cms.InputTag('selectedPatJetsEt15ForWTauNuCumulative'),
    srcTau = cms.InputTag('selectedPatTausForWTauNuEcalCrackVetoCumulative'),
    verbosity = cms.untracked.int32(0)
)

selectedHtRatio = cms.EDFilter("HtRatioSelector",
    src = cms.InputTag('htRatios'),
    cut = cms.string('Ratio() > 0.65'),
    filter = cms.bool(False)
)

selectedHtRatioLoose = cms.EDFilter("HtRatioSelector",
    src = cms.InputTag('htRatios'),
    cut = cms.string('Ratio() > 0.3'),
    filter = cms.bool(False)
)

htRatioProdConfigurator = objProdConfigurator(
    htRatios,
    pyModuleName = __name__
    )

produceHtRatio = htRatioProdConfigurator.configure(pyNameSpace = locals())

htRatioSelConfigurator = objSelConfigurator(
    [  selectedHtRatioLoose,
       selectedHtRatio ],
    src = "htRatios",
    pyModuleName = __name__,
    doSelIndividual = False
    )

selectHtRatio = htRatioSelConfigurator.configure(pyNameSpace = locals())
