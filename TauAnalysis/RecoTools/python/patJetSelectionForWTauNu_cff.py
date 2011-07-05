import FWCore.ParameterSet.Config as cms

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *

#-----------------------------------------------------------------------------------------------
# produce collections of pat::Jets passing selection criteria, specific for W-->tau nu analysis
#---------------------------------------------------------------------------------------------

# see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePhysicsCutParser
# on how to use the cut-string parser

# Select jets for W->tau nu analysis
selectedPatJetsAntiOverlapWithTausVetoForWTauNu = cms.EDFilter("PATJetAntiOverlapSelector",
                                                                  srcNotToBeFiltered = cms.VInputTag("selectedPatTausForWTauNuEcalCrackVetoCumulative"),
                                                                  dRmin = cms.double(0.7),
                                                                  filter = cms.bool(False)
                                                                  )

selectedPatJetsEtaForWTauNu = cms.EDFilter("PATJetSelector",
                                                cut = cms.string('abs(eta) < 3.0'),
                                                filter = cms.bool(False)
                                                )

selectedPatJetsEt15ForWTauNu = cms.EDFilter("PATJetSelector",
                                               cut = cms.string('et > 15.'),
                                               filter = cms.bool(False)
                                               )

selectedPatJetsEt20ForWTauNu = cms.EDFilter("PATJetSelector",
                                               cut = cms.string('et > 20.'),
                                               filter = cms.bool(False)
                                               )

selectedPatJetsEt20ForWTauNuE = cms.EDFilter("PATJetSelector",
                                             cut = cms.string('et > 20'),
                                             filter = cms.bool(False)
                                             )



patJetSelConfiguratorForWTauNu = objSelConfigurator(
    [ #selectedPatJetsAntiOverlapWithTausVetoForWTauNu,
      selectedPatJetsEtaForWTauNu,
      selectedPatJetsEt15ForWTauNu,
      selectedPatJetsEt20ForWTauNu ],
#    src = "cleanPatJets",
    src = "patJets",
    pyModuleName = __name__,
    doSelIndividual = False
    )

patJetSelConfiguratorForWTauNu2 = objSelConfigurator(
    [ selectedPatJetsAntiOverlapWithTausVetoForWTauNu,
      selectedPatJetsEt20ForWTauNuE ],
#    src = "cleanPatJets",
    src = "patJets",
    pyModuleName = __name__,
    doSelIndividual = False
    )

selectPatJetsForWTauNu = patJetSelConfiguratorForWTauNu.configure(pyNameSpace = locals())
selectPatJetsForWTauNu2 = patJetSelConfiguratorForWTauNu2.configure(pyNameSpace = locals())

selectPatSelJetsForWTauNu = cms.Sequence (selectPatJetsForWTauNu
                                          *selectPatJetsForWTauNu2
                                          ) 
