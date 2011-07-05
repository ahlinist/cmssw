import FWCore.ParameterSet.Config as cms

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *
import copy

#--------------------------------------------------------------------------------
# produce collections of pat::MET objects passing selection criteria
#--------------------------------------------------------------------------------

# see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePhysicsCutParser
# on how to use the cut-string parser
selectedPatPFMETsPt25 = cms.EDFilter("PATMETSelector",
                                     src = cms.InputTag("patPFMETs"),
                                     cut = cms.string('pt > 25.'),
                                     filter = cms.bool(False)
                                     )
selectedPatPFMETsPt30 = cms.EDFilter("PATMETSelector",
                                        src = cms.InputTag("patPFMETs"),
                                        cut = cms.string('pt > 35.'),
#                                     cut = cms.string('pt > 0.'),
                                        filter = cms.bool(False)
                                        )
selectedPatPFMETsPt45 = cms.EDFilter("PATMETSelector",
                                        src = cms.InputTag("patPFMETs"),
                                        cut = cms.string('pt > 45'),
                                        filter = cms.bool(False)
                                        )


patPFMETSelConfigurator = objSelConfigurator(
    [ selectedPatPFMETsPt25,
      selectedPatPFMETsPt30,
      selectedPatPFMETsPt45 ],
    src = 'patPFMETs',
    pyModuleName = __name__,
    doSelIndividual = False
    )

selectPatPFMETs = patPFMETSelConfigurator.configure(pyNameSpace = locals())

