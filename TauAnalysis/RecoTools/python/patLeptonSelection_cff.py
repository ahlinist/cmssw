import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.patElectronSelection_cfi import *
from TauAnalysis.RecoTools.patElectronSelectionForElecTau_cfi import *
from TauAnalysis.RecoTools.patElectronSelectionForElecMu_cfi import *
from TauAnalysis.RecoTools.patMuonSelection_cfi import *
from TauAnalysis.RecoTools.patPFTauSelection_cfi import *
from TauAnalysis.RecoTools.patPFTauSelectionForElecTau_cfi import *
from TauAnalysis.RecoTools.patPFTauSelectionForMuTau_cfi import *
from TauAnalysis.RecoTools.patPFTauSelectionForDiTau_cfi import *
from TauAnalysis.RecoTools.patPFTauSelectionForWTauNu_cfi import *

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *

#--------------------------------------------------------------------------------
# define selection criteria for pat::Electrons
# (settings made here overwrite values defined in electronPatSelector_cfi)
#--------------------------------------------------------------------------------

selectedPatElectronsTightId.cut = cms.string('(abs(superCluster.eta) < 1.479 & electronID("eidRobustTight") > 0 & eSuperClusterOverP < 1.05 & eSuperClusterOverP > 0.95) | (abs(superCluster.eta) > 1.479 & electronID("eidRobustTight") > 0 & eSuperClusterOverP < 1.12 & eSuperClusterOverP > 0.95)')
selectedPatElectronsLooseId.cut = cms.string('(abs(superCluster.eta) < 1.479 & electronID("eidRobustLoose") > 0 & eSuperClusterOverP < 1.05 & eSuperClusterOverP > 0.95) | (abs(superCluster.eta) > 1.479 & electronID("eidRobustLoose") > 0 & eSuperClusterOverP < 1.12 & eSuperClusterOverP > 0.95)')
selectedPatElectronsAntiCrackCut.cut = cms.string('abs(superCluster.eta) < 1.442 | abs(superCluster.eta) > 1.560')
selectedPatElectronsEta21.cut = cms.string('abs(eta) < 2.1')
selectedPatElectronsPt15.cut = cms.string('pt > 15.')
selectedPatElectronsTrkIso.cut = cms.string('userIsolation("pat::TrackIso") < 1.')
selectedPatElectronsEcalIso.cut = cms.string('(abs(superCluster.eta) < 1.479 & userIsolation("pat::EcalIso") < 2.5) | (abs(superCluster.eta) > 1.479 & userIsolation("pat::EcalIso") < 3.5)')
selectedPatElectronsTrk.cut = cms.string('gsfTrack.isNonnull')
selectedPatElectronsTrkIP.vertexSource = cms.InputTag("selectedPrimaryVertexHighestPtTrackSum")
selectedPatElectronsTrkIP.IpMax = cms.double(0.05)

patElectronSelConfigurator = objSelConfigurator(
    [ selectedPatElectronsTightId,
      selectedPatElectronsAntiCrackCut,
      selectedPatElectronsEta21,
      selectedPatElectronsPt15,
      selectedPatElectronsTrkIso,
      selectedPatElectronsEcalIso,
      selectedPatElectronsTrk,
      selectedPatElectronsTrkIP ],
    src = "cleanPatElectrons",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatElectrons = patElectronSelConfigurator.configure(pyNameSpace = locals())

selectedPatElectronsTrkIsoLooseIsolation.cut = cms.string('userIsolation("pat::TrackIso") < 8.')
selectedPatElectronsEcalIsoLooseIsolation.cut = cms.string('userIsolation("pat::EcalIso") < 8.')
selectedPatElectronsTrkLooseIsolation.cut = selectedPatElectronsTrk.cut
selectedPatElectronsTrkIPlooseIsolation.vertexSource = selectedPatElectronsTrkIP.vertexSource
selectedPatElectronsTrkIPlooseIsolation.IpMax = selectedPatElectronsTrkIP.IpMax

patElectronSelConfiguratorLooseIsolation = objSelConfigurator(
    [ selectedPatElectronsTightId,
      selectedPatElectronsAntiCrackCut,
      selectedPatElectronsEta21,
      selectedPatElectronsPt15,
      selectedPatElectronsTrkIsoLooseIsolation,
      selectedPatElectronsEcalIsoLooseIsolation,
      selectedPatElectronsTrkLooseIsolation,
      selectedPatElectronsTrkIPlooseIsolation ],
    src = "cleanPatElectrons",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatElectronsLooseIsolation = patElectronSelConfiguratorLooseIsolation.configure(pyNameSpace = locals())

#
# select electrons for Z->electron + tau-jet analysis
#

# VBTF WP90 electron ID and isolation
selectedPatElectronsForElecTauId.cut = cms.string('(abs(superCluster.eta) < 1.479 & abs(deltaEtaSuperClusterTrackAtVtx) < 0.007 & abs(deltaPhiSuperClusterTrackAtVtx) < 0.8 & hcalOverEcal < 0.12 & sigmaIetaIeta < 0.01) | (abs(superCluster.eta) > 1.479 & abs(deltaEtaSuperClusterTrackAtVtx) < 0.009 & abs(deltaPhiSuperClusterTrackAtVtx) <0.7 & hcalOverEcal < 0.05 & sigmaIetaIeta < 0.03)') 
selectedPatElectronsForElecTauAntiCrackCut.cut = cms.string('abs(superCluster.eta) < 1.442 | abs(superCluster.eta) > 1.560')
selectedPatElectronsForElecTauEta21.cut = cms.string('abs(eta) < 2.1')
selectedPatElectronsForElecTauPt15.cut = cms.string('pt > 15.')
selectedPatElectronsForElecTauTrkIso.cut = cms.string('(abs(superCluster.eta) < 1.479 & dr03TkSumPt/p4.Pt < 0.12) | (abs(superCluster.eta) > 1.479 & dr03TkSumPt/p4.Pt < 0.05) ')
selectedPatElectronsForElecTauEcalIso.cut = cms.string('(abs(superCluster.eta) < 1.479 & dr03EcalRecHitSumEt/p4.Pt < 0.09) | (abs(superCluster.eta) > 1.479 & dr03EcalRecHitSumEt/p4.Pt < 0.06)')
selectedPatElectronsForElecTauTrkIP.vertexSource = selectedPatElectronsTrkIP.vertexSource
selectedPatElectronsForElecTauTrkIP.IpMax = cms.double(0.05)
selectedPatElectronsForElecTauConversionVeto.cotThetaCut = cms.double(0.05)
selectedPatElectronsForElecTauConversionVeto.docaElecTrack = cms.double(0)
selectedPatElectronsForElecTauConversionVeto.dRElecTrack = cms.double(0.1)
selectedPatElectronsForElecTauConversionVeto.doPixCut = cms.bool(True)
selectedPatElectronsForElecTauConversionVeto.nTrkMax = cms.double(1)
selectedPatElectronsForElecTauConversionVeto.useConversionColl = cms.bool(False)

patElectronSelConfiguratorForElecTau = objSelConfigurator(
    [ selectedPatElectronsForElecTauId,
      selectedPatElectronsForElecTauAntiCrackCut,
      selectedPatElectronsForElecTauEta21,
      selectedPatElectronsForElecTauPt15,
      selectedPatElectronsForElecTauTrkIso,
      selectedPatElectronsForElecTauEcalIso,
      selectedPatElectronsForElecTauConversionVeto,
      selectedPatElectronsForElecTauTrkIP ],
    src = "cleanPatElectrons",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatElectronsForElecTau = patElectronSelConfiguratorForElecTau.configure(pyNameSpace = locals())

#  loose isolation
selectedPatElectronsForElecTauTrkIsoLooseIsolation.cut = cms.string('dr03TkSumPt/p4.Pt < 0.5')
selectedPatElectronsForElecTauEcalIsoLooseIsolation.cut = cms.string('dr03EcalRecHitSumEt/p4.Pt < 0.4')
selectedPatElectronsForElecTauTrkIPlooseIsolation.vertexSource = selectedPatElectronsForElecTauTrkIP.vertexSource
selectedPatElectronsForElecTauTrkIPlooseIsolation.IpMax = selectedPatElectronsForElecTauTrkIP.IpMax

selectedPatElectronsForElecTauConversionVetoLooseIsolation.nTrkMax = cms.double(10)
selectedPatElectronsForElecTauConversionVetoLooseIsolation.doPixCut = cms.bool(True)

patElectronSelConfiguratorForElecTauLooseIsolation = objSelConfigurator(
    [ selectedPatElectronsForElecTauId,
      selectedPatElectronsForElecTauAntiCrackCut,
      selectedPatElectronsForElecTauEta21,
      selectedPatElectronsForElecTauPt15,
      selectedPatElectronsForElecTauTrkIsoLooseIsolation,
      selectedPatElectronsForElecTauEcalIsoLooseIsolation,
      selectedPatElectronsForElecTauConversionVetoLooseIsolation,
      selectedPatElectronsForElecTauTrkIPlooseIsolation ],
    src = "cleanPatElectrons",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatElectronsForElecTauLooseIsolation = patElectronSelConfiguratorForElecTauLooseIsolation.configure(pyNameSpace = locals())

#
# select electrons for Z->electron + muon analysis
#

selectedPatElectronsForElecMuAntiOverlapWithMuonsVeto.dRmin = cms.double(0.3)
selectedPatElectronsForElecMuTightId.cut = cms.string('electronID("eidRobustTight") > 0')
selectedPatElectronsForElecMuAntiCrackCut.cut = selectedPatElectronsAntiCrackCut.cut
selectedPatElectronsForElecMuEta21.cut = selectedPatElectronsEta21.cut 
selectedPatElectronsForElecMuPt15.cut = selectedPatElectronsPt15.cut 
selectedPatElectronsForElecMuTrkIso.cut = cms.string('userIsolation("pat::TrackIso") < 2.')
selectedPatElectronsForElecMuEcalIso.cut = selectedPatElectronsEcalIso.cut
selectedPatElectronsForElecMuTrk.cut = selectedPatElectronsTrk.cut 
selectedPatElectronsForElecMuTrkIP.vertexSource = selectedPatElectronsTrkIP.vertexSource 
selectedPatElectronsForElecMuTrkIP.IpMax = selectedPatElectronsTrkIP.IpMax 

patElectronSelConfiguratorForElecMu = objSelConfigurator(
    [ selectedPatElectronsForElecMuAntiOverlapWithMuonsVeto,
      selectedPatElectronsForElecMuTightId,
      selectedPatElectronsForElecMuAntiCrackCut,
      selectedPatElectronsForElecMuEta21,
      selectedPatElectronsForElecMuPt15,
      selectedPatElectronsForElecMuTrkIso,
      selectedPatElectronsForElecMuEcalIso,
      selectedPatElectronsForElecMuTrk,
      selectedPatElectronsForElecMuTrkIP ],
    src = "cleanPatElectrons",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatElectronsForElecMu = patElectronSelConfiguratorForElecMu.configure(pyNameSpace = locals())

selectedPatElectronsForElecMuTrkIsoLooseIsolation.cut = cms.string('userIsolation("pat::TrackIso") < 8.')
selectedPatElectronsForElecMuEcalIsoLooseIsolation.cut = cms.string('userIsolation("pat::EcalIso") < 8.')
selectedPatElectronsForElecMuTrkLooseIsolation.cut = selectedPatElectronsForElecMuTrk.cut
selectedPatElectronsForElecMuTrkIPlooseIsolation.vertexSource = selectedPatElectronsForElecMuTrkIP.vertexSource
selectedPatElectronsForElecMuTrkIPlooseIsolation.IpMax = selectedPatElectronsForElecMuTrkIP.IpMax

patElectronSelConfiguratorForElecMuLooseIsolation = objSelConfigurator(
    [ selectedPatElectronsForElecMuAntiOverlapWithMuonsVeto,
      selectedPatElectronsForElecMuTightId,
      selectedPatElectronsForElecMuAntiCrackCut,
      selectedPatElectronsForElecMuEta21,
      selectedPatElectronsForElecMuPt15,
      selectedPatElectronsForElecMuTrkIsoLooseIsolation,
      selectedPatElectronsForElecMuEcalIsoLooseIsolation,
      selectedPatElectronsForElecMuTrkLooseIsolation,
      selectedPatElectronsForElecMuTrkIPlooseIsolation ],
    src = "cleanPatElectrons",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatElectronsForElecMuLooseIsolation = patElectronSelConfiguratorForElecMuLooseIsolation.configure(pyNameSpace = locals())

#--------------------------------------------------------------------------------
# define selection criteria for pat::Muons
# (settings made here overwrite values defined in muonPatSelector_cfi)
#--------------------------------------------------------------------------------

selectedPatMuonsGlobal.cut = cms.string('isGlobalMuon()')
selectedPatMuonsEta21.cut = cms.string('abs(eta) < 2.1')
selectedPatMuonsPt10.cut = cms.string('pt > 10.')
selectedPatMuonsVbTfId.beamSpotSource = cms.InputTag("offlineBeamSpot")
selectedPatMuonsPFRelIso.chargedHadronIso.ptMin = cms.double(-1.)
selectedPatMuonsPFRelIso.chargedHadronIso.dRvetoCone = cms.double(-1.)
selectedPatMuonsPFRelIso.chargedHadronIso.dRisoCone = cms.double(0.4)
selectedPatMuonsPFRelIso.neutralHadronIso.ptMin = cms.double(0.5)
selectedPatMuonsPFRelIso.neutralHadronIso.dRvetoCone = cms.double(0.08)
selectedPatMuonsPFRelIso.neutralHadronIso.dRisoCone = cms.double(0.4)
selectedPatMuonsPFRelIso.photonIso.ptMin = cms.double(0.5)
selectedPatMuonsPFRelIso.photonIso.dRvetoCone = cms.double(-1.)
selectedPatMuonsPFRelIso.photonIso.dRisoCone = cms.double(0.4)
selectedPatMuonsPFRelIso.sumPtMax = cms.double(0.10)
selectedPatMuonsPFRelIso.sumPtMethod = cms.string("relative")
selectedPatMuonsPionVeto.CaloCompCoefficient = cms.double(0.8)
selectedPatMuonsPionVeto.SegmCompCoefficient = cms.double(1.2)
selectedPatMuonsPionVeto.AntiPionCut = cms.double(1.0)
selectedPatMuonsTrk.cut = cms.string('innerTrack.isNonnull')
selectedPatMuonsTrkIP.vertexSource = cms.InputTag("selectedPrimaryVertexHighestPtTrackSum")
selectedPatMuonsTrkIP.IpMax = cms.double(0.05)

patMuonSelConfigurator = objSelConfigurator(
    [ selectedPatMuonsGlobal,
      selectedPatMuonsEta21,
      selectedPatMuonsPt10,
      selectedPatMuonsVbTfId,
      selectedPatMuonsPFRelIso,
      selectedPatMuonsPionVeto,
      selectedPatMuonsTrk,
      selectedPatMuonsTrkIP ],
    src = "cleanPatMuons",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatMuons = patMuonSelConfigurator.configure(pyNameSpace = locals())

selectedPatMuonsPFRelIsoLooseIsolation.sumPtMax = cms.double(0.25)
selectedPatMuonsPFRelIsoLooseIsolation.sumPtMethod = cms.string("relative")
selectedPatMuonsPionVetoLooseIsolation.CaloCompCoefficient = selectedPatMuonsPionVeto.CaloCompCoefficient
selectedPatMuonsPionVetoLooseIsolation.SegmCompCoefficient = selectedPatMuonsPionVeto.SegmCompCoefficient
selectedPatMuonsPionVetoLooseIsolation.AntiPionCut = selectedPatMuonsPionVeto.AntiPionCut
selectedPatMuonsTrkLooseIsolation.cut = selectedPatMuonsTrk.cut
selectedPatMuonsTrkIPlooseIsolation.vertexSource = selectedPatMuonsTrkIP.vertexSource
selectedPatMuonsTrkIPlooseIsolation.IpMax = selectedPatMuonsTrkIP.IpMax

patMuonSelConfiguratorLooseIsolation = objSelConfigurator(
    [ selectedPatMuonsGlobal,
      selectedPatMuonsEta21,
      selectedPatMuonsPt10,
      selectedPatMuonsVbTfId,
      selectedPatMuonsPFRelIsoLooseIsolation,
      selectedPatMuonsPionVetoLooseIsolation,
      selectedPatMuonsTrkLooseIsolation,
      selectedPatMuonsTrkIPlooseIsolation ],
    src = "cleanPatMuons",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatMuonsLooseIsolation = patMuonSelConfiguratorLooseIsolation.configure(pyNameSpace = locals())

#--------------------------------------------------------------------------------
# define selection criteria for pat::(PF)Taus
# (settings made here overwrite values defined in pftauPatSelector_cfi)
#--------------------------------------------------------------------------------

selectedPatTausEta21.cut = cms.string("abs(eta) < 2.1")
selectedPatTausPt20.cut = cut = cms.string("pt > 20.")
selectedPatTausLeadTrk.cut = cms.string('tauID("leadingTrackFinding") > 0.5')
selectedPatTausLeadTrkPt.cut = cms.string('tauID("leadingTrackPtCut") > 0.5')
selectedPatTausTaNCdiscr.cut = cms.string('tauID("byTaNCfrQuarterPercent") > -1.e+3') # cut on TaNC output disabled per default
selectedPatTausTrkIso.cut = cms.string('tauID("trackIsolation") > 0.5')
selectedPatTausEcalIso.cut = cms.string('tauID("ecalIsolation") > 0.5')
selectedPatTausProng.cut = cms.string("signalPFChargedHadrCands.size() = 1 | signalPFChargedHadrCands.size() = 3")
selectedPatTausCharge.cut = cms.string('abs(charge) > 0.5 & abs(charge) < 1.5')
selectedPatTausMuonVeto.cut = cms.string('tauID("againstMuon") > 0.5')
selectedPatTausElectronVeto.cut = cms.string('tauID("againstElectron") > 0.5')
selectedPatTausEcalCrackVeto.cut = cms.string("abs(eta) < 1.460 | abs(eta) > 1.558")

patTauSelConfigurator = objSelConfigurator(
    [ selectedPatTausEta21,
      selectedPatTausPt20,
      selectedPatTausLeadTrk,
      selectedPatTausLeadTrkPt,
      selectedPatTausTaNCdiscr,
      selectedPatTausTrkIso,
      selectedPatTausEcalIso,
      selectedPatTausProng,
      selectedPatTausCharge,
      selectedPatTausMuonVeto,
      selectedPatTausElectronVeto,
      selectedPatTausEcalCrackVeto ],
    src = "cleanPatTaus",
    pyModuleName = __name__,
    doSelCumulative = True,
    doSelIndividual = True
)

selectPatTaus = patTauSelConfigurator.configure(pyNameSpace = locals())
#
# define collections of pat::(PF)Taus used in semi-leptonic e + tau-jet channel
# (require electron and tau-jet candidates to be separated in eta-phi,
#  in order to avoid double-counting the same particle both as electron and as tau-jet;
#  apply anti-electron veto only; no need to apply anti-muon veto)
#
selectedPatTausForElecTauAntiOverlapWithElectronsVeto.dRmin = cms.double(0.3)
selectedPatTausForElecTauEta21.cut = selectedPatTausEta21.cut
selectedPatTausForElecTauPt20.cut = selectedPatTausPt20.cut
selectedPatTausForElecTauLeadTrk.cut = selectedPatTausLeadTrk.cut
selectedPatTausForElecTauLeadTrkPt.cut = selectedPatTausLeadTrkPt.cut
selectedPatTausForElecTauTaNCdiscr.cut = cms.string('tauID("byTaNCfrHalfPercent") > 0') 
selectedPatTausForElecTauTrkIso.cut = selectedPatTausTrkIso.cut
selectedPatTausForElecTauEcalIso.cut = selectedPatTausEcalIso.cut
selectedPatTausForElecTauProng.cut = selectedPatTausProng.cut
selectedPatTausForElecTauCharge.cut = selectedPatTausCharge.cut
selectedPatTausForElecTauElectronVeto.cut = selectedPatTausElectronVeto.cut
selectedPatTausForElecTauEcalCrackVeto.cut =  selectedPatTausEcalCrackVeto.cut
selectedPatTausForElecTauMuonVeto.cut = selectedPatTausMuonVeto.cut

patTauSelConfiguratorForElecTau = objSelConfigurator(
    [ selectedPatTausForElecTauAntiOverlapWithElectronsVeto,
      selectedPatTausForElecTauEta21,
      selectedPatTausForElecTauPt20,
      selectedPatTausForElecTauLeadTrk,
      selectedPatTausForElecTauLeadTrkPt,
      selectedPatTausForElecTauTaNCdiscr,
      selectedPatTausForElecTauTrkIso,
      selectedPatTausForElecTauEcalIso,
      selectedPatTausForElecTauProng,
      selectedPatTausForElecTauCharge,
      selectedPatTausForElecTauElectronVeto,
      selectedPatTausForElecTauEcalCrackVeto,
      selectedPatTausForElecTauMuonVeto ],
    src = "cleanPatTaus",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatTausForElecTau = patTauSelConfiguratorForElecTau.configure(pyNameSpace = locals())
#
# define collections of pat::(PF)Taus used in semi-leptonic mu + tau-jet channel
# (require muon and tau-jet candidates to be separated in eta-phi,
#  in order to avoid double-counting the same particle both as muon and as tau-jet;
#  apply anti-muon veto only; no need to apply anti-electron veto)
#
selectedPatTausForMuTauAntiOverlapWithMuonsVeto.dRmin = cms.double(0.3)
selectedPatTausForMuTauEta21.cut = selectedPatTausEta21.cut
selectedPatTausForMuTauPt20.cut = selectedPatTausPt20.cut
selectedPatTausForMuTauLeadTrk.cut = selectedPatTausLeadTrk.cut
selectedPatTausForMuTauLeadTrkPt.cut = selectedPatTausLeadTrkPt.cut
selectedPatTausForMuTauTaNCdiscr.cut = cms.string('tauID("byTaNCfrQuarterPercent") > 0.5')
selectedPatTausForMuTauTrkIso.cut = selectedPatTausTrkIso.cut
selectedPatTausForMuTauEcalIso.cut = selectedPatTausEcalIso.cut
selectedPatTausForMuTauProng.cut = selectedPatTausProng.cut
selectedPatTausForMuTauCharge.cut = selectedPatTausCharge.cut
selectedPatTausForMuTauMuonVeto.cut = selectedPatTausMuonVeto.cut
selectedPatTausForMuTauElectronVeto.cut = selectedPatTausElectronVeto.cut

patTauSelConfiguratorForMuTau = objSelConfigurator(
    [ selectedPatTausForMuTauAntiOverlapWithMuonsVeto,
      selectedPatTausForMuTauEta21,
      selectedPatTausForMuTauPt20,
      selectedPatTausForMuTauLeadTrk,
      selectedPatTausForMuTauLeadTrkPt,
      selectedPatTausForMuTauTaNCdiscr,
      selectedPatTausForMuTauTrkIso,
      selectedPatTausForMuTauEcalIso,
      selectedPatTausForMuTauProng,
      selectedPatTausForMuTauCharge,
      selectedPatTausForMuTauMuonVeto,
      selectedPatTausForMuTauElectronVeto ],
    src = "cleanPatTaus",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatTausForMuTau = patTauSelConfiguratorForMuTau.configure(pyNameSpace = locals())
#
# define collections of pat::(PF)Taus used in pure hadronic tau-jet + tau-jet channel
# (no need to apply anti-electron or anti-muon vetos)
#
selectedPatTausForDiTauEta21.cut = selectedPatTausEta21.cut
selectedPatTausForDiTauPt20.cut = selectedPatTausPt20.cut
selectedPatTausForDiTauLeadTrk.cut = selectedPatTausLeadTrk.cut
selectedPatTausForDiTau1stLeadTrkPt.cut = cms.string('leadPFChargedHadrCand().isNonnull() & leadPFChargedHadrCand().pt() > 12.')
selectedPatTausForDiTau1stTaNCdiscr.cut = cms.string('tauID("byTaNCfrQuarterPercent") > 0.5')
selectedPatTausForDiTau1stTrkIso.cut = selectedPatTausTrkIso.cut
selectedPatTausForDiTau1stEcalIso.cut = selectedPatTausEcalIso.cut
selectedPatTausForDiTau1stProng.cut = selectedPatTausProng.cut
selectedPatTausForDiTau1stCharge.cut = selectedPatTausCharge.cut
selectedPatTausForDiTau1stMuonVeto.cut = selectedPatTausMuonVeto.cut
selectedPatTausForDiTau1stElectronVeto.cut = selectedPatTausElectronVeto.cut
selectedPatTausForDiTau2ndLeadTrkPt.cut = cms.string('leadPFChargedHadrCand().isNonnull() & leadPFChargedHadrCand().pt() > 8.')
selectedPatTausForDiTau2ndTaNCdiscr.cut = cms.string('tauID("byTaNCfrQuarterPercent") > 0.5')
selectedPatTausForDiTau2ndTrkIso.cut = selectedPatTausTrkIso.cut
selectedPatTausForDiTau2ndEcalIso.cut = selectedPatTausEcalIso.cut
selectedPatTausForDiTau2ndProng.cut = selectedPatTausProng.cut
selectedPatTausForDiTau2ndCharge.cut = selectedPatTausCharge.cut
selectedPatTausForDiTau2ndMuonVeto.cut = selectedPatTausMuonVeto.cut
selectedPatTausForDiTau2ndElectronVeto.cut = selectedPatTausElectronVeto.cut

patTauSelConfiguratorForDiTau1st = objSelConfigurator(
    [ selectedPatTausForDiTauEta21,
      selectedPatTausForDiTauPt20,
      selectedPatTausForDiTauLeadTrk,
      selectedPatTausForDiTau1stLeadTrkPt,
      selectedPatTausForDiTau1stTaNCdiscr,
      selectedPatTausForDiTau1stTrkIso,
      selectedPatTausForDiTau1stEcalIso,
      selectedPatTausForDiTau1stProng,
      selectedPatTausForDiTau1stCharge,
      selectedPatTausForDiTau1stMuonVeto,
      selectedPatTausForDiTau1stElectronVeto ],
    src = "cleanPatTaus",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatTausForDiTau1st = patTauSelConfiguratorForDiTau1st.configure(pyNameSpace = locals())

patTauSelConfiguratorForDiTau2nd = objSelConfigurator(
    [ selectedPatTausForDiTau2ndLeadTrkPt,
      selectedPatTausForDiTau2ndTaNCdiscr,
      selectedPatTausForDiTau2ndTrkIso,
      selectedPatTausForDiTau2ndEcalIso,
      selectedPatTausForDiTau2ndProng,
      selectedPatTausForDiTau2ndCharge,
      selectedPatTausForDiTau2ndMuonVeto,
      selectedPatTausForDiTau2ndElectronVeto ],
    src = "selectedPatTausForDiTauLeadTrkCumulative",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatTausForDiTau2nd = patTauSelConfiguratorForDiTau2nd.configure(pyNameSpace = locals())

selectedPatTausForDiTau2ndLeadTrkPtLoose.cut = cms.string('leadPFChargedHadrCand().isNonnull() & leadPFChargedHadrCand().pt() > 5.')
selectedPatTausForDiTau2ndTaNCdiscrLoose.cut = cms.string('tauID("byTaNCfrQuarterPercent") > -1.')
selectedPatTausForDiTau2ndTrkIsoLoose.cut = cms.string('userIsolation("PfChargedHadronIso") < 8.')
selectedPatTausForDiTau2ndEcalIsoLoose.cut = cms.string('userIsolation("PfGammaIso") < 8.')
selectedPatTausForDiTau2ndProngLoose.cut = cms.string('signalPFChargedHadrCands.size() > -1')
selectedPatTausForDiTau2ndChargeLoose.cut = cms.string('abs(charge) > -1.')
selectedPatTausForDiTau2ndMuonVetoLoose.cut = selectedPatTausForDiTau2ndMuonVeto.cut
selectedPatTausForDiTau2ndElectronVetoLoose.cut = selectedPatTausForDiTau2ndElectronVeto.cut

patTauSelConfiguratorForDiTau2ndLoose = objSelConfigurator(
    [ selectedPatTausForDiTau2ndLeadTrkPtLoose,
      selectedPatTausForDiTau2ndTaNCdiscrLoose,
      selectedPatTausForDiTau2ndTrkIsoLoose,
      selectedPatTausForDiTau2ndEcalIsoLoose,
      selectedPatTausForDiTau2ndProngLoose,
      selectedPatTausForDiTau2ndChargeLoose,
      selectedPatTausForDiTau2ndMuonVetoLoose,
      selectedPatTausForDiTau2ndElectronVetoLoose ],
    src = "selectedPatTausForDiTauLeadTrkCumulative",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatTausForDiTau2ndLoose = patTauSelConfiguratorForDiTau2ndLoose.configure(pyNameSpace = locals())

selectPatTausForDiTau = cms.Sequence(selectPatTausForDiTau1st * selectPatTausForDiTau2nd * selectPatTausForDiTau2ndLoose)

# define collections of pat::(PF)Taus used in W->tau-jet + nu channel
selectedPatTausForWTauNuEta21.cut = selectedPatTausEta21.cut
selectedPatTausForWTauNuPt20.cut = cms.string("pt > 25. & pt < 60.")
selectedPatTausForWTauNuLeadTrk.cut = selectedPatTausLeadTrk.cut
selectedPatTausForWTauNuLeadTrkPt.cut = cms.string('leadTrack().isNonnull() & leadTrack().pt() > 20.')
selectedPatTausForWTauNuTaNCdiscr.cut = cms.string('tauID("byTaNCfrQuarterPercent") > -1.e+3') 
selectedPatTausForWTauNuEcalIso.cut = selectedPatTausEcalIso.cut
selectedPatTausForWTauNuTrkIso.cut = selectedPatTausTrkIso.cut
selectedPatTausForWTauNuProng.cut = selectedPatTausProng.cut
selectedPatTausForWTauNuCharge.cut = selectedPatTausCharge.cut
selectedPatTausForWTauNuMuonVeto.cut = selectedPatTausMuonVeto.cut
selectedPatTausForWTauNuElectronVeto.cut = selectedPatTausElectronVeto.cut
selectedPatTausForWTauNuEcalCrackVeto.cut = selectedPatTausEcalCrackVeto.cut

patTauSelConfiguratorForWTauNu = objSelConfigurator(
    [ selectedPatTausForWTauNuEta21,
      selectedPatTausForWTauNuPt20,
      selectedPatTausForWTauNuLeadTrk,
      selectedPatTausForWTauNuLeadTrkPt,
      selectedPatTausForWTauNuTaNCdiscr,
      selectedPatTausForWTauNuEcalIso,
      selectedPatTausForWTauNuTrkIso,
      selectedPatTausForWTauNuProng,
      selectedPatTausForWTauNuCharge, 
      selectedPatTausForWTauNuMuonVeto,
      selectedPatTausForWTauNuElectronVeto,
      selectedPatTausForWTauNuEcalCrackVeto ],
    src = "cleanPatTaus",
    pyModuleName = __name__,
    doSelIndividual = True
)
selectPatTausForWTauNu = patTauSelConfiguratorForWTauNu.configure(pyNameSpace = locals())

# loose isolation selection
selectedPatTausForWTauNuLeadTrkPtLooseIsolation.cut = cms.string("leadTrack().isNonnull() & leadTrack().pt() > 15.")
selectedPatTausForWTauNuTaNCdiscrLooseIsolation.cut = cms.string('tauID("byTaNCfrQuarterPercent") > -1.e+3') # cut on TaNC output disabled per default
selectedPatTausForWTauNuEcalIsoLooseIsolation.cut = cms.string("isolationPFChargedHadrCandsPtSum()<6")
selectedPatTausForWTauNuTrkIsoLooseIsolation.cut = cms.string("isolationPFChargedHadrCandsPtSum()<6")
selectedPatTausForWTauNuProngLooseIsolation.cut = selectedPatTausForWTauNuTrkIsoLooseIsolation.cut
selectedPatTausForWTauNuChargeLooseIsolation.cut = selectedPatTausForWTauNuTrkIsoLooseIsolation.cut
selectedPatTausForWTauNuMuonVetoLooseIsolation.cut = selectedPatTausForWTauNuTrkIsoLooseIsolation.cut
selectedPatTausForWTauNuElectronVetoLooseIsolation.cut = selectedPatTausForWTauNuTrkIsoLooseIsolation.cut
selectedPatTausForWTauNuEcalCrackVetoLooseIsolation.cut = selectedPatTausForWTauNuTrkIsoLooseIsolation.cut


patTauSelConfiguratorForWTauNuLooseIsolation = objSelConfigurator(
    [ selectedPatTausForWTauNuEta21,
      selectedPatTausForWTauNuPt20,
      selectedPatTausForWTauNuLeadTrk,
      selectedPatTausForWTauNuLeadTrkPtLooseIsolation,
      selectedPatTausForWTauNuTaNCdiscrLooseIsolation,
      selectedPatTausForWTauNuEcalIsoLooseIsolation,
      selectedPatTausForWTauNuTrkIsoLooseIsolation,
      selectedPatTausForWTauNuProngLooseIsolation,
      selectedPatTausForWTauNuChargeLooseIsolation, 
      selectedPatTausForWTauNuMuonVetoLooseIsolation,
      selectedPatTausForWTauNuElectronVetoLooseIsolation,
      selectedPatTausForWTauNuEcalCrackVetoLooseIsolation ],
    src = "cleanPatTaus",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatTausForWTauNuLooseIsolation = patTauSelConfiguratorForWTauNuLooseIsolation.configure(pyNameSpace = locals())

producePatSelLeptons = cms.Sequence (
    selectPatElectrons + selectPatElectronsLooseIsolation
   + selectPatMuons + selectPatMuonsLooseIsolation
   + selectPatTaus
   + selectPatElectronsForElecTau + selectPatElectronsForElecTauLooseIsolation
   + selectPatElectronsForElecMu + selectPatElectronsForElecMuLooseIsolation
   + selectPatTausForElecTau + selectPatTausForMuTau + selectPatTausForDiTau
   + selectPatTausForWTauNu +selectPatTausForWTauNuLooseIsolation
)
