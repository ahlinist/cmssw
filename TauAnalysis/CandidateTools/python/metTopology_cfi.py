import FWCore.ParameterSet.Config as cms

from TauAnalysis.CandidateTools.tools.objProdConfigurator import *
from TauAnalysis.CandidateTools.tools.objSelConfigurator import *
#--------------------------------------------------------------------------------
# produce data-formats providing information 
# about distribution of energy deposits in the event 
# with respect to direction of missing Et vector
#--------------------------------------------------------------------------------

metTopologies = cms.EDProducer("MEtTopologyProducer",
    srcEnergyDeposits = cms.VInputTag('selectedPatTausForWTauNuEcalCrackVetoCumulative',
                                      'selectedPatJetsAntiOverlapWithTausVetoForWTauNuCumulative'
                                      ),                           
    globalThreshold = cms.double(0.5),
    verbosity = cms.untracked.int32(0)
)

selectedMEtTopology05 = cms.EDFilter("MEtTopologySelector",
    src = cms.InputTag('metTopologies'),
    cut = cms.string('Vanti()/Vparallel() < 0.4'),
    filter = cms.bool(False)
)

selectedMEtTopology025 = cms.EDFilter("MEtTopologySelector",
    src = cms.InputTag('metTopologies'),
    cut = cms.string('Vanti()/Vparallel() < 0.25'),
    filter = cms.bool(False)
)


metTopologyProdConfigurator = objProdConfigurator(
    metTopologies,
    pyModuleName = __name__
    )
produceMEtTopology = metTopologyProdConfigurator.configure(pyNameSpace = locals())

metTopologySelConfigurator = objSelConfigurator(
    [selectedMEtTopology05,
     selectedMEtTopology025],
    src = "metTopologies",
    pyModuleName = __name__,
    doSelIndividual = False
    )

selectMEtTopology = metTopologySelConfigurator.configure(pyNameSpace = locals())
