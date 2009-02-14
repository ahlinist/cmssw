import FWCore.ParameterSet.Config as cms
import copy

# import config for muon histogram manager
from TauAnalysis.Core.muonHistManager_cfi import *

# import config for electron histogram manager
from TauAnalysis.Core.electronHistManager_cfi import *

# import config for missing-Et histogram manager
from TauAnalysis.Core.metHistManager_cfi import *

# import config for primary event vertex histogram manager
from TauAnalysis.Core.vertexHistManager_cfi import *

# import config for L1 & HLT histogram manager
from TauAnalysis.Core.triggerHistManager_cfi import *
triggerHistManager.l1Bits = cms.vstring('L1_SingleMu3', 'L1_SingleMu5', 'L1_SingleMu7', 'L1_SingleMu10', 'L1_SingleMu14','L1_SingleEG5', 'L1_SingleEG8', 'L1_SingleEG10', 'L1_SingleEG12', 'L1_SingleEG15', 'L1_SingleIsoEG5', 'L1_SingleIsoEG8', 'L1_SingleIsoEG10', 'L1_SingleIsoEG12', 'L1_SingleIsoEG15')
triggerHistManager.hltPaths = cms.vstring('HLT_Mu15', 'HLT_IsoMu11','HLT_IsoEle15_L1I','HLT_IsoEle8_IsoMu7')

elecMuHistManagers = cms.vstring( 'muonHistManager',
                                 'metHistManager',
                                 'electronHistManager',
                                 'vertexHistManager',
                                 'triggerHistManager' )

#--------------------------------------------------------------------------------
# define event selection criteria
#--------------------------------------------------------------------------------

# passing basic acceptance and kinematic cuts
# (NOTE: to be used for efficiency studies only !!)
#genMuonCut = cms.PSet(
#  name = cms.string('genMuonCut'),
#  type = cms.string('TauGenJetMinEventSelector'),
#  src = cms.InputTag('selectedGenTauDecaysToMuonPt15Cumulative'),
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
  src = cms.InputTag('TriggerResults::HLT'),
  triggerPaths = cms.vstring('HLT_Mu15', 'HLT_IsoMu11','HLT_IsoEle8_IsoMu7','HLT_IsoEle15_L1I')
)

# primary event vertex selection
primaryEventVertex = cms.PSet(
  name = cms.string('primaryEventVertex'),
  type = cms.string('VertexMinEventSelector'),
  src = cms.InputTag('selectedPrimaryVertexHighestPtTrackSum'),
  minNumber = cms.uint32(1)
)
primaryEventVertexQuality = cms.PSet(
  name = cms.string('primaryEventVertexQuality'),
  type = cms.string('VertexMinEventSelector'),
  src = cms.InputTag('selectedPrimaryVertexQuality'),
  minNumber = cms.uint32(1)
)
primaryEventVertexPosition = cms.PSet(
  name = cms.string('primaryEventVertexPosition'),
  type = cms.string('VertexMinEventSelector'),
  src = cms.InputTag('selectedPrimaryVertexPosition'),
  minNumber = cms.uint32(1)
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



#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------

elecMuEventDump = cms.PSet(
  name = cms.string('elecMuEventDump'),
  type = cms.string('ElecMuEventDump'),

  l1GtReadoutRecordSource = cms.InputTag('hltGtDigis::HLT'),
  l1GtObjectMapRecordSource = cms.InputTag('hltL1GtObjectMap::HLT'),
  l1BitsToPrint = cms.vstring('L1_SingleMu3', 'L1_SingleMu5', 'L1_SingleMu7', 'L1_SingleMu10', 'L1_SingleMu14','L1_SingleEG5', 'L1_SingleEG8', 'L1_SingleEG10', 'L1_SingleEG12', 'L1_SingleEG15', 'L1_SingleIsoEG5', 'L1_SingleIsoEG8', 'L1_SingleIsoEG10', 'L1_SingleIsoEG12', 'L1_SingleIsoEG15'),

  hltResultsSource = cms.InputTag('TriggerResults::HLT'),
  hltPathsToPrint = cms.vstring('HLT_Mu15', 'HLT_IsoMu11','HLT_IsoEle8_IsoMu7','HLT_IsoEle15_L1I'),
      
  genParticleSource = cms.InputTag('genParticles'),
  genTauJetSource = cms.InputTag('tauGenJets'),
  electronSource = cms.InputTag('allLayer1ElectronsSelForTauAnalyses'),
  muonSource = cms.InputTag('allLayer1MuonsSelForTauAnalyses'),
  tauSource = cms.InputTag('allLayer1PFTausSelForTauAnalyses'),
  metSource = cms.InputTag('allLayer1METs'),

  #output = cms.string("muTauEventDump.txt"),
  output = cms.string("std::cout"),

  triggerConditions = cms.vstring("electronTrkIPcut: passed_cumulative")
  #triggerConditions = cms.vstring("muonTrkIsoCut: rejected_cumulative")
  #triggerConditions = cms.vstring("muonTrkIPcut: rejected_cumulative")
  #triggerConditions = cms.vstring("muTauEventVertex: rejected_cumulative")
  #triggerConditions = cms.vstring("Trigger: rejected_cumulative",
  #                                "globalMuonCut: rejected_cumulative",
  #                                "muonEtaCut: rejected_cumulative",
  #                                "muonPtCut: rejected_cumulative",
  #                                "muonHLTmatchCut: rejected_cumulative",
  #                                "muonTrkIsoCut: rejected_cumulative",
  #                                "muonEcalIsoCut: rejected_cumulative",
  #                                "muonHcalIsoCut: rejected_cumulative",
  #                                "muonAntiPionCut: rejected_cumulative",
  #                                "tightElectronIdCut: rejected_cumulative",
  #                                "electronEtaCut: rejected_cumulative",
  #                                "electronPtCut: rejected_cumulative",
  #                                "electronHLTmatchCut: rejected_cumulative",
  #                                "electronTrkIsoCut: rejected_cumulative",
  #                                "electronEcalIsoCut: rejected_cumulative",
  #                                "electronTrkCut: rejected_cumulative",
  #                                "electronTrkIPcut: rejected_cumulative",

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

  # generator level selection of Z --> mu + e events
  # passing basic acceptance and kinematic cuts
  # (NOTE: to be used for efficiency studies only !!)
  #cms.PSet(
  #  filter = cms.string('genMuonCut'),
  #  title = cms.string('gen. Muon'),
  #),
  #cms.PSet(
  #  filter = cms.string('genTauCut'),
  #  title = cms.string('gen. Tau'),
  #),
  #cms.PSet(
  #  histManagers = muTauHistManagers
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

  # primary event vertex selection
  cms.PSet(
    filter = cms.string('primaryEventVertex'),
    title = cms.string('Vertex'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    filter = cms.string('primaryEventVertexQuality'),
    title = cms.string('p(chi2Vertex) > 0.01'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    filter = cms.string('primaryEventVertexPosition'),
    title = cms.string('-50 < zVertex < +50 cm'),
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

  )
)

