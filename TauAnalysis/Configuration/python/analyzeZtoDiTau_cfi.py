import FWCore.ParameterSet.Config as cms
import copy

# import config for histogram manager filling information about phase-space simulated in Monte Carlo sample
from TauAnalysis.Core.genPhaseSpaceEventInfoHistManager_cfi import *

# import config for tau-jet histogram manager;
# make two copies: one for the "first" (higher Pt) tau-jet
#             and  one for the "second" (lower Pt) tau-jet
from TauAnalysis.Core.pftauHistManager_cfi import *
tauHistManager1 = copy.deepcopy(tauHistManager)
tauHistManager1.name = cms.string('tauHistManager1')
tauHistManager1.tauIndicesToPlot = cms.string('0')
tauHistManager1.dqmDirectory_store = cms.string('TauQuantities1')
tauHistManager2 = copy.deepcopy(tauHistManager)
tauHistManager2.name = cms.string('tauHistManager2')
tauHistManager2.tauIndicesToPlot = cms.string('1')
tauHistManager2.dqmDirectory_store = cms.string('TauQuantities2')

# import config for di-tau histogram manager
from TauAnalysis.Core.diTauCandidateHistManager_cfi import *
diTauCandidateHistManagerForDiTau = copy.deepcopy(diTauCandidateHistManager)
diTauCandidateHistManagerForDiTau.name = cms.string('diTauCandidateHistManagerForDiTau')
diTauCandidateHistManagerForDiTau.type = cms.string('PATDiTauPairHistManager')
diTauCandidateHistManagerForDiTau.diTauCandidateSource = cms.InputTag('allDiTauPairs')

# import config for missing-Et histogram manager
from TauAnalysis.Core.metHistManager_cfi import *

# import config for central jet veto histogram manager
from TauAnalysis.Core.jetHistManager_cfi import *

# import config for primary event vertex histogram manager
from TauAnalysis.Core.vertexHistManager_cfi import *

# import config for L1 & HLT histogram manager
from TauAnalysis.Core.triggerHistManager_cfi import *
triggerHistManager.l1Bits = cms.vstring('L1_SingleEG10', 'L1_SingleEG12', 'L1_SingleEG15', 'L1_SingleEG20', 'L1_SingleEG25',
                                        'L1_SingleTauJet20', 'L1_SingleTauJet30', 'L1_SingleTauJet40', 'L1_SingleTauJet60',  
                                        'L1_DoubleTauJet20', 'L1_DoubleTauJet30', 'L1_DoubleTauJet35', 'L1_DoubleTauJet40',
                                        'L1_TauJet30_ETM30', 'L1_TauJet30_ETM40',
                                        'L1_ETM20', 'L1_ETM30', 'L1_ETM40', 'L1_ETM50', 'L1_ETM60')
triggerHistManager.hltPaths = cms.vstring('HLT_IsoTau_MET65_Trk20', 'HLT_IsoTau_MET35_Trk15_L1MET',
                                          'HLT_DoubleIsoTau_Trk3',
                                          'HLT_MET25', 'HLT_MET35', 'HLT_MET50', 'HLT_MET65', 'HLT_MET75')

diTauHistManagers = cms.vstring( 'genPhaseSpaceEventInfoHistManager',
                                 'tauHistManager1',
                                 'tauHistManager2',
                                 'diTauCandidateHistManagerForDiTau',
                                 'metHistManager',
                                 'jetHistManager',
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

# generator level selection of pure hadronic Z --> tau-jet + tau-jet events
# passing basic acceptance and kinematic cuts
# (NOTE: to be used for efficiency studies only !!)
#genDiTauCut = cms.PSet(
#  name = cms.string('genTauCut'),
#  type = cms.string('PATCandViewMinEventSelector'),
#  src = cms.InputTag('selectedGenTauDecaysToHadronsPt20Cumulative'),
#  minNumber = cms.uint32(2)
#)

# trigger selection
#Trigger = cms.PSet(
#  name = cms.string('Trigger'),
#  type = cms.string('TriggerResultEventSelector'),
#  src = cms.InputTag('TriggerResults::HLT'),
#  triggerPaths = cms.vstring('HLT_IsoTau_MET65_Trk20', 'HLT_IsoTau_MET35_Trk15_L1MET', 'HLT_DoubleIsoTau_Trk3')
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

# selection of first tau-jet candidate
firstTauEtaCut = cms.PSet(
  name = cms.string('firstTauEtaCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedDiTauPairs1stTauEta21Cumulative'),
  src_individual = cms.InputTag('selectedDiTauPairs1stTauEta21Individual'),
  minNumber = cms.uint32(1)
)
firstTauPtCut = cms.PSet(
  name = cms.string('firstTauPtCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedDiTauPairs1stTauPt20Cumulative'),
  src_individual = cms.InputTag('selectedDiTauPairs1stTauPt20Individual'),
  minNumber = cms.uint32(1)
)
firstTauLeadTrkCut = cms.PSet(
  name = cms.string('firstTauLeadTrkCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedDiTauPairs1stTauLeadTrkCumulative'),
  src_individual = cms.InputTag('selectedDiTauPairs1stTauLeadTrkIndividual'),
  minNumber = cms.uint32(1)
)
firstTauLeadTrkPtCut = cms.PSet(
  name = cms.string('firstTauLeadTrkPtCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedDiTauPairs1stTauLeadTrkPtCumulative'),
  src_individual = cms.InputTag('selectedDiTauPairs1stTauLeadTrkPtIndividual'),
  minNumber = cms.uint32(1)
)
firstTauTrkIsoCut = cms.PSet(
  name = cms.string('firstTauTrkIsoCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedDiTauPairs1stTauTrkIsoCumulative'),
  src_individual = cms.InputTag('selectedDiTauPairs1stTauTrkIsoIndividual'),
  minNumber = cms.uint32(1)
)
firstTauEcalIsoCut = cms.PSet(
  name = cms.string('firstTauEcalIsoCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedDiTauPairs1stTauEcalIsoCumulative'),
  src_individual = cms.InputTag('selectedDiTauPairs1stTauEcalIsoIndividual'),
  minNumber = cms.uint32(1)
)
firstTauProngCut = cms.PSet(
  name = cms.string('firstTauProngCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedDiTauPairs1stTauProngCumulative'),
  src_individual = cms.InputTag('selectedDiTauPairs1stTauProngIndividual'),
  minNumber = cms.uint32(1)
)

# selection of second tau-jet candidate
secondTauEtaCut = cms.PSet(
  name = cms.string('secondTauEtaCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedDiTauPairs2ndTauEta21Cumulative'),
  src_individual = cms.InputTag('selectedDiTauPairs2ndTauEta21Individual'),
  minNumber = cms.uint32(1)
)
secondTauPtCut = cms.PSet(
  name = cms.string('secondTauPtCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedDiTauPairs2ndTauPt20Cumulative'),
  src_individual = cms.InputTag('selectedDiTauPairs2ndTauPt20Individual'),
  minNumber = cms.uint32(1)
)
secondTauLeadTrkCut = cms.PSet(
  name = cms.string('secondTauLeadTrkCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedDiTauPairs2ndTauLeadTrkCumulative'),
  src_individual = cms.InputTag('selectedDiTauPairs2ndTauLeadTrkIndividual'),
  minNumber = cms.uint32(1)
)
secondTauLeadTrkPtCut = cms.PSet(
  name = cms.string('secondTauLeadTrkPtCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedDiTauPairs2ndTauLeadTrkPtCumulative'),
  src_individual = cms.InputTag('selectedDiTauPairs2ndTauLeadTrkPtIndividual'),
  minNumber = cms.uint32(1)
)
secondTauTrkIsoCut = cms.PSet(
  name = cms.string('secondTauTrkIsoCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedDiTauPairs2ndTauTrkIsoCumulative'),
  src_individual = cms.InputTag('selectedDiTauPairs2ndTauTrkIsoIndividual'),
  minNumber = cms.uint32(1)
)
secondTauEcalIsoCut = cms.PSet(
  name = cms.string('secondTauEcalIsoCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedDiTauPairs2ndTauEcalIsoCumulative'),
  src_individual = cms.InputTag('selectedDiTauPairs2ndTauEcalIsoIndividual'),
  minNumber = cms.uint32(1)
)
secondTauProngCut = cms.PSet(
  name = cms.string('secondTauProngCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedDiTauPairs2ndTauProngCumulative'),
  src_individual = cms.InputTag('selectedDiTauPairs2ndTauProngIndividual'),
  minNumber = cms.uint32(1)
)

# di-tau candidate selection
diTauCandidateForDiTauAntiOverlapVeto = cms.PSet(
  name = cms.string('diTauCandidateForDiTauAntiOverlapVeto'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedDiTauPairsAntiOverlapVetoCumulative'),
  src_individual = cms.InputTag('selectedDiTauPairsAntiOverlapVetoIndividual'),
  minNumber = cms.uint32(1)
)
diTauCandidateForDiTauAcoplanarityCut = cms.PSet(
  name = cms.string('diTauCandidateForDiTauAcoplanarityCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedDiTauPairsAcoplanarityCumulative'),
  src_individual = cms.InputTag('selectedDiTauPairsAcoplanarityIndividual'),
  minNumber = cms.uint32(1)
)
diTauCandidateForDiTauZeroChargeCut = cms.PSet(
  name = cms.string('diTauCandidateForDiTauZeroChargeCut'),
  type = cms.string('PATCandViewMinEventSelector'),
  src_cumulative = cms.InputTag('selectedDiTauPairsZeroChargeCumulative'),
  src_individual = cms.InputTag('selectedDiTauPairsZeroChargeIndividual'),
  minNumber = cms.uint32(1)
)

# veto events containing additional central jets with Et > 20 GeV
centralJetVeto = cms.PSet(
  name = cms.string('centralJetVeto'),
  type = cms.string('PATCandViewMaxEventSelector'),
  src = cms.InputTag('selectedLayer1JetsEt20Cumulative'),
  maxNumber = cms.uint32(0)
)

#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------

diTauEventDump = cms.PSet(
  name = cms.string('diTauEventDump'),
  type = cms.string('DiTauEventDump'),

  l1GtReadoutRecordSource = cms.InputTag('hltGtDigis::HLT'),
  l1GtObjectMapRecordSource = cms.InputTag('hltL1GtObjectMap::HLT'),
  l1BitsToPrint = cms.vstring('L1_SingleTauJet40', 'L1_SingleTauJet60',  
                              'L1_DoubleTauJet20', 'L1_DoubleTauJet30', 'L1_DoubleTauJet40',
                              'L1_TauJet30_ETM30', 'L1_TauJet30_ETM40'),

  hltResultsSource = cms.InputTag('TriggerResults::HLT'),
  hltPathsToPrint = cms.vstring('HLT_IsoTau_MET65_Trk20', 'HLT_IsoTau_MET35_Trk15_L1MET', 'HLT_DoubleIsoTau_Trk3'),
      
  genParticleSource = cms.InputTag('genParticles'),
  genTauJetSource = cms.InputTag('tauGenJets'),
  electronSource = cms.InputTag('cleanLayer1ElectronsSel'),
  muonSource = cms.InputTag('cleanLayer1MuonsSel'),
  tauSource = cms.InputTag('cleanLayer1TausSel'),
  diTauCandidateSource = cms.InputTag('allDiTauPairs'),
  metSource = cms.InputTag('layer1METs'),
  genMEtSource = cms.InputTag('genMETWithMu'),
  jetSource = cms.InputTag('selectedLayer1JetsEt20Cumulative'),
  recoTrackSource = cms.InputTag('generalTracks'),
  pfChargedHadronSource = cms.InputTag('pfAllChargedHadrons'),
  pfGammaSource = cms.InputTag('pfAllPhotons'),
  pfNeutralHadronSource = cms.InputTag('pfAllNeutralHadrons'),

  #output = cms.string("diTauEventDump.txt"),
  output = cms.string("std::cout"),

  triggerConditions = cms.vstring("diTauCandidateForDiTauZeroChargeCut: passed_cumulative")
)

#--------------------------------------------------------------------------------
# define analysis sequence
# (ordered list of event selection criteria and histogram filling)
#--------------------------------------------------------------------------------

diTauAnalysisSequence = cms.VPSet(
  # fill histograms for full event sample
  cms.PSet(
    histManagers = diTauHistManagers
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
    histManagers = diTauHistManagers
  ),

  # generator level selection of Z --> mu + tau-jet events
  # passing basic acceptance and kinematic cuts
  # (NOTE: to be used for efficiency studies only !!)
  #cms.PSet(
  #  filter = cms.string('genDiTauCut'),
  #  title = cms.string('gen. Tau Pair'),
  #  saveRunEventNumbers = cms.vstring('')
  #),
  #cms.PSet(
  #  histManagers = diTauHistManagers
  #),
    
  # trigger selection
  #cms.PSet(
  #  filter = cms.string('Trigger'),
  #  title = cms.string('Trigger'),
  #  saveRunEventNumbers = cms.vstring('')
  #),
  #cms.PSet(
  #  histManagers = diTauHistManagers
  #),

  # primary event vertex selection
  cms.PSet(
    filter = cms.string('primaryEventVertex'),
    title = cms.string('Vertex'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = diTauHistManagers
  ),
  cms.PSet(
    filter = cms.string('primaryEventVertexQuality'),
    title = cms.string('p(chi2Vertex) > 0.01'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = diTauHistManagers
  ),
  cms.PSet(
    filter = cms.string('primaryEventVertexPosition'),
    title = cms.string('-50 < zVertex < +50 cm'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = diTauHistManagers
  ),
    
  # selection of first tau-jet candidate
  cms.PSet(
    filter = cms.string('firstTauEtaCut'),
    title = cms.string('-2.1 < eta(1.Tau) < +2.1'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = diTauHistManagers,
    replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauEta21Cumulative')
  ),
  cms.PSet(
    filter = cms.string('firstTauPtCut'),
    title = cms.string('Pt(1.Tau) > 20 GeV'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = diTauHistManagers,
    replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauPt20Cumulative')
  ),
  cms.PSet(
    filter = cms.string('firstTauLeadTrkCut'),
    title = cms.string('1.Tau lead. Track find.'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = diTauHistManagers,
    replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauLeadTrkCumulative')
  ),
  cms.PSet(
    filter = cms.string('firstTauLeadTrkPtCut'),
    title = cms.string('1.Tau lead. Track Pt'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = diTauHistManagers,
    replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauLeadTrkPtCumulative')
  ),
  cms.PSet(
    filter = cms.string('firstTauTrkIsoCut'),
    title = cms.string('1.Tau Track iso.'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = diTauHistManagers,
    replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauTrkIsoCumulative')
  ),
  cms.PSet(
    filter = cms.string('firstTauEcalIsoCut'),
    title = cms.string('1.Tau ECAL iso.'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = diTauHistManagers,
    replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauEcalIsoCumulative')
  ),
  cms.PSet(
    filter = cms.string('firstTauProngCut'),
    title = cms.string('1.Tau 1||3-Prong'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = diTauHistManagers,
    replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauProngCumulative')
  ),

  # selection of second tau-jet candidate
  cms.PSet(
    filter = cms.string('secondTauEtaCut'),
    title = cms.string('-2.1 < eta(2.Tau) < +2.1'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = diTauHistManagers,
    replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauProngCumulative',
                          'tauHistManager2.tauSource = selectedLayer1TausForDiTauEta21Cumulative')
  ),
  cms.PSet(
    filter = cms.string('secondTauPtCut'),
    title = cms.string('Pt(2.Tau) > 20 GeV'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = diTauHistManagers,
    replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauProngCumulative',
                          'tauHistManager2.tauSource = selectedLayer1TausForDiTauPt20Cumulative')
  ),
  cms.PSet(
    filter = cms.string('secondTauLeadTrkCut'),
    title = cms.string('2.Tau lead. Track find.'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = diTauHistManagers,
    replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauProngCumulative',
                          'tauHistManager2.tauSource = selectedLayer1TausForDiTauLeadTrkCumulative')
  ),
  cms.PSet(
    filter = cms.string('secondTauLeadTrkPtCut'),
    title = cms.string('2.Tau lead. Track Pt'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = diTauHistManagers,
    replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauProngCumulative',
                          'tauHistManager2.tauSource = selectedLayer1TausForDiTauLeadTrkPtCumulative')
  ),
  cms.PSet(
    filter = cms.string('secondTauTrkIsoCut'),
    title = cms.string('2.Tau Track iso.'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = diTauHistManagers,
    replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauProngCumulative',
                          'tauHistManager2.tauSource = selectedLayer1TausForDiTauTrkIsoCumulative')
  ),
  cms.PSet(
    filter = cms.string('secondTauEcalIsoCut'),
    title = cms.string('2.Tau ECAL iso.'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = diTauHistManagers,
    replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauProngCumulative',
                          'tauHistManager2.tauSource = selectedLayer1TausForDiTauEcalIsoCumulative')
  ),
  cms.PSet(
    filter = cms.string('secondTauProngCut'),
    title = cms.string('2.Tau 1||3-Prong'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = diTauHistManagers,
    replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauProngCumulative',
                          'tauHistManager2.tauSource = selectedLayer1TausForDiTauProngCumulative')
  ),

  #selection of tau-jet + tau-jet combinations
  cms.PSet(
    filter = cms.string('diTauCandidateForDiTauAntiOverlapVeto'),
    title = cms.string('dR(1.Tau-2.Tau) > 0.7'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = diTauHistManagers,
    replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauProngCumulative',
                          'tauHistManager2.tauSource = selectedLayer1TausForDiTauProngCumulative',
                          'diTauCandidateHistManagerForDiTau.diTauCandidateSource = selectedDiTauPairsAntiOverlapVetoCumulative')
  ),  
  cms.PSet(
    filter = cms.string('diTauCandidateForDiTauAcoplanarityCut'),
    title = cms.string('dPhi(1.Tau-MET) < 2.4 || dPhi(2.Tau-MET) < 2.4'),
    saveRunEventNumbers = cms.vstring('')
  ),
  cms.PSet(
    histManagers = diTauHistManagers,
    replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauProngCumulative',
                          'tauHistManager2.tauSource = selectedLayer1TausForDiTauProngCumulative',
                          'diTauCandidateHistManagerForDiTau.diTauCandidateSource = selectedDiTauPairsAcoplanarityCumulative')
  ),
  cms.PSet(
    filter = cms.string('diTauCandidateForDiTauZeroChargeCut'),
    title = cms.string('Charge(1.Tau+2.Tau) = 0'),
    saveRunEventNumbers = cms.vstring('passed_cumulative')
  ),
  cms.PSet(
    histManagers = diTauHistManagers,
    replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauProngCumulative',
                          'tauHistManager2.tauSource = selectedLayer1TausForDiTauProngCumulative',
                          'diTauCandidateHistManagerForDiTau.diTauCandidateSource = selectedDiTauPairsZeroChargeCumulative')
  #),

  # veto events containing additional central jets with Et > 20 GeV
  #cms.PSet(
  #  filter = cms.string('centralJetVeto'),
  #  title = cms.string('central Jet Veto'),
  #  saveRunEventNumbers = cms.vstring('passed_cumulative')
  #),
  #cms.PSet(
  #  histManagers = muTauHistManagers,
  #  replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauProngCumulative',
  #                        'tauHistManager2.tauSource = selectedLayer1TausForDiTauProngCumulative',
  #                        'diTauCandidateHistManagerForDiTau.diTauCandidateSource = selectedDiTauPairsZeroChargeCumulative')
  )
)
