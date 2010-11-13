import FWCore.ParameterSet.Config as cms
import copy

# import config for histogram manager filling information about phase-space simulated in Monte Carlo sample
from TauAnalysis.Core.genPhaseSpaceEventInfoHistManager_cfi import *

# import config for event weight histogram manager
from TauAnalysis.Core.eventWeightHistManager_cfi import *

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

from TauAnalysis.Core.diTauCandidateCollinearApproxHistManager_cfi import *
diTauCandidateCollinearApproxHistManagerForDiTau = copy.deepcopy(diTauCandidateCollinearApproxHistManager)
diTauCandidateCollinearApproxHistManagerForDiTau.pluginName = cms.string('diTauCandidateCollinearApproxHistManagerForDiTau')
diTauCandidateCollinearApproxHistManagerForDiTau.pluginType = cms.string('PATDiTauPairCollinearApproxHistManager')
diTauCandidateCollinearApproxHistManagerForDiTau.diTauCandidateSource = cms.InputTag('selectedDiTauPairsPzetaDiffCumulative')

from TauAnalysis.Core.diTauCandidateSVfitHistManager_cfi import *
diTauCandidateSVfitHistManagerForDiTau = copy.deepcopy(diTauCandidateSVfitHistManager)
diTauCandidateSVfitHistManagerForDiTau.pluginName = cms.string('diTauCandidateSVfitHistManagerForDiTau')
diTauCandidateSVfitHistManagerForDiTau.pluginType = cms.string('PATDiTauPairSVfitHistManager')
diTauCandidateSVfitHistManagerForDiTau.diTauCandidateSource = cms.InputTag('selectedDiTauPairsPzetaDiffCumulative')

# import config for missing-Et histogram managers
from TauAnalysis.Core.caloMEtHistManager_cfi import *
from TauAnalysis.Core.pfMEtHistManager_cfi import *

# import config for central jet veto histogram manager
from TauAnalysis.Core.jetHistManager_cfi import *
jetHistManagerForDiTau = copy.deepcopy(jetHistManager)
jetHistManagerForDiTau.pluginName = cms.string('jetHistManagerForDiTau')
jetHistManagerForDiTau.jetSource = cms.InputTag('selectedPatJetsForZtoDiTauAntiOverlapWithLeptonsVetoCumulative')

# import config for primary event vertex histogram manager
from TauAnalysis.Core.vertexHistManager_cfi import *

# import config for L1 & HLT histogram manager
from TauAnalysis.Core.triggerHistManager_cfi import *
triggerHistManagerForDiTau = copy.deepcopy(triggerHistManager)
triggerHistManagerForDiTau.pluginName = cms.string('triggerHistManagerForDiTau')
triggerHistManagerForDiTau.l1Bits = cms.vstring(
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
triggerHistManagerForDiTau.hltPaths = cms.vstring(
    ##'HLT_IsoTau_MET65_Trk20',
    ##'HLT_IsoTau_MET35_Trk15_L1MET',
    ##'HLT_DoubleIsoTau_Trk3',
    'HLT_MET25',
    'HLT_MET35',
    'HLT_MET50',
    'HLT_MET65',
    'HLT_MET75'
)

#--------------------------------------------------------------------------------
# define event selection criteria
#--------------------------------------------------------------------------------

# generator level phase-space selection
# (NOTE: to be used in case of Monte Carlo samples
#        overlapping in simulated phase-space only !!)
evtSelGenPhaseSpace = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('genPhaseSpaceCut')
)

# trigger selection
evtSelTrigger = cms.PSet(
    pluginName = cms.string('evtSelTrigger'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('Trigger')
)

# data-quality selection
evtSelDataQuality = cms.PSet(
    pluginName = cms.string('evtSelDataQuality'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('dataQualityCutsPassed')
)

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
evtSelFirstTauTaNCdiscr = cms.PSet(
    pluginName = cms.string('evtSelFirstTauTaNCdiscr'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('firstTauTaNCdiscrCut', 'cumulative'),
    src_individual = cms.InputTag('firstTauTaNCdiscrCut', 'individual')
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
evtSelFirstTauCharge = cms.PSet(
    pluginName = cms.string('evtSelFirstTauCharge'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('firstTauChargeCut', 'cumulative'),
    src_individual = cms.InputTag('firstTauChargeCut', 'individual')
)
evtSelFirstTauMuonVeto = cms.PSet(
    pluginName = cms.string('evtSelFirstTauMuonVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('firstTauMuonVeto', 'cumulative'),
    src_individual = cms.InputTag('firstTauMuonVeto', 'individual')
)
evtSelFirstTauElectronVeto = cms.PSet(
    pluginName = cms.string('evtSelFirstTauElectronVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('firstTauElectronVeto', 'cumulative'),
    src_individual = cms.InputTag('firstTauElectronVeto', 'individual')
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
evtSelSecondTauTaNCdiscr = cms.PSet(
    pluginName = cms.string('evtSelSecondTauTaNCdiscr'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('secondTauTaNCdiscrCut', 'cumulative'),
    src_individual = cms.InputTag('secondTauTaNCdiscrCut', 'individual')
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
evtSelSecondTauCharge = cms.PSet(
    pluginName = cms.string('evtSelSecondTauCharge'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('secondTauChargeCut', 'cumulative'),
    src_individual = cms.InputTag('secondTauChargeCut', 'individual')
)
evtSelSecondTauMuonVeto = cms.PSet(
    pluginName = cms.string('evtSelSecondTauMuonVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('secondTauMuonVeto', 'cumulative'),
    src_individual = cms.InputTag('secondTauMuonVeto', 'individual')
)
evtSelSecondTauElectronVeto = cms.PSet(
    pluginName = cms.string('evtSelSecondTauElectronVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('secondTauElectronVeto', 'cumulative'),
    src_individual = cms.InputTag('secondTauElectronVeto', 'individual')
)

# di-tau candidate selection
evtSelDiTauCandidateForDiTauAntiOverlapVeto = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForDiTauAntiOverlapVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForDiTauAntiOverlapVeto', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForDiTauAntiOverlapVeto', 'individual')
)
evtSelDiTauCandidateForDiTauZeroCharge = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForDiTauZeroCharge'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForDiTauZeroChargeCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForDiTauZeroChargeCut', 'individual')
)
evtSelDiTauCandidateForDiTauAcoplanarity = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForDiTauAcoplanarity'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForDiTauAcoplanarityCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForDiTauAcoplanarityCut', 'individual')
)
evtSelDiTauCandidateForDiTauPzetaDiff = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForDiTauPzetaDiff'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForDiTauPzetaDiffCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForDiTauPzetaDiffCut', 'individual')
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

    # L1 trigger bits not contained in AOD;
    # in order to process Monte Carlo samples produced by FastSimulation,
    # disable histogram filling for now
    #l1GtReadoutRecordSource = cms.InputTag('hltGtDigis::HLT'),
    #l1GtObjectMapRecordSource = cms.InputTag('hltL1GtObjectMap::HLT'),
    l1GtReadoutRecordSource = cms.InputTag(''),
    l1GtObjectMapRecordSource = cms.InputTag(''),
    l1BitsToPrint = cms.vstring(
        'L1_SingleTauJet40', 'L1_SingleTauJet60',  
        'L1_DoubleTauJet20', 'L1_DoubleTauJet30', 'L1_DoubleTauJet40',
        'L1_TauJet30_ETM30', 'L1_TauJet30_ETM40'
    ),
    
    hltResultsSource = cms.InputTag('TriggerResults::HLT'),
    hltPathsToPrint = cms.vstring(
        ##'HLT_IsoTau_MET65_Trk20',
        ##'HLT_IsoTau_MET35_Trk15_L1MET',
        ##'HLT_DoubleIsoTau_Trk3'
    ),

    doGenInfo = cms.bool(True),
    genParticleSource = cms.InputTag('genParticles'),
    genJetSource = cms.InputTag('ak5GenJets'),
    genTauJetSource = cms.InputTag('tauGenJets'),
    genEventInfoSource = cms.InputTag('generator'),
    
    electronSource = cms.InputTag('cleanPatElectrons'),
    muonSource = cms.InputTag('cleanPatMuons'),
    tauSource = cms.InputTag('selectedPatTausForDiTau2ndElectronVetoCumulative'),
    diTauCandidateSource = cms.InputTag('selectedDiTauPairsPzetaDiffCumulative'),
    svFitAlgorithms = cms.VPSet(
        cms.PSet(
            name = cms.string("psKine_MEt_ptBalance")
        )
    ),
    jetSource = cms.InputTag('selectedPatJetsForZtoDiTauAntiOverlapWithLeptonsVetoCumulative'),
    caloMEtSource = cms.InputTag('patMETs'),
    pfMEtSource = cms.InputTag('patPFMETs'),
    genMEtSource = cms.InputTag('genMetTrue'),
    
    #output = cms.string("diTauEventDump.txt"),
    output = cms.string("std::cout"),
    
    triggerConditions = cms.vstring("evtSelDiTauCandidateForDiTauPzetaDiff: passed_cumulative")
)

#--------------------------------------------------------------------------------
# define analysis sequence
# (ordered list of event selection criteria and histogram filling)
#--------------------------------------------------------------------------------

diTauAnalysisSequence = cms.VPSet(
    # fill histograms for full event sample
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'tauHistManager1',
            'tauHistManager2',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForDiTau'
        )
    ),
    
    # generator level phase-space selection
    # (NOTE: to be used in case of Monte Carlo samples
    #        overlapping in simulated phase-space only !!)
    cms.PSet(
        filter = cms.string('genPhaseSpaceCut'),
        title = cms.string('gen. Phase-Space'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'tauHistManager1',
            'tauHistManager2',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForDiTau'
        )
    ),
    
    # trigger selection
    cms.PSet(
        filter = cms.string('evtSelTrigger'),
        title = cms.string('Trigger'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'tauHistManager1',
            'tauHistManager2',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForDiTau'
        )
    ),

    # data-quality selection
    cms.PSet(
        filter = cms.string('evtSelDataQuality'),
        title = cms.string('Data quality'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1',
            'tauHistManager2',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForDiTau'
        )
    ),

    # primary event vertex selection
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertex'),
        title = cms.string('Vertex'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1',
            'tauHistManager2',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexQuality'),
        title = cms.string('Vertex quality'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1',
            'tauHistManager2',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexPosition'),
        title = cms.string('Vertex position'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1',
            'tauHistManager2',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager'
        )
    ),

    # acceptance cuts on first tau-jet
    cms.PSet(
        filter = cms.string('evtSelFirstTauEta'),
        title = cms.string('-2.1 < eta(1.Tau) < +2.1'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1',
            'tauHistManager2',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager'
        ),
        replace = cms.vstring(
            'tauHistManager1.tauSource = selectedPatTausForDiTauEta21Cumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelFirstTauPt'),
        title = cms.string('Pt(1.Tau) > 20 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1',
            'tauHistManager2',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager'
        ),
        replace = cms.vstring(
            'tauHistManager1.tauSource = selectedPatTausForDiTauPt20Cumulative'
        )
    ),

    # acceptance cuts on second tau-jet
    cms.PSet(
        filter = cms.string('evtSelSecondTauEta'),
        title = cms.string('-2.1 < eta(2.Tau) < +2.1'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1',
            'tauHistManager2',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager'
        ),
        replace = cms.vstring(
            'tauHistManager1.tauSource = selectedPatTausForDiTauPt20Cumulative',
            'tauHistManager2.tauSource = selectedPatTausForDiTauEta21Cumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelSecondTauPt'),
        title = cms.string('Pt(2.Tau) > 20 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1',
            'tauHistManager2',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager'
        ),
        replace = cms.vstring(
            'tauHistManager1.tauSource = selectedPatTausForDiTauPt20Cumulative',
            'tauHistManager2.tauSource = selectedPatTausForDiTauPt20Cumulative'
        )
    ),
    
    # selection of first tau-jet candidate (id.)
    cms.PSet(
        filter = cms.string('evtSelFirstTauLeadTrk'),
        title = cms.string('1.Tau lead. Track find.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1'
        ),
        replace = cms.vstring(
            'tauHistManager1.tauSource = selectedPatTausForDiTauLeadTrkCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelFirstTauLeadTrkPt'),
        title = cms.string('1.Tau lead. Track Pt'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1'
        ),
        replace = cms.vstring(
            'tauHistManager1.tauSource = selectedPatTausForDiTau1stLeadTrkPtCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelFirstTauTaNCdiscr'),
        title = cms.string('1.Tau TaNC discr.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1'
        ),
        replace = cms.vstring(
            'tauHistManager1.tauSource = selectedPatTausForDiTau1stTaNCdiscrCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelFirstTauTrkIso'),
        title = cms.string('1.Tau Track iso.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1'
        ),
        replace = cms.vstring(
            'tauHistManager1.tauSource = selectedPatTausForDiTau1stTrkIsoCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelFirstTauEcalIso'),
        title = cms.string('1.Tau ECAL iso.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1'
        ),
        replace = cms.vstring(
            'tauHistManager1.tauSource = selectedPatTausForDiTau1stEcalIsoCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelFirstTauProng'),
        title = cms.string('1.Tau 1||3-Prong'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1'
        ),
        replace = cms.vstring(
            'tauHistManager1.tauSource = selectedPatTausForDiTau1stProngCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelFirstTauCharge'),
        title = cms.string('Charge(1.Tau) = +/-1'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1'
        ),
        replace = cms.vstring(
            'tauHistManager1.tauSource = selectedPatTausForDiTau1stChargeCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelFirstTauMuonVeto'),
        title = cms.string('1.Tau mu-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1'
        ),
        replace = cms.vstring(
            'tauHistManager1.tauSource = selectedPatTausForDiTau1stMuonVetoCumulative'
        )
    ),  
    cms.PSet(
        filter = cms.string('evtSelFirstTauElectronVeto'),
        title = cms.string('1.Tau e-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1',
            'tauHistManager2',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForDiTau'
        ),
        replace = cms.vstring(
            'tauHistManager1.tauSource = selectedPatTausForDiTau1stElectronVetoCumulative'
        )
    ),

    # selection of second tau-jet candidate (id.)
    cms.PSet(
        filter = cms.string('evtSelSecondTauLeadTrk'),
        title = cms.string('2.Tau lead. Track find.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1',
            'tauHistManager2'
        ),
        replace = cms.vstring(
            'tauHistManager1.tauSource = selectedPatTausForDiTau1stElectronVetoCumulative',
            'tauHistManager2.tauSource = selectedPatTausForDiTauLeadTrkCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelSecondTauLeadTrkPt'),
        title = cms.string('2.Tau lead. Track Pt'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1',
            'tauHistManager2'
        ),
        replace = cms.vstring(
            'tauHistManager1.tauSource = selectedPatTausForDiTau1stElectronVetoCumulative',
            'tauHistManager2.tauSource = selectedPatTausForDiTau2ndLeadTrkPtCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelSecondTauTaNCdiscr'),
        title = cms.string('2.Tau TaNC discr.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1',
            'tauHistManager2'
        ),
        replace = cms.vstring(
            'tauHistManager1.tauSource = selectedPatTausForDiTau1stElectronVetoCumulative',
            'tauHistManager2.tauSource = selectedPatTausForDiTau2ndTaNCdiscrCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelSecondTauTrkIso'),
        title = cms.string('2.Tau Track iso.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1',
            'tauHistManager2'
        ),
        replace = cms.vstring(
            'tauHistManager1.tauSource = selectedPatTausForDiTau1stElectronVetoCumulative',
            'tauHistManager2.tauSource = selectedPatTausForDiTau2ndTrkIsoCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelSecondTauEcalIso'),
        title = cms.string('2.Tau ECAL iso.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1',
            'tauHistManager2'
        ),
        replace = cms.vstring(
            'tauHistManager1.tauSource = selectedPatTausForDiTau1stElectronVetoCumulative',
            'tauHistManager2.tauSource = selectedPatTausForDiTau2ndEcalIsoCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelSecondTauProng'),
        title = cms.string('2.Tau 1||3-Prong'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1',
            'tauHistManager2'
        ),
        replace = cms.vstring(
            'tauHistManager1.tauSource = selectedPatTausForDiTau1stElectronVetoCumulative',
            'tauHistManager2.tauSource = selectedPatTausForDiTau2ndProngCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelSecondTauCharge'),
        title = cms.string('Charge(2.Tau) = +/-1'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1',
            'tauHistManager2'
        ),
        replace = cms.vstring(
            'tauHistManager1.tauSource = selectedPatTausForDiTau1stElectronVetoCumulative',
            'tauHistManager2.tauSource = selectedPatTausForDiTau2ndChargeCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelSecondTauMuonVeto'),
        title = cms.string('2.Tau mu-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1',
            'tauHistManager2'
        ),
        replace = cms.vstring(
            'tauHistManager1.tauSource = selectedPatTausForDiTau1stElectronVetoCumulative',
            'tauHistManager2.tauSource = selectedPatTausForDiTau2ndMuonVetoCumulative'
        )
    ),  
    cms.PSet(
        filter = cms.string('evtSelSecondTauElectronVeto'),
        title = cms.string('2.Tau e-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1',
            'tauHistManager2',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForDiTau'
        ),
        replace = cms.vstring(
            'tauHistManager1.tauSource = selectedPatTausForDiTau1stElectronVetoCumulative',
            'tauHistManager2.tauSource = selectedPatTausForDiTau2ndElectronVetoCumulative'
        )
    ),

    #selection of tau-jet + tau-jet combinations
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForDiTauAntiOverlapVeto'),
        title = cms.string('dR(1.Tau-2.Tau) > 0.7'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1',
            'tauHistManager2',
            'diTauCandidateHistManagerForDiTau',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForDiTau'
        ),
        replace = cms.vstring(
            'tauHistManager1.tauSource = selectedPatTausForDiTau1stElectronVetoCumulative',
            'tauHistManager2.tauSource = selectedPatTausForDiTau2ndElectronVetoCumulative',
            'diTauCandidateHistManagerForDiTau.diTauCandidateSource = selectedDiTauPairsAntiOverlapVetoCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForDiTauZeroCharge'),
        title = cms.string('Charge(1.Tau+2.Tau) = 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),    
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1',
            'tauHistManager2',
            'diTauCandidateHistManagerForDiTau',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForDiTau'
        ),
        replace = cms.vstring(
            'tauHistManager1.tauSource = selectedPatTausForDiTau1stElectronVetoCumulative',
            'tauHistManager2.tauSource = selectedPatTausForDiTau2ndElectronVetoCumulative',
            'diTauCandidateHistManagerForDiTau.diTauCandidateSource = selectedDiTauPairsZeroChargeCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForDiTauAcoplanarity'),
        title = cms.string('dPhi(1.Tau-MET) < 3.2 || dPhi(2.Tau-MET) < 3.2'), # cut disabled for now...
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'tauHistManager1',
            'tauHistManager2',
            'diTauCandidateHistManagerForDiTau',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForDiTau'
        ),
        replace = cms.vstring(
            'tauHistManager1.tauSource = selectedPatTausForDiTau1stElectronVetoCumulative',
            'tauHistManager2.tauSource = selectedPatTausForDiTau2ndElectronVetoCumulative',
            'diTauCandidateHistManagerForDiTau.diTauCandidateSource = selectedDiTauPairsAcoplanarityCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForDiTauPzetaDiff'),
        title = cms.string('P_{#zeta} - 1.5*P_{#zeta}^{vis} > -20 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'eventWeightHistManager',
            'tauHistManager1',
            'tauHistManager2',
            'diTauCandidateHistManagerForDiTau',
            'diTauCandidateCollinearApproxHistManagerForDiTau',
            'diTauCandidateSVfitHistManagerForDiTau',
            'jetHistManagerForDiTau',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForDiTau'
        ),
        replace = cms.vstring(
            'tauHistManager1.tauSource = selectedPatTausForDiTau1stElectronVetoCumulative',
            'tauHistManager2.tauSource = selectedPatTausForDiTau2ndElectronVetoCumulative',
            'diTauCandidateHistManagerForDiTau.diTauCandidateSource = selectedDiTauPairsPzetaDiffCumulative',
            'diTauCandidateCollinearApproxHistManagerForDiTau.diTauCandidateSource = selectedDiTauPairsPzetaDiffCumulative',
            'diTauCandidateSVfitHistManagerForDiTau.diTauCandidateSource = selectedDiTauPairsPzetaDiffCumulative',
            'jetHistManagerForDiTau.jetSource = selectedPatJetsForZtoDiTauAntiOverlapWithLeptonsVetoCumulative'
        )
    )
    
    # veto events containing additional central jets with Et > 20 GeV
    #cms.PSet(
    #    filter = cms.string('evtSelCentralJetVeto'),
    #    title = cms.string('central Jet Veto'),
    #    saveRunLumiSectionEventNumbers = cms.vstring('passed_cumulative')
    #),
    #cms.PSet(
    #    analyzers = cms.vstring(
    #        'genPhaseSpaceEventInfoHistManager',
    #        'eventWeightHistManager',
    #        'tauHistManager1',
    #        'tauHistManager2',
    #        'diTauCandidateHistManagerForDiTau',
    #        'diTauCandidateCollinearApproxHistManagerForDiTau',
    #        'diTauCandidateSVfitHistManagerForDiTau',
    #        'jetHistManagerForDiTau',
    #        'caloMEtHistManager',
    #        'pfMEtHistManager',
    #        'vertexHistManager',
    #        'triggerHistManagerForDiTau'
    #    ),
    #    replace = cms.vstring(
    #        'tauHistManager1.tauSource = selectedPatTausForDiTau1stElectronVetoCumulative',
    #        'tauHistManager2.tauSource = selectedPatTausForDiTau2ndElectronVetoCumulative',
    #        'diTauCandidateHistManagerForDiTau.diTauCandidateSource = selectedDiTauPairsAcoplanarityCumulative'
    #    )
    #)
)
