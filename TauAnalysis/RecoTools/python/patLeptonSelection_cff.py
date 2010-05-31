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
selectedPatElectronsTrkIP.vertexSource = cms.InputTag("selectedPrimaryVertexPosition")
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

selectedPatElectronsForElecTauTightId.cut = cms.string('(abs(superCluster.eta) < 1.479 & electronID("eidRobustTight") > 0 & eSuperClusterOverP < 1.05 & eSuperClusterOverP > 0.95) | (abs(superCluster.eta) > 1.479 & electronID("eidRobustTight") > 0 & eSuperClusterOverP < 1.12 & eSuperClusterOverP > 0.95)')
selectedPatElectronsForElecTauLooseId.cut = cms.string('(abs(superCluster.eta) < 1.479 & electronID("eidRobustLoose") > 0 & eSuperClusterOverP < 1.4 & eSuperClusterOverP > 0.8) | (abs(superCluster.eta) > 1.479 & electronID("eidRobustLoose") > 0 & eSuperClusterOverP < 1.6 & eSuperClusterOverP > 0.8)')
selectedPatElectronsForElecTauAntiCrackCut.cut = cms.string('abs(superCluster.eta) < 1.442 | abs(superCluster.eta) > 1.560')
selectedPatElectronsForElecTauEta21.cut = cms.string('abs(eta) < 2.1')
selectedPatElectronsForElecTauPt15.cut = cms.string('pt > 15.')
selectedPatElectronsForElecTauTrkIso.cut = cms.string('userIsolation("pat::TrackIso") < 1.')
selectedPatElectronsForElecTauEcalIso.cut = cms.string('(abs(superCluster.eta) < 1.479 & userIsolation("pat::EcalIso") < 2.5) | (abs(superCluster.eta) > 1.479 & userIsolation("pat::EcalIso") < 3.5)')
selectedPatElectronsForElecTauTrk.cut = cms.string('gsfTrack.isNonnull')
selectedPatElectronsForElecTauTrkIP.vertexSource = cms.InputTag("selectedPrimaryVertexPosition")
selectedPatElectronsForElecTauTrkIP.IpMax = cms.double(0.05)
selectedPatElectronsForElecTauConversionVeto.cotThetaCut = cms.double(0.05)
selectedPatElectronsForElecTauConversionVeto.docaElecTrack = cms.double(0)
selectedPatElectronsForElecTauConversionVeto.dRElecTrack = cms.double(0.1)
selectedPatElectronsForElecTauConversionVeto.doPixCut = cms.bool(True)
selectedPatElectronsForElecTauConversionVeto.nTrkMax = cms.double(1)
selectedPatElectronsForElecTauConversionVeto.useConversionColl = cms.bool(True)

patElectronSelConfiguratorForElecTau = objSelConfigurator(
    [ selectedPatElectronsForElecTauLooseId,
      selectedPatElectronsForElecTauAntiCrackCut,
      selectedPatElectronsForElecTauEta21,
      selectedPatElectronsForElecTauPt15,
      selectedPatElectronsForElecTauTrkIso,
      selectedPatElectronsForElecTauEcalIso,
      selectedPatElectronsForElecTauTrk,
      selectedPatElectronsForElecTauTrkIP,
	  selectedPatElectronsForElecTauConversionVeto ],
    src = "cleanPatElectrons",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatElectronsForElecTau = patElectronSelConfiguratorForElecTau.configure(pyNameSpace = locals())

patElectronSelConfiguratorForElecTauLooseIsolation = objSelConfigurator(
    [ selectedPatElectronsForElecTauLooseId,
      selectedPatElectronsForElecTauAntiCrackCut,
      selectedPatElectronsForElecTauEta21,
      selectedPatElectronsForElecTauPt15,
      selectedPatElectronsForElecTauTrkIsoLooseIsolation,
      selectedPatElectronsForElecTauEcalIsoLooseIsolation,
      selectedPatElectronsForElecTauTrkLooseIsolation,
      selectedPatElectronsForElecTauTrkIPlooseIsolation,
	  selectedPatElectronsForElecTauConversionVetoLooseIsolation ],
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
selectedPatMuonsPt15.cut = cms.string('pt > 15.')
selectedPatMuonsTrkIso.vetos = vetos = cms.vstring("0.01")
selectedPatMuonsTrkIso.dRisoCone = cms.double(0.6)
selectedPatMuonsTrkIso.sumPtMax = cms.double(1.)
selectedPatMuonsTrkIso.sumPtMethod = cms.string("absolute")
selectedPatMuonsEcalIso.cut = cms.string('userIsolation("pat::EcalIso") < 1.')
selectedPatMuonsPionVeto.CaloCompCoefficient = cms.double(0.8)
selectedPatMuonsPionVeto.SegmCompCoefficient = cms.double(1.2)
selectedPatMuonsPionVeto.AntiPionCut = cms.double(1.0)
selectedPatMuonsTrk.cut = cms.string('innerTrack.isNonnull')
selectedPatMuonsTrkIP.vertexSource = cms.InputTag("selectedPrimaryVertexPosition")
selectedPatMuonsTrkIP.IpMax = cms.double(0.05)

patMuonSelConfigurator = objSelConfigurator(
    [ selectedPatMuonsGlobal,
      selectedPatMuonsEta21,
      selectedPatMuonsPt15,
      selectedPatMuonsTrkIso,
      selectedPatMuonsEcalIso,
      selectedPatMuonsPionVeto,
      selectedPatMuonsTrk,
      selectedPatMuonsTrkIP ],
    src = "cleanPatMuons",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatMuons = patMuonSelConfigurator.configure(pyNameSpace = locals())

selectedPatMuonsTrkIsoLooseIsolation.vetos = cms.vstring("0.01")
selectedPatMuonsTrkIsoLooseIsolation.numMax = cms.int32(-1)
selectedPatMuonsTrkIsoLooseIsolation.sumPtMax = cms.double(8.)
selectedPatMuonsTrkIsoLooseIsolation.sumPtMethod = cms.string("absolute")
selectedPatMuonsEcalIsoLooseIsolation.cut = cms.string('userIsolation("pat::EcalIso") < 8.')
selectedPatMuonsPionVetoLooseIsolation.CaloCompCoefficient = selectedPatMuonsPionVeto.CaloCompCoefficient
selectedPatMuonsPionVetoLooseIsolation.SegmCompCoefficient = selectedPatMuonsPionVeto.SegmCompCoefficient
selectedPatMuonsPionVetoLooseIsolation.AntiPionCut = selectedPatMuonsPionVeto.AntiPionCut
selectedPatMuonsTrkLooseIsolation.cut = selectedPatMuonsTrk.cut
selectedPatMuonsTrkIPlooseIsolation.vertexSource = selectedPatMuonsTrkIP.vertexSource
selectedPatMuonsTrkIPlooseIsolation.IpMax = selectedPatMuonsTrkIP.IpMax

patMuonSelConfiguratorLooseIsolation = objSelConfigurator(
    [ selectedPatMuonsGlobal,
      selectedPatMuonsEta21,
      selectedPatMuonsPt15,
      selectedPatMuonsTrkIsoLooseIsolation,
      selectedPatMuonsEcalIsoLooseIsolation,
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
selectedPatTausForElecTauTaNCdiscr.cut = cms.string('tauID("byTaNCfrQuarterPercent") > -1.e+3') # cut on TaNC output disabled per default
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
selectedPatTausForMuTauTaNCdiscr.cut = cms.string('tauID("byTaNCfrQuarterPercent") > -1.e+3') # cut on TaNC output disabled per default
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
selectedPatTausForDiTauLeadTrkPt.cut = selectedPatTausLeadTrkPt.cut
selectedPatTausForDiTauTaNCdiscr.cut = cms.string('tauID("byTaNCfrQuarterPercent") > -1.e+3') # cut on TaNC output disabled per default
selectedPatTausForDiTauTrkIso.cut = selectedPatTausTrkIso.cut
selectedPatTausForDiTauEcalIso.cut = selectedPatTausEcalIso.cut
selectedPatTausForDiTauProng.cut = selectedPatTausProng.cut
selectedPatTausForDiTauCharge.cut = selectedPatTausCharge.cut

patTauSelConfiguratorForDiTau = objSelConfigurator(
    [ selectedPatTausForDiTauEta21,
      selectedPatTausForDiTauPt20,
      selectedPatTausForDiTauLeadTrk,
      selectedPatTausForDiTauLeadTrkPt,
      selectedPatTausForDiTauTaNCdiscr,
      selectedPatTausForDiTauTrkIso,
      selectedPatTausForDiTauEcalIso,
      selectedPatTausForDiTauProng,
      selectedPatTausForDiTauCharge ],
    src = "cleanPatTaus",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatTausForDiTau = patTauSelConfiguratorForDiTau.configure(pyNameSpace = locals())

# define collections of pat::(PF)Taus used in W->tau-jet + nu channel

selectedPatTausForWTauNuEta21.cut = selectedPatTausEta21.cut
selectedPatTausForWTauNuPt20.cut = cms.string("pt > 25. & pt < 60.")
selectedPatTausForWTauNuLeadTrk.cut = selectedPatTausLeadTrk.cut
selectedPatTausForWTauNuLeadTrkPt.cut = cms.string("leadTrack().isNonnull() & leadTrack().pt() > 20.")
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
