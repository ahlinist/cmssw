import FWCore.ParameterSet.Config as cms
import copy

# import config for electron histogram manager
from TauAnalysis.Core.electronHistManager_cfi import *

# import config for muon histogram manager
from TauAnalysis.Core.muonHistManager_cfi import *

# import config for di-tau histogram manager
from TauAnalysis.Core.diTauCandidateHistManager_cfi import *
diTauCandidateHistManagerForElecMu = copy.deepcopy(diTauCandidateHistManager)
diTauCandidateHistManagerForElecMu.name = cms.string('diTauCandidateHistManagerForElecMu')
diTauCandidateHistManagerForElecMu.type = cms.string('PATElecMuPairHistManager')
diTauCandidateHistManagerForElecMu.diTauCandidateSource = cms.InputTag('allElecMuPairs')

# import config for central jet veto histogram manager
from TauAnalysis.Core.jetHistManager_cfi import *
jetHistManager.centralJetsToBeVetoed.etMin = cms.vdouble()
jetHistManager.centralJetsToBeVetoed.etaMax = cms.vdouble()
jetHistManager.centralJetsToBeVetoed.alphaMin = cms.vdouble()

# import config for missing-Et histogram manager
from TauAnalysis.Core.metHistManager_cfi import *

# import config for primary event vertex histogram manager
from TauAnalysis.Core.vertexHistManager_cfi import *

# import config for L1 & HLT histogram manager
from TauAnalysis.Core.triggerHistManager_cfi import *
triggerHistManager.l1Bits = cms.vstring('L1_SingleEG5', 'L1_SingleEG8', 'L1_SingleEG10', 'L1_SingleEG12', 'L1_SingleEG15',
                                        'L1_SingleIsoEG5', 'L1_SingleIsoEG8', 'L1_SingleIsoEG10', 'L1_SingleIsoEG12', 'L1_SingleIsoEG15',
                                        'L1_SingleMu3', 'L1_SingleMu5', 'L1_SingleMu7', 'L1_SingleMu10', 'L1_SingleMu14')
triggerHistManager.hltPaths = cms.vstring('HLT_IsoEle15_L1I', 'HLT_Mu15', 'HLT_IsoMu11', 'HLT_IsoEle8_IsoMu7')

elecMuHistManagers = cms.vstring( 'electronHistManager',
                                  'muonHistManager',
                                  'diTauCandidateHistManagerForElecMu',
                                  'metHistManager',
                                  'jetHistManager',
                                  'vertexHistManager',
                                  'triggerHistManager'
                                  )

#--------------------------------------------------------------------------------
# define event selection criteria
#--------------------------------------------------------------------------------

# generator level phase-space selection
# (NOTE: to be used in case of Monte Carlo samples
#        overlapping in simulated phase-space only !!)
genPhaseSpaceCut = cms.PSet(
  name = cms.string('genPhaseSpaceCut'),
  type = cms.string('GenPhaseSpaceEventInfoSelector'),
  src = cms.InputTag('genPhaseSpaceEventInfo'),
  cut = cms.string('')
)

# passing basic acceptance and kinematic cuts
# (NOTE: to be used for efficiency studies only !!)
#genElectronCut = cms.PSet(
#  name = cms.string('genElectronCut'),
#  type = cms.string('TauGenJetMinEventSelector'),
#  src = cms.InputTag('selectedGenTauDecaysToElectronPt15Cumulative'),
#  minNumber = cms.uint32(1)
#)
#genMuonCut = cms.PSet(
#  name = cms.string('genMuonCut'),
#  type = cms.string('TauGenJetMinEventSelector'),
#  src = cms.InputTag('selectedGenTauDecaysToMuonPt15Cumulative'),
#  minNumber = cms.uint32(1)
#)

# trigger selection
Trigger = cms.PSet(
  name = cms.string('Trigger'),
  type = cms.string('TriggerResultEventSelector'),
  src = cms.InputTag('TriggerResults::HLT'),
  triggerPaths = cms.vstring('HLT_IsoEle15_L1I', 'HLT_Mu15', 'HLT_IsoMu11', 'HLT_IsoEle8_IsoMu7')
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

# electron candidate selection
tightElectronIdMinCut = cms.PSet(
  name = cms.string('tightElectronIdMinCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src = cms.InputTag('selectedLayer1ElectronsTightId'),
  minNumber = cms.uint32(1)
)
tightElectronIdMaxCut = cms.PSet(
  name = cms.string('tightElectronIdMaxCut'),
  type = cms.string('PATCandViewMaxEventSelector'),
  src = cms.InputTag('selectedLayer1ElectronsTightId'),
  maxNumber = cms.uint32(1)
)
electronAntiCrackCut = cms.PSet(
  name = cms.string('electronAntiCrackCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1ElectronsAntiCrackCutCumulative'),
  src_individual = cms.InputTag('selectedLayer1ElectronsAntiCrackCutIndividual'),
  minNumber = cms.uint32(1)
)
electronEtaCut = cms.PSet(
  name = cms.string('electronEtaCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1ElectronsEta21Cumulative'),
  src_individual = cms.InputTag('selectedLayer1ElectronsEta21Individual'),
  minNumber = cms.uint32(1)
)
electronPtCut = cms.PSet(
  name = cms.string('electronPtCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1ElectronsPt15Cumulative'),
  src_individual = cms.InputTag('selectedLayer1ElectronsPt15Individual'),
  minNumber = cms.uint32(1)
)
electronTrkIsoCut = cms.PSet(
  name = cms.string('electronTrkIsoCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1ElectronsTrkIsoCumulative'),
  src_individual = cms.InputTag('selectedLayer1ElectronsTrkIsoIndividual'),
  minNumber = cms.uint32(1)
)
electronEcalIsoCut = cms.PSet(
  name = cms.string('electronEcalIsoCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1ElectronsEcalIsoCumulative'),
  src_individual = cms.InputTag('selectedLayer1ElectronsEcalIsoIndividual'),
  minNumber = cms.uint32(1)
)
electronTrkCut = cms.PSet(
  name = cms.string('electronTrkCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1ElectronsTrkCumulative'),
  src_individual = cms.InputTag('selectedLayer1ElectronsTrkIndividual'),
  minNumber = cms.uint32(1)
)
#electronTrkIPcut = cms.PSet(
#  name = cms.string('electronTrkIPcut'),
#  type = cms.string('PATCandViewMinEventSelector'),
#  src_cumulative = cms.InputTag('selectedLayer1ElectronsTrkIPcumulative'),
#  src_individual = cms.InputTag('selectedLayer1ElectronsTrkIPindividual'),
#  minNumber = cms.uint32(1)
#)

# muon candidate selection
globalMuonMinCut = cms.PSet(
  name = cms.string('globalMuonMinCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src = cms.InputTag('selectedLayer1MuonsGlobal'),
  minNumber = cms.uint32(1)
)
globalMuonMaxCut = cms.PSet(
  name = cms.string('globalMuonMaxCut'),
  type = cms.string('PATCandViewMaxEventSelector'),
  src = cms.InputTag('selectedLayer1MuonsGlobal'),
  maxNumber = cms.uint32(1)
)
muonEtaCut = cms.PSet(
  name = cms.string('muonEtaCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1MuonsEta21Cumulative'),
  src_individual = cms.InputTag('selectedLayer1MuonsEta21Individual'),
  minNumber = cms.uint32(1)
)
muonPtCut = cms.PSet(
  name = cms.string('muonPtCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1MuonsPt15Cumulative'),
  src_individual = cms.InputTag('selectedLayer1MuonsPt15Individual'),
  minNumber = cms.uint32(1)
)
muonTrkIsoCut = cms.PSet(
  name = cms.string('muonTrkIsoCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1MuonsTrkIsoCumulative'),
  src_individual = cms.InputTag('selectedLayer1MuonsTrkIsoIndividual'),
  minNumber = cms.uint32(1)
)
muonEcalIsoCut = cms.PSet(
  name = cms.string('muonEcalIsoCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1MuonsEcalIsoCumulative'),
  src_individual = cms.InputTag('selectedLayer1MuonsEcalIsoIndividual'),
  minNumber = cms.uint32(1)
)
#muonHcalIsoCut = cms.PSet(
#  name = cms.string('muonHcalIsoCut'),
#  type = cms.string('PATCandViewMinEventSelector'),
#  src_cumulative = cms.InputTag('selectedLayer1MuonsHcalIsoCumulative'),
#  src_individual = cms.InputTag('selectedLayer1MuonsHcalIsoIndividual'),
#  minNumber = cms.uint32(1)
#)
muonAntiPionCut = cms.PSet(
  name = cms.string('muonAntiPionCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1MuonsPionVetoCumulative'),
  src_individual = cms.InputTag('selectedLayer1MuonsPionVetoIndividual'),
  minNumber = cms.uint32(1)
)
#muonTrkIPcut = cms.PSet(
#  name = cms.string('muonTrkIPcut'),
#  type = cms.string('PATCandViewMinEventSelector'),
#  src_cumulative = cms.InputTag('selectedLayer1MuonsTrkIPcumulative'),
#  src_individual = cms.InputTag('selectedLayer1MuonsTrkIPindividual'),
#  minNumber = cms.uint32(1)
#)

# di-tau candidate selection
#diTauCandidateForElecMuAcoplanarityCut = cms.PSet(
#  name = cms.string('diTauCandidateForElecMuAcoplanarityCut'),
#  type = cms.string('PATCandViewMinEventSelector'),
#  src = cms.InputTag('selectedElecMuPairsAcoplanarity'),
#  minNumber = cms.uint32(1)
#)
diTauCandidateForElecMuZeroChargeCut = cms.PSet(
  name = cms.string('diTauCandidateForElecMuZeroChargeCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedElecMuPairsZeroChargeCumulative'),
  src_individual = cms.InputTag('selectedElecMuPairsZeroChargeIndividual'),
  minNumber = cms.uint32(1)
)
diTauCandidateForElecMuDPhiCut = cms.PSet(
  name = cms.string('diTauCandidateForElecMuDPhiCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedElecMuPairsDPhiCumulative'),
  src_individual = cms.InputTag('selectedElecMuPairsDPhiIndividual'),
  minNumber = cms.uint32(1)
)
diTauCandidateForElecMuImpParamSigCut = cms.PSet(
  name = cms.string('diTauCandidateForElecMuImpParamSigCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedElecMuPairsImpParamSigCumulative'),
  src_individual = cms.InputTag('selectedElecMuPairsImpParamSigIndividual'),
  minNumber = cms.uint32(1)
)

# jet candidate selection
jetMinCut = cms.PSet(
  name = cms.string('jetMinCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src = cms.InputTag('selectedLayer1JetsEt20'),
  minNumber = cms.uint32(0)
)
jetMaxCut = cms.PSet(
  name = cms.string('jetMaxCut'),
  type = cms.string('PATCandViewMaxEventSelector'),
  src = cms.InputTag('selectedLayer1JetsEt20'),
  maxNumber = cms.uint32(2)
)
jetBtagMinCut = cms.PSet(
  name = cms.string('jetBtagMinCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src = cms.InputTag('selectedLayer1JetsBtag'),
  minNumber = cms.uint32(0)
)
jetBtagMaxCut = cms.PSet(
  name = cms.string('jetBtagMaxCut'),
  type = cms.string('PATCandViewMaxEventSelector'),
  src = cms.InputTag('selectedLayer1JetsBtag'),
  maxNumber = cms.uint32(1)
)


#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------

elecMuEventDump = cms.PSet(
  name = cms.string('elecMuEventDump'),
  type = cms.string('ElecMuEventDump'),

  l1GtReadoutRecordSource = cms.InputTag('hltGtDigis::HLT'),
  l1GtObjectMapRecordSource = cms.InputTag('hltL1GtObjectMap::HLT'),
  l1BitsToPrint = cms.vstring('L1_SingleEG5', 'L1_SingleEG8', 'L1_SingleEG10', 'L1_SingleEG12', 'L1_SingleEG15',
                              'L1_SingleIsoEG5', 'L1_SingleIsoEG8', 'L1_SingleIsoEG10', 'L1_SingleIsoEG12', 'L1_SingleIsoEG15',
                              'L1_SingleMu3', 'L1_SingleMu5', 'L1_SingleMu7', 'L1_SingleMu10', 'L1_SingleMu14'),

  hltResultsSource = cms.InputTag('TriggerResults::HLT'),
  hltPathsToPrint = cms.vstring('HLT_IsoEle15_L1I', 'HLT_Mu15', 'HLT_IsoMu11', 'HLT_IsoEle8_IsoMu7'),
      
  genParticleSource = cms.InputTag('genParticles'),
  genTauJetSource = cms.InputTag('tauGenJets'),
  electronSource = cms.InputTag('allLayer1ElectronsSelForTauAnalyses'),
  muonSource = cms.InputTag('allLayer1MuonsSelForTauAnalyses'),
  tauSource = cms.InputTag('allLayer1PFTausSelForTauAnalyses'),
  metSource = cms.InputTag('allLayer1METs'),
  genMEtSource = cms.InputTag('genMETWithMu'),
  jetSource = cms.InputTag('selectedLayer1JetsEt20'),

  #output = cms.string("elecMuEventDump.txt"),
  output = cms.string("std::cout"),

  triggerConditions = cms.vstring("diTauCandidateForElecMuZeroChargeCut: passed_cumulative")
  #triggerConditions = cms.vstring("Trigger: rejected_cumulative",
  #                                "primaryEventVertex: rejected_cumulative",
  #                                "primaryEventVertexQuality: rejected_cumulative",
  #                                "primaryEventVertexPosition: rejected_cumulative",
  #                                "tightElectronIdCut: rejected_cumulative",
  #                                "electronEtaCut: rejected_cumulative",
  #                                "electronPtCut: rejected_cumulative",
  #                                "electronTrkIsoCut: rejected_cumulative",
  #                                "electronEcalIsoCut: rejected_cumulative",
  #                                "electronTrkCut: rejected_cumulative",
  #                                "electronTrkIPcut: rejected_cumulative",
  #                                "globalMuonCut: rejected_cumulative",
  #                                "muonEtaCut: rejected_cumulative",
  #                                "muonPtCut: rejected_cumulative",
  #                                "muonTrkIsoCut: rejected_cumulative",
  #                                "muonEcalIsoCut: rejected_cumulative",
  #                                "muonHcalIsoCut: rejected_cumulative",
  #                                "muonAntiPionCut: rejected_cumulative",
  #                                "muonTrkIPcut: rejected_cumulative",
  #                                "diTauCandidateForElecMuAntiOverlapVeto: rejected_cumulative",
  #                                "diTauCandidateForElecMuAcoplanarityCut: rejected_cumulative",
  #                                "diTauCandidateForElecMuZeroChargeCut: rejected_cumulative")
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

  # generator level selection of Z --> e + mu events
  # passing basic acceptance and kinematic cuts
  # (NOTE: to be used for efficiency studies only !!)
  #cms.PSet(
  #  filter = cms.string('genElectronCut'),
  #  title = cms.string('gen. Electron'),
  #),
  #cms.PSet(
  #  filter = cms.string('genMuonCut'),
  #  title = cms.string('gen. Muon'),
  #),
  #cms.PSet(
  #  histManagers = elecMuHistManagers
  #),
    
  # trigger selection
  cms.PSet(
    filter = cms.string('Trigger'),
    title = cms.string('Elec || Muon Triggers'),
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

  #only 1 electron and 1 muon
  cms.PSet(
    filter = cms.string('tightElectronIdMinCut'),
    title = cms.string('tight Electron Id. Min'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTightId')
  ),
  cms.PSet(
    filter = cms.string('tightElectronIdMaxCut'),
    title = cms.string('tight Electron Id. Max'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTightId')
  ),
  cms.PSet(
    filter = cms.string('globalMuonMinCut'),
    title = cms.string('global Muon min'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTightId',
                          'muonHistManager.muonSource = selectedLayer1MuonsGlobal')
  ),
  cms.PSet(
    filter = cms.string('globalMuonMaxCut'),
    title = cms.string('global Muon max'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTightId',
                          'muonHistManager.muonSource = selectedLayer1MuonsGlobal')
  ),

# kinematic cuts
  cms.PSet(
    filter = cms.string('electronAntiCrackCut'),
    title = cms.string('crack-Veto'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsAntiCrackCutCumulative',
                          'muonHistManager.muonSource = selectedLayer1MuonsGlobal')
  ),
  cms.PSet(
    filter = cms.string('electronEtaCut'),
    title = cms.string('-2.4 < eta(Elec) < +2.4'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsEta21Cumulative',
                          'muonHistManager.muonSource = selectedLayer1MuonsGlobal')
  ),
  cms.PSet(
    filter = cms.string('muonEtaCut'),
    title = cms.string('-2.4 < eta(Muon) < +2.4'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsEta21Cumulative',
                          'muonHistManager.muonSource = selectedLayer1MuonsEta21Cumulative')
  ),
  cms.PSet(
    filter = cms.string('electronPtCut'),
    title = cms.string('Pt(Electron) > 15 GeV'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsPt15Cumulative',
                          'muonHistManager.muonSource = selectedLayer1MuonsEta21Cumulative')
  ),
  cms.PSet(
    filter = cms.string('muonPtCut'),
    title = cms.string('Pt(Muon) > 15 GeV'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsEta21Cumulative',
                          'muonHistManager.muonSource = selectedLayer1MuonsPt15Cumulative')
  ),

#electron isolation
  cms.PSet(
    filter = cms.string('electronTrkIsoCut'),
    title = cms.string('Electron Track iso.'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkIsoCumulative',
                          'muonHistManager.muonSource = selectedLayer1MuonsPt15Cumulative')
  ),
  cms.PSet(
    filter = cms.string('electronEcalIsoCut'),
    title = cms.string('Electron ECAL iso.'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsEcalIsoCumulative',
                          'muonHistManager.muonSource = selectedLayer1MuonsPt15Cumulative')
  ),
  cms.PSet(
    filter = cms.string('electronTrkCut'),
    title = cms.string('Electron Track find.'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkCumulative',
                          'muonHistManager.muonSource = selectedLayer1MuonsPt15Cumulative')
  ),

# muon isolation
  cms.PSet(
    filter = cms.string('muonTrkIsoCut'),
    title = cms.string('Muon Track iso.'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkCumulative',
                          'muonHistManager.muonSource = selectedLayer1MuonsTrkIsoCumulative')
  ),
  cms.PSet(
    filter = cms.string('muonEcalIsoCut'),
    title = cms.string('Muon ECAL iso.'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkCumulative',
                          'muonHistManager.muonSource = selectedLayer1MuonsEcalIsoCumulative')
  ),
#  cms.PSet(
#    filter = cms.string('muonHcalIsoCut'),
#    title = cms.string('Muon HCAL iso.'),
#    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
#  ),
#  cms.PSet(
#    histManagers = elecMuHistManagers,
#    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkCumulative',
#                          'muonHistManager.muonSource = selectedLayer1MuonsHcalIsoCumulative')
#  ),
  cms.PSet(
    filter = cms.string('muonAntiPionCut'),
    title = cms.string('Muon pi-Veto'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkCumulative',
                          'muonHistManager.muonSource = selectedLayer1MuonsPionVetoCumulative')
  ),

  #selection of electron + muon combinations
#  cms.PSet(
#    filter = cms.string('diTauCandidateForElecMuAcoplanarityCut'),
#    #title = cms.string('cos(dPhi(Electron-MET)) > 0.5 || cos(dPhi(Muon-MET)) > 0.5'),
#    title = cms.string('cos(dPhi(lept-MET))>0.5'),
#    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
#  ),
#  cms.PSet(
#    histManagers = elecMuHistManagers,
#    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkCumulative',
#                          'muonHistManager.muonSource = selectedLayer1MuonsTrkCumulative',
#                          'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsAcoplanarity')
#  ),
  cms.PSet(
    filter = cms.string('diTauCandidateForElecMuZeroChargeCut'),
    title = cms.string('Charge(Elec+Muon) = 0'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkCumulative',
                          'muonHistManager.muonSource = selectedLayer1MuonsTrkCumulative',
                          'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsZeroChargeCumulative')
  ),
  cms.PSet(
    filter = cms.string('diTauCandidateForElecMuDPhiCut'),
    title = cms.string('DPhi(Elec,Muon) < 3.07'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkCumulative',
                          'muonHistManager.muonSource = selectedLayer1MuonsTrkCumulative',
                          'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsDPhiCumulative')
  ),
  cms.PSet(
    filter = cms.string('diTauCandidateForElecMuImpParamSigCut'),
    title = cms.string('Imp.Param. Significance'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkCumulative',
                          'muonHistManager.muonSource = selectedLayer1MuonsTrkCumulative',
                          'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsImpParamSigCumulative')
  ),

  #selection of jets
  cms.PSet(
    filter = cms.string('jetMinCut'),
    title = cms.string('Jet min'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkCumulative',
                          'muonHistManager.muonSource = selectedLayer1MuonsTrkCumulative',
                          'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsImpParamSigCumulative',
                          'jetHistManager.jetSource = selectedLayer1JetsEt20')
  ),
  cms.PSet(
    filter = cms.string('jetMaxCut'),
    title = cms.string('Jet max'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkCumulative',
                          'muonHistManager.muonSource = selectedLayer1MuonsTrkCumulative',
                          'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsImpParamSigCumulative',
                          'jetHistManager.jetSource = selectedLayer1JetsEt20')
  ),
  cms.PSet(
    filter = cms.string('jetBtagMinCut'),
    title = cms.string('b-tagging min cut'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkCumulative',
                          'muonHistManager.muonSource = selectedLayer1MuonsTrkCumulative',
                          'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsImpParamSigCumulative',
                          'jetHistManager.jetSource = selectedLayer1JetsBtag')
  ),
  cms.PSet(
    filter = cms.string('jetBtagMaxCut'),
    title = cms.string('b-tagging max cut'),
    saveRunEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecMuHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkCumulative',
                          'muonHistManager.muonSource = selectedLayer1MuonsTrkCumulative',
                          'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsImpParamSigCumulative',
                          'jetHistManager.jetSource = selectedLayer1JetsBtag')
  )
)

