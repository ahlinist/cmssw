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

selectedPatJetsEta21ForWTauNu = cms.EDFilter("PATJetSelector",
                                                cut = cms.string('abs(eta) < 2.1'),
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

patJetSelConfiguratorForWTauNu = objSelConfigurator(
    [ selectedPatJetsAntiOverlapWithTausVetoForWTauNu,
      selectedPatJetsEta21ForWTauNu,
      selectedPatJetsEt15ForWTauNu,
      selectedPatJetsEt20ForWTauNu ],
    src = "cleanPatJets",
    pyModuleName = __name__,
    doSelIndividual = False
    )
selectPatJetsForWTauNu =patJetSelConfiguratorForWTauNu.configure(pyNameSpace = locals())

#select jets for W->taunu analysis, loose isolation
selectedPatJetsAntiOverlapWithTausVetoForWTauNuLooseIsolation = cms.EDFilter("PATJetAntiOverlapSelector",
                                                                                srcNotToBeFiltered = cms.VInputTag("selectedPatTausForWTauNuEcalCrackVetoLooseIsolationCumulative"),
                                                                                dRmin = cms.double(0.7),
                                                                                filter = cms.bool(False)
                                                                                )
selectedPatJetsEta21ForWTauNuLooseIsolation = cms.EDFilter("PATJetSelector",
                                                              cut = cms.string('abs(eta) < 2.1'),
                                                              filter = cms.bool(False)
                                                              )

selectedPatJetsEt15ForWTauNuLooseIsolation = cms.EDFilter("PATJetSelector",
                                                             cut = cms.string('et > 15.'),
                                                             filter = cms.bool(False)
                                                             )

selectedPatJetsEt20ForWTauNuLooseIsolation = cms.EDFilter("PATJetSelector",
                                                             cut = cms.string('et > 20.'),
                                                             filter = cms.bool(False)
                                                             )

patJetSelConfiguratorForWTauNuLooseIsolation = objSelConfigurator(
    [ selectedPatJetsAntiOverlapWithTausVetoForWTauNuLooseIsolation,
      selectedPatJetsEta21ForWTauNuLooseIsolation,
      selectedPatJetsEt15ForWTauNuLooseIsolation,
      selectedPatJetsEt20ForWTauNuLooseIsolation ],
    src = "cleanPatJets",
    pyModuleName = __name__,
    doSelIndividual = False
    )
selectPatJetsForWTauNuLooseIsolation =patJetSelConfiguratorForWTauNuLooseIsolation.configure(pyNameSpace = locals())

selectPatSelJetsForWTauNu = cms.Sequence (selectPatJetsForWTauNu
                                             *selectPatJetsForWTauNuLooseIsolation) 
