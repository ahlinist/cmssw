import FWCore.ParameterSet.Config as cms
import copy

# define template for all kins of plots
# (specific to Z --> e + mu analysis)
plots_ZtoElecMu = cms.PSet(
    plots = cms.PSet(  
        dqmMonitorElements = cms.vstring(''),
        processes = cms.vstring( 'Ztautau',
                                 'Zee',
                                 'Zmumu',
                                 'WplusJets',
                                 'qcdSum' )
    ),
    xAxis = cms.string('unlabeled'),
    #yAxis = cms.string('numEntries_linear'),
    yAxis = cms.string('numEntries_log'),
    legend = cms.string('regular'),
    labels = cms.vstring('mcNormScale'),                   
    drawOptionSet = cms.string('default'),
    stack = cms.vstring( 'Ztautau',
                         'Zee',
                         'Zmumu',
                         'WplusJets',
                         'qcdSum' )
)

#--------------------------------------------------------------------------------
# define cut-flow control plots;
# show distribution of each quantity used in event selection
# (**before** quantity is cutted on)
#--------------------------------------------------------------------------------

plots_ZtoElecMu_vertexChi2Prob_afterPrimaryEventVertex = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_vertexChi2Prob_afterPrimaryEventVertex.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelPrimaryEventVertex_beforeEvtSelPrimaryEventVertexQuality/VertexQuantities/VertexChi2Prob'
)
plots_ZtoElecMu_vertexChi2Prob_afterPrimaryEventVertex.title = cms.string('P(#Chi^{2}_{vtx} (after primary Event Vertex Cut)')
plots_ZtoElecMu_vertexChi2Prob_afterPrimaryEventVertex.xAxis = cms.string('prob')

plots_ZtoElecMu_vertexZ_afterPrimaryEventVertexQuality = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_vertexZ_afterPrimaryEventVertexQuality.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelPrimaryEventVertexQuality_beforeEvtSelPrimaryEventVertexPosition/VertexQuantities/VertexZ'
)
plots_ZtoElecMu_vertexZ_afterPrimaryEventVertexQuality.title = cms.string('z_{vtx} (after primary Event Vertex quality Cut)')
plots_ZtoElecMu_vertexZ_afterPrimaryEventVertexQuality.xAxis = cms.string('posZ')

plots_ZtoElecMu_electron_afterPrimaryEventVertexPosition = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_electron_afterPrimaryEventVertexPosition.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelPrimaryEventVertexPosition_beforeEvtSelTightElectronId/ElectronQuantities/Electron#PAR#'
)
plots_ZtoElecMu_electron_afterPrimaryEventVertexPosition.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoElecMu_electron_afterPrimaryEventVertexPosition.title = cms.string('Electron (after primary Event Vertex position Cut)')
plots_ZtoElecMu_electron_afterPrimaryEventVertexPosition.xAxis = cms.string('#PAR#')

plots_ZtoElecMu_electron_afterTightElectronId = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_electron_afterTightElectronId.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelTightElectronId_beforeEvtSelElectronAntiCrack/ElectronQuantities/Electron#PAR#'
)
plots_ZtoElecMu_electron_afterTightElectronId.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoElecMu_electron_afterTightElectronId.title = cms.string('Electron (after Electron id. Cut)')
plots_ZtoElecMu_electron_afterTightElectronId.xAxis = cms.string('#PAR#')

plots_ZtoElecMu_electron_afterElectronAntiCrack = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_electron_afterElectronAntiCrack.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelElectronAntiCrack_beforeEvtSelElectronEta/ElectronQuantities/Electron#PAR#'
)
plots_ZtoElecMu_electron_afterElectronAntiCrack.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoElecMu_electron_afterElectronAntiCrack.title = cms.string('Electron (after Electron anti-Crack Cut)')
plots_ZtoElecMu_electron_afterElectronAntiCrack.xAxis = cms.string('#PAR#')

plots_ZtoElecMu_electron_afterElectronEta = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_electron_afterElectronEta.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelElectronEta_beforeEvtSelElectronPt/ElectronQuantities/Electron#PAR#'
)
plots_ZtoElecMu_electron_afterElectronEta.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoElecMu_electron_afterElectronEta.title = cms.string('Electron (after Electron #eta Cut)')
plots_ZtoElecMu_electron_afterElectronEta.xAxis = cms.string('#PAR#')

plots_ZtoElecMu_electronTrkIso_afterElectronPt = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_electronTrkIso_afterElectronPt.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelElectronPt_beforeEvtSelElectronTrkIso/ElectronQuantities/ElectronTrkIsoPt'
)
plots_ZtoElecMu_electronTrkIso_afterElectronPt.title = cms.string('Electron Track iso. (after Electron P_{T} Cut)')
plots_ZtoElecMu_electronTrkIso_afterElectronPt.xAxis = cms.string('Pt')

plots_ZtoElecMu_electronEcalIso_afterElectronTrkIso = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_electronEcalIso_afterElectronTrkIso.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelElectronTrkIso_beforeEvtSelElectronEcalIso/ElectronQuantities/ElectronEcalIsoPt'
)
plots_ZtoElecMu_electronEcalIso_afterElectronTrkIso.title = cms.string('Electron ECAL iso. (after Electron Track iso. Cut)')
plots_ZtoElecMu_electronEcalIso_afterElectronTrkIso.xAxis = cms.string('Pt')

plots_ZtoElecMu_electron_afterElectronEcalIso = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_electron_afterElectronEcalIso.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelElectronEcalIso_beforeEvtSelElectronTrk/ElectronQuantities/Electron#PAR#'
)
plots_ZtoElecMu_electron_afterElectronEcalIso.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoElecMu_electron_afterElectronEcalIso.title = cms.string('Electron (after Electron P_{T} Cut)')
plots_ZtoElecMu_electron_afterElectronEcalIso.xAxis = cms.string('#PAR#')

plots_ZtoElecMu_electronTrkIP_afterElectronTrk = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_electronTrkIP_afterElectronTrk.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelElectronTrk_beforeEvtSelElectronTrkIP/ElectronQuantities/ElectronTrackIP#PAR#'
)
plots_ZtoElecMu_electronTrkIP_afterElectronTrk.parameter = cms.vstring('xy', 'z')
plots_ZtoElecMu_electronTrkIP_afterElectronTrk.title = cms.string('Electron Track IP_{#PAR#}(after Electron Track Cut)')
plots_ZtoElecMu_electronTrkIP_afterElectronTrk.xAxis = cms.string('IP#PAR#')

plots_ZtoElecMu_muon_afterElectronTrackIP = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_muon_afterElectronTrackIP.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelElectronTrkIP_beforeEvtSelGlobalMuon/MuonQuantities/Muon#PAR#'
)
plots_ZtoElecMu_muon_afterElectronTrackIP.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoElecMu_muon_afterElectronTrackIP.title = cms.string('Muon (after Electron Track IP_{xy} Cut)')
plots_ZtoElecMu_muon_afterElectronTrackIP.xAxis = cms.string('#PAR#')

plots_ZtoElecMu_muon_afterGlobalMuon = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_muon_afterGlobalMuon.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelGlobalMuon_beforeEvtSelMuonEta/MuonQuantities/Muon#PAR#'
)
plots_ZtoElecMu_muon_afterGlobalMuon.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoElecMu_muon_afterGlobalMuon.title = cms.string('Muon (after global Muon Cut)')
plots_ZtoElecMu_muon_afterGlobalMuon.xAxis = cms.string('#PAR#')

plots_ZtoElecMu_muon_afterMuonEta = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_muon_afterMuonEta.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelMuonEta_beforeEvtSelMuonPt/MuonQuantities/Muon#PAR#'
)
plots_ZtoElecMu_muon_afterMuonEta.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoElecMu_muon_afterMuonEta.title = cms.string('Muon (after Muon #eta Cut)')
plots_ZtoElecMu_muon_afterMuonEta.xAxis = cms.string('#PAR#')

plots_ZtoElecMu_muonTrkIso_afterMuonPt = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_muonTrkIso_afterMuonPt.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelMuonPt_beforeEvtSelMuonTrkIso/MuonQuantities/MuonTrkIsoPt'
)
plots_ZtoElecMu_muonTrkIso_afterMuonPt.title = cms.string('Muon Track iso. (after Muon P_{T} Cut)')
plots_ZtoElecMu_muonTrkIso_afterMuonPt.xAxis = cms.string('Pt')

plots_ZtoElecMu_muonEcalIso_afterMuonTrkIso = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_muonEcalIso_afterMuonTrkIso.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelMuonTrkIso_beforeEvtSelMuonEcalIso/MuonQuantities/MuonEcalIsoPt'
)
plots_ZtoElecMu_muonEcalIso_afterMuonTrkIso.title = cms.string('Muon ECAL iso. (after Muon Track iso. Cut)')
plots_ZtoElecMu_muonEcalIso_afterMuonTrkIso.xAxis = cms.string('Pt')

plots_ZtoElecMu_muonComp_afterMuonEcalIso = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_muonComp_afterMuonEcalIso.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelMuonEcalIso_beforeEvtSelMuonAntiPion/MuonQuantities/Muon#PAR#Compatibility'
)
plots_ZtoElecMu_muonComp_afterMuonEcalIso.parameter = cms.vstring('Calo', 'Segment')
plots_ZtoElecMu_muonComp_afterMuonEcalIso.title = cms.string('Muon #PAR# compatibility (after Muon ECAL iso. Cut)')
plots_ZtoElecMu_muonComp_afterMuonEcalIso.xAxis = cms.string('prob')

plots_ZtoElecMu_muonTrkIP_afterMuonAntiPionVeto = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_muonTrkIP_afterMuonAntiPionVeto.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelMuonAntiPion_beforeEvtSelMuonTrkIP/MuonQuantities/MuonTrackIP#PAR#'
)
plots_ZtoElecMu_muonTrkIP_afterMuonAntiPionVeto.parameter = cms.vstring('xy', 'z')
plots_ZtoElecMu_muonTrkIP_afterMuonAntiPionVeto.title = cms.string('Muon Track IP_{#PAR#}(after Muon #pi-Veto Cut)')
plots_ZtoElecMu_muonTrkIP_afterMuonAntiPionVeto.xAxis = cms.string('IP#PAR#')


plots_ZtoElecMu_dR12_afterMuonTrkIPcut = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_dR12_afterMuonTrkIPcut.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelMuonTrkIP_beforeEvtSelDiTauCandidateForElecMuAntiOverlapVeto/DiTauCandidateQuantities/DR12')
plots_ZtoElecMu_dR12_afterMuonTrkIPcut.title = cms.string('#Delta R(Muon,Electron) (after  #mu-IP Cut')
plots_ZtoElecMu_dR12_afterMuonTrkIPcut.xAxis = cms.string('dPhi')



plots_ZtoElecMu_diTauCharge_afterAntiOverlapVeto = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_diTauCharge_afterAntiOverlapVeto.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelDiTauCandidateForElecMuAntiOverlapVeto_beforeEvtSelDiTauCandidateForElecMuZeroCharge/DiTauCandidateQuantities/DiTauCandidateCharge'
)
plots_ZtoElecMu_diTauCharge_afterAntiOverlapVeto.title = cms.string('Charge(Muon + Electron) (after Acoplanarity Cut)')
plots_ZtoElecMu_diTauCharge_afterAntiOverlapVeto.xAxis = cms.string('unlabeled')

plots_ZtoElecMu_mtElectronMET_afterZeroCharge = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_mtElectronMET_afterZeroCharge.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelDiTauCandidateForElecMuZeroCharge_beforeEvtSelDiTauCandidateForElecMuMt1MET/DiTauCandidateQuantities/Mt1MET'
)
plots_ZtoElecMu_mtElectronMET_afterZeroCharge.title = cms.string('M_{T}(Electron+ MET) (after opposite Charge Cut)')
plots_ZtoElecMu_mtElectronMET_afterZeroCharge.xAxis = cms.string('Mt')

plots_ZtoElecMu_mtMuonMET_aftermtElectronMET = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_mtMuonMET_aftermtElectronMET.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelDiTauCandidateForElecMuMt1MET_beforeEvtSelDiTauCandidateForElecMuMt2MET/DiTauCandidateQuantities/Mt2MET'
)
plots_ZtoElecMu_mtMuonMET_aftermtElectronMET.title = cms.string('M_{T}(Muon + MET) (after M_{T}(Electron + MET) Cut)')
plots_ZtoElecMu_mtMuonMET_aftermtElectronMET.xAxis = cms.string('Mt')

#--------------------------------------------------------------------------------
# define distributions to be plotted
# for events passing all event selection criteria
#--------------------------------------------------------------------------------

plots_ZtoElecMu_electron_finalEventSample = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_electron_finalEventSample.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelDiTauCandidateForElecMuMt2MET/ElectronQuantities/Electron#PAR#'
)
plots_ZtoElecMu_electron_finalEventSample.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoElecMu_electron_finalEventSample.title = cms.string('Electron (final Event sample)')
plots_ZtoElecMu_electron_finalEventSample.xAxis = cms.string('#PAR#')

plots_ZtoElecMu_muon_finalEventSample = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_muon_finalEventSample.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelDiTauCandidateForElecMuMt2MET/MuonQuantities/Muon#PAR#'
)
plots_ZtoElecMu_muon_finalEventSample.parameter = cms.vstring('Pt', 'Eta', 'Phi')
plots_ZtoElecMu_muon_finalEventSample.title = cms.string('Muon (final Event sample)')
plots_ZtoElecMu_muon_finalEventSample.xAxis = cms.string('#PAR#')

plots_ZtoElecMu_met_finalEventSample = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_met_finalEventSample.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelDiTauCandidateForElecMuMt2MET/MEtQuantities/RAWplusJESplusMUON_MEtPt'
)
plots_ZtoElecMu_met_finalEventSample.title = cms.string('MET (final Event sample)')
plots_ZtoElecMu_met_finalEventSample.xAxis = cms.string('Pt')

plots_ZtoElecMu_mtElectronMET_finalEventSample = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_mtElectronMET_finalEventSample.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelDiTauCandidateForElecMuMt2MET/DiTauCandidateQuantities/Mt1MET'
)
plots_ZtoElecMu_mtElectronMET_finalEventSample.title = cms.string('M_{T}(Electron + MET) (final Event sample)')
plots_ZtoElecMu_mtElectronMET_finalEventSample.xAxis = cms.string('Mt')
plots_ZtoElecMu_mtMuonMET_finalEventSample = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_mtMuonMET_finalEventSample.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelDiTauCandidateForElecMuMt2MET/DiTauCandidateQuantities/Mt2MET'
)
plots_ZtoElecMu_mtMuonMET_finalEventSample.title = cms.string('M_{T}(Muon + MET) (final Event sample)')
plots_ZtoElecMu_mtMuonMET_finalEventSample.xAxis = cms.string('Mt')

plots_ZtoElecMu_mtElectronMuonMET_finalEventSample = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_mtElectronMuonMET_finalEventSample.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelDiTauCandidateForElecMuMt2MET/DiTauCandidateQuantities/Mt12MET'
)
plots_ZtoElecMu_mtElectronMuonMET_finalEventSample.title = cms.string('M_{T}(Electron + Muon + MET) (final Event sample)')
plots_ZtoElecMu_mtElectronMuonMET_finalEventSample.xAxis = cms.string('Mt')

plots_ZtoElecMu_mCDFmethod_finalEventSample = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_mCDFmethod_finalEventSample.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelDiTauCandidateForElecMuMt2MET/DiTauCandidateQuantities/CDFmethodMass'
)
plots_ZtoElecMu_mCDFmethod_finalEventSample.title = cms.string('M(Electron + Muon), CDF method (final Event sample)')
plots_ZtoElecMu_mCDFmethod_finalEventSample.xAxis = cms.string('M')

plots_ZtoElecMu_mCollApprox_finalEventSample = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_mCollApprox_finalEventSample.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelDiTauCandidateForElecMuMt2MET/DiTauCandidateQuantities/CollinearApproxMass'
)
plots_ZtoElecMu_mCollApprox_finalEventSample.title = cms.string('M(Electron + Muon), collinear Approx. (final Event sample)')
plots_ZtoElecMu_mCollApprox_finalEventSample.xAxis = cms.string('M')

plots_ZtoElecMu_numCentralJets_finalEventSample = copy.deepcopy(plots_ZtoElecMu)
plots_ZtoElecMu_numCentralJets_finalEventSample.plots.dqmMonitorElements = cms.vstring(
    '#PROCESSDIR#/zElecMuAnalyzer/afterEvtSelDiTauCandidateForElecMuMt2MET/JetQuantities/numJetsEtGt#PAR#_0EtaLt2_1AlphaGt0_3'
)
plots_ZtoElecMu_numCentralJets_finalEventSample.parameter = cms.vstring('15', '20', '30')
plots_ZtoElecMu_numCentralJets_finalEventSample.title = cms.string('N_{jets} with E_{T} > #PAR# GeV, |#eta| < 2.1, #alpha > 0.3 (final Event sample)')
plots_ZtoElecMu_numCentralJets_finalEventSample.xAxis = cms.string('N')
