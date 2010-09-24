import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *

#--------------------------------------------------------------------------------  
# produce collections of pat::Jets passing selection criteria
#--------------------------------------------------------------------------------

# see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePhysicsCutParser
# on how to use the cut-string parser

# select central jets
selectedPatJetsForZtoDiTauEta21 = cms.EDFilter("PATJetSelector",
    cut = cms.string('abs(eta) < 2.1'),
    filter = cms.bool(False)
)

# select jets with Et > 20 GeV
selectedPatJetsForZtoDiTauEt20 = cms.EDFilter("PATJetSelector",
    cut = cms.string('et > 20.'), 
    filter = cms.bool(False)
)

# select jets not identified as tau-jets
selectedPatJetsForZtoDiTauAntiOverlapWithLeptonsVeto = cms.EDFilter("PATJetAntiOverlapSelector",
    srcNotToBeFiltered = cms.VInputTag(
        "selectedPatTausForDiTau1stElectronVetoCumulative",
        "selectedPatTausForDiTau2ndElectronVetoCumulative"
    ),                                                           
    dRmin = cms.double(0.7),
    filter = cms.bool(False)                                           
)

patJetSelConfiguratorForZtoDiTau = objSelConfigurator(
    [ selectedPatJetsForZtoDiTauEta21,
      selectedPatJetsForZtoDiTauEt20,
      selectedPatJetsForZtoDiTauAntiOverlapWithLeptonsVeto ],
    src = "cleanPatJets",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatJetsForZtoDiTau = patJetSelConfiguratorForZtoDiTau.configure(pyNameSpace = locals())

# define additional collections of jets
# not overlapping with "loosely" selected tau-jets
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of muon isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)

selectedPatJetsForZtoDiTauAntiOverlapWithLeptonsVetoLoose2ndTau = selectedPatJetsForZtoDiTauAntiOverlapWithLeptonsVeto.clone(
    srcNotToBeFiltered = cms.VInputTag(
        "selectedPatTausForDiTau1stElectronVetoCumulative",
        "selectedPatTausForDiTau2ndElectronVetoLooseCumulative"
    )
)

patJetSelConfiguratorForZtoDiTauLoose2ndTau = objSelConfigurator(
    [ selectedPatJetsForZtoDiTauEta21,
      selectedPatJetsForZtoDiTauEt20,
      selectedPatJetsForZtoDiTauAntiOverlapWithLeptonsVetoLoose2ndTau ],
    src = "cleanPatJets",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatJetsForZtoDiTauLoose2ndTau = patJetSelConfiguratorForZtoDiTauLoose2ndTau.configure(pyNameSpace = locals())
