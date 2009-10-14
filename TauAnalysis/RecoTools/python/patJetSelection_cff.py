import FWCore.ParameterSet.Config as cms

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *

#--------------------------------------------------------------------------------  
# produce collections of pat::Jets passing selection criteria
#--------------------------------------------------------------------------------

# see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePhysicsCutParser
# on how to use the cut-string parser

# select jets not identified as electron, muon or tau-jets
selectedLayer1JetsAntiOverlapWithLeptonsVeto = cms.EDFilter("PATJetAntiOverlapSelector",
    srcNotToBeFiltered = cms.VInputTag("selectedLayer1ElectronsTrkIPcumulative",
                                       "selectedLayer1MuonsTrkIPcumulative",
                                       "selectedLayer1TausProngCumulative"),                                                           
    dRmin = cms.double(0.7),
    filter = cms.bool(False)                                           
)

# select central jets
selectedLayer1JetsEta21 = cms.EDFilter("PATJetSelector",
    cut = cms.string('abs(eta) < 2.1'),
    filter = cms.bool(False)
)

# select jets with Et > 20 GeV
selectedLayer1JetsEt20 = cms.EDFilter("PATJetSelector",
    cut = cms.string('et > 20.'), 
    filter = cms.bool(False)
)

patJetSelConfigurator = objSelConfigurator(
    [ selectedLayer1JetsAntiOverlapWithLeptonsVeto,
      selectedLayer1JetsEta21,
      selectedLayer1JetsEt20 ],
    src = "cleanLayer1Jets",
    pyModuleName = __name__,
    doSelIndividual = False
)
selectLayer1Jets = patJetSelConfigurator.configure(namespace = locals())

# Select jets for W->tau nu analysis
selectedLayer1JetsAntiOverlapWithTausVetoForWTauNu = cms.EDFilter("PATJetAntiOverlapSelector",
     srcNotToBeFiltered = cms.VInputTag("selectedLayer1TausForWTauNuChargeCumulative"),
     dRmin = cms.double(0.7),
     filter = cms.bool(False)
)

selectedLayer1JetsEta21ForWTauNu = cms.EDFilter("PATJetSelector",
    cut = cms.string('abs(eta) < 2.1'),
    filter = cms.bool(False)
)

selectedLayer1JetsEt15ForWTauNu = cms.EDFilter("PATJetSelector",                 
    cut = cms.string('et > 15.'),
    filter = cms.bool(False)
)

selectedLayer1JetsEt20ForWTauNu = cms.EDFilter("PATJetSelector",                 
    cut = cms.string('et > 20.'),
    filter = cms.bool(False)
)

patJetSelConfiguratorForWTauNu = objSelConfigurator(
    [ selectedLayer1JetsAntiOverlapWithTausVetoForWTauNu,
      selectedLayer1JetsEta21ForWTauNu,
      selectedLayer1JetsEt15ForWTauNu,
      selectedLayer1JetsEt20ForWTauNu ],
    src = "cleanLayer1Jets",
    pyModuleName = __name__,
    doSelIndividual = False
)

selectLayer1JetsForWTauNu = patJetSelConfiguratorForWTauNu.configure(namespace = locals())


