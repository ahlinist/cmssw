import FWCore.ParameterSet.Config as cms
import copy

# import config for histogram manager filling information about phase-space simulated in Monte Carlo sample
from TauAnalysis.Core.genPhaseSpaceEventInfoHistManager_cfi import *

# import config for event weight histogram manager
from TauAnalysis.Core.eventWeightHistManager_cfi import *

# import config for muon histogram manager
from TauAnalysis.Core.muonHistManager_cfi import *

# import config for tau-jet histogram manager
from TauAnalysis.Core.pftauHistManager_cfi import *

# import config for di-tau histogram manager
from TauAnalysis.Core.diTauCandidateHistManager_cfi import *
diTauCandidateHistManagerForMuTau = copy.deepcopy(diTauCandidateHistManager)
diTauCandidateHistManagerForMuTau.pluginName = cms.string('diTauCandidateHistManagerForMuTau')
diTauCandidateHistManagerForMuTau.pluginType = cms.string('PATMuTauPairHistManager')
diTauCandidateHistManagerForMuTau.diTauCandidateSource = cms.InputTag('allMuTauPairs')
diTauCandidateHistManagerForMuTau.visMassHypothesisSource = cms.InputTag('')
from TauAnalysis.Core.diTauCandidateCollinearApproxHistManager_cfi import *
diTauCandidateCollinearApproxHistManagerBinnedForMuTau = copy.deepcopy(diTauCandidateCollinearApproxHistManagerBinned)
diTauCandidateCollinearApproxHistManagerBinnedForMuTau.pluginName = cms.string('diTauCandidateCollinearApproxHistManagerBinnedForMuTau')
diTauCandidateCollinearApproxHistManagerForMuTau = copy.deepcopy(diTauCandidateCollinearApproxHistManager)
diTauCandidateCollinearApproxHistManagerForMuTau.pluginName = cms.string('diTauCandidateCollinearApproxHistManagerForMuTau')
diTauCandidateCollinearApproxHistManagerForMuTau.pluginType = cms.string('PATMuTauPairCollinearApproxHistManager')
diTauCandidateCollinearApproxHistManagerForMuTau.diTauCandidateSource = cms.InputTag('selectedMuTauPairsPzetaDiffCumulative')
diTauCandidateCollinearApproxHistManagerBinnedForMuTau.histManagers = cms.VPSet(
    diTauCandidateHistManagerForMuTau.clone(
        diTauCandidateSource = cms.InputTag('selectedMuTauPairsPzetaDiffCumulative')
    ),
    diTauCandidateCollinearApproxHistManagerForMuTau,
    pfMEtHistManager,
    caloMEtHistManager,
    genPhaseSpaceEventInfoHistManager
)
from TauAnalysis.Core.diTauCandidateZllHypothesisHistManager_cfi import *
diTauCandidateZmumuHypothesisHistManagerForMuTau = copy.deepcopy(ZllHypothesisHistManager)
diTauCandidateZmumuHypothesisHistManagerForMuTau.pluginName = cms.string('diTauCandidateZmumuHypothesisHistManagerForMuTau')
diTauCandidateZmumuHypothesisHistManagerForMuTau.pluginType = cms.string('ZllHypothesisMuTauHistManager')
diTauCandidateZmumuHypothesisHistManagerForMuTau.ZllHypothesisSource = cms.InputTag('muTauPairZmumuHypotheses')
diTauCandidateZmumuHypothesisHistManagerForMuTau.dqmDirectory_store = cms.string('DiTauCandidateZmumuHypothesisQuantities')

# import config for Zmumu veto histogram manager
muPairHistManager = copy.deepcopy(diTauCandidateHistManager)
muPairHistManager.pluginName = cms.string('muPairHistManager')
muPairHistManager.pluginType = cms.string('DiCandidatePairHistManager')
muPairHistManager.diTauCandidateSource = cms.InputTag('allDiMuPairZmumuHypotheses')
muPairHistManager.dqmDirectory_store = cms.string('DiMuZmumuHypothesisQuantities')

# import config for central jet veto histogram manager
from TauAnalysis.Core.jetHistManager_cfi import *

# import config for missing-Et histogram managers
from TauAnalysis.Core.caloMEtHistManager_cfi import *
from TauAnalysis.Core.pfMEtHistManager_cfi import *

# import config for particle multiplicity histogram manager
from TauAnalysis.Core.particleMultiplicityHistManager_cfi import *

# import config for primary event vertex histogram manager
from TauAnalysis.Core.vertexHistManager_cfi import *

# import config for L1 & HLT histogram manager
from TauAnalysis.Core.triggerHistManager_cfi import *
triggerHistManagerForMuTau = copy.deepcopy(triggerHistManager)
triggerHistManagerForMuTau.pluginName = cms.string('triggerHistManagerForMuTau')
triggerHistManagerForMuTau.l1Bits = cms.vstring(
    'L1_SingleMu3',
    'L1_SingleMu5',
    'L1_SingleMu7',
    'L1_SingleMu10',
    'L1_SingleMu14'
)

triggerHistManagerForMuTau.hltPaths = cms.vstring(
    'HLT_Mu9',
    'HLT_IsoMu9',
    'HLT_Mu11',
    'HLT_Mu15'
)

# import config for event weight histogram manager
from TauAnalysis.Core.eventWeightHistManager_cfi import *

diTauCandidateCollinearApproxBinnerForMuTau = cms.PSet(
    pluginName = cms.string('diTauCandidateCollinearApproxBinnerForMuTau'),
    pluginType = cms.string('DataBinner'),
    binning = collinearApproxBinning,
    binningService = cms.PSet(
        pluginType = cms.string("DataBinningService")
    ),
    dqmDirectory_store = cms.string('collinearApproxBinningResults4regions')
)

# import config for binning results
# used to estimate acceptance of event selection
from TauAnalysis.Core.modelBinner_cfi import *
modelBinnerForMuTau = copy.deepcopy(modelBinner)
modelBinnerForMuTau.pluginName = cms.string('modelBinnerForMuTau')
modelBinnerForMuTau.srcGenFlag = cms.InputTag("isGenZtoMuTau")
modelBinnerForMuTau.srcRecFlag = cms.InputTag("isRecZtoMuTau")
modelBinnerForMuTau.dqmDirectory_store = cms.string('modelBinningResults')
modelBinnerForMuTauWithinAcceptance = copy.deepcopy(modelBinner)
modelBinnerForMuTauWithinAcceptance.pluginName = cms.string('modelBinnerForMuTauWithinAcceptance')
modelBinnerForMuTauWithinAcceptance.srcGenFlag = cms.InputTag("isGenZtoMuTauWithinAcceptance")
modelBinnerForMuTauWithinAcceptance.srcRecFlag = cms.InputTag("isRecZtoMuTau")
modelBinnerForMuTauWithinAcceptance.dqmDirectory_store = cms.string('modelBinningResultsWithinAcceptance')

# import config for binning results
# used to estimate systematic uncertainties
from TauAnalysis.Core.sysUncertaintyBinner_cfi import *
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *
sysUncertaintyBinnerForMuTau = copy.deepcopy(sysUncertaintyBinner)
sysUncertaintyBinnerForMuTau.pluginName = cms.string('sysUncertaintyBinnerForMuTau')
sysUncertaintyBinnerForMuTau.binning.systematics = cms.vstring(
    getSysUncertaintyNames(
        [ muonSystematics,
          tauSystematics,
          theorySystematics ]
    )
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
    src_individual = cms.InputTag('globalMuonCut', 'individual'),
    systematics = cms.vstring(muonSystematics.keys())
)
evtSelMuonEta = cms.PSet(
    pluginName = cms.string('evtSelMuonEta'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('muonEtaCut', 'cumulative'),
    src_individual = cms.InputTag('muonEtaCut', 'individual'),
    systematics = cms.vstring(muonSystematics.keys())
)
evtSelMuonPt = cms.PSet(
    pluginName = cms.string('evtSelMuonPt'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('muonPtCut', 'cumulative'),
    src_individual = cms.InputTag('muonPtCut', 'individual'),
    systematics = cms.vstring(muonSystematics.keys())
)

# tau acceptance cuts
evtSelTauAntiOverlapWithMuonsVeto = cms.PSet(
    pluginName = cms.string('evtSelTauAntiOverlapWithMuonsVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauAntiOverlapWithMuonsVeto', 'cumulative'),
    src_individual = cms.InputTag('tauAntiOverlapWithMuonsVeto', 'individual'),
    systematics = cms.vstring(tauSystematics.keys())
)
evtSelTauEta = cms.PSet(
    pluginName = cms.string('evtSelTauEta'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauEtaCut', 'cumulative'),
    src_individual = cms.InputTag('tauEtaCut', 'individual'),
    systematics = cms.vstring(tauSystematics.keys())
)
evtSelTauPt = cms.PSet(
    pluginName = cms.string('evtSelTauPt'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauPtCut', 'cumulative'),
    src_individual = cms.InputTag('tauPtCut', 'individual'),
    systematics = cms.vstring(tauSystematics.keys())
)

# muon candidate (isolation & id.) selection
evtSelMuonTrkIso = cms.PSet(
    pluginName = cms.string('evtSelMuonTrkIso'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('muonTrkIsoCut', 'cumulative'),
    src_individual = cms.InputTag('muonTrkIsoCut', 'individual'),
    systematics = cms.vstring(muonSystematics.keys())
)
evtSelMuonEcalIso = cms.PSet(
    pluginName = cms.string('evtSelMuonEcalIso'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('muonEcalIsoCut', 'cumulative'),
    src_individual = cms.InputTag('muonEcalIsoCut', 'individual'),
    systematics = cms.vstring(muonSystematics.keys())
)
evtSelMuonAntiPion = cms.PSet(
    pluginName = cms.string('evtSelMuonAntiPion'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('muonAntiPionCut', 'cumulative'),
    src_individual = cms.InputTag('muonAntiPionCut', 'individual'),
    systematics = cms.vstring(muonSystematics.keys())
)
evtSelMuonTrkIP = cms.PSet(
    pluginName = cms.string('evtSelMuonTrkIP'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('muonTrkIPcut', 'cumulative'),
    src_individual = cms.InputTag('muonTrkIPcut', 'individual'),
    systematics = cms.vstring(muonSystematics.keys())
)

# tau candidate (id.) selection
evtSelTauLeadTrk = cms.PSet(
    pluginName = cms.string('evtSelTauLeadTrk'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauLeadTrkCut', 'cumulative'),
    src_individual = cms.InputTag('tauLeadTrkCut', 'individual'),
    systematics = cms.vstring(tauSystematics.keys())
)
evtSelTauLeadTrkPt = cms.PSet(
    pluginName = cms.string('evtSelTauLeadTrkPt'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauLeadTrkPtCut', 'cumulative'),
    src_individual = cms.InputTag('tauLeadTrkPtCut', 'individual'),
    systematics = cms.vstring(tauSystematics.keys())
)
evtSelTauTrkIso = cms.PSet(
    pluginName = cms.string('evtSelTauTrkIso'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauTrkIsoCut', 'cumulative'),
    src_individual = cms.InputTag('tauTrkIsoCut', 'individual'),
    systematics = cms.vstring(tauSystematics.keys())
)
evtSelTauEcalIso = cms.PSet(
    pluginName = cms.string('evtSelTauEcalIso'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauEcalIsoCut', 'cumulative'),
    src_individual = cms.InputTag('tauEcalIsoCut', 'individual'),
    systematics = cms.vstring(tauSystematics.keys())
)
evtSelTauProng = cms.PSet(
    pluginName = cms.string('evtSelTauProng'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauProngCut', 'cumulative'),
    src_individual = cms.InputTag('tauProngCut', 'individual'),
    systematics = cms.vstring(tauSystematics.keys())
)
evtSelTauCharge = cms.PSet(
    pluginName = cms.string('evtSelTauCharge'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauChargeCut', 'cumulative'),
    src_individual = cms.InputTag('tauChargeCut', 'individual'),
    systematics = cms.vstring(tauSystematics.keys())
)
evtSelTauMuonVeto = cms.PSet(
    pluginName = cms.string('evtSelTauMuonVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauMuonVeto', 'cumulative'),
    src_individual = cms.InputTag('tauMuonVeto', 'individual'),
    systematics = cms.vstring(tauSystematics.keys())
)

# di-tau candidate selection
evtSelDiTauCandidateForMuTauAntiOverlapVeto = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForMuTauAntiOverlapVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForMuTauAntiOverlapVeto', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForMuTauAntiOverlapVeto', 'individual'),
    systematics = cms.vstring(muTauPairSystematics.keys())
)
evtSelDiTauCandidateForMuTauZeroCharge = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForMuTauZeroCharge'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForMuTauZeroChargeCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForMuTauZeroChargeCut', 'individual'),
    systematics = cms.vstring(muTauPairSystematics.keys())
    )
evtSelDiTauCandidateForMuTauAcoplanarity12 = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForMuTauAcoplanarity12'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForMuTauAcoplanarity12Cut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForMuTauAcoplanarity12Cut', 'individual'),
    systematics = cms.vstring(muTauPairSystematics.keys())
    )
evtSelDiTauCandidateForMuTauMt1MET = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForMuTauMt1MET'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForMuTauMt1METcut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForMuTauMt1METcut', 'individual'),
    systematics = cms.vstring(muTauPairSystematics.keys())
)
evtSelDiTauCandidateForMuTauPzetaDiff = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForMuTauPzetaDiff'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForMuTauPzetaDiffCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForMuTauPzetaDiffCut', 'individual'),
    systematics = cms.vstring(muTauPairSystematics.keys())
)

# veto events compatible with Z --> mu+ mu- hypothesis
# (based on reconstructed (visible) invariant mass of muon + muon pairs)
evtSelDiMuPairZmumuHypothesisVeto = cms.PSet(
    pluginName = cms.string('evtSelDiMuPairZmumuHypothesisVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('diMuPairZmumuHypothesisVeto')
)

#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------

muTauEventDump = cms.PSet(
    pluginName = cms.string('muTauEventDump'),
    pluginType = cms.string('MuTauEventDump'),

    # L1 trigger bits not contained in AOD;
    # in order to process Monte Carlo samples produced by FastSimulation,
    # disable histogram filling for now
    #l1GtReadoutRecordSource = cms.InputTag('hltGtDigis::HLT'),
    #l1GtObjectMapRecordSource = cms.InputTag('hltL1GtObjectMap::HLT'),
    l1GtReadoutRecordSource = cms.InputTag(''),
    l1GtObjectMapRecordSource = cms.InputTag(''),
    l1BitsToPrint = cms.vstring('L1_SingleMu3', 'L1_SingleMu5', 'L1_SingleMu7', 'L1_SingleMu10', 'L1_SingleMu14'),
    
    hltResultsSource = cms.InputTag('TriggerResults::HLT'),
    hltPathsToPrint = cms.vstring('HLT_Mu9', 'HLT_IsoMu9', 'HLT_Mu11', 'HLT_Mu15'),
        
    genParticleSource = cms.InputTag('genParticles'),
    genJetSource = cms.InputTag('iterativeCone5GenJets'),
    genTauJetSource = cms.InputTag('tauGenJets'),
    
    electronSource = cms.InputTag('cleanLayer1Electrons'),
    muonSource = cms.InputTag('cleanLayer1Muons'),
    tauSource = cms.InputTag('selectedLayer1TausPt20Cumulative'),
    diTauCandidateSource = cms.InputTag('allMuTauPairs'),
    muTauZmumuHypothesisSource = cms.InputTag('muTauPairZmumuHypotheses'),
    diMuZmumuHypothesisSource = cms.InputTag('allDiMuPairZmumuHypotheses'),
    jetSource = cms.InputTag('allLayer1Jets'),
    caloMEtSource = cms.InputTag('layer1METs'),
    pfMEtSource = cms.InputTag('layer1PFMETs'),
    genMEtSource = cms.InputTag('genMetTrue'),
    
    #output = cms.string("muTauEventDump.txt"),
    output = cms.string("std::cout"),
    
    #triggerConditions = cms.vstring("evtSelTauTrkIso: rejected_cumulative")
    triggerConditions = cms.vstring("evtSelDiMuPairZmumuHypothesisVeto: passed_cumulative")
)

#--------------------------------------------------------------------------------
# define analysis sequence
# (ordered list of event selection criteria and histogram filling)
#--------------------------------------------------------------------------------

muTauAnalysisSequence = cms.VPSet(
    # fill histograms for full event sample
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        )
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
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau',
            'modelBinnerForMuTau',
            'modelBinnerForMuTauWithinAcceptance'
        )
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
    #    analyzers = cms.vstring(
    #        'genPhaseSpaceEventInfoHistManager',
    #        'muonHistManager',
    #        'tauHistManager',
    #        'caloMEtHistManager',
    #        'pfMEtHistManager',
    #        'vertexHistManager',
    #        'triggerHistManagerForMuTau'
    #    )
    #),
    
    # trigger selection
    cms.PSet(
        filter = cms.string('evtSelTrigger'),
        title = cms.string('Muon Trigger'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        )
    ),

    # primary event vertex selection
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertex'),
        title = cms.string('Vertex'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'vertexHistManager'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexQuality'),
        title = cms.string('p(chi2Vertex) > 0.01'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexPosition'),
        title = cms.string('-25 < zVertex < +25 cm'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager'
        ),
    ),

    # muon acceptance cuts
    cms.PSet(
        filter = cms.string('evtSelGlobalMuon'),
        title = cms.string('global Muon'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsGlobalCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonEta'),
        title = cms.string('-2.1 < eta(Muon) < +2.1'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsEta21Cumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonPt'),
        title = cms.string('Pt(Muon) > 15 GeV'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsPt15Cumulative')
    ),

    # tau acceptance cuts
    cms.PSet(
        filter = cms.string('evtSelTauAntiOverlapWithMuonsVeto'),
        title = cms.string('Tau not overlapping w. Muon'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsPt15Cumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauAntiOverlapWithMuonsVetoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauEta'),
        title = cms.string('-2.1 < eta(Tau) < +2.1'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsPt15Cumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauEta21Cumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauPt'),
        title = cms.string('Pt(Tau) > 20 GeV'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        ),
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
        analyzers = cms.vstring(
            'muonHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIsoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonEcalIso'),
        title = cms.string('Muon ECAL iso.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsEcalIsoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonAntiPion'),
        title = cms.string('Muon pi-Veto'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsPionVetoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonTrkIP'),
        title = cms.string('Muon Track IP'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        ),
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
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauLeadTrkCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauLeadTrkPt'),
        title = cms.string('Tau lead. Track Pt'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauLeadTrkPtCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauTrkIso'),
        title = cms.string('Tau Track iso.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauTrkIsoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauEcalIso'),
        title = cms.string('Tau ECAL iso.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauEcalIsoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauProng'),
        title = cms.string('Tau 1||3-Prong'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauProngCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauCharge'),
        title = cms.string('Charge(Tau) = +/-1'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauChargeCumulative')
    ),    
    cms.PSet(
        filter = cms.string('evtSelTauMuonVeto'),
        title = cms.string('Tau mu-Veto'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        ),
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
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        ),
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
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauMuonVetoCumulative',
                              'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsZeroChargeCumulative'),
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForMuTauAcoplanarity12'),
        title = cms.string('Acoplanarity(Muon+Tau)'),
        saveRunEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauMuonVetoCumulative',
                              'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsAcoplanarity12Cumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForMuTauMt1MET'),
        title = cms.string('M_{T}(Muon-MET) < 50 GeV'),
        saveRunEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau'
        ),
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
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauMuonVetoCumulative',
                              'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsPzetaDiffCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiMuPairZmumuHypothesisVeto'),
        title = cms.string('not 80 < M (Muon-Muon) < 100 GeV'),
        saveRunEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'eventWeightHistManager',
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateCollinearApproxHistManagerForMuTau',
            'diTauCandidateCollinearApproxHistManagerBinnedForMuTau',
            'diTauCandidateCollinearApproxBinnerForMuTau',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau',
            'muPairHistManager',
            'jetHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'particleMultiplicityHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau',
            'sysUncertaintyBinnerForMuTau'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedLayer1MuonsTrkIPcumulative',
                              'tauHistManager.tauSource = selectedLayer1TausForMuTauMuonVetoCumulative',
                              'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsPzetaDiffCumulative',
                              'diTauCandidateHistManagerForMuTau.visMassHypothesisSource = muTauPairVisMassHypotheses',
                              'diTauCandidateZmumuHypothesisHistManagerForMuTau.ZllHypothesisSource = muTauPairZmumuHypotheses')
    )
)
