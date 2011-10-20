import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *

#--------------------------------------------------------------------------------  
# produce collections of pat::Jets passing selection criteria
#--------------------------------------------------------------------------------

# see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePhysicsCutParser
# on how to use the cut-string parser

# select central jets
selectedPatJetsForAHtoElecTauEta = cms.EDFilter("PATJetSelector",
    cut = cms.string('abs(eta) < 4.5'),
    filter = cms.bool(False)
)

# select jets with Et > 20 GeV
selectedPatJetsForAHtoElecTauEt = cms.EDFilter("PATJetSelector",
    cut = cms.string('et > 20.'), 
    filter = cms.bool(False)
)

# select jets not identified as electron, muon or tau-jets
selectedPatJetsForAHtoElecTauAntiOverlapWithLeptonsVeto = cms.EDFilter("PATJetAntiOverlapSelector",
    srcNotToBeFiltered = cms.VInputTag(
        "selectedPatElectronsForElecTauTrkIPcumulative",
        "selectedPatMuonsTrkIPcumulative",
        "selectedPatTausForElecTauMuonVetoCumulative"
    ),                                                           
    dRmin = cms.double(0.5),
    filter = cms.bool(False)                                           
)

# select jets with Et > 30 GeV
selectedPatJetsForAHtoElecTauJetTag = cms.EDFilter("PATJetSelector",
    cut = cms.string('et > 30.'), 
    filter = cms.bool(False)
)

# select jets passing b-tagging discriminator
# (>= 3 tracks with impact parameter significance > 2.0)
selectedPatJetsForAHtoElecTauBtag = cms.EDFilter("PATJetSelector",
    cut = cms.string('bDiscriminator("trackCountingHighEffBJetTags") > 3.3'), 
    filter = cms.bool(False)
)

patJetSelConfiguratorForAHtoElecTauBtag = objSelConfigurator(
    [ selectedPatJetsForAHtoElecTauEta,
      selectedPatJetsForAHtoElecTauEt,
      selectedPatJetsForAHtoElecTauAntiOverlapWithLeptonsVeto,
      selectedPatJetsForAHtoElecTauBtag ],
    src = "cleanPatJets",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatJetsForAHtoElecTauBtag = patJetSelConfiguratorForAHtoElecTauBtag.configure(pyNameSpace = locals())

patJetSelConfiguratorForAHtoElecTauJetTag = objSelConfigurator(
    [ selectedPatJetsForAHtoElecTauJetTag ],
    src = 'selectedPatJetsForAHtoElecTauAntiOverlapWithLeptonsVetoCumulative',
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatJetsForAHtoElecTauJetTag = patJetSelConfiguratorForAHtoElecTauJetTag.configure(pyNameSpace = locals())

selectPatJetsForAHtoElecTau = cms.Sequence(selectPatJetsForAHtoElecTauBtag + selectPatJetsForAHtoElecTauJetTag)

# define additional collections of jets
# not overlapping with "loosely" isolated electrons
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of electron isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)

selectedPatJetsForAHtoElecTauAntiOverlapWithLeptonsVetoLooseElectronIsolation = selectedPatJetsForAHtoElecTauAntiOverlapWithLeptonsVeto.clone(
    srcNotToBeFiltered = cms.VInputTag(
        "selectedPatElectronsForElecTauTrkIPlooseIsolationCumulative",
        "selectedPatMuonsTrkIPcumulative",
        "selectedPatTausProngCumulative"
    )
)

selectedPatJetsForAHtoElecTauBtagLooseElectronIsolation = copy.deepcopy(selectedPatJetsForAHtoElecTauBtag)

patJetSelConfiguratorForAHtoElecTauBtagLooseElectronIsolation = objSelConfigurator(
    [ selectedPatJetsForAHtoElecTauEta,
      selectedPatJetsForAHtoElecTauEt,
      selectedPatJetsForAHtoElecTauAntiOverlapWithLeptonsVetoLooseElectronIsolation,
      selectedPatJetsForAHtoElecTauBtagLooseElectronIsolation ],
    src = "cleanPatJets",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatJetsForAHtoElecTauBtagLooseElectronIsolation = patJetSelConfiguratorForAHtoElecTauBtagLooseElectronIsolation.configure(pyNameSpace = locals())

selectedPatJetsForAHtoElecTauJetTagLooseElectronIsolation = copy.deepcopy(selectedPatJetsForAHtoElecTauJetTag)

patJetSelConfiguratorForAHtoElecTauJetTagLooseElectronIsolation = objSelConfigurator(
    [ selectedPatJetsForAHtoElecTauJetTagLooseElectronIsolation ],
    src = 'selectedPatJetsForAHtoElecTauAntiOverlapWithLeptonsVetoLooseElectronIsolationCumulative',
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatJetsForAHtoElecTauJetTagLooseElectronIsolation = patJetSelConfiguratorForAHtoElecTauJetTagLooseElectronIsolation.configure(pyNameSpace = locals())

selectPatJetsForAHtoElecTauLooseElectronIsolation = cms.Sequence(selectPatJetsForAHtoElecTauBtagLooseElectronIsolation + selectPatJetsForAHtoElecTauJetTagLooseElectronIsolation)
