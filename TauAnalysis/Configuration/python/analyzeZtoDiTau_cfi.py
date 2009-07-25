import FWCore.ParameterSet.Config as cms
import copy

# import config for histogram manager filling information about phase-space simulated in Monte Carlo sample
from TauAnalysis.Core.genPhaseSpaceEventInfoHistManager_cfi import *

# import config for tau-jet histogram manager;
# make two copies: one for the "first" (higher Pt) tau-jet
#             and  one for the "second" (lower Pt) tau-jet
from TauAnalysis.Core.pftauHistManager_cfi import *
tauHistManager1 = copy.deepcopy(tauHistManager)
tauHistManager1.pluginName = cms.string('tauHistManager1')
tauHistManager1.tauIndicesToPlot = cms.string('0')
tauHistManager1.dqmDirectory_store = cms.string('TauQuantities1')
tauHistManager2 = copy.deepcopy(tauHistManager)
tauHistManager2.pluginName = cms.string('tauHistManager2')
tauHistManager2.tauIndicesToPlot = cms.string('1')
tauHistManager2.dqmDirectory_store = cms.string('TauQuantities2')

# import config for di-tau histogram manager
from TauAnalysis.Core.diTauCandidateHistManager_cfi import *
diTauCandidateHistManagerForDiTau = copy.deepcopy(diTauCandidateHistManager)
diTauCandidateHistManagerForDiTau.pluginName = cms.string('diTauCandidateHistManagerForDiTau')
diTauCandidateHistManagerForDiTau.pluginType = cms.string('PATDiTauPairHistManager')
diTauCandidateHistManagerForDiTau.diTauCandidateSource = cms.InputTag('allDiTauPairs')

# import config for missing-Et histogram manager
from TauAnalysis.Core.metHistManager_cfi import *

# import config for central jet veto histogram manager
from TauAnalysis.Core.jetHistManager_cfi import *

# import config for primary event vertex histogram manager
from TauAnalysis.Core.vertexHistManager_cfi import *

# import config for L1 & HLT histogram manager
from TauAnalysis.Core.triggerHistManager_cfi import *
triggerHistManager.l1Bits = cms.vstring(
    'L1_SingleEG10',
    'L1_SingleEG12',
    'L1_SingleEG15',
    'L1_SingleEG20',
    'L1_SingleEG25',
    'L1_SingleTauJet20',
    'L1_SingleTauJet30',
    'L1_SingleTauJet40',
    'L1_SingleTauJet60',  
    'L1_DoubleTauJet20',
    'L1_DoubleTauJet30',
    'L1_DoubleTauJet35',
    'L1_DoubleTauJet40',
    'L1_TauJet30_ETM30',
    'L1_TauJet30_ETM40',
    'L1_ETM20',
    'L1_ETM30',
    'L1_ETM40',
    'L1_ETM50',
    'L1_ETM60'
)
triggerHistManager.hltPaths = cms.vstring(
    'HLT_IsoTau_MET65_Trk20',
    'HLT_IsoTau_MET35_Trk15_L1MET',
    'HLT_DoubleIsoTau_Trk3',
    'HLT_MET25',
    'HLT_MET35',
    'HLT_MET50',
    'HLT_MET65',
    'HLT_MET75'
)

diTauHistManagers = cms.vstring(
    'genPhaseSpaceEventInfoHistManager',
    'tauHistManager1',
    'tauHistManager2',
    'diTauCandidateHistManagerForDiTau',
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
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

# generator level selection of pure hadronic Z --> tau-jet + tau-jet events
# passing basic acceptance and kinematic cuts
# (NOTE: to be used for efficiency studies only !!)
#genDiTauCut = cms.PSet(
#    pluginName = cms.string('genTauCut'),
#    pluginType = cms.string('PATCandViewMinEventSelector'),
#    src = cms.InputTag('selectedGenTauDecaysToHadronsPt20Cumulative'),
#    minNumber = cms.uint32(2)
#)

# trigger selection
#evtSelTrigger = cms.PSet(
#    pluginName = cms.string('evtSelTrigger'),
#    pluginType = cms.string('BoolEventSelector'),
#    src = cms.InputTag('Trigger')
#)

# primary event vertex selection
evtSelPrimaryEventVertex = cms.PSet(
    pluginName = cms.string('evtSelPrimaryEventVertex'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('primaryEventVertex')

)
evtSelPrimaryEventVertexQuality = cms.PSet(
    pluginName = cms.string('evtSelPrimaryEventVertexQuality'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('primaryEventVertexQuality')
)
evtSelPrimaryEventVertexPosition = cms.PSet(
    pluginName = cms.string('evtSelPrimaryEventVertexPosition'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('primaryEventVertexPosition')
)

# acceptance cuts on first tau-jet candidate
evtSelFirstTauEta = cms.PSet(
    pluginName = cms.string('evtSelFirstTauEta'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('firstTauEtaCut', 'cumulative'),
    src_individual = cms.InputTag('firstTauEtaCut', 'individual')
)
evtSelFirstTauPt = cms.PSet(
    pluginName = cms.string('evtSelFirstTauPt'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('firstTauPtCut', 'cumulative'),
    src_individual = cms.InputTag('firstTauPtCut', 'individual')
)

# acceptance cuts on second tau-jet candidate
evtSelSecondTauEta = cms.PSet(
    pluginName = cms.string('evtSelSecondTauEta'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('secondTauEtaCut', 'cumulative'),
    src_individual = cms.InputTag('secondTauEtaCut', 'individual')
)
evtSelSecondTauPt = cms.PSet(
    pluginName = cms.string('evtSelSecondTauPt'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('secondTauPtCut', 'cumulative'),
    src_individual = cms.InputTag('secondTauPtCut', 'individual')
)

# (id.) selection of first tau-jet candidate
evtSelFirstTauLeadTrk = cms.PSet(
    pluginName = cms.string('evtSelFirstTauLeadTrk'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('firstTauLeadTrkCut', 'cumulative'),
    src_individual = cms.InputTag('firstTauLeadTrkCut', 'individual')
)
evtSelFirstTauLeadTrkPt = cms.PSet(
    pluginName = cms.string('evtSelFirstTauLeadTrkPt'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('firstTauLeadTrkPtCut', 'cumulative'),
    src_individual = cms.InputTag('firstTauLeadTrkPtCut', 'individual')
)
evtSelFirstTauTrkIso = cms.PSet(
    pluginName = cms.string('evtSelFirstTauTrkIso'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('firstTauTrkIsoCut', 'cumulative'),
    src_individual = cms.InputTag('firstTauTrkIsoCut', 'individual')
)
evtSelFirstTauEcalIso = cms.PSet(
    pluginName = cms.string('evtSelFirstTauEcalIso'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('firstTauEcalIsoCut', 'cumulative'),
    src_individual = cms.InputTag('firstTauEcalIsoCut', 'individual')
)
evtSelFirstTauProng = cms.PSet(
    pluginName = cms.string('evtSelFirstTauProng'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('firstTauProngCut', 'cumulative'),
    src_individual = cms.InputTag('firstTauProngCut', 'individual')
)

# (id.) selection of second tau-jet candidate
evtSelSecondTauLeadTrk = cms.PSet(
    pluginName = cms.string('evtSelSecondTauLeadTrk'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('secondTauLeadTrkCut', 'cumulative'),
    src_individual = cms.InputTag('secondTauLeadTrkCut', 'individual')
)
evtSelSecondTauLeadTrkPt = cms.PSet(
    pluginName = cms.string('evtSelSecondTauLeadTrkPt'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('secondTauLeadTrkPtCut', 'cumulative'),
    src_individual = cms.InputTag('secondTauLeadTrkPtCut', 'individual')
)
evtSelSecondTauTrkIso = cms.PSet(
    pluginName = cms.string('evtSelSecondTauTrkIso'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('secondTauTrkIsoCut', 'cumulative'),
    src_individual = cms.InputTag('secondTauTrkIsoCut', 'individual')
)
evtSelSecondTauEcalIso = cms.PSet(
    pluginName = cms.string('evtSelSecondTauEcalIso'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('secondTauEcalIsoCut', 'cumulative'),
    src_individual = cms.InputTag('secondTauEcalIsoCut', 'individual')
)
evtSelSecondTauProng = cms.PSet(
    pluginName = cms.string('evtSelSecondTauProng'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('secondTauProngCut', 'cumulative'),
    src_individual = cms.InputTag('secondTauProngCut', 'individual')
)

# di-tau candidate selection
evtSelDiTauCandidateForDiTauAntiOverlapVeto = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForDiTauAntiOverlapVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForDiTauAntiOverlapVeto', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForDiTauAntiOverlapVeto', 'individual')
)
evtSelDiTauCandidateForDiTauAcoplanarity = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForDiTauAcoplanarity'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForDiTauAcoplanarityCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForDiTauAcoplanarityCut', 'individual')
)
evtSelDiTauCandidateForDiTauZeroCharge = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForDiTauZeroCharge'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForDiTauZeroChargeCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForDiTauZeroChargeCut', 'individual')
)

# veto events containing additional central jets with Et > 20 GeV
#evtSelCentralJetVeto = cms.PSet(
#    pluginName = cms.string('evtSelCentralJetVeto'),
#    pluginType = cms.string('BoolEventSelector'),
#    src_cumulative = cms.InputTag('centralJetVeto', 'cumulative'),
#    src_individual = cms.InputTag('centralJetVeto', 'individual')
#)

#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------

diTauEventDump = cms.PSet(
    pluginName = cms.string('diTauEventDump'),
    pluginType = cms.string('DiTauEventDump'),

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
    
    triggerConditions = cms.vstring("evtSelDiTauCandidateForDiTauZeroCharge: passed_cumulative")
)

#--------------------------------------------------------------------------------
# define analysis sequence
# (ordered list of event selection criteria and histogram filling)
#--------------------------------------------------------------------------------

diTauAnalysisSequence = cms.VPSet(
    # fill histograms for full event sample
    cms.PSet(
    analyzers = diTauHistManagers
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
        analyzers = diTauHistManagers
    ),

    # generator level selection of Z --> mu + tau-jet events
    # passing basic acceptance and kinematic cuts
    # (NOTE: to be used for efficiency studies only !!)
    #cms.PSet(
    #    filter = cms.string('genDiTauCut'),
    #    title = cms.string('gen. Tau Pair'),
    #    saveRunEventNumbers = cms.vstring('')
    #),
    #cms.PSet(
    #    analyzers = diTauHistManagers
    #),
    
    # trigger selection
    #cms.PSet(
    #    filter = cms.string('evtSelTrigger'),
    #    title = cms.string('Trigger'),
    #    saveRunEventNumbers = cms.vstring('')
    #),
    #cms.PSet(
    #    analyzers = diTauHistManagers
    #),

    # primary event vertex selection
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertex'),
        title = cms.string('Vertex'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = diTauHistManagers
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexQuality'),
        title = cms.string('p(chi2Vertex) > 0.01'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = diTauHistManagers
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexPosition'),
        title = cms.string('-25 < zVertex < +25 cm'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = diTauHistManagers
    ),

    # acceptance cuts on first tau-jet
    cms.PSet(
        filter = cms.string('evtSelFirstTauEta'),
        title = cms.string('-2.1 < eta(1.Tau) < +2.1'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = diTauHistManagers,
        replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauEta21Cumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelFirstTauPt'),
        title = cms.string('Pt(1.Tau) > 20 GeV'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = diTauHistManagers,
        replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauPt20Cumulative')
    ),

    # acceptance cuts on second tau-jet
    cms.PSet(
        filter = cms.string('evtSelSecondTauEta'),
        title = cms.string('-2.1 < eta(2.Tau) < +2.1'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = diTauHistManagers,
        replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauPt20Cumulative',
                              'tauHistManager2.tauSource = selectedLayer1TausForDiTauEta21Cumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelSecondTauPt'),
        title = cms.string('Pt(2.Tau) > 20 GeV'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = diTauHistManagers,
        replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauPt20Cumulative',
                              'tauHistManager2.tauSource = selectedLayer1TausForDiTauPt20Cumulative')
    ),
    
    # selection of first tau-jet candidate (id.)
    cms.PSet(
        filter = cms.string('evtSelFirstTauLeadTrk'),
        title = cms.string('1.Tau lead. Track find.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = diTauHistManagers,
        replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauLeadTrkCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelFirstTauLeadTrkPt'),
        title = cms.string('1.Tau lead. Track Pt'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = diTauHistManagers,
        replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauLeadTrkPtCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelFirstTauTrkIso'),
        title = cms.string('1.Tau Track iso.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = diTauHistManagers,
        replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauTrkIsoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelFirstTauEcalIso'),
        title = cms.string('1.Tau ECAL iso.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = diTauHistManagers,
        replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauEcalIsoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelFirstTauProng'),
        title = cms.string('1.Tau 1||3-Prong'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = diTauHistManagers,
        replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauProngCumulative')
    ),

    # selection of second tau-jet candidate (id.)
    cms.PSet(
        filter = cms.string('evtSelSecondTauLeadTrk'),
        title = cms.string('2.Tau lead. Track find.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = diTauHistManagers,
        replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauProngCumulative',
                              'tauHistManager2.tauSource = selectedLayer1TausForDiTauLeadTrkCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelSecondTauLeadTrkPt'),
        title = cms.string('2.Tau lead. Track Pt'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = diTauHistManagers,
        replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauProngCumulative',
                              'tauHistManager2.tauSource = selectedLayer1TausForDiTauLeadTrkPtCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelSecondTauTrkIso'),
        title = cms.string('2.Tau Track iso.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = diTauHistManagers,
        replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauProngCumulative',
                              'tauHistManager2.tauSource = selectedLayer1TausForDiTauTrkIsoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelSecondTauEcalIso'),
        title = cms.string('2.Tau ECAL iso.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = diTauHistManagers,
        replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauProngCumulative',
                              'tauHistManager2.tauSource = selectedLayer1TausForDiTauEcalIsoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelSecondTauProng'),
        title = cms.string('2.Tau 1||3-Prong'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = diTauHistManagers,
        replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauProngCumulative',
                              'tauHistManager2.tauSource = selectedLayer1TausForDiTauProngCumulative')
    ),

    #selection of tau-jet + tau-jet combinations
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForDiTauAntiOverlapVeto'),
        title = cms.string('dR(1.Tau-2.Tau) > 0.7'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = diTauHistManagers,
        replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauProngCumulative',
                              'tauHistManager2.tauSource = selectedLayer1TausForDiTauProngCumulative',
                              'diTauCandidateHistManagerForDiTau.diTauCandidateSource = selectedDiTauPairsAntiOverlapVetoCumulative')
    ),  
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForDiTauAcoplanarity'),
        title = cms.string('dPhi(1.Tau-MET) < 2.4 || dPhi(2.Tau-MET) < 2.4'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = diTauHistManagers,
        replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauProngCumulative',
                              'tauHistManager2.tauSource = selectedLayer1TausForDiTauProngCumulative',
                              'diTauCandidateHistManagerForDiTau.diTauCandidateSource = selectedDiTauPairsAcoplanarityCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForDiTauZeroCharge'),
        title = cms.string('Charge(1.Tau+2.Tau) = 0'),
        saveRunEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = diTauHistManagers,
        replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauProngCumulative',
                              'tauHistManager2.tauSource = selectedLayer1TausForDiTauProngCumulative',
                              'diTauCandidateHistManagerForDiTau.diTauCandidateSource = selectedDiTauPairsZeroChargeCumulative')
    #),

    # veto events containing additional central jets with Et > 20 GeV
    #cms.PSet(
    #    filter = cms.string('evtSelCentralJetVeto'),
    #    title = cms.string('central Jet Veto'),
    #    saveRunEventNumbers = cms.vstring('passed_cumulative')
    #),
    #cms.PSet(
    #    analyzers = muTauHistManagers,
    #    replace = cms.vstring('tauHistManager1.tauSource = selectedLayer1TausForDiTauProngCumulative',
    #                          'tauHistManager2.tauSource = selectedLayer1TausForDiTauProngCumulative',
    #                          'diTauCandidateHistManagerForDiTau.diTauCandidateSource = selectedDiTauPairsZeroChargeCumulative')
  )
)
