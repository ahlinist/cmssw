import FWCore.ParameterSet.Config as cms
import copy

# define template for all kins of plots
# (specific to Z --> mu + tau-jet analysis)
plots_ZtoElecTau = cms.PSet(
    plots = cms.PSet(  
        dqmMonitorElements = cms.vstring(''),
        processes = cms.vstring( 'Ztautau',
#                                 'Zmumu',
#                                 'WplusJets',
                                 'gammaPlusJets',
#                                 'qcdSum'
                                 )
    ),
    xAxis = cms.string('unlabeled'),
    yAxis = cms.string('numEntries_linear'),
    #yAxis = cms.string('numEntries_log'),
    legend = cms.string('regular'),
    labels = cms.vstring('mcNormScale'),                   
    drawOptionSet = cms.string('default'),
    stack = cms.vstring( 'Ztautau',
#                         'Zmumu',
#                         'WplusJets',
                          'gammaPlusJets',                         
#                         'qcdSum'
                         )
)

#--------------------------------------------------------------------------------
# define cut-flow control plots;
# show distribution of each quantity used in event selection
# (**before** quantity is cutted on)
#--------------------------------------------------------------------------------


plots_ZtoElecTau_vertexChi2Prob_afterPrimaryEventVertex = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_vertexChi2Prob_afterPrimaryEventVertex.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelPrimaryEventVertex_beforeEvtSelPrimaryEventVertexQuality/VertexQuantities/VertexChi2Prob'
)
plots_ZtoElecTau_vertexChi2Prob_afterPrimaryEventVertex.title = cms.string('P(#Chi^{2}_{vtx} (after primary Event Vertex Cut)')
plots_ZtoElecTau_vertexChi2Prob_afterPrimaryEventVertex.xAxis = cms.string('prob')

plots_ZtoElecTau_vertexZ_afterPrimaryEventVertexQuality = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_vertexZ_afterPrimaryEventVertexQuality.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelPrimaryEventVertexQuality_beforeEvtSelPrimaryEventVertexPosition/VertexQuantities/VertexZ'
)
plots_ZtoElecTau_vertexZ_afterPrimaryEventVertexQuality.title = cms.string('z_{vtx} (after primary Event Vertex quality Cut)')
plots_ZtoElecTau_vertexZ_afterPrimaryEventVertexQuality.xAxis = cms.string('posZ')

plots_ZtoElecTau_electron_afterPrimaryEventVertexPosition = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_electron_afterPrimaryEventVertexPosition.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelPrimaryEventVertexPosition_beforeEvtSelTightElectronId/ElectronQuantities/Electron#PAR#'
)
plots_ZtoElecTau_electron_afterPrimaryEventVertexPosition.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoElecTau_electron_afterPrimaryEventVertexPosition.title = cms.string('Electron (after primary Event Vertex position Cut)')
plots_ZtoElecTau_electron_afterPrimaryEventVertexPosition.xAxis = cms.string('#PAR#')

plots_ZtoElecTau_electron_afterTightElectronId = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_electron_afterTightElectronId.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelTightElectronId_beforeEvtSelElectronAntiCrack/ElectronQuantities/Electron#PAR#'
)
plots_ZtoElecTau_electron_afterTightElectronId.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoElecTau_electron_afterTightElectronId.title = cms.string('Electron (after Electron id. Cut)')
plots_ZtoElecTau_electron_afterTightElectronId.xAxis = cms.string('#PAR#')

plots_ZtoElecTau_electron_afterElectronAntiCrack = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_electron_afterElectronAntiCrack.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelElectronAntiCrack_beforeEvtSelElectronEta/ElectronQuantities/Electron#PAR#'
)
plots_ZtoElecTau_electron_afterElectronAntiCrack.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoElecTau_electron_afterElectronAntiCrack.title = cms.string('Electron (after Electron anti-Crack Cut)')
plots_ZtoElecTau_electron_afterElectronAntiCrack.xAxis = cms.string('#PAR#')

plots_ZtoElecTau_electron_afterElectronEta = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_electron_afterElectronEta.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelElectronEta_beforeEvtSelElectronPt/ElectronQuantities/Electron#PAR#'
)
plots_ZtoElecTau_electron_afterElectronEta.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoElecTau_electron_afterElectronEta.title = cms.string('Electron (after Electron #eta Cut)')
plots_ZtoElecTau_electron_afterElectronEta.xAxis = cms.string('#PAR#')

plots_ZtoElecTau_electronTrkIso_afterElectronPt = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_electronTrkIso_afterElectronPt.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelElectronPt_beforeEvtSelElectronTrkIso/ElectronQuantities/ElectronTrkIsoPt'
)
plots_ZtoElecTau_electronTrkIso_afterElectronPt.title = cms.string('Electron Track iso. (after Electron P_{T} Cut)')
plots_ZtoElecTau_electronTrkIso_afterElectronPt.xAxis = cms.string('Pt')

plots_ZtoElecTau_electronEcalIso_afterElectronTrkIso = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_electronEcalIso_afterElectronTrkIso.plots.dqmMonitorElements = cms.vstring(
       '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelElectronTrkIso_beforeEvtSelElectronEcalIso/ElectronQuantities/ElectronEcalIsoPt'
)
plots_ZtoElecTau_electronEcalIso_afterElectronTrkIso.title = cms.string('Electron ECAL iso. (after Electron Track iso. Cut)')
plots_ZtoElecTau_electronEcalIso_afterElectronTrkIso.xAxis = cms.string('Pt')

plots_ZtoElecTau_electron_afterElectronEcalIso = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_electron_afterElectronEcalIso.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelElectronEcalIso_beforeEvtSelElectronTrk/ElectronQuantities/Electron#PAR#'
)
plots_ZtoElecTau_electron_afterElectronEcalIso.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoElecTau_electron_afterElectronEcalIso.title = cms.string('Electron (after Electron P_{T} Cut)')
plots_ZtoElecTau_electron_afterElectronEcalIso.xAxis = cms.string('#PAR#')

plots_ZtoElecTau_electronTrkIP_afterElectronTrk = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_electronTrkIP_afterElectronTrk.plots.dqmMonitorElements = cms.vstring(
       '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelElectronTrk_beforeEvtSelElectronTrkIP/ElectronQuantities/ElectronTrackIP#PAR#'
)
plots_ZtoElecTau_electronTrkIP_afterElectronTrk.parameter = cms.vstring('xy', 'z')
plots_ZtoElecTau_electronTrkIP_afterElectronTrk.title = cms.string('Electron Track IP_{#PAR#}(after Electron Track Cut)')
plots_ZtoElecTau_electronTrkIP_afterElectronTrk.xAxis = cms.string('IP#PAR#')


plots_ZtoElecTau_tau_afterElectronTrkIP = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_tau_afterElectronTrkIP.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelElectronTrkIP_beforeEvtSelTauAntiOverlapWithElectronsVeto/TauQuantities/Tau#PAR#'
)
plots_ZtoElecTau_tau_afterElectronTrkIP.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoElecTau_tau_afterElectronTrkIP.title = cms.string('Tau (after Electron Track IP_{xy} )')
plots_ZtoElecTau_tau_afterElectronTrkIP.xAxis = cms.string('#PAR#')
plots_ZtoElecTau_tauLeadTrkPt_afterElectronTrkIP = copy.deepcopy(plots_ZtoElecTau)

plots_ZtoElecTau_tauLeadTrkPt_afterElectronTrkIP.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelElectronTrkIP_beforeEvtSelTauAntiOverlapWithElectronsVeto/TauQuantities/TauLeadTrkPt'
)
plots_ZtoElecTau_tauLeadTrkPt_afterElectronTrkIP.title = cms.string('Tau lead. Track (after Electron Track IP_{xy} Cut)')
plots_ZtoElecTau_tauLeadTrkPt_afterElectronTrkIP.xAxis = cms.string('Pt')


plots_ZtoElecTau_tau_afterTauAntiOverlapWithElectronsVeto = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_tau_afterTauAntiOverlapWithElectronsVeto.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelTauAntiOverlapWithElectronsVeto_beforeEvtSelTauEta/TauQuantities/Tau#PAR#'
)
plots_ZtoElecTau_tau_afterTauAntiOverlapWithElectronsVeto.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoElecTau_tau_afterTauAntiOverlapWithElectronsVeto.title = cms.string('Tau (after Electron-Tau overlap Veto)')
plots_ZtoElecTau_tau_afterTauAntiOverlapWithElectronsVeto.xAxis = cms.string('#PAR#')
plots_ZtoElecTau_tauLeadTrkPt_afterTauAntiOverlapWithElectronsVeto = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_tauLeadTrkPt_afterTauAntiOverlapWithElectronsVeto.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelTauAntiOverlapWithElectronsVeto_beforeEvtSelTauEta/TauQuantities/TauLeadTrkPt'
)
plots_ZtoElecTau_tauLeadTrkPt_afterTauAntiOverlapWithElectronsVeto.title = cms.string('Tau lead. Track (after Electron-Tau overlap Veto)')
plots_ZtoElecTau_tauLeadTrkPt_afterTauAntiOverlapWithElectronsVeto.xAxis = cms.string('Pt')

plots_ZtoElecTau_tau_afterTauEta = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_tau_afterTauEta.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelTauEta_beforeEvtSelTauPt/TauQuantities/Tau#PAR#'
)
plots_ZtoElecTau_tau_afterTauEta.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoElecTau_tau_afterTauEta.title = cms.string('Tau (after Tau P_{T} Cut)')
plots_ZtoElecTau_tau_afterTauEta.xAxis = cms.string('#PAR#')
plots_ZtoElecTau_tauLeadTrkPt_afterTauEta = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_tauLeadTrkPt_afterTauEta.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelTauEta_beforeEvtSelTauPt/TauQuantities/TauLeadTrkPt'
)
plots_ZtoElecTau_tauLeadTrkPt_afterTauEta.title = cms.string('Tau lead. Track (after Tau P_{T} Cut)')
plots_ZtoElecTau_tauLeadTrkPt_afterTauEta.xAxis = cms.string('Pt')

plots_ZtoElecTau_tau_afterTauPt = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_tau_afterTauPt.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelTauPt_beforeEvtSelTauLeadTrk/TauQuantities/Tau#PAR#'
)
plots_ZtoElecTau_tau_afterTauPt.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoElecTau_tau_afterTauPt.title = cms.string('Tau (after Tau #eta Cut)')
plots_ZtoElecTau_tau_afterTauPt.xAxis = cms.string('#PAR#')
plots_ZtoElecTau_tauLeadTrkPt_afterTauPt = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_tauLeadTrkPt_afterTauPt.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelTauPt_beforeEvtSelTauLeadTrk/TauQuantities/TauLeadTrkPt'
)
plots_ZtoElecTau_tauLeadTrkPt_afterTauPt.title = cms.string('Tau lead. Track (after Tau #eta Cut)')
plots_ZtoElecTau_tauLeadTrkPt_afterTauPt.xAxis = cms.string('Pt')

plots_ZtoElecTau_tau_afterTauLeadTrk = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_tau_afterTauLeadTrk.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelTauLeadTrk_beforeEvtSelTauLeadTrkPt/TauQuantities/Tau#PAR#'
)
plots_ZtoElecTau_tau_afterTauLeadTrk.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoElecTau_tau_afterTauLeadTrk.title = cms.string('Tau (after Tau lead. Track Cut)')
plots_ZtoElecTau_tau_afterTauLeadTrk.xAxis = cms.string('#PAR#')
plots_ZtoElecTau_tauLeadTrkPt_afterTauLeadTrk = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_tauLeadTrkPt_afterTauLeadTrk.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelTauLeadTrk_beforeEvtSelTauLeadTrkPt/TauQuantities/TauLeadTrkPt'
)
plots_ZtoElecTau_tauLeadTrkPt_afterTauLeadTrk.title = cms.string('Tau lead. Track (after Tau lead. Track Cut)')
plots_ZtoElecTau_tauLeadTrkPt_afterTauLeadTrk.xAxis = cms.string('Pt')

plots_ZtoElecTau_tau_afterTauLeadTrkPt = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_tau_afterTauLeadTrkPt.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelTauLeadTrkPt_beforeEvtSelTauTrkIso/TauQuantities/TauTrkIsoPt'
)
plots_ZtoElecTau_tau_afterTauLeadTrkPt.title = cms.string('Tau Track iso. (after Tau lead. Track P_{T} Cut)')
plots_ZtoElecTau_tau_afterTauLeadTrkPt.xAxis = cms.string('Pt')

plots_ZtoElecTau_tau_afterTauTrkIso = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_tau_afterTauTrkIso.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelTauTrkIso_beforeEvtSelTauEcalIso/TauQuantities/TauEcalIsoPt'
)
plots_ZtoElecTau_tau_afterTauTrkIso.title = cms.string('Tau ECAL iso. (after Tau Track iso. Cut)')
plots_ZtoElecTau_tau_afterTauTrkIso.xAxis = cms.string('Pt')

plots_ZtoElecTau_tauNumTracksSignalCone_afterTauEcalIso = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_tauNumTracksSignalCone_afterTauEcalIso.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelTauEcalIso_beforeEvtSelTauProng/TauQuantities/TauNumTracksSignalCone'
)
plots_ZtoElecTau_tauNumTracksSignalCone_afterTauEcalIso.title = cms.string('Tau Tracks in Signal Cone (after Tau ECAL iso. Cut)')
plots_ZtoElecTau_tauNumTracksSignalCone_afterTauEcalIso.xAxis = cms.string('unlabeled')

plots_ZtoElecTau_tau_afterTauProng = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_tau_afterTauProng.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelTauProng_beforeEvtSelTauElectronVeto/TauQuantities/Tau#PAR#'
)
plots_ZtoElecTau_tau_afterTauProng.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoElecTau_tau_afterTauProng.title = cms.string('Tau (after Tau 1-Prong||3-Prong )')
plots_ZtoElecTau_tau_afterTauProng.xAxis = cms.string('#PAR#')
plots_ZtoElecTau_tauAntiElectronDiscr_afterTauProng = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_tauAntiElectronDiscr_afterTauProng.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelTauProng_beforeEvtSelTauElectronVeto/TauQuantities/TauDiscriminatorAgainstElectrons'
)
plots_ZtoElecTau_tauAntiElectronDiscr_afterTauProng.title = cms.string('Tau anti-Electron Discr. (after Tau 1-Prong||3-Prong Cut)')
plots_ZtoElecTau_tauAntiElectronDiscr_afterTauProng.xAxis = cms.string('unlabeled')

plots_ZtoElecTau_dR12_afterTauElectronVeto = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_dR12_afterTauElectronVeto.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelTauElectronVeto_beforeEvtSelDiTauCandidateForElecTauAntiOverlapVeto/DiTauCandidateQuantities/DR12'
)
plots_ZtoElecTau_dR12_afterTauElectronVeto.title = cms.string('#Delta R(Electron,Tau) (after Tau #mu-Veto Cut')
plots_ZtoElecTau_dR12_afterTauElectronVeto.xAxis = cms.string('dPhi')

plots_ZtoElecTau_diTauCharge_afterAntiOverlapVeto = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_diTauCharge_afterAntiOverlapVeto.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelDiTauCandidateForElecTauAntiOverlapVeto_beforeEvtSelDiTauCandidateForElecTauZeroCharge/DiTauCandidateQuantities/DiTauCandidateCharge'
)
plots_ZtoElecTau_diTauCharge_afterAntiOverlapVeto.title = cms.string('Charge(Electron + Tau) (after Acoplanarity Cut)')
plots_ZtoElecTau_diTauCharge_afterAntiOverlapVeto.xAxis = cms.string('unlabeled')

plots_ZtoElecTau_mtElectronMET_afterZeroCharge = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_mtElectronMET_afterZeroCharge.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelDiTauCandidateForElecTauZeroCharge_beforeEvtSelDiTauCandidateForElecTauMt1MET/DiTauCandidateQuantities/Mt1MET'
)
plots_ZtoElecTau_mtElectronMET_afterZeroCharge.title = cms.string('M_{T}(Electron + MET) (after opposite Charge Cut)')
plots_ZtoElecTau_mtElectronMET_afterZeroCharge.xAxis = cms.string('Mt')






#--------------------------------------------------------------------------------
# define distributions to be plotted
# for events passing all event selection criteria
#--------------------------------------------------------------------------------
plots_ZtoElecTau_electron_finalEventSample = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_electron_finalEventSample.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelDiTauCandidateForElecTauMt1MET/ElectronQuantities/Electron#PAR#'
)
plots_ZtoElecTau_electron_finalEventSample.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoElecTau_electron_finalEventSample.title = cms.string('Electron (final Event sample)')
plots_ZtoElecTau_electron_finalEventSample.xAxis = cms.string('#PAR#')


plots_ZtoElecTau_tau_finalEventSample = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_tau_finalEventSample.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelDiTauCandidateForElecTauMt1MET/TauQuantities/Tau#PAR#'
)
plots_ZtoElecTau_tau_finalEventSample.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoElecTau_tau_finalEventSample.title = cms.string('Tau (final Event sample)')
plots_ZtoElecTau_tau_finalEventSample.xAxis = cms.string('#PAR#')
plots_ZtoElecTau_tauLeadTrkPt_finalEventSample = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_tauLeadTrkPt_finalEventSample.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelDiTauCandidateForElecTauMt1MET/TauQuantities/TauLeadTrkPt'
)
plots_ZtoElecTau_tauLeadTrkPt_finalEventSample.title = cms.string('Tau lead. Track (final Event sample)')
plots_ZtoElecTau_tauLeadTrkPt_finalEventSample.xAxis = cms.string('Pt')
plots_ZtoElecTau_tauNumTracksSignalCone_finalEventSample = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_tauNumTracksSignalCone_finalEventSample.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelDiTauCandidateForElecTauMt1MET/TauQuantities/TauNumTracksSignalCone'
)
plots_ZtoElecTau_tauNumTracksSignalCone_finalEventSample.title = cms.string('Tau Tracks in Signal Cone (final Event sample)')
plots_ZtoElecTau_tauNumTracksSignalCone_finalEventSample.xAxis = cms.string('unlabeled')

plots_ZtoElecTau_met_finalEventSample = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_met_finalEventSample.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelDiTauCandidateForElecTauMt1MET/MEtQuantities/MEtPt'
)
plots_ZtoElecTau_met_finalEventSample.title = cms.string('MET (final Event sample)')
plots_ZtoElecTau_met_finalEventSample.xAxis = cms.string('Pt')

plots_ZtoElecTau_mtElectronMET_finalEventSample = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_mtElectronMET_finalEventSample.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelDiTauCandidateForElecTauMt1MET/DiTauCandidateQuantities/Mt1MET'
)
plots_ZtoElecTau_mtElectronMET_finalEventSample.title = cms.string('M_{T}(Electron + MET) (final Event sample)')
plots_ZtoElecTau_mtElectronMET_finalEventSample.xAxis = cms.string('Mt')
plots_ZtoElecTau_mtTauMET_finalEventSample = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_mtTauMET_finalEventSample.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelDiTauCandidateForElecTauMt1MET/DiTauCandidateQuantities/Mt2MET'
)
plots_ZtoElecTau_mtTauMET_finalEventSample.title = cms.string('M_{T}(Tau + MET) (final Event sample)')
plots_ZtoElecTau_mtTauMET_finalEventSample.xAxis = cms.string('Mt')

plots_ZtoElecTau_mtElectronTauMET_finalEventSample = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_mtElectronTauMET_finalEventSample.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelDiTauCandidateForElecTauMt1MET/DiTauCandidateQuantities/Mt12MET'
)
plots_ZtoElecTau_mtElectronTauMET_finalEventSample.title = cms.string('M_{T}(Electron + Tau + MET) (final Event sample)')
plots_ZtoElecTau_mtElectronTauMET_finalEventSample.xAxis = cms.string('Mt')
#plots_ZtoElecTau_mtElectronTauMET_finalEventSample.

plots_ZtoElecTau_mCDFmethod_finalEventSample = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_mCDFmethod_finalEventSample.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelDiTauCandidateForElecTauMt1MET/DiTauCandidateQuantities/CDFmethodMass'
)
plots_ZtoElecTau_mCDFmethod_finalEventSample.title = cms.string('M(Electron + Tau), CDF method (final Event sample)')
plots_ZtoElecTau_mCDFmethod_finalEventSample.xAxis = cms.string('M')

plots_ZtoElecTau_mCollApprox_finalEventSample = copy.deepcopy(plots_ZtoElecTau)
plots_ZtoElecTau_mCollApprox_finalEventSample.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelDiTauCandidateForElecTauMt1MET/DiTauCandidateQuantities/CollinearApproxMass'
)
plots_ZtoElecTau_mCollApprox_finalEventSample.title = cms.string('M(Electron + Tau), collinear Approx. (final Event sample)')
plots_ZtoElecTau_mCollApprox_finalEventSample.xAxis = cms.string('M')

#plots_ZtoElecTau_numCentralJets_finalEventSample = copy.deepcopy(plots_ZtoElecTau)
#plots_ZtoElecTau_numCentralJets_finalEventSample.plots.dqmMonitorElements = cms.vstring(
#  '#PROCESSDIR#/zElecTauAnalyzer/afterEvtSelDiTauCandidateForElecTauMt1MET/JetQuantities/numJetsEtGt#PAR#_0EtaLt2_1AlphaGt0_3'
#)
#plots_ZtoElecTau_numCentralJets_finalEventSample.parameter = cms.vstring('15', '20', '30')
#plots_ZtoElecTau_numCentralJets_finalEventSample.title = cms.string('N_{jets} with E_{T} > #PAR# GeV, |#eta| < 2.1, #alpha > 0.3 (final Event sample)')
#plots_ZtoElecTau_numCentralJets_finalEventSample.xAxis = cms.string('N')
