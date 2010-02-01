import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------  
# produce collections of pat::MET objects passing selection criteria
#--------------------------------------------------------------------------------

# see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePhysicsCutParser
# on how to use the cut-string parser

# require at least 30 GeV missing transverse energy in the event
selectedLayer1METsPt25 = cms.EDFilter("PATMETSelector",
    src = cms.InputTag("layer1METs"),                                 
    cut = cms.string('pt > 25.'),
    filter = cms.bool(False)
)

selectedMetSignificance15 = cms.EDFilter("METSignificanceSelector",
                                           src = cms.InputTag("metsignificance"),
                                           cut = cms.string('metSignificance() > 15.'),
                                           filter = cms.bool(False)
                                       )


selectLayer1METs = cms.Sequence( selectedLayer1METsPt25
                                 *selectedMetSignificance15)

