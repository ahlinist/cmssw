import FWCore.ParameterSet.Config as cms
import copy

# import config for histogram manager filling information about phase-space simulated in Monte Carlo sample
from TauAnalysis.Core.genPhaseSpaceEventInfoHistManager_cfi import *

# import config for electron histogram manager
from TauAnalysis.Core.electronHistManager_cfi import *

# import config for tau histogram manager
from TauAnalysis.Core.pftauHistManager_cfi import *

# import config for di-tau histogram manager
from TauAnalysis.Core.diTauCandidateHistManager_cfi import *
diTauCandidateHistManagerForElecTau = copy.deepcopy(diTauCandidateHistManager)
diTauCandidateHistManagerForElecTau.name = cms.string('diTauCandidateHistManagerForElecTau')
diTauCandidateHistManagerForElecTau.type = cms.string('PATElecTauPairHistManager')
diTauCandidateHistManagerForElecTau.diTauCandidateSource = cms.InputTag('allElecTauPairs')

# import config for missing-Et histogram manager
from TauAnalysis.Core.metHistManager_cfi import *

# import config for primary event vertex histogram manager
from TauAnalysis.Core.vertexHistManager_cfi import *

# import config for L1 & HLT histogram manager
from TauAnalysis.Core.triggerHistManager_cfi import *
triggerHistManager.l1Bits = cms.vstring('L1_SingleEG5', 'L1_SingleEG8', 'L1_SingleEG10', 'L1_SingleEG12', 'L1_SingleEG15',
                                        'L1_SingleIsoEG5', 'L1_SingleIsoEG8', 'L1_SingleIsoEG10', 'L1_SingleIsoEG12', 'L1_SingleIsoEG15')
triggerHistManager.hltPaths = cms.vstring('HLT_IsoEle15_L1I')

elecTauHistManagers = cms.vstring( 'genPhaseSpaceEventInfoHistManager',
                                   'electronHistManager',
                                   'tauHistManager',
                                   'diTauCandidateHistManagerForElecTau',
                                   'metHistManager',
                                   'vertexHistManager',
                                   'triggerHistManager' )

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

# generator level selection of Z --> e + tau-jet events
# passing basic acceptance and kinematic cuts
# (NOTE: to be used for efficiency studies only !!)
#genElectronCut = cms.PSet(
#  name = cms.string('genElectronCut'),
#  type = cms.string('PATCandViewMinEventSelector'),
#  src = cms.InputTag('selectedGenTauDecaysToElectronPt15Cumulative'),
#  minNumber = cms.uint32(1)
#)
#genTauCut = cms.PSet(
#  name = cms.string('genTauCut'),
#  type = cms.string('PATCandViewMinEventSelector'),
#  src = cms.InputTag('selectedGenTauDecaysToHadronsPt20Cumulative'),
#  minNumber = cms.uint32(1)
#)

# trigger selection
#Trigger = cms.PSet(
#  name = cms.string('Trigger'),
#  type = cms.string('TriggerResultEventSelector'),
#  src = cms.InputTag('TriggerResults::HLT'),
#  triggerPaths = cms.vstring('HLT_IsoEle15_L1I')
#)

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
tightElectronIdCut = cms.PSet(
  name = cms.string('tightElectronIdCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1ElectronsTightIdCumulative'),
  src_individual = cms.InputTag('selectedLayer1ElectronsTightIdIndividual'),
  minNumber = cms.uint32(1)
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
electronTrkIPcut = cms.PSet(
  name = cms.string('electronTrkIPcut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1ElectronsTrkIPcumulative'),
  src_individual = cms.InputTag('selectedLayer1ElectronsTrkIPindividual'),
  minNumber = cms.uint32(1)
)

# tau candidate selection
tauAntiOverlapWithElectronsVeto = cms.PSet(
  name = cms.string('tauAntiOverlapWithElectronsVeto'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1TausForElecTauAntiOverlapWithElectronsVetoCumulative'),
  src_individual = cms.InputTag('selectedLayer1TausForElecTauAntiOverlapWithElectronsVetoIndividual'),
  minNumber = cms.uint32(1)
)
tauEtaCut = cms.PSet(
  name = cms.string('tauEtaCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1TausForElecTauEta21Cumulative'),
  src_individual = cms.InputTag('selectedLayer1TausEta21Individual'),
  minNumber = cms.uint32(1)
)
tauPtCut = cms.PSet(
  name = cms.string('tauPtCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1TausForElecTauPt20Cumulative'),
  src_individual = cms.InputTag('selectedLayer1TausPt20Individual'),
  minNumber = cms.uint32(1)
)
tauLeadTrkCut = cms.PSet(
  name = cms.string('tauLeadTrkCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1TausForElecTauLeadTrkCumulative'),
  src_individual = cms.InputTag('selectedLayer1TausLeadTrkIndividual'),
  minNumber = cms.uint32(1)
)
tauLeadTrkPtCut = cms.PSet(
  name = cms.string('tauLeadTrkPtCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1TausForElecTauLeadTrkPtCumulative'),
  src_individual = cms.InputTag('selectedLayer1TausLeadTrkPtIndividual'),
  minNumber = cms.uint32(1)
)
tauTrkIsoCut = cms.PSet(
  name = cms.string('tauTrkIsoCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1TausForElecTauTrkIsoCumulative'),
  src_individual = cms.InputTag('selectedLayer1TausTrkIsoIndividual'),
  minNumber = cms.uint32(1)
)
tauEcalIsoCut = cms.PSet(
  name = cms.string('tauEcalIsoCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1TausForElecTauEcalIsoCumulative'),
  src_individual = cms.InputTag('selectedLayer1TausEcalIsoIndividual'),
  minNumber = cms.uint32(1)
)
tauProngCut = cms.PSet(
  name = cms.string('tauProngCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1TausForElecTauProngCumulative'),
  src_individual = cms.InputTag('selectedLayer1TausProngIndividual'),
  minNumber = cms.uint32(1)
)
tauElectronVeto = cms.PSet(
  name = cms.string('tauElectronVeto'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedLayer1TausForElecTauElectronVetoCumulative'),
  src_individual = cms.InputTag('selectedLayer1TausElectronVetoIndividual'),
  minNumber = cms.uint32(1)
)

# di-tau candidate selection
diTauCandidateForElecTauAntiOverlapVeto = cms.PSet(
  name = cms.string('diTauCandidateForElecTauAntiOverlapVeto'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedElecTauPairsAntiOverlapVetoCumulative'),
  src_individual = cms.InputTag('selectedElecTauPairsAntiOverlapVetoIndividual'),
  minNumber = cms.uint32(1)
)
diTauCandidateForElecTauZeroChargeCut = cms.PSet(
  name = cms.string('diTauCandidateForElecTauZeroChargeCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedElecTauPairsZeroChargeCumulative'),
  src_individual = cms.InputTag('selectedElecTauPairsZeroChargeIndividual'),
  minNumber = cms.uint32(1)
)
diTauCandidateForElecTauMt1METCut = cms.PSet(
  name = cms.string('diTauCandidateForElecTauMt1METCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedElecTauPairsMt1METcumulative'),
  src_individual = cms.InputTag('selectedElecTauPairsMt1METindividual'),
  minNumber = cms.uint32(1)
)

# veto events containing additional central jets with Et > 20 GeV
#centralJetVeto = cms.PSet(
#  name = cms.string('centralJetVeto'),
#  type = cms.string('PATCandViewMaxEventSelector'),
#  src = cms.InputTag('selectedLayer1JetsEt20Cumulative'),
#  maxNumber = cms.uint32(0)
#)

#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------

elecTauEventDump = cms.PSet(
  name = cms.string('elecTauEventDump'),
  type = cms.string('ElecTauEventDump'),

  l1GtReadoutRecordSource = cms.InputTag('hltGtDigis::HLT'),
  l1GtObjectMapRecordSource = cms.InputTag('hltL1GtObjectMap::HLT'),
  l1BitsToPrint = cms.vstring('L1_SingleEG5', 'L1_SingleEG8', 'L1_SingleEG10', 'L1_SingleEG12', 'L1_SingleEG15',
                              'L1_SingleIsoEG5', 'L1_SingleIsoEG8', 'L1_SingleIsoEG10', 'L1_SingleIsoEG12', 'L1_SingleIsoEG15'),
    
  hltResultsSource = cms.InputTag('TriggerResults::HLT'),
  hltPathsToPrint = cms.vstring('HLT_IsoEle15_L1I'),

  genParticleSource = cms.InputTag('genParticles'),
  genTauJetSource = cms.InputTag('tauGenJets'),
  #electronSource = cms.InputTag('cleanLayer1ElectronsSel'),
  electronSource = cms.InputTag('selectedLayer1ElectronsTrkIPcumulative'),
  #tauSource = cms.InputTag('cleanLayer1TausSel'),
  tauSource = cms.InputTag('selectedLayer1TausForElecTauElectronVetoCumulative'),
  diTauCandidateSource = cms.InputTag('allElecTauPairs'),
  metSource = cms.InputTag('layer1METs'),
  genMEtSource = cms.InputTag('genMETWithMu'),
  jetSource = cms.InputTag('selectedLayer1JetsEt20Cumulative'),
  recoTrackSource = cms.InputTag('generalTracks'),
  pfChargedHadronSource = cms.InputTag('pfAllChargedHadrons'),
  pfGammaSource = cms.InputTag('pfAllPhotons'),
  pfNeutralHadronSource = cms.InputTag('pfAllNeutralHadrons'),

  #output = cms.string("elecTauEventDump.txt"),
  output = cms.string("std::cout"),

  #triggerConditions = cms.vstring("tauElectronVeto: passed_cumulative")
  triggerConditions = cms.vstring("tightElectronIdCut: rejected_cumulative")
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

  # generator level phase-space selection
  # (NOTE: to be used in case of Monte Carlo samples
  #        overlapping in simulated phase-space only !!)
  cms.PSet(
    filter = cms.string('genPhaseSpaceCut'),
    title = cms.string('gen. Phase-Space'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers
  ),

  # generator level selection of Z --> e + tau-jet events
  # passing basic acceptance and kinematic cuts
  # (NOTE: to be used for efficiency studies only !!)
  #cms.PSet(
  #  filter = cms.string('genElectronCut'),
  #  title = cms.string('gen. Electron'),
  #  saveRunEventNumbers = cms.vstring('')
  #),
  #cms.PSet(
  #  filter = cms.string('genTauCut'),
  #  title = cms.string('gen. Tau'),
  #  saveRunEventNumbers = cms.vstring('')
  #),
  #cms.PSet(
  #  histManagers = elecTauHistManagers
  #),
  
  # trigger selection
  #cms.PSet(
  #  filter = cms.string('Trigger'),
  #  title = cms.string('isoEle15 Trigger'),
  #  saveRunEventNumbers = cms.vstring('')
  #),
  #cms.PSet(
  #  histManagers = elecTauHistManagers
  #),

  # primary event vertex selection
  cms.PSet(
    filter = cms.string('primaryEventVertex'),
    title = cms.string('Vertex'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers
  ),
  cms.PSet(
    filter = cms.string('primaryEventVertexQuality'),
    title = cms.string('p(chi2Vertex) > 0.01'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers
  ),
  cms.PSet(
    filter = cms.string('primaryEventVertexPosition'),
    title = cms.string('-50 < zVertex < +50 cm'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers
  ),
  
  # selection of electron candidate
  # produced in electronic tau decay
  cms.PSet(
    filter = cms.string('tightElectronIdCut'),
    title = cms.string('tight Electron Id.'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTightIdCumulative')
  ),
  cms.PSet(
    filter = cms.string('electronAntiCrackCut'),
    title = cms.string('crack-Veto'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsAntiCrackCutCumulative')
  ),
  cms.PSet(
    filter = cms.string('electronEtaCut'),
    title = cms.string('-2.1 < eta(Electron) < +2.1'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsEta21Cumulative')
  ),
  cms.PSet(
    filter = cms.string('electronPtCut'),
    title = cms.string('Pt(Electron) > 15 GeV'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsPt15Cumulative')
  ),
  cms.PSet(
    filter = cms.string('electronTrkIsoCut'),
    title = cms.string('Electron Track iso.'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkIsoCumulative')
  ),
  cms.PSet(
    filter = cms.string('electronEcalIsoCut'),
    title = cms.string('Electron ECAL iso.'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsEcalIsoCumulative')
  ),
  cms.PSet(
    filter = cms.string('electronTrkCut'),
    title = cms.string('Electron Track find.'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkCumulative')
  ),
  cms.PSet(
    filter = cms.string('electronTrkIPcut'),
    title = cms.string('Electron Track IP'),
    saveRunEventNumbers = cms.vstring('')
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
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkIPcumulative',
                          'tauHistManager.tauSource = selectedLayer1TausForElecTauAntiOverlapWithElectronsVetoCumulative')
  ),
  cms.PSet(
    filter = cms.string('tauEtaCut'),
    title = cms.string('-2.1 < eta(Tau) < +2.1'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkIPcumulative',
                          'tauHistManager.tauSource = selectedLayer1TausForElecTauEta21Cumulative')
  ),
  cms.PSet(
    filter = cms.string('tauPtCut'),
    title = cms.string('Pt(Tau) > 20 GeV'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkIPcumulative',
                          'tauHistManager.tauSource = selectedLayer1TausForElecTauPt20Cumulative')
  ),
  cms.PSet(
    filter = cms.string('tauLeadTrkCut'),
    title = cms.string('Tau lead. Track find.'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkIPcumulative',
                          'tauHistManager.tauSource = selectedLayer1TausForElecTauLeadTrkCumulative')
  ),
  cms.PSet(
    filter = cms.string('tauLeadTrkPtCut'),
    title = cms.string('Tau lead. Track Pt'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkIPcumulative',
                          'tauHistManager.tauSource = selectedLayer1TausForElecTauLeadTrkPtCumulative')
  ),
  cms.PSet(
    filter = cms.string('tauTrkIsoCut'),
    title = cms.string('Tau Track iso.'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkIPcumulative',
                          'tauHistManager.tauSource = selectedLayer1TausForElecTauTrkIsoCumulative')
  ),
  cms.PSet(
    filter = cms.string('tauEcalIsoCut'),
    title = cms.string('Tau ECAL iso.'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkIPcumulative',
                          'tauHistManager.tauSource = selectedLayer1TausForElecTauEcalIsoCumulative')
  ),
  cms.PSet(
    filter = cms.string('tauProngCut'),
    title = cms.string('Tau 1||3-Prong'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkIPcumulative',
                          'tauHistManager.tauSource = selectedLayer1TausForElecTauProngCumulative')
  ),
  cms.PSet(
    filter = cms.string('tauElectronVeto'),
    title = cms.string('Tau e-Veto'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkIPcumulative',
                          'tauHistManager.tauSource = selectedLayer1TausForElecTauElectronVetoCumulative')
  ),

  #selection of electron + tau-jet combinations
  cms.PSet(
    filter = cms.string('diTauCandidateForElecTauAntiOverlapVeto'),
    title = cms.string('dR(Electron-Tau) > 0.7'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkIPcumulative',
                          'tauHistManager.tauSource = selectedLayer1TausForElecTauElectronVetoCumulative',
                          'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsAntiOverlapVetoCumulative')
  ),
  cms.PSet(
    filter = cms.string('diTauCandidateForElecTauZeroChargeCut'),
    title = cms.string('Charge(Electron+Tau) = 0'),
    saveRunEventNumbers = cms.vstring('passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkIPcumulative',
                          'tauHistManager.tauSource = selectedLayer1TausForElecTauElectronVetoCumulative',
                          'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsZeroChargeCumulative')
  ),
  cms.PSet(
    filter = cms.string('diTauCandidateForElecTauMt1METCut'),
    title = cms.string('M_{T}(Electron-MET) < 60 GeV'),
    saveRunEventNumbers = cms.vstring('passed_cumulative')
  ),
  cms.PSet(
    histManagers = elecTauHistManagers,
    replace = cms.vstring('electronHistManager.electronSource = selectedLayer1MuonsTrkIPcumulative',
                          'tauHistManager.tauSource = selectedLayer1TausForElecTauMuonVetoCumulative',
                          'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsMt1METcumulative')  
  #),

  # veto events containing additional central jets with Et > 20 GeV
  #cms.PSet(
  #  filter = cms.string('centralJetVeto'),
  #  title = cms.string('central Jet Veto'),
  #  saveRunEventNumbers = cms.vstring('passed_cumulative')
  #),
  #cms.PSet(
  #  histManagers = elecTauHistManagers,
  #  replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkIPcumulative',
  #                        'tauHistManager.tauSource = selectedLayer1TausForElecTauElectronVetoCumulative',
  #                        'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsZeroChargeCumulative')
  )
)
