import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------  
# produce collections of pat::MET objects passing selection criteria
#--------------------------------------------------------------------------------

# see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePhysicsCutParser
# on how to use the cut-string parser

# require at least 30 GeV missing transverse energy in the event
selectedLayer1METsPt15 = cms.EDFilter("PATMETSelector",
    src = cms.InputTag("layer1METs"),                                 
    cut = cms.string('pt > 15.'),
    filter = cms.bool(False)
)

selectLayer1METs = cms.Sequence( selectedLayer1METsPt15 )
