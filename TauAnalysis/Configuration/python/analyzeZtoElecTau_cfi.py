import FWCore.ParameterSet.Config as cms

# import config for electron histogram manager
from TauAnalysis.DQMTools.electronHistManager_cfi import *

# import config for tau histogram manager
from TauAnalysis.DQMTools.pftauHistManager_cfi import *

elecTauHistManagers = cms.vstring('electronHistManager', 'tauHistManager')

#--------------------------------------------------------------------------------
# define event selection criteria
#--------------------------------------------------------------------------------

# generator level selection of Z --> e + tau-jet events
# passing basic acceptance and kinematic cuts
# (NOTE: to be used for efficiency studies only !!)
#genElectronCut = cms.PSet(
#  name = cms.string('genElectronCut'),
#  type = cms.string('TauGenJetMinEventSelector'),
#  src = cms.InputTag('selectedGenTauDecaysToElectronPt15Cumulative'),
#  minNumber = cms.uint32(1)
#)
#genTauCut = cms.PSet(
#  name = cms.string('genTauCut'),
#  type = cms.string('TauGenJetMinEventSelector'),
#  src = cms.InputTag('selectedGenTauDecaysToHadronsPt20Cumulative'),
#  minNumber = cms.uint32(1)
#)

# trigger selection
Trigger = cms.PSet(
  name = cms.string('Trigger'),
  type = cms.string('TriggerResultEventSelector'),
  src = cms.InputTag('TriggerResults', '', 'HLT'),
  triggerPaths = cms.vstring('HLT_IsoEle15_L1I')
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

# tau candidate selection
tauAntiOverlapWithElectronsVeto = cms.PSet(
  name = cms.string('tauAntiOverlapWithElectronsVeto'),
  type = cms.string('PATTauMinEventSelector'),
  src = cms.InputTag('selectedLayer1TausForElecTauAntiOverlapWithElectronsVeto'),
  minNumber = cms.uint32(1)
)
tauEtaCut = cms.PSet(
  name = cms.string('tauEtaCut'),
  type = cms.string('PATTauMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1TausForElecTauEta21Cumulative'),
  src_individual = cms.InputTag('selectedLayer1TausEta21Individual'),
  minNumber = cms.uint32(1)
)
tauPtCut = cms.PSet(
  name = cms.string('tauPtCut'),
  type = cms.string('PATTauMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1TausForElecTauPt20Cumulative'),
  src_individual = cms.InputTag('selectedLayer1TausPt20Individual'),
  minNumber = cms.uint32(1)
)
tauLeadTrkCut = cms.PSet(
  name = cms.string('tauLeadTrkCut'),
  type = cms.string('PATTauMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1TausForElecTauLeadTrkCumulative'),
  src_individual = cms.InputTag('selectedLayer1TausLeadTrkIndividual'),
  minNumber = cms.uint32(1)
)
tauLeadTrkPtCut = cms.PSet(
  name = cms.string('tauLeadTrkPtCut'),
  type = cms.string('PATTauMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1TausForElecTauLeadTrkPtCumulative'),
  src_individual = cms.InputTag('selectedLayer1TausLeadTrkPtIndividual'),
  minNumber = cms.uint32(1)
)
tauTrkIsoCut = cms.PSet(
  name = cms.string('tauTrkIsoCut'),
  type = cms.string('PATTauMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1TausForElecTauTrkIsoCumulative'),
  src_individual = cms.InputTag('selectedLayer1TausTrkIsoIndividual'),
  minNumber = cms.uint32(1)
)
tauEcalIsoCut = cms.PSet(
  name = cms.string('tauEcalIsoCut'),
  type = cms.string('PATTauMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1TausForElecTauEcalIsoCumulative'),
  src_individual = cms.InputTag('selectedLayer1TausEcalIsoIndividual'),
  minNumber = cms.uint32(1)
)
tauProngCut = cms.PSet(
  name = cms.string('tauProngCut'),
  type = cms.string('PATTauMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1TausForElecTauProngCumulative'),
  src_individual = cms.InputTag('selectedLayer1TausProngIndividual'),
  minNumber = cms.uint32(1)
)
tauElectronVeto = cms.PSet(
  name = cms.string('tauElectronVeto'),
  type = cms.string('PATTauMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1TausForElecTauElectronVetoCumulative'),
  src_individual = cms.InputTag('selectedLayer1TausElectronVetoIndividual'),
  minNumber = cms.uint32(1)
)

# require common event vertex for electron and tau-jet candidate
#elecTauEventVertex = cms.PSet(
#  name = cms.string('elecTauEventVertex'),
#  type = cms.string('VertexEventSelector'),
#  src1 = cms.InputTag('selectedLayer1ElectronsTrkIPcumulative'),
#  src2 = cms.InputTag('selectedLayer1TausForElecTauElectronVetoCumulative'),
#  dXYmax = cms.double(0.5),
#  dZmax = cms.double(5.),
#  zMin1 = cms.double(-50.),
#  zMax1 = cms.double(50.),
#  zMin2 = cms.double(-50.),
#  zMax2 = cms.double(50.)
#)

# acoplanarity cut between electron and tau-jet candidate
# ( dPhi(elec,tau) < 2.4 rad; not implemented yet)

#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------

elecTauEventDump = cms.PSet(
  name = cms.string('elecTauEventDump'),
  type = cms.string('ElecTauEventDump'),

  triggerResultsSource = cms.InputTag('TriggerResults', '', 'HLT'),
  triggerPathsToPrint = cms.vstring('HLT_IsoEle15_L1I'),

  genParticleSource = cms.InputTag('genParticles'),
  genTauJetSource = cms.InputTag('tauGenJets'),
  electronSource = cms.InputTag('allLayer1ElectronsSelForTauAnalyses'),
  tauSource = cms.InputTag('allLayer1PFTausSelForTauAnalyses'),
  metSource = cms.InputTag('allLayer1METs'),

  #output = cms.string("elecTauEventDump.txt"),
  output = cms.string("std::cout"),

  triggerConditions = cms.vstring("tauElectronVeto: passed_cumulative")
)

#--------------------------------------------------------------------------------
# define analysis sequence
# (ordered list of event selection criteria and histogram filling)
#--------------------------------------------------------------------------------

elecTauAnalysisSequence = cms.VPSet(
  # fill histograms for full event sample
  cms.PSet(
    histManagers = elecTauHistManagers
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
    title = cms.string('isoEle15 Trigger'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers
  ),
  
  # selection of electron candidate
  # produced in electronic tau decay
  cms.PSet(
    filter = cms.string('tightElectronIdCut'),
    title = cms.string('tight Electron Id.'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTightId')
  ),
  cms.PSet(
    filter = cms.string('electronAntiCrackCut'),
    title = cms.string('crack-Veto'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsAntiCrackCutCumulative')
  ),
  cms.PSet(
    filter = cms.string('electronEtaCut'),
    title = cms.string('-2.1 < eta(Electron) < +2.1'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsEta21Cumulative')
  ),
  cms.PSet(
    filter = cms.string('electronPtCut'),
    title = cms.string('Pt(Electron) > 15 GeV'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsPt15Cumulative')
  ),
  cms.PSet(
    filter = cms.string('electronHLTmatchCut'),
    title = cms.string('Electron Trigger match'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsHLTmatchCumulative')
  ),
  cms.PSet(
    filter = cms.string('electronTrkIsoCut'),
    title = cms.string('Electron Track iso.'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkIsoCumulative')
  ),
  cms.PSet(
    filter = cms.string('electronEcalIsoCut'),
    title = cms.string('Electron ECAL iso.'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsHcalIsoCumulative')
  ),
  cms.PSet(
    filter = cms.string('electronTrkCut'),
    title = cms.string('Electron Track find.'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkCumulative')
  ),
  cms.PSet(
    filter = cms.string('electronTrkIPcut'),
    title = cms.string('Electron Track IP'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkIPcumulative')
  ),
  
  # selection of tau-jet candidate
  # produced in hadronic tau decay
  cms.PSet(
    filter = cms.string('tauAntiOverlapWithElectronsVeto'),
    title = cms.string('Tau not overlapping with Electron'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('tauHistManager.tauSource = selectedLayer1TausForElecTauAntiOverlapWithElectronsVeto')
  ),
  cms.PSet(
    filter = cms.string('tauEtaCut'),
    title = cms.string('-2.1 < eta(Tau) < +2.1'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('tauHistManager.tauSource = selectedLayer1TausForElecTauEta21Cumulative')
  ),
  cms.PSet(
    filter = cms.string('tauPtCut'),
    title = cms.string('Pt(Tau) > 20 GeV'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('tauHistManager.tauSource = selectedLayer1TausForElecTauPt20Cumulative')
  ),
  cms.PSet(
    filter = cms.string('tauLeadTrkCut'),
    title = cms.string('Tau lead. Track find.'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('tauHistManager.tauSource = selectedLayer1TausForElecTauLeadTrkCumulative')
  ),
  cms.PSet(
    filter = cms.string('tauLeadTrkPtCut'),
    title = cms.string('Tau lead. Track Pt'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('tauHistManager.tauSource = selectedLayer1TausForElecTauLeadTrkPtCumulative')
  ),
  cms.PSet(
    filter = cms.string('tauTrkIsoCut'),
    title = cms.string('Tau Track iso.'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('tauHistManager.tauSource = selectedLayer1TausForElecTauTrkIsoCumulative')
  ),
  cms.PSet(
    filter = cms.string('tauEcalIsoCut'),
    title = cms.string('Tau ECAL iso.'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('tauHistManager.tauSource = selectedLayer1TausForElecTauEcalIsoCumulative')
  ),
  cms.PSet(
    filter = cms.string('tauProngCut'),
    title = cms.string('Tau 1||3-Prong'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('tauHistManager.tauSource = selectedLayer1TausForElecTauProngCumulative')
  ),
  cms.PSet(
    filter = cms.string('tauElectronVeto'),
    title = cms.string('Tau e-Veto'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('tauHistManager.tauSource = selectedLayer1TausForElecTauElectronVetoCumulative')
  ),

  # require common event vertex for electron and tau-jet candidate
  #cms.PSet(
  #  filter = cms.string('elecTauEventVertex'),
  #  title = cms.string('Vertex'),
  #  saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  #)
  
  # acoplanarity cut between electron and tau-jet candidate
  # (not implemented yet)
)
