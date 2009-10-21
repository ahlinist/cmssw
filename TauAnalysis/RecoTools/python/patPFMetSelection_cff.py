import FWCore.ParameterSet.Config as cms

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *
import copy

#--------------------------------------------------------------------------------
# produce collections of pat::MET objects passing selection criteria
#--------------------------------------------------------------------------------

# see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePhysicsCutParser
# on how to use the cut-string parser

selectedLayer1PFMETsPt25 = cms.EDFilter("PATMETSelector",
                                            src = cms.InputTag("layer1PFMETs"),
                                            cut = cms.string('pt > 25.'),
                                            filter = cms.bool(False)
                                        )
selectLayer1PFMETs = cms.Sequence(selectedLayer1PFMETsPt25)
