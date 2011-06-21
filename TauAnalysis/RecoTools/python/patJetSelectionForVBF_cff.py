import FWCore.ParameterSet.Config as cms

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *

#--------------------------------------------------------------------------------  
# produce collections of pat::Jets passing selection criteria
#--------------------------------------------------------------------------------

# see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePhysicsCutParser
# on how to use the cut-string parser

# select forward/backward jets within geometric acceptance
selectedPatTagJetsForVBFEta99 = cms.EDFilter("PATJetSelector",
    cut = cms.string('abs(eta) < 9.9'),
    filter = cms.bool(False)
)

# select forward/backward jets jets with Et > 20 GeV
selectedPatTagJetsForVBFEt20 = cms.EDFilter("PATJetSelector",
    cut = cms.string('et > 20.'), 
    filter = cms.bool(False)
)

# select central jets
selectedPatCentralJetsForVBFEta25 = cms.EDFilter("PATJetSelector",
    cut = cms.string('abs(eta) < 2.5'),
    filter = cms.bool(False)
)

# select central jets with Et > 15 GeV
selectedPatCentralJetsForVBFEt15 = cms.EDFilter("PATJetSelector",
    cut = cms.string('et > 15.'),
    filter = cms.bool(False)
)

patJetSelConfiguratorForVBF = objSelConfigurator(
    [ selectedPatTagJetsForVBFEta99,
      selectedPatTagJetsForVBFEt20,
      selectedPatCentralJetsForVBFEta25,
      selectedPatCentralJetsForVBFEt15 ],
    src = "patJets",
    pyModuleName = __name__,
    doSelIndividual = False
)

selectPatJetsForVBF = patJetSelConfiguratorForVBF.configure(pyNameSpace = locals())
