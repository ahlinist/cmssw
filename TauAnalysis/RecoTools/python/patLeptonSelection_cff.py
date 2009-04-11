import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.RecoTools.patElectronSelection_cfi import *
from TauAnalysis.RecoTools.patMuonSelection_cfi import *
from TauAnalysis.RecoTools.patPFTauSelection_cfi import *
from TauAnalysis.RecoTools.patPFTauSelectionForElecTau_cfi import *
from TauAnalysis.RecoTools.patPFTauSelectionForMuTau_cfi import *
from TauAnalysis.RecoTools.patPFTauSelectionForDiTau_cfi import *

from TauAnalysis.RecoTools.objSelConfigurator import *

#--------------------------------------------------------------------------------
# define selection criteria for pat::Electrons
# (settings made here overwrite values defined in electronPatSelector_cfi)
#--------------------------------------------------------------------------------

selectedLayer1ElectronsTightId.cut = cms.string('(abs(superCluster.eta) < 1.479 & electronID("robust") > 0 & eSuperClusterOverP < 1.05 & eSuperClusterOverP > 0.95) | (abs(superCluster.eta) > 1.479 & electronID("robust") > 0 & eSuperClusterOverP < 1.12 & eSuperClusterOverP > 0.95)')
selectedLayer1ElectronsAntiCrackCut.cut = cms.string('abs(superCluster.eta) < 1.442 | abs(superCluster.eta) > 1.560')
selectedLayer1ElectronsEta21.cut = cms.string('abs(eta) < 2.1')
selectedLayer1ElectronsPt15.cut = cms.string('pt > 15.')
selectedLayer1ElectronsTrkIso.cut = cms.string('trackIso < 1.')
selectedLayer1ElectronsEcalIso.cut = cms.string('(abs(superCluster.eta) < 1.479 & ecalIso < 1.0) | (abs(superCluster.eta) > 1.479 & ecalIso < 2.5)')
selectedLayer1ElectronsTrk.cut = cms.string('gsfTrack.isNonnull')
selectedLayer1ElectronsTrkIP.vertexSource = cms.InputTag("selectedPrimaryVertexPosition")
selectedLayer1ElectronsTrkIP.IpMax = cms.double(0.05)

patElectronSelConfigurator = objSelConfigurator(
    [ selectedLayer1ElectronsTightId,
      selectedLayer1ElectronsAntiCrackCut,
      selectedLayer1ElectronsEta21,
      selectedLayer1ElectronsPt15,
      selectedLayer1ElectronsTrkIso,
      selectedLayer1ElectronsEcalIso,
      selectedLayer1ElectronsTrk,
      selectedLayer1ElectronsTrkIP ],
    src = "cleanLayer1Electrons",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectLayer1Electrons = patElectronSelConfigurator.configure()

#--------------------------------------------------------------------------------
# define selection criteria for pat::Muons
# (settings made here overwrite values defined in muonPatSelector_cfi)
#--------------------------------------------------------------------------------

selectedLayer1MuonsGlobal.cut = cms.string('isGlobalMuon()')
selectedLayer1MuonsEta21Individual.cut = cms.string('abs(eta) < 2.1')
selectedLayer1MuonsPt15Individual.cut = cms.string('pt > 15.')
selectedLayer1MuonsTrkIsoIndividual.vetos = vetos = cms.vstring("0.01")
selectedLayer1MuonsTrkIsoIndividual.dRisoCone = cms.double(0.6)
selectedLayer1MuonsTrkIsoIndividual.sumPtMax = cms.double(1.)
selectedLayer1MuonsEcalIsoIndividual.cut = cms.string('ecalIso < 1.')
selectedLayer1MuonsPionVetoIndividual.CaloCompCoefficient = cms.double(0.8)
selectedLayer1MuonsPionVetoIndividual.SegmCompCoefficient = cms.double(1.2)
selectedLayer1MuonsPionVetoIndividual.AntiPionCut = cms.double(1.0)
selectedLayer1MuonsTrkIndividual.cut = cms.string('innerTrack.isNonnull')
selectedLayer1MuonsTrkIPindividual.vertexSource = cms.InputTag("selectedPrimaryVertexPosition")
selectedLayer1MuonsTrkIPindividual.IpMax = cms.double(0.05)

selectedLayer1MuonsEta21Cumulative.cut = selectedLayer1MuonsEta21Individual.cut
selectedLayer1MuonsPt15Cumulative.cut = selectedLayer1MuonsPt15Individual.cut
selectedLayer1MuonsTrkIsoCumulative.vetos = selectedLayer1MuonsTrkIsoIndividual.vetos
selectedLayer1MuonsTrkIsoCumulative.dRisoCone = selectedLayer1MuonsTrkIsoIndividual.dRisoCone
selectedLayer1MuonsTrkIsoCumulative.sumPtMax = selectedLayer1MuonsTrkIsoIndividual.sumPtMax
selectedLayer1MuonsEcalIsoCumulative.cut = selectedLayer1MuonsEcalIsoIndividual.cut
selectedLayer1MuonsPionVetoCumulative.CaloCompCoefficient = selectedLayer1MuonsPionVetoIndividual.CaloCompCoefficient
selectedLayer1MuonsPionVetoCumulative.SegmCompCoefficient = selectedLayer1MuonsPionVetoIndividual.SegmCompCoefficient
selectedLayer1MuonsPionVetoCumulative.AntiPionCut = selectedLayer1MuonsPionVetoIndividual.AntiPionCut
selectedLayer1MuonsTrkCumulative.cut = selectedLayer1MuonsTrkIndividual.cut
selectedLayer1MuonsTrkIPcumulative.vertexSource = selectedLayer1MuonsTrkIPindividual.vertexSource
selectedLayer1MuonsTrkIPcumulative.IpMax = selectedLayer1MuonsTrkIPindividual.IpMax

#--------------------------------------------------------------------------------
# define selection criteria for pat::(PF)Taus
# (settings made here overwrite values defined in pftauPatSelector_cfi)
#--------------------------------------------------------------------------------

selectedLayer1TausEta21Individual.cut = cms.string("abs(eta) < 2.1")
selectedLayer1TausPt20Individual.cut = cut = cms.string("pt > 20.")
selectedLayer1TausLeadTrkIndividual.cut = cms.string('tauID("leadingTrackFinding") > 0.5')
selectedLayer1TausLeadTrkPtIndividual.cut = cms.string('tauID("leadingTrackPtCut") > 0.5')
selectedLayer1TausTrkIsoIndividual.cut = cms.string('tauID("trackIsolation") > 0.5')
selectedLayer1TausEcalIsoIndividual.cut = cms.string('tauID("ecalIsolation") > 0.5')
selectedLayer1TausProngIndividual.cut = cms.string("signalTracks.size() = 1 | signalTracks.size() = 3")
selectedLayer1TausElectronVetoIndividual.cut = cms.string('tauID("againstElectron") > 0.5')
selectedLayer1TausMuonVetoIndividual.cut = cms.string('tauID("againstMuon") > 0.5')
#
# define collections of pat::(PF)Taus used in semi-leptonic e + tau-jet channel
# (require electron and tau-jet candidates to be separated in eta-phi,
#  in order to avoid double-counting the same particle both as electron and as tau-jet;
#  apply anti-electron veto only; no need to apply anti-muon veto)
#
selectedLayer1TausForElecTauAntiOverlapWithElectronsVeto.dRmin = cms.double(0.3)
selectedLayer1TausForElecTauEta21Cumulative.cut = selectedLayer1TausEta21Individual.cut
selectedLayer1TausForElecTauPt20Cumulative.cut = selectedLayer1TausPt20Individual.cut
selectedLayer1TausForElecTauLeadTrkCumulative.cut = selectedLayer1TausLeadTrkIndividual.cut
selectedLayer1TausForElecTauLeadTrkPtCumulative.cut = selectedLayer1TausLeadTrkPtIndividual.cut
selectedLayer1TausForElecTauTrkIsoCumulative.cut = selectedLayer1TausTrkIsoIndividual.cut
selectedLayer1TausForElecTauEcalIsoCumulative.cut = selectedLayer1TausEcalIsoIndividual.cut
selectedLayer1TausForElecTauProngCumulative.cut = selectedLayer1TausProngIndividual.cut
selectedLayer1TausForElecTauElectronVetoCumulative.cut = selectedLayer1TausElectronVetoIndividual.cut
#
# define collections of pat::(PF)Taus used in semi-leptonic mu + tau-jet channel
# (require muon and tau-jet candidates to be separated in eta-phi,
#  in order to avoid double-counting the same particle both as muon and as tau-jet;
#  apply anti-muon veto only; no need to apply anti-electron veto)
#
selectedLayer1TausForMuTauAntiOverlapWithMuonsVeto.dRmin = cms.double(0.3)
selectedLayer1TausForMuTauEta21Cumulative.cut = selectedLayer1TausEta21Individual.cut
selectedLayer1TausForMuTauPt20Cumulative.cut = selectedLayer1TausPt20Individual.cut
selectedLayer1TausForMuTauLeadTrkCumulative.cut = selectedLayer1TausLeadTrkIndividual.cut
selectedLayer1TausForMuTauLeadTrkPtCumulative.cut = selectedLayer1TausLeadTrkPtIndividual.cut
selectedLayer1TausForMuTauTrkIsoCumulative.cut = selectedLayer1TausTrkIsoIndividual.cut
selectedLayer1TausForMuTauEcalIsoCumulative.cut = selectedLayer1TausEcalIsoIndividual.cut
selectedLayer1TausForMuTauProngCumulative.cut = selectedLayer1TausProngIndividual.cut
selectedLayer1TausForMuTauMuonVetoCumulative.cut = selectedLayer1TausMuonVetoIndividual.cut
#
# define collections of pat::(PF)Taus used in pure hadronic tau-jet + tau-jet channel
# (no need to apply anti-electron or anti-muon vetos)
#
selectedLayer1TausForDiTauEta21Cumulative.cut = selectedLayer1TausEta21Individual.cut
selectedLayer1TausForDiTauPt20Cumulative.cut = selectedLayer1TausPt20Individual.cut
selectedLayer1TausForDiTauLeadTrkCumulative.cut = selectedLayer1TausLeadTrkIndividual.cut
selectedLayer1TausForDiTauLeadTrkPtCumulative.cut = selectedLayer1TausLeadTrkPtIndividual.cut
selectedLayer1TausForDiTauTrkIsoCumulative.cut = selectedLayer1TausEcalIsoIndividual.cut
selectedLayer1TausForDiTauEcalIsoCumulative.cut = selectedLayer1TausEcalIsoIndividual.cut
selectedLayer1TausForDiTauProngCumulative.cut = selectedLayer1TausMuonVetoIndividual.cut

produceLayer1SelLeptons = cms.Sequence ( selectLayer1Electrons + produceLayer1SelElectrons
                                        +produceLayer1SelMuons + selectLayer1MuonsLooseIsolation
                                        +produceLayer1SelTaus
                                        +selectLayer1TausForElecTau + selectLayer1TausForMuTau + selectLayer1TausForDiTau )
