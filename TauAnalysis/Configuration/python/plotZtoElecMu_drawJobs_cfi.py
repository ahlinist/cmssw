import FWCore.ParameterSet.Config as cms
import copy

# define template for all kins of plots
# (specific to Z --> e + mu e + mu analysis)
plots_ZtoElecMu_template = cms.PSet(
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

# define transverse momentum distributions plots
plots_ZtoElecMu_electronPt_afterElectronEtaCut = copy.deepcopy(plots_ZtoElecMu_template)
plots_ZtoElecMu_electronPt_afterElectronEtaCut.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecMuAnalyzer/afterElectronEtaCut_beforeElectronPtCut/ElectronQuantities/ElectronPt',
)
plots_ZtoElecMu_electronPt_afterElectronEtaCut.title = cms.string('Electron P_{T} (after Electron #eta Cut)')
plots_ZtoElecMu_electronPt_afterElectronEtaCut.xAxis = cms.string('pt')

# define eta distributions plots
plots_ZtoElecMu_muonEta_afterGlobalMuonCut = copy.deepcopy(plots_ZtoElecMu_template)
plots_ZtoElecMu_muonEta_afterGlobalMuonCut.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecMuAnalyzer/afterGlobalMuonCut_beforeMuonEtaCut/MuonQuantities/MuonEta'
)
plots_ZtoElecMu_muonEta_afterGlobalMuonCut.title = cms.string('Muon #eta (after global Muon Cut)')
plots_ZtoElecMu_muonEta_afterGlobalMuonCut.xAxis = cms.string('eta')

# define transverse impact parameter distributions plots
plots_ZtoElecMu_muonTrackIPxy_afterMuonAntiPionCut = copy.deepcopy(plots_ZtoElecMu_template)
plots_ZtoElecMu_muonTrackIPxy_afterMuonAntiPionCut.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecMuAnalyzer/afterMuonAntiPionCut_beforeMuonTrkIPcut/MuonQuantities/MuonTrackIPxy'
)
plots_ZtoElecMu_muonTrackIPxy_afterMuonAntiPionCut.title = cms.string('Muon Track IP_{xy} (after Muon anti-#pi Cut)')
plots_ZtoElecMu_muonTrackIPxy_afterMuonAntiPionCut.xAxis = cms.string('ipXY')

# define acoplanarity angle distributions plots
plots_ZtoElecMu_dPhi1MET_afterMuonTrkIPcut = copy.deepcopy(plots_ZtoElecMu_template)
plots_ZtoElecMu_dPhi1MET_afterMuonTrkIPcut.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecMuAnalyzer/afterMuonTrkIPcut_beforeDiTauCandidateForElecMuAcoplanarityCut/DiTauCandidateQuantities/DPhi1MET',
)
plots_ZtoElecMu_dPhi1MET_afterMuonTrkIPcut.title = cms.string('#Delta Phi(Muon,MET) (after Muon Track IP_{xy} Cut)')
plots_ZtoElecMu_dPhi1MET_afterMuonTrkIPcut.xAxis = cms.string('dPhi')

plots_ZtoElecMu_dPhi2MET_afterMuonTrkIPcut = copy.deepcopy(plots_ZtoElecMu_template)
plots_ZtoElecMu_dPhi2MET_afterMuonTrkIPcut.plots.dqmMonitorElements = cms.vstring(
  '#PROCESSDIR#/zElecMuAnalyzer/afterMuonTrkIPcut_beforeDiTauCandidateForElecMuAcoplanarityCut/DiTauCandidateQuantities/DPhi2MET',
)
plots_ZtoElecMu_dPhi2MET_afterMuonTrkIPcut.title = cms.string('#Delta Phi(#tau-Jet,MET) (after Muon Track IP_{xy} Cut)')
plots_ZtoElecMu_dPhi2MET_afterMuonTrkIPcut.xAxis = cms.string('dPhi')
