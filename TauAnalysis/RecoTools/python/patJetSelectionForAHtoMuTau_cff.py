import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *

#--------------------------------------------------------------------------------  
# produce collections of pat::Jets passing selection criteria
#--------------------------------------------------------------------------------

# see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePhysicsCutParser
# on how to use the cut-string parser

# select central jets
selectedLayer1JetsForAHtoMuTauEta21 = cms.EDFilter("PATJetSelector",
    cut = cms.string('abs(eta) < 2.1'),
    filter = cms.bool(False)
)

# select jets with Et > 20 GeV
selectedLayer1JetsForAHtoMuTauEt20 = cms.EDFilter("PATJetSelector",
    cut = cms.string('et > 20.'), 
    filter = cms.bool(False)
)

# select jets not identified as electron, muon or tau-jets
selectedLayer1JetsForAHtoMuTauAntiOverlapWithLeptonsVeto = cms.EDFilter("PATJetAntiOverlapSelector",
    srcNotToBeFiltered = cms.VInputTag(
        "selectedLayer1ElectronsTrkIPcumulative",
        "selectedLayer1MuonsTrkIPcumulative",
        "selectedLayer1TausProngCumulative"
    ),                                                           
    dRmin = cms.double(0.7),
    filter = cms.bool(False)                                           
)

# select jets passing b-tagging discriminator
# (>= 3 tracks with impact parameter significance > 2.0)
selectedLayer1JetsForAHtoMuTauBtag = cms.EDFilter("PATJetSelector",
    cut = cms.string('bDiscriminator("trackCountingHighEffBJetTags") > 2.5'), 
    filter = cms.bool(False)
)

patJetSelConfiguratorForAHtoMuTau = objSelConfigurator(
    [ selectedLayer1JetsForAHtoMuTauEta21,
      selectedLayer1JetsForAHtoMuTauEt20,
      selectedLayer1JetsForAHtoMuTauAntiOverlapWithLeptonsVeto,
      selectedLayer1JetsForAHtoMuTauBtag ],
    src = "cleanLayer1Jets",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectLayer1JetsForAHtoMuTau = patJetSelConfiguratorForAHtoMuTau.configure(pyNameSpace = locals())

# define additional collections of jets
# not overlapping with "loosely" isolated muons
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of muon isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)

selectedLayer1JetsForAHtoMuTauAntiOverlapWithLeptonsVetoLooseMuonIsolation = selectedLayer1JetsForAHtoMuTauAntiOverlapWithLeptonsVeto.clone(
    srcNotToBeFiltered = cms.VInputTag(
        "selectedLayer1ElectronsTrkIPcumulative",
        "selectedLayer1MuonsTrkIPlooseIsolationCumulative",
        "selectedLayer1TausProngCumulative"
    )
)

selectedLayer1JetsForAHtoMuTauBtagLooseMuonIsolation = copy.deepcopy(selectedLayer1JetsForAHtoMuTauBtag)

patJetSelConfiguratorForAHtoMuTauLooseMuonIsolation = objSelConfigurator(
    [ selectedLayer1JetsForAHtoMuTauEta21,
      selectedLayer1JetsForAHtoMuTauEt20,
      selectedLayer1JetsForAHtoMuTauAntiOverlapWithLeptonsVetoLooseMuonIsolation,
      selectedLayer1JetsForAHtoMuTauBtagLooseMuonIsolation ],
    src = "cleanLayer1Jets",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectLayer1JetsForAHtoMuTauLooseMuonIsolation = patJetSelConfiguratorForAHtoMuTauLooseMuonIsolation.configure(pyNameSpace = locals())
