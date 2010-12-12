import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *

#--------------------------------------------------------------------------------  
# produce collections of pat::Jets passing selection criteria
#--------------------------------------------------------------------------------

# see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePhysicsCutParser
# on how to use the cut-string parser

# select central jets
selectedPatJetsForAHtoMuTauEta21 = cms.EDFilter("PATJetSelector",
    cut = cms.string('abs(eta) < 2.4'),
    filter = cms.bool(False)
)

# select jets with Et > 20 GeV
selectedPatJetsForAHtoMuTauEt20 = cms.EDFilter("PATJetSelector",
    cut = cms.string('et > 20.'), 
    filter = cms.bool(False)
)

# select jets not identified as electron, muon or tau-jets
selectedPatJetsForAHtoMuTauAntiOverlapWithLeptonsVeto = cms.EDFilter("PATJetAntiOverlapSelector",
    srcNotToBeFiltered = cms.VInputTag(
        "selectedPatElectronsTrkIPcumulative",
        "selectedPatMuonsTrkIPcumulative",
        "selectedPatTausProngCumulative"
    ),                                                           
    dRmin = cms.double(0.7),
    filter = cms.bool(False)                                           
)

# select jets passing b-tagging discriminator
# (>= 3 tracks with impact parameter significance > 2.0)
selectedPatJetsForAHtoMuTauBtag = cms.EDFilter("PATJetSelector",
    cut = cms.string('bDiscriminator("trackCountingHighEffBJetTags") > 2.5'), 
    filter = cms.bool(False)
)

patJetSelConfiguratorForAHtoMuTau = objSelConfigurator(
    [ selectedPatJetsForAHtoMuTauEta21,
      selectedPatJetsForAHtoMuTauEt20,
      selectedPatJetsForAHtoMuTauAntiOverlapWithLeptonsVeto,
      selectedPatJetsForAHtoMuTauBtag ],
    src = "cleanPatJets",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatJetsForAHtoMuTau = patJetSelConfiguratorForAHtoMuTau.configure(pyNameSpace = locals())

# define additional collections of jets
# not overlapping with "loosely" isolated muons
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of muon isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)

selectedPatJetsForAHtoMuTauAntiOverlapWithLeptonsVetoLooseMuonIsolation = selectedPatJetsForAHtoMuTauAntiOverlapWithLeptonsVeto.clone(
    srcNotToBeFiltered = cms.VInputTag(
        "selectedPatElectronsTrkIPcumulative",
        "selectedPatMuonsTrkIPlooseIsolationCumulative",
        "selectedPatTausProngCumulative"
    )
)

selectedPatJetsForAHtoMuTauBtagLooseMuonIsolation = copy.deepcopy(selectedPatJetsForAHtoMuTauBtag)

patJetSelConfiguratorForAHtoMuTauLooseMuonIsolation = objSelConfigurator(
    [ selectedPatJetsForAHtoMuTauEta21,
      selectedPatJetsForAHtoMuTauEt20,
      selectedPatJetsForAHtoMuTauAntiOverlapWithLeptonsVetoLooseMuonIsolation,
      selectedPatJetsForAHtoMuTauBtagLooseMuonIsolation ],
    src = "cleanPatJets",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatJetsForAHtoMuTauLooseMuonIsolation = patJetSelConfiguratorForAHtoMuTauLooseMuonIsolation.configure(pyNameSpace = locals())
