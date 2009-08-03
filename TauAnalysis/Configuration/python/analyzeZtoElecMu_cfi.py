import FWCore.ParameterSet.Config as cms
import copy

# import config for histogram manager filling information about phase-space simulated in Monte Carlo sample
from TauAnalysis.Core.genPhaseSpaceEventInfoHistManager_cfi import *

# import config for electron histogram manager
from TauAnalysis.Core.electronHistManager_cfi import *

# import config for muon histogram manager
from TauAnalysis.Core.muonHistManager_cfi import *

# import config for di-tau histogram manager
from TauAnalysis.Core.diTauCandidateHistManager_cfi import *
diTauCandidateHistManagerForElecMu = copy.deepcopy(diTauCandidateHistManager)
diTauCandidateHistManagerForElecMu.pluginName = cms.string('diTauCandidateHistManagerForElecMu')
diTauCandidateHistManagerForElecMu.pluginType = cms.string('PATElecMuPairHistManager')
diTauCandidateHistManagerForElecMu.diTauCandidateSource = cms.InputTag('allElecMuPairs')

# import config for missing-Et histogram manager
from TauAnalysis.Core.metHistManager_cfi import *

# import config for central jet veto histogram manager
from TauAnalysis.Core.jetHistManager_cfi import *

# import config for primary event vertex histogram manager
from TauAnalysis.Core.vertexHistManager_cfi import *

# import config for L1 & HLT histogram manager
from TauAnalysis.Core.triggerHistManager_cfi import *
triggerHistManager.l1Bits = cms.vstring(
    'L1_SingleEG5',
    'L1_SingleEG8',
    'L1_SingleEG10',
    'L1_SingleEG12',
    'L1_SingleEG15',
    'L1_SingleIsoEG5',
    'L1_SingleIsoEG8',
    'L1_SingleIsoEG10',
    'L1_SingleIsoEG12',
    'L1_SingleIsoEG15',
    'L1_SingleMu3',
    'L1_SingleMu5',
    'L1_SingleMu7',
    'L1_SingleMu10',
    'L1_SingleMu14'
)
triggerHistManager.hltPaths = cms.vstring(
    'HLT_IsoEle15_L1I',
    'HLT_Mu15',
    'HLT_IsoMu11',
    'HLT_IsoEle8_IsoMu7'
)

elecMuHistManagers = cms.vstring(
    'genPhaseSpaceEventInfoHistManager',
    'electronHistManager',
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
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

# passing basic acceptance and kinematic cuts
# (NOTE: to be used for efficiency studies only !!)
#genElectronCut = cms.PSet(
#    pluginName = cms.string('genElectronCut'),
#    pluginType = cms.string('PATCandViewMinEventSelector'),
#    src = cms.InputTag('selectedGenTauDecaysToElectronPt15Cumulative'),
#    minNumber = cms.uint32(1)
#)
#genMuonCut = cms.PSet(
#    pluginName = cms.string('genMuonCut'),
#    pluginType = cms.string('PATCandViewMinEventSelector'),
#    src = cms.InputTag('selectedGenTauDecaysToMuonPt15Cumulative'),
#    minNumber = cms.uint32(1)
#)

# trigger selection
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

# electron acceptance cuts
evtSelTightElectronId = cms.PSet(
    pluginName = cms.string('evtSelTightElectronId'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tightElectronIdCut', 'cumulative'),
    src_individual = cms.InputTag('tightElectronIdCut', 'individual')
)
evtSelElectronAntiCrack = cms.PSet(
    pluginName = cms.string('evtSelElectronAntiCrack'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronAntiCrackCut', 'cumulative'),
    src_individual = cms.InputTag('electronAntiCrackCut', 'individual')
)
evtSelElectronEta = cms.PSet(
    pluginName = cms.string('evtSelElectronEta'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronEtaCut', 'cumulative'),
    src_individual = cms.InputTag('electronEtaCut', 'individual')
)
evtSelElectronPt = cms.PSet(
    pluginName = cms.string('evtSelElectronPt'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronPtCut', 'cumulative'),
    src_individual = cms.InputTag('electronPtCut', 'individual')
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

# electron candidate (isolation & id.) selection
evtSelElectronTrkIso = cms.PSet(
    pluginName = cms.string('evtSelElectronTrkIso'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronTrkIsoCut', 'cumulative'),
    src_individual = cms.InputTag('electronTrkIsoCut', 'individual')
)
evtSelElectronEcalIso = cms.PSet(
    pluginName = cms.string('evtSelElectronEcalIso'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronEcalIsoCut', 'cumulative'),
    src_individual = cms.InputTag('electronEcalIsoCut', 'individual')
)
evtSelElectronTrk = cms.PSet(
    pluginName = cms.string('evtSelElectronTrk'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronTrkCut', 'cumulative'),
    src_individual = cms.InputTag('electronTrkCut', 'individual')
)
evtSelElectronTrkIP = cms.PSet(
    pluginName = cms.string('evtSelElectronTrkIP'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronTrkIPcut', 'cumulative'),
    src_individual = cms.InputTag('electronTrkIPcut', 'individual')
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

# di-tau candidate selection
evtSelDiTauCandidateForElecMuAntiOverlapVeto = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForElecMuAntiOverlapVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForElecMuAntiOverlapVeto', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForElecMuAntiOverlapVeto', 'individual')
)
evtSelDiTauCandidateForElecMuZeroCharge = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForElecMuZeroCharge'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForElecMuZeroChargeCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForElecMuZeroChargeCut', 'individual')
)
evtSelDiTauCandidateForElecMuAcoplanarity12 = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForElecMuAcoplanarity12'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForElecMuAcoplanarity12Cut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForElecMuAcoplanarity12Cut', 'individual')
)
evtSelDiTauCandidateForElecMuMt1MET = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForElecMuMt1MET'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForElecMuMt1METcut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForElecMuMt1METcut', 'individual')
)
evtSelDiTauCandidateForElecMuMt2MET = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForElecMuMt2MET'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForElecMuMt2METcut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForElecMuMt2METcut', 'individual')
)
evtSelDiTauCandidateForElecMuPzetaDiff = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForElecMuPzetaDiff'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForElecMuPzetaDiffCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForElecMuPzetaDiffCut', 'individual')
)

#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------

elecMuEventDump = cms.PSet(
    pluginName = cms.string('elecMuEventDump'),
    pluginType = cms.string('ElecMuEventDump'),

    l1GtReadoutRecordSource = cms.InputTag('hltGtDigis::HLT'),
    l1GtObjectMapRecordSource = cms.InputTag('hltL1GtObjectMap::HLT'),
    l1BitsToPrint = cms.vstring('L1_SingleEG5', 'L1_SingleEG8', 'L1_SingleEG10', 'L1_SingleEG12', 'L1_SingleEG15',
                                'L1_SingleIsoEG5', 'L1_SingleIsoEG8', 'L1_SingleIsoEG10', 'L1_SingleIsoEG12', 'L1_SingleIsoEG15',
                                'L1_SingleMu3', 'L1_SingleMu5', 'L1_SingleMu7', 'L1_SingleMu10', 'L1_SingleMu14'),

    hltResultsSource = cms.InputTag('TriggerResults::HLT'),
    hltPathsToPrint = cms.vstring('HLT_IsoEle15_L1I', 'HLT_Mu15', 'HLT_IsoMu11', 'HLT_IsoEle8_IsoMu7'),
    
    genParticleSource = cms.InputTag('genParticles'),
    genTauJetSource = cms.InputTag('tauGenJets'),
    #electronSource = cms.InputTag('cleanLayer1ElectronsSel'),
    electronSource = cms.InputTag('selectedLayer1ElectronsTrkIPcumulative'),
    #muonSource = cms.InputTag('cleanLayer1MuonsSel'),
    muonSource = cms.InputTag('selectedLayer1MuonsTrkIPcumulative'),
    tauSource = cms.InputTag('cleanLayer1TausSel'),
    diTauCandidateSource = cms.InputTag('allElecMuPairs'),
    metSource = cms.InputTag('layer1METs'),
    genMEtSource = cms.InputTag('genMETWithMu'),
    jetSource = cms.InputTag('selectedLayer1JetsEt20Cumulative'),
    #recoTrackSource = cms.InputTag('generalTracks'),
    #pfChargedHadronSource = cms.InputTag('pfAllChargedHadrons'),
    #pfGammaSource = cms.InputTag('pfAllPhotons'),
    #pfNeutralHadronSource = cms.InputTag('pfAllNeutralHadrons'),

    #output = cms.string("elecMuEventDump.txt"),
    output = cms.string("std::cout"),

    triggerConditions = cms.vstring("evtSelDiTauCandidateForElecMuPzetaDiff: passed_cumulative")
)

#--------------------------------------------------------------------------------
# define analysis sequence
# (ordered list of event selection criteria and histogram filling)
#--------------------------------------------------------------------------------

elecMuAnalysisSequence = cms.VPSet(
    # fill histograms for full event sample
    cms.PSet(
        analyzers = elecMuHistManagers
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
        analyzers = elecMuHistManagers
    ),

    # generator level selection of Z --> e + mu events
    # passing basic acceptance and kinematic cuts
    # (NOTE: to be used for efficiency studies only !!)
    #cms.PSet(
    #    filter = cms.string('genElectronCut'),
    #    title = cms.string('gen. Electron'),
    #    saveRunEventNumbers = cms.vstring('')
    #),
    #cms.PSet(
    #    filter = cms.string('genMuonCut'),
    #    title = cms.string('gen. Muon'),
    #    saveRunEventNumbers = cms.vstring('')
    #),
    #cms.PSet(
    #    analyzers = elecMuHistManagers
    #),
    
    # trigger selection
    cms.PSet(
        filter = cms.string('evtSelTrigger'),
        title = cms.string('Electron || Muon Triggers'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers
    ),

    # primary event vertex selection
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertex'),
        title = cms.string('Vertex'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexQuality'),
        title = cms.string('p(chi2Vertex) > 0.01'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexPosition'),
        title = cms.string('-25 < zVertex < +25 cm'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers
    ),

    # electron acceptance cuts
    cms.PSet(
        filter = cms.string('evtSelTightElectronId'),
        title = cms.string('tight Electron Id.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTightIdCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelElectronAntiCrack'),
        title = cms.string('crack-Veto'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsAntiCrackCutCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelElectronEta'),
        title = cms.string('-2.1 < eta(Electron) < +2.1'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsEta21Cumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelElectronPt'),
        title = cms.string('Pt(Electron) > 15 GeV'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsPt15Cumulative')
    ),

    # muon acceptance cuts
    cms.PSet(
        filter = cms.string('evtSelGlobalMuon'),
        title = cms.string('global Muon'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsPt15Cumulative',
                              'muonHistManager.muonSource = selectedLayer1MuonsGlobalCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonEta'),
        title = cms.string('-2.1 < eta(Muon) < +2.1'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsPt15Cumulative',
                              'muonHistManager.muonSource = selectedLayer1MuonsEta21Cumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonPt'),
        title = cms.string('Pt(Muon) > 15 GeV'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsPt15Cumulative',
                              'muonHistManager.muonSource = selectedLayer1MuonsPt15Cumulative')
    ),

    # selection of electron candidate (isolation & id.)
    # produced in electronic tau decay
    cms.PSet(
        filter = cms.string('evtSelElectronTrkIso'),
        title = cms.string('Electron Track iso.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkIsoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelElectronEcalIso'),
        title = cms.string('Electron ECAL iso.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsEcalIsoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelElectronTrk'),
        title = cms.string('Electron Track find.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelElectronTrkIP'),
        title = cms.string('Electron Track IP'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkIPcumulative')
    ),  
  
    # selection of muon candidate (isolation & id.)
    # produced in muonic tau decay
    cms.PSet(
        filter = cms.string('evtSelMuonTrkIso'),
        title = cms.string('Muon Track iso.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkIPcumulative',
                              'muonHistManager.muonSource = selectedLayer1MuonsTrkIsoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonEcalIso'),
        title = cms.string('Muon ECAL iso.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkIPcumulative',
                              'muonHistManager.muonSource = selectedLayer1MuonsEcalIsoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonAntiPion'),
        title = cms.string('Muon pi-Veto'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkIPcumulative',
                              'muonHistManager.muonSource = selectedLayer1MuonsPionVetoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonTrkIP'),
        title = cms.string('Muon Track IP'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkIPcumulative',
                              'muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative')
    ),

    # selection of electron + muon combinations
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForElecMuAntiOverlapVeto'),
        title = cms.string('dR(Muon-Electron) > 1.57'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative',
                              'electronHistManager.electronSource = selectedLayer1ElectronsTrkIPcumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsAntiOverlapVetoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForElecMuZeroCharge'),
        title = cms.string('Charge(Electron+Muon) = 0'),
        saveRunEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkIPcumulative',
                              'muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsZeroChargeCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForElecMuAcoplanarity12'),
        title = cms.string('Acoplanarity(Electron+Muon)'),
        saveRunEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedLayer1ElectronsTrkIPcumulative',
                              'muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsAcoplanarity12Cumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForElecMuMt1MET'),
        title = cms.string('M_{T}(Electron-MET) < 50 GeV'),
        saveRunEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative',
                              'electronHistManager.electronSource = selectedLayer1ElectronsTrkIPcumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsMt1METcumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForElecMuMt2MET'),
        title = cms.string('M_{T}(Muon-MET) < 50 GeV'),
        saveRunEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative',
                              'electronHistManager.electronSource = selectedLayer1ElectronsTrkIPcumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsMt2METcumulative')

    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForElecMuPzetaDiff'),
        title = cms.string('P_{#zeta} - 1.5*P_{#zeta}^{vis} > -20 GeV'),
        saveRunEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative',
                              'electronHistManager.electronSource = selectedLayer1ElectronsTrkIPcumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsPzetaDiffCumulative')
    )
)

