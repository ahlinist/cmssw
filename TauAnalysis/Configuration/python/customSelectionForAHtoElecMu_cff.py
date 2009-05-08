import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.producePatTuple_cff import *
#from TauAnalysis.RecoTools.patLeptonSelection_cff import *
#from TauAnalysis.CandidateTools.elecMuPairProduction_cff import *
#from TauAnalysis.CandidateTools.diTauPairSelectionAllKinds_cff import *
#from TauAnalysis.RecoTools.patJetSelection_cff import *

#------------------------------------- met --------------------------------------#
layer1METs.metSource  = cms.InputTag("corMetType1Icone5Muons")

#---------------------------------- electrons -----------------------------------#
#******* custom reco ********
allLayer1Electrons.isolation.tracker.deltaR = cms.double(0.3)
allLayer1Electrons.isolation.tracker.vetos = cms.vstring('0.015','Threshold(1.0)')
allLayer1Electrons.isolation.ecal.deltaR = cms.double(0.4)
allLayer1Electrons.isolation.ecal.vetos = cms.vstring(
    'EcalBarrel:0.045', #0.045 #egamma default
    'EcalBarrel:RectangularEtaPhiVeto(-0.02,0.02,-0.5,0.5)', #-0.02,0.02,-0.5,0.5
    'EcalEndcaps:0.1', #0.07
    'EcalEndcaps:RectangularEtaPhiVeto(-0.05,0.05,-0.5,0.5)',#-0.02,0.02,-0.5,0.5
    'EcalBarrel:ThresholdFromTransverse(0.12)', #0.08
    'EcalEndcaps:ThresholdFromTransverse(0.3)'
)
allLayer1Electrons.isolation.hcal.deltaR = cms.double(0.4)
#***** custom selection *****
selectedLayer1ElectronsTightId.cut = cms.string('(abs(superCluster.eta) < 1.479 & electronID("loose") > 0 & eSuperClusterOverP < 1.05 & eSuperClusterOverP > 0.95) | (abs(superCluster.eta) > 1.479 & electronID("loose") > 0 & eSuperClusterOverP < 1.12 & eSuperClusterOverP > 0.95)')
selectedLayer1ElectronsEta21.cut = cms.string('abs(eta) < 2.4')
#selectedLayer1ElectronsPt15.cut = cms.string('pt > 15.')
selectedLayer1ElectronsIso = cms.EDFilter("PATElectronSelector",
     cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.1'),
     filter = cms.bool(False)
)
selectedLayer1ElectronsTrkIso = cms.EDFilter("PATElectronSelector",
     cut = cms.string('trackIso < 1.5'),
     filter = cms.bool(False)
)
selectedLayer1ElectronsEcalIso = cms.EDFilter("PATElectronSelector",
     cut = cms.string('ecalIso < 1.5'),
     filter = cms.bool(False)
)
selectedLayer1ElectronsHcalIso = cms.EDFilter("PATElectronSelector",
     cut = cms.string('hcalIso < 1.5'),
     filter = cms.bool(False)
)
patElectronSelConfigurator = objSelConfigurator(
    [ selectedLayer1ElectronsTightId,
      selectedLayer1ElectronsAntiCrackCut,
      selectedLayer1ElectronsEta21,
      selectedLayer1ElectronsPt15,
      selectedLayer1ElectronsIso,
      #selectedLayer1ElectronsTrkIso,
      #selectedLayer1ElectronsEcalIso,
      #selectedLayer1ElectronsHcalIso,
      selectedLayer1ElectronsTrk,
      selectedLayer1ElectronsTrkIP ],
    src = "cleanLayer1Electrons",
    pyModuleName = __name__,
    doSelIndividual = True
)
selectLayer1Electrons = patElectronSelConfigurator.configure(namespace = locals())
cleanLayer1ElectronsSel.selFlags = cms.PSet(
    tauAnalysisSelElectronTightIdGlobal = cms.PSet(
      src = cms.InputTag('selectedLayer1ElectronsTightIdIndividual')
    ),
    tauAnalysisSelElectronAntiCrackCut = cms.PSet(
      src = cms.InputTag('selectedLayer1ElectronsAntiCrackCutIndividual')
    ),
    tauAnalysisSelElectronEta21 = cms.PSet(
      src = cms.InputTag('selectedLayer1ElectronsEta21Individual')
    ),
    tauAnalysisSelElectronPt15 = cms.PSet(
      src = cms.InputTag('selectedLayer1ElectronsPt15Individual')
    ),
    tauAnalysisSelElectronIso = cms.PSet(
      src = cms.InputTag('selectedLayer1ElectronsIsoIndividual')
    ),
    #tauAnalysisSelElectronTrkIso = cms.PSet(
    #  src = cms.InputTag('selectedLayer1ElectronsTrkIsoIndividual')
    #),
    #tauAnalysisSelElectronEcalIso = cms.PSet(
    #  src = cms.InputTag('selectedLayer1ElectronsEcalIsoIndividual')
    #),
    #tauAnalysisSelElectronHcalIso = cms.PSet(
    #  src = cms.InputTag('selectedLayer1ElectronsHcalIsoIndividual')
    #),
    tauAnalysisSelElectronTrk = cms.PSet(
      src = cms.InputTag('selectedLayer1ElectronsTrkIndividual')
    ),
    tauAnalysisSelElectronTrkIP = cms.PSet(
      src = cms.InputTag('selectedLayer1ElectronsTrkIPindividual')
    )
)

#------------------------------------ muons -------------------------------------#
#******* custom reco ********
allLayer1Muons.isolation.tracker.deltaR = cms.double(0.3)
allLayer1Muons.isolation.ecal.deltaR = cms.double(0.3)
allLayer1Muons.isolation.hcal.deltaR = cms.double(0.3)
allLayer1Muons.isolation.user.deltaR = cms.double(0.3)
#***** custom selection *****
selectedLayer1MuonsEta21.cut = cms.string('abs(eta) < 2.4')
#selectedLayer1MuonsPt15.cut = cms.string('pt > 15.')
selectedLayer1MuonsIso = cms.EDFilter("PATMuonSelector",
     cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.1'),
     filter = cms.bool(False)
)
selectedLayer1MuonsTrkIso.vetos = cms.vstring("0.01", "Threshold(0.9)")                          
selectedLayer1MuonsTrkIso.dRisoCone = cms.double(0.3)
selectedLayer1MuonsTrkIso.sumPtMax = cms.double(1.)
selectedLayer1MuonsEcalIso.cut= cms.string('ecalIso < 1.')
selectedLayer1MuonsHcalIso = cms.EDFilter("PATMuonSelector",
    cut = cms.string('hcalIso < 1.'),
    filter = cms.bool(False)
)
patMuonSelConfigurator = objSelConfigurator(
    [ selectedLayer1MuonsGlobal,
      selectedLayer1MuonsEta21,
      selectedLayer1MuonsPt15,
      selectedLayer1MuonsIso,
      #selectedLayer1MuonsTrkIso,
      #selectedLayer1MuonsEcalIso,
      #selectedLayer1MuonsHcalIso,
      selectedLayer1MuonsPionVeto,
      selectedLayer1MuonsTrk,
      selectedLayer1MuonsTrkIP ],
    src = "cleanLayer1Muons",
    pyModuleName = __name__,
    doSelIndividual = True
)
selectLayer1Muons = patMuonSelConfigurator.configure(namespace = locals())
cleanLayer1MuonsSel.selFlags = cms.PSet(
    tauAnalysisSelMuonGlobal = cms.PSet(
      src = cms.InputTag('selectedLayer1MuonsGlobalIndividual')
    ),
    tauAnalysisSelMuonEta21 = cms.PSet(
      src = cms.InputTag('selectedLayer1MuonsEta21Individual')
    ),
    tauAnalysisSelMuonPt15 = cms.PSet(
      src = cms.InputTag('selectedLayer1MuonsPt15Individual')
    ),
    tauAnalysisSelMuonIso = cms.PSet(
      src = cms.InputTag('selectedLayer1MuonsIsoIndividual')
    ),
    #tauAnalysisSelMuonTrkIso = cms.PSet(
    #  src = cms.InputTag('selectedLayer1MuonsTrkIsoIndividual')
    #),
    #tauAnalysisSelMuonEcalIso = cms.PSet(
    #  src = cms.InputTag('selectedLayer1MuonsEcalIsoIndividual')
    #),
    #tauAnalysisSelMuonHcalIso = cms.PSet(
    #  src = cms.InputTag('selectedLayer1MuonsHcalIsoIndividual')
    #),
    tauAnalysisSelMuonPionVeto = cms.PSet(
      src = cms.InputTag('selectedLayer1MuonsPionVetoIndividual')
    ),
    tauAnalysisSelMuonTrkIP = cms.PSet(
      src = cms.InputTag('selectedLayer1MuonsTrkIPindividual')
    )
)

#----------------------------------- leptons ------------------------------------#
produceLayer1SelLeptons = cms.Sequence ( selectLayer1Electrons + produceLayer1SelElectrons
                                        +selectLayer1Muons + produceLayer1SelMuons )

#----------------------------- electron + muon pairs ----------------------------#
allElecMuPairs.srcLeg1 = cms.InputTag('selectedLayer1ElectronsTightIdCumulative')
allElecMuPairs.srcLeg2 = cms.InputTag('selectedLayer1MuonsGlobalCumulative')
allElecMuPairs.useLeadingTausOnly = cms.bool(True)
# require muon and tau not to be back-to-back
selectedElecMuPairsDPhi = cms.EDFilter("PATElecMuPairSelector",
     cut = cms.string('dPhi12 < 3.07'),
     filter = cms.bool(False)
)
# cut on combined impact parameter significance
selectedElecMuPairsImpParamSig = cms.EDFilter("PATElecMuIpSigSelector",
     vertexSource = cms.InputTag("selectedPrimaryVertexPosition"),
     IpMin = cms.double(1.5),
     filter = cms.bool(False)
)
patElecMuPairSelConfigurator = objSelConfigurator(
    [ selectedElecMuPairsZeroCharge,
      selectedElecMuPairsAcoplanarity #,
      #selectedElecMuPairsDPhi,
      #selectedElecMuPairsImpParamSig
      ],
    src = "allElecMuPairs",
    pyModuleName = __name__,
    doSelIndividual = True
)
selectElecMuPairs = patElecMuPairSelConfigurator.configure(namespace = locals())
#redefine produce sequence to make new selection work
produceElecMuPairs = cms.Sequence( allElecMuPairs  )
selectDiTauPairsAllKinds = cms.Sequence( selectElecMuPairs )
produceDiTauPairsAllKinds = cms.Sequence( produceElecMuPairs )

#------------------------------------- jets -------------------------------------#
selectedLayer1JetsAntiOverlapWithLeptonsVeto.srcNotToBeFiltered = cms.VInputTag("selectedLayer1ElectronsTrkCumulative",
                                                                                "selectedLayer1MuonsPionVetoCumulative")
selectedLayer1JetsEta21.cut = cms.string('abs(eta) < 2.4')
# select jets with b-tagging
selectedLayer1JetsBtag = cms.EDFilter("PATJetSelector",                                                                            
     cut = cms.string('bDiscriminator("trackCountingHighEffBJetTags")>2'),
     filter = cms.bool(False)
)
patJetSelConfigurator = objSelConfigurator(
    [ selectedLayer1JetsEt20,
      selectedLayer1JetsAntiOverlapWithLeptonsVeto,
      selectedLayer1JetsEta21,
      selectedLayer1JetsBtag ],
    src = "cleanLayer1Jets",
    pyModuleName = __name__,
    doSelIndividual = False
)
selectLayer1Jets = patJetSelConfigurator.configure(namespace = locals())

#----------------------------------- pattuple -----------------------------------#
producePatTuple = cms.Sequence( producePrePat      # comment-out if running on "officially" produced PAT-tuples
                               +patDefaultSequence # comment-out if running on "officially" produced PAT-tuples
                               +producePostPat
                               +produceLayer1SelLeptons + selectLayer1Jets
                               +produceDiTauPairsAllKinds
                               +selectDiTauPairsAllKinds )
