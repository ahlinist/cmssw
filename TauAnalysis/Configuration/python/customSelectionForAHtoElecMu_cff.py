import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.producePatTuple_cff import *
from TauAnalysis.RecoTools.patLeptonSelection_cff import *
from TauAnalysis.RecoTools.patJetSelection_cff import *
from TauAnalysis.CandidateTools.elecMuPairProduction_cff import *
from TauAnalysis.CandidateTools.elecMuPairSelection_cfi import *
from TauAnalysis.CandidateTools.tools.objSelConfigurator import objSelConfigurator

#------------------------------------- met --------------------------------------#
#******* custom reco ********
layer1METs.metSource  = cms.InputTag("corMetType1Icone5Muons") #no tau jet specific correction
#***** custom selection *****
selectedMETMax = cms.EDFilter("PATMETSelector",
     cut = cms.string('et < 60'),
     filter = cms.bool(False)
)
patMETSelConfigurator = objSelConfigurator(
    [ selectedMETMax ],
    src = "layer1METs",
    pyModuleName = __name__,
    doSelIndividual = True
)
selectLayer1METs = patMETSelConfigurator.configure(namespace = locals())

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
selectedLayer1ElectronsId = selectedLayer1ElectronsTightId.clone()
selectedLayer1ElectronsId.cut = cms.string('(abs(superCluster.eta) < 1.479 & electronID("loose") > 0 & eSuperClusterOverP < 1.05 & eSuperClusterOverP > 0.95) | (abs(superCluster.eta) > 1.479 & electronID("loose") > 0 & eSuperClusterOverP < 1.12 & eSuperClusterOverP > 0.95)')
selectedLayer1ElectronsEta = selectedLayer1ElectronsEta21.clone(cut = cms.string('abs(eta) < 2.4'))
selectedLayer1ElectronsPt = selectedLayer1ElectronsPt15.clone(cut = cms.string('pt > 10.'))
selectedLayer1ElectronsIso = cms.EDFilter("PATElectronSelector",
     cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.20'),
     filter = cms.bool(False)
)
#cut optimization
selectedLayer1ElectronsPt10 = selectedLayer1ElectronsPt15.clone(cut = cms.string('pt > 10.'))
selectedLayer1ElectronsPt11 = selectedLayer1ElectronsPt15.clone(cut = cms.string('pt > 11.'))
selectedLayer1ElectronsPt12 = selectedLayer1ElectronsPt15.clone(cut = cms.string('pt > 12.'))
selectedLayer1ElectronsPt13 = selectedLayer1ElectronsPt15.clone(cut = cms.string('pt > 13.'))
selectedLayer1ElectronsPt14 = selectedLayer1ElectronsPt15.clone(cut = cms.string('pt > 14.'))
selectedLayer1ElectronsPt15 = selectedLayer1ElectronsPt15.clone(cut = cms.string('pt > 15.'))
selectedLayer1ElectronsPt16 = selectedLayer1ElectronsPt15.clone(cut = cms.string('pt > 16.'))
selectedLayer1ElectronsPt17 = selectedLayer1ElectronsPt15.clone(cut = cms.string('pt > 17.'))
selectedLayer1ElectronsPt18 = selectedLayer1ElectronsPt15.clone(cut = cms.string('pt > 18.'))
selectedLayer1ElectronsPt19 = selectedLayer1ElectronsPt15.clone(cut = cms.string('pt > 19.'))
selectedLayer1ElectronsPt20 = selectedLayer1ElectronsPt15.clone(cut = cms.string('pt > 20.'))
selectedLayer1ElectronsLooseId = selectedLayer1ElectronsId.clone(cut = cms.string('(abs(superCluster.eta) < 1.479 & electronID("loose") > 0 & eSuperClusterOverP < 1.05 & eSuperClusterOverP > 0.95) | (abs(superCluster.eta) > 1.479 & electronID("loose") > 0 & eSuperClusterOverP < 1.12 & eSuperClusterOverP > 0.95)'))
selectedLayer1ElectronsTightId = selectedLayer1ElectronsId.clone(cut = cms.string('(abs(superCluster.eta) < 1.479 & electronID("tight") > 0 & eSuperClusterOverP < 1.05 & eSuperClusterOverP > 0.95) | (abs(superCluster.eta) > 1.479 & electronID("tight") > 0 & eSuperClusterOverP < 1.12 & eSuperClusterOverP > 0.95)'))
selectedLayer1ElectronsRobustId = selectedLayer1ElectronsId.clone(cut = cms.string('(abs(superCluster.eta) < 1.479 & electronID("robust") > 0 & eSuperClusterOverP < 1.05 & eSuperClusterOverP > 0.95) | (abs(superCluster.eta) > 1.479 & electronID("robust") > 0 & eSuperClusterOverP < 1.12 & eSuperClusterOverP > 0.95)'))
selectedLayer1ElectronsIso005 = selectedLayer1ElectronsIso.clone(cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.05'))
selectedLayer1ElectronsIso010 = selectedLayer1ElectronsIso.clone(cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.10'))
selectedLayer1ElectronsIso015 = selectedLayer1ElectronsIso.clone(cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.15'))
selectedLayer1ElectronsIso020 = selectedLayer1ElectronsIso.clone(cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.20'))
selectedLayer1ElectronsIso025 = selectedLayer1ElectronsIso.clone(cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.25'))
selectedLayer1ElectronsIso030 = selectedLayer1ElectronsIso.clone(cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.30'))
selectedLayer1ElectronsLooseId = selectedLayer1ElectronsId.clone(cut = cms.string('(abs(superCluster.eta) < 1.479 & electronID("loose") > 0 & eSuperClusterOverP < 1.05 & eSuperClusterOverP > 0.95) | (abs(superCluster.eta) > 1.479 & electronID("loose") > 0 & eSuperClusterOverP < 1.12 & eSuperClusterOverP > 0.95)'))
selectedLayer1ElectronsTightId = selectedLayer1ElectronsId.clone(cut = cms.string('(abs(superCluster.eta) < 1.479 & electronID("tight") > 0 & eSuperClusterOverP < 1.05 & eSuperClusterOverP > 0.95) | (abs(superCluster.eta) > 1.479 & electronID("tight") > 0 & eSuperClusterOverP < 1.12 & eSuperClusterOverP > 0.95)'))
selectedLayer1ElectronsRobustId = selectedLayer1ElectronsId.clone(cut = cms.string('(abs(superCluster.eta) < 1.479 & electronID("robust") > 0 & eSuperClusterOverP < 1.05 & eSuperClusterOverP > 0.95) | (abs(superCluster.eta) > 1.479 & electronID("robust") > 0 & eSuperClusterOverP < 1.12 & eSuperClusterOverP > 0.95)'))
###########################
## selectedLayer1ElectronsTrkIso = cms.EDFilter("PATElectronSelector",
##      cut = cms.string('trackIso < 1.5'),
##      filter = cms.bool(False)
## )
## selectedLayer1ElectronsEcalIso = cms.EDFilter("PATElectronSelector",
##      cut = cms.string('ecalIso < 1.5'),
##      filter = cms.bool(False)
## )
## selectedLayer1ElectronsHcalIso = cms.EDFilter("PATElectronSelector",
##      cut = cms.string('hcalIso < 1.5'),
##      filter = cms.bool(False)
## )
patElectronSelConfigurator = objSelConfigurator(
    [
##       ##uncomment for optimization
##       selectedLayer1ElectronsPt10,
##       selectedLayer1ElectronsPt11,
##       selectedLayer1ElectronsPt12,
##       selectedLayer1ElectronsPt13,
##       selectedLayer1ElectronsPt14,
##       selectedLayer1ElectronsPt15,
##       selectedLayer1ElectronsPt16,
##       selectedLayer1ElectronsPt17,
##       selectedLayer1ElectronsPt18,
##       selectedLayer1ElectronsPt19,
##       selectedLayer1ElectronsPt20,
##       selectedLayer1ElectronsIso005,
##       selectedLayer1ElectronsIso010,
##       selectedLayer1ElectronsIso015,
##       selectedLayer1ElectronsIso020,
##       selectedLayer1ElectronsIso025,
##       selectedLayer1ElectronsIso030,
##       selectedLayer1ElectronsLooseId,
##       selectedLayer1ElectronsTightId,
##       selectedLayer1ElectronsRobustId,
##       ###########################
      selectedLayer1ElectronsId,
      selectedLayer1ElectronsAntiCrackCut,
      selectedLayer1ElectronsEta,
      selectedLayer1ElectronsPt,
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
    tauAnalysisSelElectronIdGlobal = cms.PSet(
      src = cms.InputTag('selectedLayer1ElectronsIdIndividual')
    ),
    tauAnalysisSelElectronAntiCrackCut = cms.PSet(
      src = cms.InputTag('selectedLayer1ElectronsAntiCrackCutIndividual')
    ),
    tauAnalysisSelElectronEta21 = cms.PSet(
      src = cms.InputTag('selectedLayer1ElectronsEtaIndividual')
    ),
    tauAnalysisSelElectronPt15 = cms.PSet(
      src = cms.InputTag('selectedLayer1ElectronsPtIndividual')
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
selectedLayer1MuonsEta = selectedLayer1MuonsEta21.clone(cut = cms.string('abs(eta) < 2.4'))
selectedLayer1MuonsPt = selectedLayer1MuonsPt15.clone(cut = cms.string('pt > 10.'))
selectedLayer1MuonsIso = cms.EDFilter("PATMuonSelector",
     cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.15'),
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
##cut optimization
selectedLayer1MuonsPt10 = selectedLayer1MuonsPt15.clone(cut = cms.string('pt > 10.'))
selectedLayer1MuonsPt11 = selectedLayer1MuonsPt15.clone(cut = cms.string('pt > 11.'))
selectedLayer1MuonsPt12 = selectedLayer1MuonsPt15.clone(cut = cms.string('pt > 12.'))
selectedLayer1MuonsPt13 = selectedLayer1MuonsPt15.clone(cut = cms.string('pt > 13.'))
selectedLayer1MuonsPt14 = selectedLayer1MuonsPt15.clone(cut = cms.string('pt > 14.'))
selectedLayer1MuonsPt15 = selectedLayer1MuonsPt15.clone(cut = cms.string('pt > 14.'))
selectedLayer1MuonsPt16 = selectedLayer1MuonsPt15.clone(cut = cms.string('pt > 16.'))
selectedLayer1MuonsPt17 = selectedLayer1MuonsPt15.clone(cut = cms.string('pt > 17.'))
selectedLayer1MuonsPt18 = selectedLayer1MuonsPt15.clone(cut = cms.string('pt > 18.'))
selectedLayer1MuonsPt19 = selectedLayer1MuonsPt15.clone(cut = cms.string('pt > 19.'))
selectedLayer1MuonsPt20 = selectedLayer1MuonsPt15.clone(cut = cms.string('pt > 20.'))
selectedLayer1MuonsIso005 = selectedLayer1MuonsIso.clone(cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.05'))
selectedLayer1MuonsIso010 = selectedLayer1MuonsIso.clone(cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.10'))
selectedLayer1MuonsIso015 = selectedLayer1MuonsIso.clone(cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.15'))
selectedLayer1MuonsIso020 = selectedLayer1MuonsIso.clone(cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.20'))
selectedLayer1MuonsIso025 = selectedLayer1MuonsIso.clone(cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.25'))
selectedLayer1MuonsIso030 = selectedLayer1MuonsIso.clone(cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.30'))
###########################
patMuonSelConfigurator = objSelConfigurator(
    [
##       ##uncomment for optimization
##       selectedLayer1MuonsPt10,
##       selectedLayer1MuonsPt11,
##       selectedLayer1MuonsPt12,
##       selectedLayer1MuonsPt13,
##       selectedLayer1MuonsPt14,
##       selectedLayer1MuonsPt15,
##       selectedLayer1MuonsPt16,
##       selectedLayer1MuonsPt17,
##       selectedLayer1MuonsPt18,
##       selectedLayer1MuonsPt19,
##       selectedLayer1MuonsPt20,
##       selectedLayer1MuonsIso005,
##       selectedLayer1MuonsIso010,
##       selectedLayer1MuonsIso015,
##       selectedLayer1MuonsIso020,
##       selectedLayer1MuonsIso025,
##       selectedLayer1MuonsIso030,
##       ############################
      selectedLayer1MuonsGlobal,
      selectedLayer1MuonsEta,
      selectedLayer1MuonsPt,
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
    tauAnalysisSelMuonEta = cms.PSet(
      src = cms.InputTag('selectedLayer1MuonsEtaIndividual')
    ),
    tauAnalysisSelMuonPt = cms.PSet(
      src = cms.InputTag('selectedLayer1MuonsPtIndividual')
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
allElecMuPairs.srcLeg1 = cms.InputTag('allLayer1Electrons')#CHECK!!!!!
allElecMuPairs.srcLeg2 = cms.InputTag('allLayer1Muons')
allElecMuPairs.useLeadingTausOnly = cms.bool(True)
# require missing transverse momentum to point either in direction of the electron
# or in direction of the muon in the transverse plane
selectedElecMuPairsAcoplanarity = cms.EDFilter("PATElecMuPairSelector",
    cut = cms.string('cos(dPhi1MET) > 0.5 | cos(dPhi2MET) > 0.5'),
    filter = cms.bool(False)
)
# require muon and tau not to be back-to-back
selectedElecMuPairsDPhi = cms.EDFilter("PATElecMuPairSelector",
     #cut = cms.string('dPhi12 < 3.07'),
     cut = cms.string('dPhi12 > 2.'),
     filter = cms.bool(False)
)
# cut on combined impact parameter significance
selectedElecMuPairsImpParamSig = cms.EDFilter("PATElecMuIpSigSelector",
     vertexSource = cms.InputTag("selectedPrimaryVertexPosition"),
     IpMin = cms.double(1.5),
     filter = cms.bool(False)
)
# at least one leg with pT > cut
selectedElecMuPairsOneLegPt = cms.EDFilter("PATElecMuPairSelector",
    cut = cms.string('leg1.pt > 20 | leg2.pt > 20'),
    filter = cms.bool(False)
)
## cut optimization
selectedElecMuPairsOneLegPt18 = selectedElecMuPairsOneLegPt.clone(cut = cms.string('leg1.pt > 18 | leg2.pt > 18'))
selectedElecMuPairsOneLegPt20 = selectedElecMuPairsOneLegPt.clone(cut = cms.string('leg1.pt > 20 | leg2.pt > 20'))
selectedElecMuPairsOneLegPt22 = selectedElecMuPairsOneLegPt.clone(cut = cms.string('leg1.pt > 22 | leg2.pt > 22'))
selectedElecMuPairsOneLegPt24 = selectedElecMuPairsOneLegPt.clone(cut = cms.string('leg1.pt > 24 | leg2.pt > 24'))
selectedElecMuPairsOneLegPt26 = selectedElecMuPairsOneLegPt.clone(cut = cms.string('leg1.pt > 26 | leg2.pt > 26'))
selectedElecMuPairsOneLegPt28 = selectedElecMuPairsOneLegPt.clone(cut = cms.string('leg1.pt > 28 | leg2.pt > 28'))
selectedElecMuPairsOneLegPt30 = selectedElecMuPairsOneLegPt.clone(cut = cms.string('leg1.pt > 30 | leg2.pt > 30'))
#############################
patElecMuPairSelConfigurator = objSelConfigurator(
    [ selectedElecMuPairsZeroCharge,
      #selectedElecMuPairsAcoplanarity #,
      selectedElecMuPairsDPhi,
      #selectedElecMuPairsImpParamSig
      selectedElecMuPairsOneLegPt,
##       ## uncomment for optimization
##       selectedElecMuPairsOneLegPt18,
##       selectedElecMuPairsOneLegPt20,
##       selectedElecMuPairsOneLegPt22,
##       selectedElecMuPairsOneLegPt24,
##       selectedElecMuPairsOneLegPt26,
##       selectedElecMuPairsOneLegPt28,
##       selectedElecMuPairsOneLegPt30
##       #############################
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
selectedLayer1JetsEta = selectedLayer1JetsEta21.clone(cut = cms.string('abs(eta) < 2.4'))
# select jets with b-tagging
selectedLayer1JetsBtag = cms.EDFilter("PATJetSelector",                                                                            
     cut = cms.string('bDiscriminator("trackCountingHighEffBJetTags")>2'),
     filter = cms.bool(False)
)
patJetSelConfigurator = objSelConfigurator(
    [ selectedLayer1JetsEt20,
      selectedLayer1JetsAntiOverlapWithLeptonsVeto,
      selectedLayer1JetsEta,
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
                               +produceLayer1SelLeptons
                               +selectLayer1Jets
                               +selectLayer1METs
                               +produceDiTauPairsAllKinds
                               +selectDiTauPairsAllKinds )
