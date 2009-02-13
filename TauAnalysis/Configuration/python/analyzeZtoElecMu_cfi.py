import FWCore.ParameterSet.Config as cms

# import config for electron histogram manager
from TauAnalysis.DQMTools.electronHistManager_cfi import *
# import config for muon histogram manager
from TauAnalysis.DQMTools.muonHistManager_cfi import *

elecMuHistManagers = cms.vstring('electronHistManager', 'muonHistManager')
# trigger selection
Trigger = cms.PSet(
  name = cms.string('Trigger'),
  type = cms.string('TriggerResultEventSelector'),
  src = cms.InputTag('TriggerResults', '', 'HLT'),
  triggerPaths = cms.vstring('HLT_IsoEle8_IsoMu7')
)

# electron candidate selection
tightElectronIdCut = cms.PSet(
  name = cms.string('tightElectronIdCut'),
  type = cms.string('PATElectronMinEventSelector'),
  src = cms.InputTag('selectedLayer1ElectronsTightId'),
  minNumber = cms.uint32(1)
)
electronAntiCrackCut = cms.PSet(
  name = cms.string('electronAntiCrackCut'),
  type = cms.string('PATElectronMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1ElectronsAntiCrackCutCumulative'),
  src_individual = cms.InputTag('selectedLayer1ElectronsAntiCrackCutIndividual'),
  minNumber = cms.uint32(1)
)
electronEtaCut = cms.PSet(
  name = cms.string('electronEtaCut'),
  type = cms.string('PATElectronMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1ElectronsEta21Cumulative'),
  src_individual = cms.InputTag('selectedLayer1ElectronsEta21Individual'),
  minNumber = cms.uint32(1)
)
electronPtCut = cms.PSet(
  name = cms.string('electronPtCut'),
  type = cms.string('PATElectronMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1ElectronsPt15Cumulative'),
  src_individual = cms.InputTag('selectedLayer1ElectronsPt15Individual'),
  minNumber = cms.uint32(1)
)
electronHLTmatchCut = cms.PSet(
  name = cms.string('electronHLTmatchCut'),
  type = cms.string('PATElectronMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1ElectronsHLTmatchCumulative'),
  src_individual = cms.InputTag('selectedLayer1ElectronsHLTmatchIndividual'),
  minNumber = cms.uint32(1)
)
electronTrkIsoCut = cms.PSet(
  name = cms.string('electronTrkIsoCut'),
  type = cms.string('PATElectronMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1ElectronsTrkIsoCumulative'),
  src_individual = cms.InputTag('selectedLayer1ElectronsTrkIsoIndividual'),
  minNumber = cms.uint32(1)
)
electronEcalIsoCut = cms.PSet(
  name = cms.string('electronEcalIsoCut'),
  type = cms.string('PATElectronMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1ElectronsHcalIsoCumulative'),
  src_individual = cms.InputTag('selectedLayer1ElectronsHcalIsoIndividual'),
  minNumber = cms.uint32(1)
)
electronTrkCut = cms.PSet(
  name = cms.string('electronTrkCut'),
  type = cms.string('PATElectronMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1ElectronsTrkCumulative'),
  src_individual = cms.InputTag('selectedLayer1ElectronsTrkIndividual'),
  minNumber = cms.uint32(1)
)
electronTrkIPcut = cms.PSet(
  name = cms.string('electronTrkIPcut'),
  type = cms.string('PATElectronMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1ElectronsTrkIPcumulative'),
  src_individual = cms.InputTag('selectedLayer1ElectronsTrkIPindividual'),
  minNumber = cms.uint32(1)
)

# trigger selection
Trigger = cms.PSet(
  name = cms.string('Trigger'),
  type = cms.string('TriggerResultEventSelector'),
  src = cms.InputTag('TriggerResults', '', 'HLT'),
  triggerPaths = cms.vstring('HLT_IsoEle8_IsoMu7')
)

# muon candidate selection
globalMuonCut = cms.PSet(
  name = cms.string('globalMuonCut'),
  type = cms.string('PATMuonMinEventSelector'),
  src = cms.InputTag('selectedLayer1MuonsGlobal'),
  minNumber = cms.uint32(1)
)
muonEtaCut = cms.PSet(
  name = cms.string('muonEtaCut'),
  type = cms.string('PATMuonMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1MuonsEta21Cumulative'),
  src_individual = cms.InputTag('selectedLayer1MuonsEta21Individual'),
  minNumber = cms.uint32(1)
)
muonPtCut = cms.PSet(
  name = cms.string('muonPtCut'),
  type = cms.string('PATMuonMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1MuonsPt15Cumulative'),
  src_individual = cms.InputTag('selectedLayer1MuonsPt15Individual'),
  minNumber = cms.uint32(1)
)
muonHLTmatchCut = cms.PSet(
  name = cms.string('muonHLTmatchCut'),
  type = cms.string('PATMuonMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1MuonsHLTmatchCumulative'),
  src_individual = cms.InputTag('selectedLayer1MuonsHLTmatchIndividual'),
  minNumber = cms.uint32(1)
)
muonTrkIsoCut = cms.PSet(
  name = cms.string('muonTrkIsoCut'),
  type = cms.string('PATMuonMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1MuonsTrkIsoCumulative'),
  src_individual = cms.InputTag('selectedLayer1MuonsTrkIsoIndividual'),
  minNumber = cms.uint32(1)
)
muonEcalIsoCut = cms.PSet(
  name = cms.string('muonEcalIsoCut'),
  type = cms.string('PATMuonMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1MuonsEcalIsoCumulative'),
  src_individual = cms.InputTag('selectedLayer1MuonsEcalIsoIndividual'),
  minNumber = cms.uint32(1)
)
muonHcalIsoCut = cms.PSet(
  name = cms.string('muonHcalIsoCut'),
  type = cms.string('PATMuonMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1MuonsHcalIsoCumulative'),
  src_individual = cms.InputTag('selectedLayer1MuonsHcalIsoIndividual'),
  minNumber = cms.uint32(1)
)
muonAntiPionCut = cms.PSet(
  name = cms.string('muonAntiPionCut'),
  type = cms.string('PATMuonMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1MuonsPionVetoCumulative'),
  src_individual = cms.InputTag('selectedLayer1MuonsPionVetoIndividual'),
  minNumber = cms.uint32(1)
)
muonTrkIPcut = cms.PSet(
  name = cms.string('muonTrkIPcut'),
  type = cms.string('PATMuonMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1MuonsTrkIPcumulative'),
  src_individual = cms.InputTag('selectedLayer1MuonsTrkIPindividual'),
  minNumber = cms.uint32(1)
)

#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------

elecMuEventDump = cms.PSet(
  name = cms.string('elecMuEventDump'),
  type = cms.string('ElecMuEventDump'),

  triggerResultsSource = cms.InputTag('TriggerResults', '', 'HLT'),
  triggerPathsToPrint = cms.vstring('HLT_IsoEle8_IsoMu7'),

  genParticleSource = cms.InputTag('genParticles'),
  genTauJetSource = cms.InputTag('tauGenJets'),
#  genTauJetSource = cms.InputTag(''),
  electronSource = cms.InputTag('allLayer1ElectronsSelForTauAnalyses'),
  muonSource = cms.InputTag('allLayer1MuonsSelForTauAnalyses'),
  tauSource = cms.InputTag('allLayer1PFTausSelForTauAnalyses'),
  metSource = cms.InputTag('allLayer1METs'),

  #output = cms.string("elecTauEventDump.txt"),
  output = cms.string("std::cout"),

  triggerConditions = cms.vstring("muonTrkIPcut: passed_cumulative")
)

#--------------------------------------------------------------------------------
# define analysis sequence
# (ordered list of event selection criteria and histogram filling)
#--------------------------------------------------------------------------------

elecMuAnalysisSequence = cms.VPSet(
  # fill histograms for full event sample
  cms.PSet(
    histManagers = elecMuHistManagers
  ),

  # generator level selection of Z --> e + tau-jet events
  # passing basic acceptance and kinematic cuts
  # (NOTE: to be used for efficiency studies only !!)
  #cms.PSet(
  #  filter = cms.string('genElectronCut'),
  #  title = cms.string('gen. Electron'),
  #),
  #cms.PSet(
  #  filter = cms.string('genTauCut'),
  #  title = cms.string('gen. Tau'),
  #),
  #cms.PSet(
  #  histManagers = elecTauHistManagers
  #),

  # trigger selection
  cms.PSet(
    filter = cms.string('Trigger'),
    title = cms.string('X Electron Muon Trigger'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers
  ),

  # selection of electron candidate
  # produced in electronic tau decay
  cms.PSet(
    filter = cms.string('tightElectronIdCut'),
    title = cms.string('tight Electron Id.'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTightId')
  ),
  cms.PSet(
    filter = cms.string('electronAntiCrackCut'),
    title = cms.string('crack-Veto'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsAntiCrackCutCumulative')
  ),
  cms.PSet(
    filter = cms.string('electronEtaCut'),
    title = cms.string('-2.1 < eta(Electron) < +2.1'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsEta21Cumulative')
  ),
  cms.PSet(
    filter = cms.string('electronPtCut'),
    title = cms.string('Pt(Electron) > 15 GeV'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsPt15Cumulative')
  ),
  cms.PSet(
    filter = cms.string('electronHLTmatchCut'),
    title = cms.string('Electron Trigger match'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsHLTmatchCumulative')
  ),
  cms.PSet(
    filter = cms.string('electronTrkIsoCut'),
    title = cms.string('Electron Track iso.'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkIsoCumulative')
  ),
  cms.PSet(
    filter = cms.string('electronEcalIsoCut'),
    title = cms.string('Electron ECAL iso.'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsHcalIsoCumulative')
  ),
  cms.PSet(
    filter = cms.string('electronTrkCut'),
    title = cms.string('Electron Track find.'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkCumulative')
  ),
  cms.PSet(
    filter = cms.string('electronTrkIPcut'),
    title = cms.string('Electron Track IP'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkIPcumulative')
  ),

  # trigger selection
  cms.PSet(
    filter = cms.string('Trigger'),
    title = cms.string('X Electron Muon Trigger'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers
  ),

  # selection of muon candidate
  # produced in muonic tau decay
  cms.PSet(
    filter = cms.string('globalMuonCut'),
    title = cms.string('global Muon'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsGlobal')
  ),
  cms.PSet(
    filter = cms.string('muonEtaCut'),
    title = cms.string('-2.1 < eta(Muon) < +2.1'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsEta21Cumulative')
  ),
  cms.PSet(
    filter = cms.string('muonPtCut'),
    title = cms.string('Pt(Muon) > 15 GeV'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsPt15Cumulative')
  ),
  cms.PSet(
    filter = cms.string('muonHLTmatchCut'),
    title = cms.string('Muon Trigger match'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsHLTmatchCumulative')
  ),
  cms.PSet(
    filter = cms.string('muonTrkIsoCut'),
    title = cms.string('Muon Track iso.'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIsoCumulative')
  ),
  cms.PSet(
    filter = cms.string('muonEcalIsoCut'),
    title = cms.string('Muon ECAL iso.'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsEcalIsoCumulative')
  ),
  cms.PSet(
    filter = cms.string('muonHcalIsoCut'),
    title = cms.string('Muon HCAL iso.'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsHcalIsoCumulative')
  ),
  cms.PSet(
    filter = cms.string('muonAntiPionCut'),
    title = cms.string('Muon pi-Veto'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsPionVetoCumulative')
  ),
  cms.PSet(
    filter = cms.string('muonTrkIPcut'),
    title = cms.string('Muon Track IP'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative')
  ),
)
