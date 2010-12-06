import FWCore.ParameterSet.Config as cms

from TauAnalysis.CandidateTools.tools.objSelConfigurator import objSelConfigurator

#--------------------------------------------------------------------------------
# produce all objects specific to Z --> electron + muon channel
# which do **not** get stored in PATTuple
#--------------------------------------------------------------------------------
from TauAnalysis.RecoTools.recoVertexSelection_cff import *
selectedPrimaryVertexQuality.cut = cms.string("isValid & ndof > 3")
selectedPrimaryVertexPosition.cut = cms.string("z > -15 & z < +15 & sqrt(x*x+y*y) < 2")

#
# produce collections of pat::Electrons passing different selection criteria
#
selectedPatElectronsId = cms.EDFilter("PATElectronSelector",
  cut = cms.string('!isEBEEGap & (isEB & sigmaIetaIeta<0.01 & deltaPhiSuperClusterTrackAtVtx<0.06 & deltaPhiSuperClusterTrackAtVtx>-0.06 & abs(deltaEtaSuperClusterTrackAtVtx)<0.006 & hadronicOverEm<0.04 | isEE & sigmaIetaIeta<0.03 & deltaPhiSuperClusterTrackAtVtx<0.04 & deltaPhiSuperClusterTrackAtVtx>-0.04 & abs(deltaEtaSuperClusterTrackAtVtx)<999 & hadronicOverEm<0.025)'),
    filter = cms.bool(False)
)
selectedPatElectronsAntiCrackCut = cms.EDFilter("PATElectronSelector",
    cut = cms.string('abs(superCluster.eta) < 1.442 | abs(superCluster.eta) > 1.560'),
    filter = cms.bool(False)
)
selectedPatElectronsEta = cms.EDFilter("PATElectronSelector",
    cut = cms.string('abs(eta) < 2.4'),
    filter = cms.bool(False)
)
selectedPatElectronsPt = cms.EDFilter("PATElectronSelector",
    cut = cms.string('pt > 10.'),
    filter = cms.bool(False)
)
selectedPatElectronsIso = cms.EDFilter("PATElectronSelector",
     cut = cms.string('userIsolation("TrackIso")/pt + userIsolation("EcalIso")/pt + userIsolation("HcalIso")/pt < 0.25'),
     filter = cms.bool(False)
)
selectedPatElectronsTrk = cms.EDFilter("PATElectronSelector",
    cut = cms.string('gsfTrack.isNonnull'),
    filter = cms.bool(False)
)
selectedPatElectronsTrkIP = cms.EDFilter("PATElectronIpSelector",
    vertexSource = cms.InputTag("selectedPrimaryVertexPosition"),
    IpMax = cms.double(0.05),
    filter = cms.bool(False)                                               
)
patElectronSelConfigurator = objSelConfigurator(
    [
      selectedPatElectronsId,
      selectedPatElectronsAntiCrackCut,
      selectedPatElectronsEta,
      selectedPatElectronsPt,
      selectedPatElectronsIso,
      selectedPatElectronsTrk,
      selectedPatElectronsTrkIP ],
    src = "cleanPatElectrons",
    pyModuleName = __name__,
    doSelIndividual = True
)
selectPatElectrons = patElectronSelConfigurator.configure(pyNameSpace = locals())

#
# produce collections of pat::Muons passing different selection criteria
#

selectedPatMuonsGlobal = cms.EDFilter("PATMuonSelector",
    cut = cms.string('isGlobalMuon'),
    filter = cms.bool(False)
)
selectedPatMuonsEta = cms.EDFilter("PATMuonSelector",
    cut = cms.string('abs(eta) < 2.1'),
    filter = cms.bool(False)
)
selectedPatMuonsPt = cms.EDFilter("PATMuonSelector",
    cut = cms.string('pt > 10.'),
    filter = cms.bool(False)
)
selectedPatMuonsIso = cms.EDFilter("PATMuonSelector",
     cut = cms.string('userIsolation("TrackIso")/pt + userIsolation("EcalIso")/pt + userIsolation("HcalIso")/pt < 0.20'),
     filter = cms.bool(False)
)
selectedPatMuonsPionVeto = cms.EDFilter("PATMuonAntiPionSelector",
    CaloCompCoefficient = cms.double(0.8),
    SegmCompCoefficient = cms.double(1.2),
    AntiPionCut = cms.double(1.0),
    filter = cms.bool(False)
)
selectedPatMuonsTrk = cms.EDFilter("PATMuonSelector",
    cut = cms.string('innerTrack.isNonnull'),
    filter = cms.bool(False)
)
selectedPatMuonsTrkIP = cms.EDFilter("PATMuonIpSelector",
    vertexSource = cms.InputTag("selectedPrimaryVertexPosition"),
    IpMax = cms.double(0.05),
    filter = cms.bool(False)                                               
)
patMuonSelConfigurator = objSelConfigurator(
    [
      selectedPatMuonsGlobal,
      selectedPatMuonsEta,
      selectedPatMuonsPt,
      selectedPatMuonsIso,
      selectedPatMuonsPionVeto,
      selectedPatMuonsTrk,
      selectedPatMuonsTrkIP ],
    src = "cleanPatMuons",
    pyModuleName = __name__,
    doSelIndividual = True
)
selectPatMuons = patMuonSelConfigurator.configure(pyNameSpace = locals())

#
# produce collections of electron + muon pairs
# passing different selection criteria
#
from TauAnalysis.CandidateTools.elecMuPairProduction_cff import *
allElecMuPairs.srcLeg1 = cms.InputTag('selectedPatElectronsIdCumulative') 
allElecMuPairs.srcLeg2 = cms.InputTag('selectedPatMuonsGlobalCumulative')
allElecMuPairs.srcMET = cms.InputTag('patPFMETs')
allElecMuPairs.dRmin12 = cms.double(0.)

selectedElecMuPairsDRmin = cms.EDFilter("PATElecMuPairSelector",
     cut = cms.string('dR12 > 0.5'),
     filter = cms.bool(False)
)
selectedElecMuPairsZeroCharge = cms.EDFilter("PATElecMuPairSelector",
     cut = cms.string('charge = 0'),
     filter = cms.bool(False)
)
selectedElecMuPairsMt1MET = cms.EDFilter("PATElecMuPairSelector",
    cut = cms.string('mt1MET < 60.'),
    filter = cms.bool(False)
)
selectedElecMuPairsMt2MET = cms.EDFilter("PATElecMuPairSelector",
    cut = cms.string('mt2MET < 60.'),
    filter = cms.bool(False)
)
selectedElecMuPairsDPhi = cms.EDFilter("PATElecMuPairSelector",
     #cut = cms.string('dPhi12 < 3.07'),
     cut = cms.string('dPhi12 > 2.'),
     filter = cms.bool(False)
)
selectedElecMuPairsOneLegPt = cms.EDFilter("PATElecMuPairSelector",
    cut = cms.string('leg1.pt > 20 | leg2.pt > 20'),
    filter = cms.bool(False)
)
patElecMuPairSelConfigurator = objSelConfigurator(
    [ selectedElecMuPairsDRmin,
      selectedElecMuPairsZeroCharge,
      selectedElecMuPairsMt1MET,
      selectedElecMuPairsMt2MET,
      selectedElecMuPairsDPhi,
      #selectedElecMuPairsOneLegPt,
      ],
    src = "allElecMuPairs",
    pyModuleName = __name__,
    doSelIndividual = True
)
selectElecMuPairs = patElecMuPairSelConfigurator.configure(pyNameSpace = locals())

#
# produce collection of pat::Jets passing Et threshold and
# Eta acceptance cuts and not overlapping with any object
# passing selection criteria for pat::Electron, pat::Muon or pat::(PF)Tau
# (pat::Jet collection to be considered for central-jet veto)
#
selectedPatJetsEt20 = cms.EDFilter("PATJetSelector",
    cut = cms.string('et > 20.'), 
    filter = cms.bool(False)
)
selectedPatJetsEta = cms.EDFilter("PATJetSelector",
    cut = cms.string('abs(eta) < 2.4'),
    filter = cms.bool(False)
)
selectedPatJetsAntiOverlapWithLeptonsVeto = cms.EDFilter("PATJetAntiOverlapSelector",
    srcNotToBeFiltered = cms.VInputTag(
        "selectedPatElectronsTrkIPcumulative",
        "selectedPatMuonsTrkIPcumulative",
    ),                                                           
    dRmin = cms.double(0.7),
    filter = cms.bool(False)                                           
)
selectedPatJetsBtag = cms.EDFilter("PATJetSelector",                                                                            
     cut = cms.string('bDiscriminator("trackCountingHighEffBJetTags")>2'),
     filter = cms.bool(False)
)
patJetSelConfigurator = objSelConfigurator(
    [ selectedPatJetsEt20,
      selectedPatJetsEta,
      selectedPatJetsAntiOverlapWithLeptonsVeto,
      selectedPatJetsBtag ],
    src = "cleanPatJets",
    pyModuleName = __name__,
    doSelIndividual = False
)
selectPatJets = patJetSelConfigurator.configure(pyNameSpace = locals())

#
# produce collections of pat::MET objects
# passing different selection criteria
#
#met: controllare se ci sono correzione specifiche al tau
selectedMETMax = cms.EDFilter("PATMETSelector",
     cut = cms.string('et < 60'),
     filter = cms.bool(False)
)
patMETSelConfigurator = objSelConfigurator(
    [ selectedMETMax ],
    src = "patPFMETs",
    pyModuleName = __name__,
    doSelIndividual = True
)
selectPatMETs = patMETSelConfigurator.configure(pyNameSpace = locals())

# check for tau candidates
selectedPatTausForElecMu = cms.EDFilter("PATTauSelector",
    cut = cms.string('abs(eta) < 2.4 & pt > 10. & tauID("leadingTrackFinding") > 0.5 & tauID("leadingTrackPtCut") > 0.5 & tauID("byTaNCfrQuarterPercent") > 0.5 & tauID("trackIsolation") > 0.5 & tauID("ecalIsolation") > 0.5 & (signalPFChargedHadrCands.size() = 1 | signalPFChargedHadrCands.size() = 3) & abs(charge) > 0.5 & abs(charge) < 1.5 & tauID("againstElectron") > 0.5 & (abs(eta) < 1.460 | abs(eta) > 1.558) & tauID("againstMuon") > 0.5 & leadPFChargedHadrCand().isNonnull() & leadPFChargedHadrCand().pt() > 10.'),
    filter = cms.bool(False)                                 
)
patTauSelConfiguratorForElecMu = objSelConfigurator(
    [ selectedPatTausForElecMu, ],
    src = "cleanPatTaus",
    pyModuleName = __name__,
    doSelIndividual = False
)
selectPatTausForElecMu = patTauSelConfiguratorForElecMu.configure(pyNameSpace = locals())

#
# produce data-formats providing information about compatibility
# of an electron + muon pair with the hypothesis
# of being an electron pair/muon pair resulting from a Z --> e+ e-/Z --> mu+ mu- decay
#
#from TauAnalysis.RecoTools.elecMuPairZmumuHypothesis_cff import *

producePatTupleZtoElecMuSpecific = cms.Sequence(
    selectPatMuons 
   + selectPatElectrons 
   + produceElecMuPairs
   + selectElecMuPairs
   + selectPatJets
   + selectPatMETs
   + selectPatTausForElecMu
)

