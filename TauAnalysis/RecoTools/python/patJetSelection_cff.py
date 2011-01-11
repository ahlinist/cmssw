import FWCore.ParameterSet.Config as cms

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *

#--------------------------------------------------------------------------------  
# produce collections of pat::Jets passing selection criteria
#--------------------------------------------------------------------------------

# see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePhysicsCutParser
# on how to use the cut-string parser

# select jets not identified as electron, muon or tau-jets
selectedPatJetsAntiOverlapWithLeptonsVeto = cms.EDFilter("PATJetAntiOverlapSelector",
    srcNotToBeFiltered = cms.VInputTag(
        "selectedPatElectronsTrkIPcumulative",
        "selectedPatMuonsTrkIPcumulative",
        "selectedPatTausProngCumulative"
    ),                                                           
    dRmin = cms.double(0.7),
    filter = cms.bool(False)                                           
)

# select central jets
selectedPatJetsEta21 = cms.EDFilter("PATJetSelector",
    cut = cms.string('abs(eta) < 2.1'),
    filter = cms.bool(False)
)

# select jets with Et > 20 GeV
selectedPatJetsEt20 = cms.EDFilter("PATJetSelector",
    cut = cms.string('et > 20.'), 
    filter = cms.bool(False)
)

patJetSelConfigurator = objSelConfigurator(
    [ selectedPatJetsAntiOverlapWithLeptonsVeto,
      selectedPatJetsEta21,
      selectedPatJetsEt20 ],
    src = "patJets",
    pyModuleName = __name__,
    doSelIndividual = False
)
selectPatJets = patJetSelConfigurator.configure(pyNameSpace = locals())
