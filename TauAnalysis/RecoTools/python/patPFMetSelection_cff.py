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
                                        cut = cms.string('pt > 30.'),
                                        filter = cms.bool(False)
                                        )
selectedPatPFMETsPt40 = cms.EDFilter("PATMETSelector",
                                        src = cms.InputTag("patPFMETs"),
                                        cut = cms.string('pt > 40'),
                                        filter = cms.bool(False)
                                        )

selectPatPFMETs = cms.Sequence(
    selectedPatPFMETsPt25
    *selectedPatPFMETsPt30
    *selectedPatPFMETsPt40)
