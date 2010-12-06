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
patMETs.metSource  = cms.InputTag("corMetType1Icone5Muons") #no tau jet specific correction
#***** custom selection *****
selectedMETMax = cms.EDFilter("PATMETSelector",
     cut = cms.string('et < 60'),
     filter = cms.bool(False)
)
patMETSelConfigurator = objSelConfigurator(
    [ selectedMETMax ],
    src = "patMETs",
    pyModuleName = __name__,
    doSelIndividual = True
)
selectLayer1METs = patMETSelConfigurator.configure(pyNameSpace = locals())

#---------------------------------- electrons -----------------------------------#
#******* custom reco ********
patElectrons.userIsolation.tracker.deltaR = cms.double(0.3)
patElectrons.userIsolation.tracker.vetos = cms.vstring('0.015','Threshold(1.0)')
patElectrons.userIsolation.ecal.deltaR = cms.double(0.4)
patElectrons.userIsolation.ecal.vetos = cms.vstring(
    'EcalBarrel:0.045', #0.045 #egamma default
    'EcalBarrel:RectangularEtaPhiVeto(-0.02,0.02,-0.5,0.5)', #-0.02,0.02,-0.5,0.5
    'EcalEndcaps:0.1', #0.07
    'EcalEndcaps:RectangularEtaPhiVeto(-0.05,0.05,-0.5,0.5)',#-0.02,0.02,-0.5,0.5
    'EcalBarrel:ThresholdFromTransverse(0.12)', #0.08
    'EcalEndcaps:ThresholdFromTransverse(0.3)'
)
patElectrons.userIsolation.hcal.deltaR = cms.double(0.4)
#***** custom selection *****
selectedPatElectronsId = selectedPatElectronsTightId.clone()
selectedPatElectronsId.cut = cms.string('(abs(superCluster.eta) < 1.479 & electronID("loose") > 0 & eSuperClusterOverP < 1.05 & eSuperClusterOverP > 0.95) | (abs(superCluster.eta) > 1.479 & electronID("loose") > 0 & eSuperClusterOverP < 1.12 & eSuperClusterOverP > 0.95)')
selectedPatElectronsEta = selectedPatElectronsEta21.clone(cut = cms.string('abs(eta) < 2.4'))
selectedPatElectronsPt = selectedPatElectronsPt15.clone(cut = cms.string('pt > 10.'))
selectedPatElectronsIso = cms.EDFilter("PATElectronSelector",
     cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.20'),
     filter = cms.bool(False)
)
#cut optimization
selectedPatElectronsPt10 = selectedPatElectronsPt15.clone(cut = cms.string('pt > 10.'))
selectedPatElectronsPt11 = selectedPatElectronsPt15.clone(cut = cms.string('pt > 11.'))
selectedPatElectronsPt12 = selectedPatElectronsPt15.clone(cut = cms.string('pt > 12.'))
selectedPatElectronsPt13 = selectedPatElectronsPt15.clone(cut = cms.string('pt > 13.'))
selectedPatElectronsPt14 = selectedPatElectronsPt15.clone(cut = cms.string('pt > 14.'))
selectedPatElectronsPt15 = selectedPatElectronsPt15.clone(cut = cms.string('pt > 15.'))
selectedPatElectronsPt16 = selectedPatElectronsPt15.clone(cut = cms.string('pt > 16.'))
selectedPatElectronsPt17 = selectedPatElectronsPt15.clone(cut = cms.string('pt > 17.'))
selectedPatElectronsPt18 = selectedPatElectronsPt15.clone(cut = cms.string('pt > 18.'))
selectedPatElectronsPt19 = selectedPatElectronsPt15.clone(cut = cms.string('pt > 19.'))
selectedPatElectronsPt20 = selectedPatElectronsPt15.clone(cut = cms.string('pt > 20.'))
selectedPatElectronsLooseId = selectedPatElectronsId.clone(cut = cms.string('(abs(superCluster.eta) < 1.479 & electronID("loose") > 0 & eSuperClusterOverP < 1.05 & eSuperClusterOverP > 0.95) | (abs(superCluster.eta) > 1.479 & electronID("loose") > 0 & eSuperClusterOverP < 1.12 & eSuperClusterOverP > 0.95)'))
selectedPatElectronsTightId = selectedPatElectronsId.clone(cut = cms.string('(abs(superCluster.eta) < 1.479 & electronID("tight") > 0 & eSuperClusterOverP < 1.05 & eSuperClusterOverP > 0.95) | (abs(superCluster.eta) > 1.479 & electronID("tight") > 0 & eSuperClusterOverP < 1.12 & eSuperClusterOverP > 0.95)'))
selectedPatElectronsRobustId = selectedPatElectronsId.clone(cut = cms.string('(abs(superCluster.eta) < 1.479 & electronID("robust") > 0 & eSuperClusterOverP < 1.05 & eSuperClusterOverP > 0.95) | (abs(superCluster.eta) > 1.479 & electronID("robust") > 0 & eSuperClusterOverP < 1.12 & eSuperClusterOverP > 0.95)'))
selectedPatElectronsIso005 = selectedPatElectronsIso.clone(cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.05'))
selectedPatElectronsIso010 = selectedPatElectronsIso.clone(cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.10'))
selectedPatElectronsIso015 = selectedPatElectronsIso.clone(cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.15'))
selectedPatElectronsIso020 = selectedPatElectronsIso.clone(cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.20'))
selectedPatElectronsIso025 = selectedPatElectronsIso.clone(cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.25'))
selectedPatElectronsIso030 = selectedPatElectronsIso.clone(cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.30'))
selectedPatElectronsLooseId = selectedPatElectronsId.clone(cut = cms.string('(abs(superCluster.eta) < 1.479 & electronID("loose") > 0 & eSuperClusterOverP < 1.05 & eSuperClusterOverP > 0.95) | (abs(superCluster.eta) > 1.479 & electronID("loose") > 0 & eSuperClusterOverP < 1.12 & eSuperClusterOverP > 0.95)'))
selectedPatElectronsTightId = selectedPatElectronsId.clone(cut = cms.string('(abs(superCluster.eta) < 1.479 & electronID("tight") > 0 & eSuperClusterOverP < 1.05 & eSuperClusterOverP > 0.95) | (abs(superCluster.eta) > 1.479 & electronID("tight") > 0 & eSuperClusterOverP < 1.12 & eSuperClusterOverP > 0.95)'))
selectedPatElectronsRobustId = selectedPatElectronsId.clone(cut = cms.string('(abs(superCluster.eta) < 1.479 & electronID("robust") > 0 & eSuperClusterOverP < 1.05 & eSuperClusterOverP > 0.95) | (abs(superCluster.eta) > 1.479 & electronID("robust") > 0 & eSuperClusterOverP < 1.12 & eSuperClusterOverP > 0.95)'))
###########################
## selectedPatElectronsTrkIso = cms.EDFilter("PATElectronSelector",
##      cut = cms.string('trackIso < 1.5'),
##      filter = cms.bool(False)
## )
## selectedPatElectronsEcalIso = cms.EDFilter("PATElectronSelector",
##      cut = cms.string('ecalIso < 1.5'),
##      filter = cms.bool(False)
## )
## selectedPatElectronsHcalIso = cms.EDFilter("PATElectronSelector",
##      cut = cms.string('hcalIso < 1.5'),
##      filter = cms.bool(False)
## )
patElectronSelConfigurator = objSelConfigurator(
    [
##       ##uncomment for optimization
##       selectedPatElectronsPt10,
##       selectedPatElectronsPt11,
##       selectedPatElectronsPt12,
##       selectedPatElectronsPt13,
##       selectedPatElectronsPt14,
##       selectedPatElectronsPt15,
##       selectedPatElectronsPt16,
##       selectedPatElectronsPt17,
##       selectedPatElectronsPt18,
##       selectedPatElectronsPt19,
##       selectedPatElectronsPt20,
##       selectedPatElectronsIso005,
##       selectedPatElectronsIso010,
##       selectedPatElectronsIso015,
##       selectedPatElectronsIso020,
##       selectedPatElectronsIso025,
##       selectedPatElectronsIso030,
##       selectedPatElectronsLooseId,
##       selectedPatElectronsTightId,
##       selectedPatElectronsRobustId,
##       ###########################
      selectedPatElectronsId,
      selectedPatElectronsAntiCrackCut,
      selectedPatElectronsEta,
      selectedPatElectronsPt,
      selectedPatElectronsIso,
      #selectedPatElectronsTrkIso,
      #selectedPatElectronsEcalIso,
      #selectedPatElectronsHcalIso,
      selectedPatElectronsTrk,
      selectedPatElectronsTrkIP ],
    src = "cleanPatElectrons",
    pyModuleName = __name__,
    doSelIndividual = True
)
selectLayer1Electrons = patElectronSelConfigurator.configure(pyNameSpace = locals())
#cleanPatElectronsSel.selFlags = cms.PSet(
#    tauAnalysisSelElectronIdGlobal = cms.PSet(
#      src = cms.InputTag('selectedPatElectronsIdIndividual')
#    ),
#    tauAnalysisSelElectronAntiCrackCut = cms.PSet(
#      src = cms.InputTag('selectedPatElectronsAntiCrackCutIndividual')
#    ),
#    tauAnalysisSelElectronEta21 = cms.PSet(
#      src = cms.InputTag('selectedPatElectronsEtaIndividual')
#    ),
#    tauAnalysisSelElectronPt15 = cms.PSet(
#      src = cms.InputTag('selectedPatElectronsPtIndividual')
#    ),
#    tauAnalysisSelElectronIso = cms.PSet(
#      src = cms.InputTag('selectedPatElectronsIsoIndividual')
#    ),
#    #tauAnalysisSelElectronTrkIso = cms.PSet(
#    #  src = cms.InputTag('selectedPatElectronsTrkIsoIndividual')
#    #),
#    #tauAnalysisSelElectronEcalIso = cms.PSet(
#    #  src = cms.InputTag('selectedPatElectronsEcalIsoIndividual')
#    #),
#    #tauAnalysisSelElectronHcalIso = cms.PSet(
#    #  src = cms.InputTag('selectedPatElectronsHcalIsoIndividual')
#    #),
#    tauAnalysisSelElectronTrk = cms.PSet(
#      src = cms.InputTag('selectedPatElectronsTrkIndividual')
#    ),
#    tauAnalysisSelElectronTrkIP = cms.PSet(
#      src = cms.InputTag('selectedPatElectronsTrkIPindividual')
#    )
#)

#------------------------------------ muons -------------------------------------#
#******* custom reco ********
patMuons.userIsolation.tracker.deltaR = cms.double(0.3)
patMuons.userIsolation.ecal.deltaR = cms.double(0.3)
patMuons.userIsolation.hcal.deltaR = cms.double(0.3)
patMuons.userIsolation.user.deltaR = cms.double(0.3)
#***** custom selection *****
selectedPatMuonsEta = selectedPatMuonsEta21.clone(cut = cms.string('abs(eta) < 2.4'))
selectedPatMuonsPt = selectedPatMuonsPt15.clone(cut = cms.string('pt > 10.'))
selectedPatMuonsIso = cms.EDFilter("PATMuonSelector",
     cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.15'),
     filter = cms.bool(False)
)
##cut optimization
selectedPatMuonsPt10 = selectedPatMuonsPt15.clone(cut = cms.string('pt > 10.'))
selectedPatMuonsPt11 = selectedPatMuonsPt15.clone(cut = cms.string('pt > 11.'))
selectedPatMuonsPt12 = selectedPatMuonsPt15.clone(cut = cms.string('pt > 12.'))
selectedPatMuonsPt13 = selectedPatMuonsPt15.clone(cut = cms.string('pt > 13.'))
selectedPatMuonsPt14 = selectedPatMuonsPt15.clone(cut = cms.string('pt > 14.'))
selectedPatMuonsPt15 = selectedPatMuonsPt15.clone(cut = cms.string('pt > 14.'))
selectedPatMuonsPt16 = selectedPatMuonsPt15.clone(cut = cms.string('pt > 16.'))
selectedPatMuonsPt17 = selectedPatMuonsPt15.clone(cut = cms.string('pt > 17.'))
selectedPatMuonsPt18 = selectedPatMuonsPt15.clone(cut = cms.string('pt > 18.'))
selectedPatMuonsPt19 = selectedPatMuonsPt15.clone(cut = cms.string('pt > 19.'))
selectedPatMuonsPt20 = selectedPatMuonsPt15.clone(cut = cms.string('pt > 20.'))
selectedPatMuonsIso005 = selectedPatMuonsIso.clone(cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.05'))
selectedPatMuonsIso010 = selectedPatMuonsIso.clone(cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.10'))
selectedPatMuonsIso015 = selectedPatMuonsIso.clone(cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.15'))
selectedPatMuonsIso020 = selectedPatMuonsIso.clone(cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.20'))
selectedPatMuonsIso025 = selectedPatMuonsIso.clone(cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.25'))
selectedPatMuonsIso030 = selectedPatMuonsIso.clone(cut = cms.string('hcalIso/pt + ecalIso/pt + trackIso/pt < 0.30'))
###########################
patMuonSelConfigurator = objSelConfigurator(
    [
##       ##uncomment for optimization
##       selectedPatMuonsPt10,
##       selectedPatMuonsPt11,
##       selectedPatMuonsPt12,
##       selectedPatMuonsPt13,
##       selectedPatMuonsPt14,
##       selectedPatMuonsPt15,
##       selectedPatMuonsPt16,
##       selectedPatMuonsPt17,
##       selectedPatMuonsPt18,
##       selectedPatMuonsPt19,
##       selectedPatMuonsPt20,
##       selectedPatMuonsIso005,
##       selectedPatMuonsIso010,
##       selectedPatMuonsIso015,
##       selectedPatMuonsIso020,
##       selectedPatMuonsIso025,
##       selectedPatMuonsIso030,
##       ############################
      selectedPatMuonsGlobal,
      selectedPatMuonsEta,
      selectedPatMuonsPt,
      selectedPatMuonsIso,
      selectedPatMuonsTrk,
      selectedPatMuonsTrkIP ],
    src = "cleanPatMuons",
    pyModuleName = __name__,
    doSelIndividual = True
)
selectLayer1Muons = patMuonSelConfigurator.configure(pyNameSpace = locals())
#cleanPatMuonsSel.selFlags = cms.PSet(
#    tauAnalysisSelMuonGlobal = cms.PSet(
#      src = cms.InputTag('selectedPatMuonsGlobalIndividual')
#    ),
#    tauAnalysisSelMuonEta = cms.PSet(
#      src = cms.InputTag('selectedPatMuonsEtaIndividual')
#    ),
#    tauAnalysisSelMuonPt = cms.PSet(
#      src = cms.InputTag('selectedPatMuonsPtIndividual')
#    ),
#    tauAnalysisSelMuonIso = cms.PSet(
#      src = cms.InputTag('selectedPatMuonsIsoIndividual')
#    ),
#    tauAnalysisSelMuonTrkIP = cms.PSet(
#      src = cms.InputTag('selectedPatMuonsTrkIPindividual')
#    )
#)

#----------------------------------- leptons ------------------------------------#
produceLayer1SelLeptons = cms.Sequence ( selectLayer1Electrons + selectLayer1Muons )
#produceLayer1SelLeptons = cms.Sequence ( selectLayer1Electrons + produceLayer1SelElectrons
#                                        +selectLayer1Muons + produceLayer1SelMuons )

#----------------------------- electron + muon pairs ----------------------------#
allElecMuPairs.srcLeg1 = cms.InputTag('patElectrons')#CHECK!!!!!
allElecMuPairs.srcLeg2 = cms.InputTag('patMuons')
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
selectElecMuPairs = patElecMuPairSelConfigurator.configure(pyNameSpace = locals())
#redefine produce sequence to make new selection work
produceElecMuPairs = cms.Sequence( allElecMuPairs  )
selectDiTauPairsAllKinds = cms.Sequence( selectElecMuPairs )
produceDiTauPairsAllKinds = cms.Sequence( produceElecMuPairs )

#------------------------------------- jets -------------------------------------#
selectedPatJetsAntiOverlapWithLeptonsVeto.srcNotToBeFiltered = cms.VInputTag("selectedPatElectronsTrkCumulative",
                                                                                "selectedPatMuonsTrkCumulative")
selectedPatJetsEta = selectedPatJetsEta21.clone(cut = cms.string('abs(eta) < 2.4'))
# select jets with b-tagging
selectedPatJetsBtag = cms.EDFilter("PATJetSelector",                                                                            
     cut = cms.string('bDiscriminator("trackCountingHighEffBJetTags")>2'),
     filter = cms.bool(False)
)
patJetSelConfigurator = objSelConfigurator(
    [ selectedPatJetsEt20,
      selectedPatJetsAntiOverlapWithLeptonsVeto,
      selectedPatJetsEta,
      selectedPatJetsBtag ],
    src = "cleanPatJets",
    pyModuleName = __name__,
    doSelIndividual = False
)
selectLayer1Jets = patJetSelConfigurator.configure(pyNameSpace = locals())

#----------------------------------- pattuple -----------------------------------#
producePatTuple = cms.Sequence( producePrePat      # comment-out if running on "officially" produced PAT-tuples
                               +patDefaultSequence # comment-out if running on "officially" produced PAT-tuples
                               +producePostPat                               
                               +produceLayer1SelLeptons
                               +selectLayer1Jets
                               +selectLayer1METs
                               +produceDiTauPairsAllKinds
                               +selectDiTauPairsAllKinds )
