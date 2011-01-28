import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *

#--------------------------------------------------------------------------------  
# produce collections of pat::Jets passing selection criteria
#--------------------------------------------------------------------------------

# see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePhysicsCutParser
# on how to use the cut-string parser

# select central jets
selectedPatJetsForAHtoElecTauEta21 = cms.EDFilter("PATJetSelector",
    cut = cms.string('abs(eta) < 2.4'),
    filter = cms.bool(False)
)

# select jets with Et > 20 GeV
selectedPatJetsForAHtoElecTauEt20 = cms.EDFilter("PATJetSelector",
    cut = cms.string('et > 20.'), 
    filter = cms.bool(False)
)

# select jets not identified as electron, muon or tau-jets
selectedPatJetsForAHtoElecTauAntiOverlapWithLeptonsVeto = cms.EDFilter("PATJetAntiOverlapSelector",
    srcNotToBeFiltered = cms.VInputTag(
        "selectedPatElectronsForElecTauTrkIPcumulative",
        "selectedPatMuonsTrkIPcumulative",
        "selectedPatTausForElecTauProngCumulative"
    ),                                                           
    dRmin = cms.double(0.7),
    filter = cms.bool(False)                                           
)

# select jets passing b-tagging discriminator
# (>= 3 tracks with impact parameter significance > 2.0)
selectedPatJetsForAHtoElecTauBtag = cms.EDFilter("PATJetSelector",
    cut = cms.string('bDiscriminator("trackCountingHighEffBJetTags") > 2.5'), 
    filter = cms.bool(False)
)

patJetSelConfiguratorForAHtoElecTau = objSelConfigurator(
    [ selectedPatJetsForAHtoElecTauEta21,
      selectedPatJetsForAHtoElecTauEt20,
      selectedPatJetsForAHtoElecTauAntiOverlapWithLeptonsVeto,
      selectedPatJetsForAHtoElecTauBtag ],
    src = "cleanPatJets",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatJetsForAHtoElecTau = patJetSelConfiguratorForAHtoElecTau.configure(pyNameSpace = locals())

# define additional collections of jets
# not overlapping with "loosely" isolated electrons
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of muon isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)

selectedPatJetsForAHtoElecTauAntiOverlapWithLeptonsVetoLooseElectronIsolation = selectedPatJetsForAHtoElecTauAntiOverlapWithLeptonsVeto.clone(
    srcNotToBeFiltered = cms.VInputTag(
        "selectedPatElectronsForElecTauTrkIPlooseIsolationCumulative",
        "selectedPatMuonsTrkIPcumulative",
        "selectedPatTausProngCumulative"
    )
)

selectedPatJetsForAHtoElecTauBtagLooseElectronIsolation = copy.deepcopy(selectedPatJetsForAHtoElecTauBtag)

patJetSelConfiguratorForAHtoElecTauLooseElectronIsolation = objSelConfigurator(
    [ selectedPatJetsForAHtoElecTauEta21,
      selectedPatJetsForAHtoElecTauEt20,
      selectedPatJetsForAHtoElecTauAntiOverlapWithLeptonsVetoLooseElectronIsolation,
      selectedPatJetsForAHtoElecTauBtagLooseElectronIsolation ],
    src = "cleanPatJets",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatJetsForAHtoElecTauLooseElectronIsolation = patJetSelConfiguratorForAHtoElecTauLooseElectronIsolation.configure(pyNameSpace = locals())
