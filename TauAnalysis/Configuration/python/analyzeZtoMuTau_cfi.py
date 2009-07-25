import FWCore.ParameterSet.Config as cms
import copy

# import config for histogram manager filling information about phase-space simulated in Monte Carlo sample
from TauAnalysis.Core.genPhaseSpaceEventInfoHistManager_cfi import *

# import config for muon histogram manager
from TauAnalysis.Core.muonHistManager_cfi import *

# import config for tau-jet histogram manager
from TauAnalysis.Core.pftauHistManager_cfi import *
from TauAnalysis.Core.pftauRecoilEnergyHistManager_cfi import *

# import config for di-tau histogram manager
from TauAnalysis.Core.diTauCandidateHistManager_cfi import *
diTauCandidateHistManagerForMuTau = copy.deepcopy(diTauCandidateHistManager)
diTauCandidateHistManagerForMuTau.pluginName = cms.string('diTauCandidateHistManagerForMuTau')
diTauCandidateHistManagerForMuTau.pluginType = cms.string('PATMuTauPairHistManager')
diTauCandidateHistManagerForMuTau.diTauCandidateSource = cms.InputTag('allMuTauPairs')

# import config for missing-Et histogram managers
from TauAnalysis.Core.metHistManager_cfi import *
from TauAnalysis.Core.metTopologyHistManager_cfi import *

# import config for central jet veto histogram manager
from TauAnalysis.Core.jetHistManager_cfi import *

# import config for primary event vertex histogram manager
from TauAnalysis.Core.vertexHistManager_cfi import *

# import config for L1 & HLT histogram manager
from TauAnalysis.Core.triggerHistManager_cfi import *
triggerHistManager.l1Bits = cms.vstring(
    'L1_SingleMu3',
    'L1_SingleMu5',
    'L1_SingleMu7',
    'L1_SingleMu10',
    'L1_SingleMu14'
)
triggerHistManager.hltPaths = cms.vstring(
    'HLT_Mu15',
    'HLT_IsoMu11'
)

muTauHistManagers = cms.vstring(
    'genPhaseSpaceEventInfoHistManager',
    'muonHistManager',
    'tauHistManager',
    'tauRecoilEnergyFromJetsHistManager',
    'tauRecoilEnergyFromCaloTowersHistManager',
    'diTauCandidateHistManagerForMuTau',
    'metHistManager',
    'metTopologyHistManager',
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

# generator level selection of Z --> mu + tau-jet events
# passing basic acceptance and kinematic cuts
# (NOTE: to be used for efficiency studies only !!)
#genMuonCut = cms.PSet(
#    pluginName = cms.string('genMuonCut'),
#    pluginType = cms.string('PATCandViewMinEventSelector'),
#    src = cms.InputTag('selectedGenTauDecaysToMuonPt15Cumulative'),
#    minNumber = cms.uint32(1)
#)
#genTauCut = cms.PSet(
#    pluginName = cms.string('genTauCut'),
#    pluginType = cms.string('PATCandViewMinEventSelector'),
#    src = cms.InputTag('selectedGenTauDecaysToHadronsPt20Cumulative'),
#    minNumber = cms.uint32(1)
#)

evtSelTrigger = cms.PSet(
    pluginName = cms.string('evtSelTrigger'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('Trigger')
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

# muon acceptance cuts
evtSelGlobalMuon = cms.PSet(
    pluginName = cms.string('evtSelGlobalMuon'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('globalMuonCut', 'cumulative'),
    src_individual = cms.InputTag('globalMuonCut', 'individual')
)
evtSelMuonEta = cms.PSet(
    pluginName = cms.string('evtSelMuonEta'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('muonEtaCut', 'cumulative'),
    src_individual = cms.InputTag('muonEtaCut', 'individual')
)
evtSelMuonPt = cms.PSet(
    pluginName = cms.string('evtSelMuonPt'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('muonPtCut', 'cumulative'),
    src_individual = cms.InputTag('muonPtCut', 'individual')
)

# tau acceptance cuts
evtSelTauAntiOverlapWithMuonsVeto = cms.PSet(
    pluginName = cms.string('evtSelTauAntiOverlapWithMuonsVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauAntiOverlapWithMuonsVeto', 'cumulative'),
    src_individual = cms.InputTag('tauAntiOverlapWithMuonsVeto', 'individual')
)
evtSelTauEta = cms.PSet(
    pluginName = cms.string('evtSelTauEta'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauEtaCut', 'cumulative'),
    src_individual = cms.InputTag('tauEtaCut', 'individual')
)
evtSelTauPt = cms.PSet(
    pluginName = cms.string('evtSelTauPt'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauPtCut', 'cumulative'),
    src_individual = cms.InputTag('tauPtCut', 'individual')
)

# muon candidate (isolation & id.) selection
evtSelMuonTrkIso = cms.PSet(
    pluginName = cms.string('evtSelMuonTrkIso'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('muonTrkIsoCut', 'cumulative'),
    src_individual = cms.InputTag('muonTrkIsoCut', 'individual')
)
evtSelMuonEcalIso = cms.PSet(
    pluginName = cms.string('evtSelMuonEcalIso'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('muonEcalIsoCut', 'cumulative'),
    src_individual = cms.InputTag('muonEcalIsoCut', 'individual')
)
evtSelMuonAntiPion = cms.PSet(
    pluginName = cms.string('evtSelMuonAntiPion'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('muonAntiPionCut', 'cumulative'),
    src_individual = cms.InputTag('muonAntiPionCut', 'individual')
)
evtSelMuonTrkIP = cms.PSet(
    pluginName = cms.string('evtSelMuonTrkIP'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('muonTrkIPcut', 'cumulative'),
    src_individual = cms.InputTag('muonTrkIPcut', 'individual')
)

# tau candidate (id.) selection
evtSelTauLeadTrk = cms.PSet(
    pluginName = cms.string('evtSelTauLeadTrk'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauLeadTrkCut', 'cumulative'),
    src_individual = cms.InputTag('tauLeadTrkCut', 'individual')
)
evtSelTauLeadTrkPt = cms.PSet(
    pluginName = cms.string('evtSelTauLeadTrkPt'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauLeadTrkPtCut', 'cumulative'),
    src_individual = cms.InputTag('tauLeadTrkPtCut', 'individual')
)
evtSelTauTrkIso = cms.PSet(
    pluginName = cms.string('evtSelTauTrkIso'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauTrkIsoCut', 'cumulative'),
    src_individual = cms.InputTag('tauTrkIsoCut', 'individual')
)
evtSelTauEcalIso = cms.PSet(
    pluginName = cms.string('evtSelTauEcalIso'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauEcalIsoCut', 'cumulative'),
    src_individual = cms.InputTag('tauEcalIsoCut', 'individual')
)
evtSelTauProng = cms.PSet(
    pluginName = cms.string('evtSelTauProng'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauProngCut', 'cumulative'),
    src_individual = cms.InputTag('tauProngCut', 'individual')
)
evtSelTauMuonVeto = cms.PSet(
    pluginName = cms.string('evtSelTauMuonVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauMuonVeto', 'cumulative'),
    src_individual = cms.InputTag('tauMuonVeto', 'individual')
)

# di-tau candidate selection
evtSelDiTauCandidateForMuTauAntiOverlapVeto = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForMuTauAntiOverlapVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForMuTauAntiOverlapVeto', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForMuTauAntiOverlapVeto', 'individual')
)
evtSelDiTauCandidateForMuTauZeroCharge = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForMuTauZeroCharge'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForMuTauZeroChargeCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForMuTauZeroChargeCut', 'individual')
)
evtSelDiTauCandidateForMuTauMt1MET = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForMuTauMt1MET'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForMuTauMt1METcut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForMuTauMt1METcut', 'individual')
)
evtSelDiTauCandidateForMuTauPzetaDiff = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForMuTauPzetaDiff'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForMuTauPzetaDiffCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForMuTauPzetaDiffCut', 'individual')
)

# veto events containing additional central jets with Et > 20 GeV
#evtSelCentralJetVeto = cms.PSet(
#    pluginName = cms.string('evtSelCentralJetVeto'),
#    pluginType = cms.string('BoolEventSelector'),
#    src = cms.InputTag('centralJetVeto')
#)

#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------

muTauEventDump = cms.PSet(
    pluginName = cms.string('muTauEventDump'),
    pluginType = cms.string('MuTauEventDump'),

    l1GtReadoutRecordSource = cms.InputTag('hltGtDigis::HLT'),
    l1GtObjectMapRecordSource = cms.InputTag('hltL1GtObjectMap::HLT'),
    l1BitsToPrint = cms.vstring('L1_SingleMu3', 'L1_SingleMu5', 'L1_SingleMu7', 'L1_SingleMu10', 'L1_SingleMu14'),
    
    hltResultsSource = cms.InputTag('TriggerResults::HLT'),
    hltPathsToPrint = cms.vstring('HLT_Mu15', 'HLT_IsoMu11'),
    
    genParticleSource = cms.InputTag('genParticles'),
    genTauJetSource = cms.InputTag('tauGenJets'),
    electronSource = cms.InputTag('cleanLayer1ElectronsSel'),
    #muonSource = cms.InputTag('cleanLayer1MuonsSel'),
    muonSource = cms.InputTag('selectedLayer1MuonsTrkIPcumulative'),
    tauSource = cms.InputTag('cleanLayer1TausSel'),
    #tauSource = cms.InputTag('selectedLayer1TausForMuTauMuonVetoCumulative'),
    diTauCandidateSource = cms.InputTag('allMuTauPairs'),
    metSource = cms.InputTag('layer1METs'),
    genMEtSource = cms.InputTag('genMETWithMu'),
    jetSource = cms.InputTag('selectedLayer1JetsEt20Cumulative'),
    recoTrackSource = cms.InputTag('generalTracks'),
    pfChargedHadronSource = cms.InputTag('pfAllChargedHadrons'),
    pfGammaSource = cms.InputTag('pfAllPhotons'),
    pfNeutralHadronSource = cms.InputTag('pfAllNeutralHadrons'),
    
    #output = cms.string("muTauEventDump.txt"),
    output = cms.string("std::cout"),
    
    #triggerConditions = cms.vstring("evtSelTauTrkIso: rejected_cumulative")
    triggerConditions = cms.vstring("evtSelDiTauCandidateForMuTauPzetaDiff: passed_cumulative")
)

#--------------------------------------------------------------------------------
# define analysis sequence
# (ordered list of event selection criteria and histogram filling)
#--------------------------------------------------------------------------------

muTauAnalysisSequence = cms.VPSet(
    # fill histograms for full event sample
    cms.PSet(
        analyzers = muTauHistManagers
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
        analyzers = muTauHistManagers
    ),

    # generator level selection of Z --> mu + tau-jet events
    # passing basic acceptance and kinematic cuts
    # (NOTE: to be used for efficiency studies only !!)
    #cms.PSet(
    #    filter = cms.string('genMuonCut'),
    #    title = cms.string('gen. Muon'),
    #    saveRunEventNumbers = cms.vstring('')
    #),
    #cms.PSet(
    #    filter = cms.string('genTauCut'),
    #    title = cms.string('gen. Tau'),
    #    saveRunEventNumbers = cms.vstring('')
    #),
    #cms.PSet(
    #    analyzers = muTauHistManagers
    #),
    
    # trigger selection
    cms.PSet(
        filter = cms.string('evtSelTrigger'),
        title = cms.string('mu15 || isoMu11 Trigger'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = muTauHistManagers
    ),

    # primary event vertex selection
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertex'),
        title = cms.string('Vertex'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = muTauHistManagers
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexQuality'),
        title = cms.string('p(chi2Vertex) > 0.01'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = muTauHistManagers
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexPosition'),
        title = cms.string('-25 < zVertex < +25 cm'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = muTauHistManagers
    ),

    # muon acceptance cuts
    cms.PSet(
        filter = cms.string('evtSelGlobalMuon'),
        title = cms.string('global Muon'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = muTauHistManagers,
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsGlobalCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonEta'),
        title = cms.string('-2.1 < eta(Muon) < +2.1'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = muTauHistManagers,
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsEta21Cumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonPt'),
        title = cms.string('Pt(Muon) > 15 GeV'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = muTauHistManagers,
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsPt15Cumulative')
    ),

    # tau acceptance cuts
    cms.PSet(
        filter = cms.string('evtSelTauAntiOverlapWithMuonsVeto'),
        title = cms.string('Tau not overlapping w. Muon'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = muTauHistManagers,
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsPt15Cumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauAntiOverlapWithMuonsVetoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauEta'),
        title = cms.string('-2.1 < eta(Tau) < +2.1'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = muTauHistManagers,
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsPt15Cumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauEta21Cumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauPt'),
        title = cms.string('Pt(Tau) > 20 GeV'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = muTauHistManagers,
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsPt15Cumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauPt20Cumulative')
    ),
    
    # selection of muon candidate (isolation & id.)
    # produced in muonic tau decay
    cms.PSet(
        filter = cms.string('evtSelMuonTrkIso'),
        title = cms.string('Muon Track iso.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = muTauHistManagers,
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIsoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonEcalIso'),
        title = cms.string('Muon ECAL iso.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = muTauHistManagers,
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsEcalIsoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonAntiPion'),
        title = cms.string('Muon pi-Veto'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = muTauHistManagers,
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsPionVetoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonTrkIP'),
        title = cms.string('Muon Track IP'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = muTauHistManagers,
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative')
    ),
    
    # selection of tau-jet candidate (id.)
    # produced in hadronic tau decay
    cms.PSet(
        filter = cms.string('evtSelTauLeadTrk'),
        title = cms.string('Tau lead. Track find.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = muTauHistManagers,
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauLeadTrkCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauLeadTrkPt'),
        title = cms.string('Tau lead. Track Pt'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = muTauHistManagers,
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauLeadTrkPtCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauTrkIso'),
        title = cms.string('Tau Track iso.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = muTauHistManagers,
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauTrkIsoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauEcalIso'),
        title = cms.string('Tau ECAL iso.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = muTauHistManagers,
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauEcalIsoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauProng'),
        title = cms.string('Tau 1||3-Prong'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = muTauHistManagers,
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauProngCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauMuonVeto'),
        title = cms.string('Tau mu-Veto'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = muTauHistManagers,
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauMuonVetoCumulative')
    ),

    # selection of muon + tau-jet combinations
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForMuTauAntiOverlapVeto'),
        title = cms.string('dR(Muon-Tau) > 0.7'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = muTauHistManagers,
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauMuonVetoCumulative',
                              'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsAntiOverlapVetoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForMuTauZeroCharge'),
        title = cms.string('Charge(Muon+Tau) = 0'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = muTauHistManagers,
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauMuonVetoCumulative',
                              'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsZeroChargeCumulative'),
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForMuTauMt1MET'),
        title = cms.string('M_{T}(Muon-MET) < 50 GeV'),
        saveRunEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = muTauHistManagers,
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauMuonVetoCumulative',
                              'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsMt1METcumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForMuTauPzetaDiff'),
        title = cms.string('P_{#zeta} - 1.5*P_{#zeta}^{vis} > -20 GeV'),
        saveRunEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = muTauHistManagers,
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauMuonVetoCumulative',
                              'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsPzetaDiffCumulative')
    ),
)
