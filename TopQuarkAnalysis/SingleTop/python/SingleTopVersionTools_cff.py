import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.tools.jetTools import *


def run35xOn31xMC(process,
                  jetSrc = cms.InputTag("antikt5CaloJets"),
                    #jetSrc = "antikt5CaloJets",
                    jetIdTag = "antikt5"):
    addJetID(process,jetSrc,jetIdTag)
    #addJetID(process,"antikt5CaloJets",jetIdTag)
    switchJetCollection(process,
                        cms.InputTag('antikt5CaloJets'),
                        doJTA                 = True,
                        doBTagging            = True,
                        jetCorrLabel          = ('AK5','Calo'),
                        doType1MET            = True,
                        genJetCollection      = cms.InputTag("antikt5GenJets"),
                        doJetID               = True,
                        jetIdLabel            = "antikt5",
                        )
