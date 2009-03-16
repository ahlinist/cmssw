import FWCore.ParameterSet.Config as cms
import copy

# define template for all kins of plots
# (specific to Z --> mu + tau-jet analysis)
plots_ZtoMuTau = cms.PSet(
  plots = cms.PSet(  
    dqmMonitorElements = cms.vstring(''),
    processes = cms.vstring( 'Ztautau',
                             'Zmumu',
                             'WplusJets',
                             'qcdSum' )
  ),
  xAxis = cms.string('unlabeled'),
  yAxis = cms.string('numEntries_linear'),
  #yAxis = cms.string('numEntries_log'),
  legend = cms.string('regular'),
  labels = cms.vstring('mcNormScale'),                   
  drawOptionSet = cms.string('default'),
  stack = cms.vstring( 'Ztautau',
                       'Zmumu',
                       'WplusJets',
                       'qcdSum' )
)

#--------------------------------------------------------------------------------
# define cut-flow control plots;
# show distribution of each quantity used in event selection
# (**before** quantity is cutted on)
#--------------------------------------------------------------------------------

plots_ZtoMuTau_vertexChi2Prob_afterPrimaryEventVertex = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_vertexChi2Prob_afterPrimaryEventVertex.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterPrimaryEventVertex_beforePrimaryEventVertexQuality/VertexQuantities/VertexChi2Prob'
)
plots_ZtoMuTau_vertexChi2Prob_afterPrimaryEventVertex.title = cms.string('P(#Chi^{2}_{vtx} (after primary Event Vertex Cut)')
plots_ZtoMuTau_vertexChi2Prob_afterPrimaryEventVertex.xAxis = cms.string('prob')

plots_ZtoMuTau_vertexZ_afterPrimaryEventVertexQuality = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_vertexZ_afterPrimaryEventVertexQuality.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterPrimaryEventVertexQuality_beforePrimaryEventVertexPosition/VertexQuantities/VertexZ'
)
plots_ZtoMuTau_vertexZ_afterPrimaryEventVertexQuality.title = cms.string('z_{vtx} (after primary Event Vertex quality Cut)')
plots_ZtoMuTau_vertexZ_afterPrimaryEventVertexQuality.xAxis = cms.string('posZ')

plots_ZtoMuTau_muon_afterPrimaryEventVertexPosition = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_muon_afterPrimaryEventVertexPosition.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterPrimaryEventVertexPosition_beforeGlobalMuonCut/MuonQuantities/Muon#PAR#'
)
plots_ZtoMuTau_muon_afterPrimaryEventVertexPosition.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoMuTau_muon_afterPrimaryEventVertexPosition.title = cms.string('Muon (after primary Event Vertex position Cut)')
plots_ZtoMuTau_muon_afterPrimaryEventVertexPosition.xAxis = cms.string('#PAR#')

plots_ZtoMuTau_muon_afterGlobalMuon = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_muon_afterGlobalMuon.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterGlobalMuonCut_beforeMuonEtaCut/MuonQuantities/Muon#PAR#'
)
plots_ZtoMuTau_muon_afterGlobalMuon.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoMuTau_muon_afterGlobalMuon.title = cms.string('Muon (after global Muon Cut)')
plots_ZtoMuTau_muon_afterGlobalMuon.xAxis = cms.string('#PAR#')

plots_ZtoMuTau_muon_afterMuonEta = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_muon_afterMuonEta.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterMuonEtaCut_beforeMuonPtCut/MuonQuantities/Muon#PAR#'
)
plots_ZtoMuTau_muon_afterMuonEta.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoMuTau_muon_afterMuonEta.title = cms.string('Muon (after Muon #eta Cut)')
plots_ZtoMuTau_muon_afterMuonEta.xAxis = cms.string('#PAR#')

plots_ZtoMuTau_muon_afterMuonPt = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_muon_afterMuonPt.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterMuonPtCut_beforeMuonHLTmatchCut/MuonQuantities/Muon#PAR#'
)
plots_ZtoMuTau_muon_afterMuonPt.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoMuTau_muon_afterMuonPt.title = cms.string('Muon (after Muon P_{T} Cut)')
plots_ZtoMuTau_muon_afterMuonPt.xAxis = cms.string('#PAR#')

plots_ZtoMuTau_muonTrkIso_afterMuonHLTmatch = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_muonTrkIso_afterMuonHLTmatch.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterMuonHLTmatchCut_beforeMuonTrkIsoCut/MuonQuantities/MuonTrkIsoPt'
)
plots_ZtoMuTau_muonTrkIso_afterMuonHLTmatch.title = cms.string('Muon Track iso. (after Muon HLT-match Cut)')
plots_ZtoMuTau_muonTrkIso_afterMuonHLTmatch.xAxis = cms.string('Pt')

plots_ZtoMuTau_muonEcalIso_afterMuonTrkIso = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_muonEcalIso_afterMuonTrkIso.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterMuonTrkIsoCut_beforeMuonEcalIsoCut/MuonQuantities/MuonEcalIsoPt'
)
plots_ZtoMuTau_muonEcalIso_afterMuonTrkIso.title = cms.string('Muon ECAL iso. (after Muon Track iso. Cut)')
plots_ZtoMuTau_muonEcalIso_afterMuonTrkIso.xAxis = cms.string('Pt')

plots_ZtoMuTau_muonComp_afterMuonEcalIso = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_muonComp_afterMuonEcalIso.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterMuonEcalIsoCut_beforeMuonAntiPionCut/MuonQuantities/Muon#PAR#Compatibility'
)
plots_ZtoMuTau_muonComp_afterMuonEcalIso.parameter = cms.vstring('Calo', 'Segment')
plots_ZtoMuTau_muonComp_afterMuonEcalIso.title = cms.string('Muon #PAR# compatibility (after Muon ECAL iso. Cut)')
plots_ZtoMuTau_muonComp_afterMuonEcalIso.xAxis = cms.string('prob')

plots_ZtoMuTau_muonTrkIP_afterMuonAntiPionVeto = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_muonTrkIP_afterMuonAntiPionVeto.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterMuonAntiPionCut_beforeMuonTrkIPcut/MuonQuantities/MuonTrackIP#PAR#'
)
plots_ZtoMuTau_muonTrkIP_afterMuonAntiPionVeto.parameter = cms.vstring('xy', 'z')
plots_ZtoMuTau_muonTrkIP_afterMuonAntiPionVeto.title = cms.string('Muon Track IP_{#PAR#}(after Muon #pi-Veto Cut)')
plots_ZtoMuTau_muonTrkIP_afterMuonAntiPionVeto.xAxis = cms.string('IP#PAR#')

plots_ZtoMuTau_tau_afterMuonTrkIP = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_tau_afterMuonTrkIP.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterMuonTrkIPcut_beforeTauAntiOverlapWithMuonsVeto/TauQuantities/Tau#PAR#'
)
plots_ZtoMuTau_tau_afterMuonTrkIP.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoMuTau_tau_afterMuonTrkIP.title = cms.string('Tau (after Muon Track IP_{xy} Cut)')
plots_ZtoMuTau_tau_afterMuonTrkIP.xAxis = cms.string('#PAR#')
plots_ZtoMuTau_tauLeadTrkPt_afterMuonTrkIP = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_tauLeadTrkPt_afterMuonTrkIP.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterMuonTrkIPcut_beforeTauAntiOverlapWithMuonsVeto/TauQuantities/TauLeadTrkPt'
)
plots_ZtoMuTau_tauLeadTrkPt_afterMuonTrkIP.title = cms.string('Tau lead. Track (after Muon Track IP_{xy} Cut)')
plots_ZtoMuTau_tauLeadTrkPt_afterMuonTrkIP.xAxis = cms.string('Pt')

plots_ZtoMuTau_tau_afterTauAntiOverlapWithMuonsVeto = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_tau_afterTauAntiOverlapWithMuonsVeto.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterTauAntiOverlapWithMuonsVeto_beforeTauEtaCut/TauQuantities/Tau#PAR#'
)
plots_ZtoMuTau_tau_afterTauAntiOverlapWithMuonsVeto.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoMuTau_tau_afterTauAntiOverlapWithMuonsVeto.title = cms.string('Tau (after Muon-Tau overlap Veto)')
plots_ZtoMuTau_tau_afterTauAntiOverlapWithMuonsVeto.xAxis = cms.string('#PAR#')
plots_ZtoMuTau_tauLeadTrkPt_afterTauAntiOverlapWithMuonsVeto = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_tauLeadTrkPt_afterTauAntiOverlapWithMuonsVeto.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterTauAntiOverlapWithMuonsVeto_beforeTauEtaCut/TauQuantities/TauLeadTrkPt'
)
plots_ZtoMuTau_tauLeadTrkPt_afterTauAntiOverlapWithMuonsVeto.title = cms.string('Tau lead. Track (after Muon-Tau overlap Veto)')
plots_ZtoMuTau_tauLeadTrkPt_afterTauAntiOverlapWithMuonsVeto.xAxis = cms.string('Pt')

plots_ZtoMuTau_tau_afterTauEta = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_tau_afterTauEta.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterTauEtaCut_beforeTauPtCut/TauQuantities/Tau#PAR#'
)
plots_ZtoMuTau_tau_afterTauEta.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoMuTau_tau_afterTauEta.title = cms.string('Tau (after Tau P_{T} Cut)')
plots_ZtoMuTau_tau_afterTauEta.xAxis = cms.string('#PAR#')
plots_ZtoMuTau_tauLeadTrkPt_afterTauEta = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_tauLeadTrkPt_afterTauEta.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterTauEtaCut_beforeTauPtCut/TauQuantities/TauLeadTrkPt'
)
plots_ZtoMuTau_tauLeadTrkPt_afterTauEta.title = cms.string('Tau lead. Track (after Tau P_{T} Cut)')
plots_ZtoMuTau_tauLeadTrkPt_afterTauEta.xAxis = cms.string('Pt')

plots_ZtoMuTau_tau_afterTauPt = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_tau_afterTauPt.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterTauPtCut_beforeTauLeadTrkCut/TauQuantities/Tau#PAR#'
)
plots_ZtoMuTau_tau_afterTauPt.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoMuTau_tau_afterTauPt.title = cms.string('Tau (after Tau #eta Cut)')
plots_ZtoMuTau_tau_afterTauPt.xAxis = cms.string('#PAR#')
plots_ZtoMuTau_tauLeadTrkPt_afterTauPt = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_tauLeadTrkPt_afterTauPt.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterTauPtCut_beforeTauLeadTrkCut/TauQuantities/TauLeadTrkPt'
)
plots_ZtoMuTau_tauLeadTrkPt_afterTauPt.title = cms.string('Tau lead. Track (after Tau #eta Cut)')
plots_ZtoMuTau_tauLeadTrkPt_afterTauPt.xAxis = cms.string('Pt')

plots_ZtoMuTau_tau_afterTauLeadTrk = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_tau_afterTauLeadTrk.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterTauLeadTrkCut_beforeTauLeadTrkPtCut/TauQuantities/Tau#PAR#'
)
plots_ZtoMuTau_tau_afterTauLeadTrk.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoMuTau_tau_afterTauLeadTrk.title = cms.string('Tau (after Tau lead. Track Cut)')
plots_ZtoMuTau_tau_afterTauLeadTrk.xAxis = cms.string('#PAR#')
plots_ZtoMuTau_tauLeadTrkPt_afterTauLeadTrk = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_tauLeadTrkPt_afterTauLeadTrk.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterTauLeadTrkCut_beforeTauLeadTrkPtCut/TauQuantities/TauLeadTrkPt'
)
plots_ZtoMuTau_tauLeadTrkPt_afterTauLeadTrk.title = cms.string('Tau lead. Track (after Tau lead. Track Cut)')
plots_ZtoMuTau_tauLeadTrkPt_afterTauLeadTrk.xAxis = cms.string('Pt')

plots_ZtoMuTau_tau_afterTauLeadTrkPt = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_tau_afterTauLeadTrkPt.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterTauLeadTrkPtCut_beforeTauTrkIsoCut/TauQuantities/TauTrkIsoPt'
)
plots_ZtoMuTau_tau_afterTauLeadTrkPt.title = cms.string('Tau Track iso. (after Tau lead. Track P_{T} Cut)')
plots_ZtoMuTau_tau_afterTauLeadTrkPt.xAxis = cms.string('Pt')

plots_ZtoMuTau_tau_afterTauTrkIso = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_tau_afterTauTrkIso.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterTauTrkIsoCut_beforeTauEcalIsoCut/TauQuantities/TauEcalIsoPt'
)
plots_ZtoMuTau_tau_afterTauTrkIso.title = cms.string('Tau ECAL iso. (after Tau Track iso. Cut)')
plots_ZtoMuTau_tau_afterTauTrkIso.xAxis = cms.string('Pt')

plots_ZtoMuTau_tauNumTracksSignalCone_afterTauEcalIso = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_tauNumTracksSignalCone_afterTauEcalIso.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterTauEcalIsoCut_beforeTauProngCut/TauQuantities/TauNumTracksSignalCone'
)
plots_ZtoMuTau_tauNumTracksSignalCone_afterTauEcalIso.title = cms.string('Tau Tracks in Signal Cone (after Tau ECAL iso. Cut)')
plots_ZtoMuTau_tauNumTracksSignalCone_afterTauEcalIso.xAxis = cms.string('unlabeled')

plots_ZtoMuTau_tau_afterTauProng = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_tau_afterTauProng.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterTauProngCut_beforeTauMuonVeto/TauQuantities/Tau#PAR#'
)
plots_ZtoMuTau_tau_afterTauProng.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoMuTau_tau_afterTauProng.title = cms.string('Tau (after Tau 1-Prong||3-Prong Cut)')
plots_ZtoMuTau_tau_afterTauProng.xAxis = cms.string('#PAR#')
plots_ZtoMuTau_tauAntiMuonDiscr_afterTauProng = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_tauAntiMuonDiscr_afterTauProng.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterTauProngCut_beforeTauMuonVeto/TauQuantities/TauDiscriminatorAgainstMuons'
)
plots_ZtoMuTau_tauAntiMuonDiscr_afterTauProng.title = cms.string('Tau anti-Muon Discr. (after Tau 1-Prong||3-Prong Cut)')
plots_ZtoMuTau_tauAntiMuonDiscr_afterTauProng.xAxis = cms.string('unlabeled')

plots_ZtoMuTau_dR12_afterTauMuonVeto = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_dR12_afterTauMuonVeto.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterTauMuonVeto_beforeDiTauCandidateForMuTauAntiOverlapVeto/DiTauCandidateQuantities/DR12'
)
plots_ZtoMuTau_dR12_afterTauMuonVeto.title = cms.string('#Delta R(Muon,Tau) (after Tau #mu-Veto Cut')
plots_ZtoMuTau_dR12_afterTauMuonVeto.xAxis = cms.string('dPhi')

plots_ZtoMuTau_dPhi1MET_afterAntiOverlapVeto = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_dPhi1MET_afterAntiOverlapVeto.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterDiTauCandidateForMuTauAntiOverlapVeto_beforeDiTauCandidateForMuTauAcoplanarityCut/DiTauCandidateQuantities/DPhi1MET'
)
plots_ZtoMuTau_dPhi1MET_afterAntiOverlapVeto.title = cms.string('#Delta #phi(Muon,MET) (after anti-Overlap Cut)')
plots_ZtoMuTau_dPhi1MET_afterAntiOverlapVeto.xAxis = cms.string('dPhi')
plots_ZtoMuTau_dPhi2MET_afterAntiOverlapVeto = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_dPhi2MET_afterAntiOverlapVeto.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterDiTauCandidateForMuTauAntiOverlapVeto_beforeDiTauCandidateForMuTauAcoplanarityCut/DiTauCandidateQuantities/DPhi2MET'
)
plots_ZtoMuTau_dPhi2MET_afterAntiOverlapVeto.title = cms.string('#Delta #phi(Tau,MET) (after anti-Overlap Cut)')
plots_ZtoMuTau_dPhi2MET_afterAntiOverlapVeto.xAxis = cms.string('dPhi')

plots_ZtoMuTau_diTauCharge_afterAcoplanarity = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_diTauCharge_afterAcoplanarity.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterDiTauCandidateForMuTauAcoplanarityCut_beforeDiTauCandidateForMuTauZeroChargeCut/DiTauCandidateQuantities/DiTauCandidateCharge'
)
plots_ZtoMuTau_diTauCharge_afterAcoplanarity.title = cms.string('Charge(Muon + Tau) (after Acoplanarity Cut)')
plots_ZtoMuTau_diTauCharge_afterAcoplanarity.xAxis = cms.string('unlabeled')

plots_ZtoMuTau_mtMuonMET_afterZeroCharge = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_mtMuonMET_afterZeroCharge.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterDiTauCandidateForMuTauZeroChargeCut_beforeDiTauCandidateForMuTauMt1METCut/DiTauCandidateQuantities/Mt1MET'
)
plots_ZtoMuTau_mtMuonMET_afterZeroCharge.title = cms.string('M_{T}(Muon + MET) (after opposite Charge Cut)')
plots_ZtoMuTau_mtMuonMET_afterZeroCharge.xAxis = cms.string('Mt')

#--------------------------------------------------------------------------------
# define distributions to be plotted
# for events passing all event selection criteria
#--------------------------------------------------------------------------------

plots_ZtoMuTau_muon_finalEventSample = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_muon_finalEventSample.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterDiTauCandidateForMuTauMt1METCut/MuonQuantities/Muon#PAR#'
)
plots_ZtoMuTau_muon_finalEventSample.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoMuTau_muon_finalEventSample.title = cms.string('Muon (final Event sample)')
plots_ZtoMuTau_muon_finalEventSample.xAxis = cms.string('#PAR#')

plots_ZtoMuTau_tau_finalEventSample = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_tau_finalEventSample.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterDiTauCandidateForMuTauMt1METCut/TauQuantities/Tau#PAR#'
)
plots_ZtoMuTau_tau_finalEventSample.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoMuTau_tau_finalEventSample.title = cms.string('Tau (final Event sample)')
plots_ZtoMuTau_tau_finalEventSample.xAxis = cms.string('#PAR#')
plots_ZtoMuTau_tauLeadTrkPt_finalEventSample = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_tauLeadTrkPt_finalEventSample.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterDiTauCandidateForMuTauMt1METCut/TauQuantities/TauLeadTrkPt'
)
plots_ZtoMuTau_tauLeadTrkPt_finalEventSample.title = cms.string('Tau lead. Track (final Event sample)')
plots_ZtoMuTau_tauLeadTrkPt_finalEventSample.xAxis = cms.string('Pt')
plots_ZtoMuTau_plots_ZtoMuTau_tauNumTracksSignalCone_finalEventSample = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_plots_ZtoMuTau_tauNumTracksSignalCone_finalEventSample.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterDiTauCandidateForMuTauMt1METCut/TauQuantities/TauNumTracksSignalCone'
)
plots_ZtoMuTau_plots_ZtoMuTau_tauNumTracksSignalCone_finalEventSample.title = cms.string('Tau Tracks in Signal Cone (final Event sample)')
plots_ZtoMuTau_plots_ZtoMuTau_tauNumTracksSignalCone_finalEventSample.xAxis = cms.string('unlabeled')

plots_ZtoMuTau_met_finalEventSample = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_met_finalEventSample.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterDiTauCandidateForMuTauMt1METCut/MEtQuantities/MEtPt'
)
plots_ZtoMuTau_met_finalEventSample.title = cms.string('MET (final Event sample)')
plots_ZtoMuTau_met_finalEventSample.xAxis = cms.string('Pt')

plots_ZtoMuTau_mtMuonMET_finalEventSample = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_mtMuonMET_finalEventSample.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterDiTauCandidateForMuTauMt1METCut/DiTauCandidateQuantities/Mt1MET'
)
plots_ZtoMuTau_mtMuonMET_finalEventSample.title = cms.string('M_{T}(Muon + MET) (final Event sample)')
plots_ZtoMuTau_mtMuonMET_finalEventSample.xAxis = cms.string('Mt')
plots_ZtoMuTau_mtTauMET_finalEventSample = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_mtTauMET_finalEventSample.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterDiTauCandidateForMuTauMt1METCut/DiTauCandidateQuantities/Mt2MET'
)
plots_ZtoMuTau_mtTauMET_finalEventSample.title = cms.string('M_{T}(Tau + MET) (final Event sample)')
plots_ZtoMuTau_mtTauMET_finalEventSample.xAxis = cms.string('Mt')

plots_ZtoMuTau_mtMuonTauMET_finalEventSample = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_mtMuonTauMET_finalEventSample.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterDiTauCandidateForMuTauMt1METCut/DiTauCandidateQuantities/Mt12MET'
)
plots_ZtoMuTau_mtMuonTauMET_finalEventSample.title = cms.string('M_{T}(Muon + Tau + MET) (final Event sample)')
plots_ZtoMuTau_mtMuonTauMET_finalEventSample.xAxis = cms.string('Mt')
#plots_ZtoMuTau_mtMuonTauMET_finalEventSample.

plots_ZtoMuTau_mCDFmethod_finalEventSample = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_mCDFmethod_finalEventSample.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterDiTauCandidateForMuTauMt1METCut/DiTauCandidateQuantities/CDFmethodMass'
)
plots_ZtoMuTau_mCDFmethod_finalEventSample.title = cms.string('M(Muon + Tau), CDF method (final Event sample)')
plots_ZtoMuTau_mCDFmethod_finalEventSample.xAxis = cms.string('M')

plots_ZtoMuTau_mCollApprox_finalEventSample = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_mCollApprox_finalEventSample.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterDiTauCandidateForMuTauMt1METCut/DiTauCandidateQuantities/CollinearApproxMass'
)
plots_ZtoMuTau_mCollApprox_finalEventSample.title = cms.string('M(Muon + Tau), collinear Approx. (final Event sample)')
plots_ZtoMuTau_mCollApprox_finalEventSample.xAxis = cms.string('M')

plots_ZtoMuTau_numCentralJets_finalEventSample = copy.deepcopy(plots_ZtoMuTau)
plots_ZtoMuTau_numCentralJets_finalEventSample.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zMuTauAnalyzer/afterDiTauCandidateForMuTauMt1METCut/JetQuantities/numJetsEtGt#PAR#_0EtaLt2_1AlphaGt0_3'
)
plots_ZtoMuTau_numCentralJets_finalEventSample.parameter = cms.vstring('15', '20', '30')
plots_ZtoMuTau_numCentralJets_finalEventSample.title = cms.string('N_{jets} with E_{T} > #PAR# GeV, |#eta| < 2.1, #alpha > 0.3 (final Event sample)')
plots_ZtoMuTau_numCentralJets_finalEventSample.xAxis = cms.string('N')
