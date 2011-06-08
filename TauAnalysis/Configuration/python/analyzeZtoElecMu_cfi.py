import FWCore.ParameterSet.Config as cms
import copy

# import config for histogram manager filling information about phase-space simulated in Monte Carlo sample
from TauAnalysis.Core.genPhaseSpaceEventInfoHistManager_cfi import *

# import config for electron histogram manager
from TauAnalysis.Core.electronHistManager_cfi import *
#electronHistManager.electronSource = cms.InputTag('selectedPatElectronsIdCumulative') #no crappy electrons in plots!
electronHistManager.electronSource = cms.InputTag('cleanPatElectrons')

# import config for muon histogram manager
from TauAnalysis.Core.muonHistManager_cfi import *
muonHistManager.muonSource = cms.InputTag('selectedPatMuonsGlobalCumulative') #no crappy muons in plots!

# import config for tau-jet histogram manager
from TauAnalysis.Core.pftauHistManager_cfi import *
tauHistManager.tauSource = cms.InputTag('selectedPatTausForElecMuCumulative')

# import config for di-tau histogram manager
from TauAnalysis.Core.diTauCandidateHistManager_cfi import *
diTauCandidateHistManagerForElecMu = copy.deepcopy(diTauCandidateHistManager)
diTauCandidateHistManagerForElecMu.pluginName = cms.string('diTauCandidateHistManagerForElecMu')
diTauCandidateHistManagerForElecMu.pluginType = cms.string('PATElecMuPairHistManager')
diTauCandidateHistManagerForElecMu.diTauCandidateSource = cms.InputTag('allElecMuPairs')
diTauCandidateHistManagerForElecMu.visMassHypothesisSource = cms.InputTag('')

from TauAnalysis.Core.diTauCandidateNSVfitHistManager_cfi import *
diTauCandidateNSVfitHistManagerForElecMu = copy.deepcopy(diTauCandidateNSVfitHistManager)
diTauCandidateNSVfitHistManagerForElecMu.pluginName = cms.string('diTauCandidateNSVfitHistManagerForElecMu')
diTauCandidateNSVfitHistManagerForElecMu.pluginType = cms.string('PATElecMuPairNSVfitHistManager')
diTauCandidateNSVfitHistManagerForElecMu.diTauCandidateSource = cms.InputTag('allElecMuPairs')
diTauCandidateNSVfitHistManagerForElecMu.nSVfitEventHypotheses = cms.PSet(
    psKine_MEt_logM_fit = cms.string('psKine_MEt_logM_fit'),
    psKine_MEt_logM_int = cms.string('psKine_MEt_logM_int')
)

from TauAnalysis.Core.diTauCandidateZllHypothesisHistManager_cfi import *
diTauCandidateZmumuHypothesisHistManagerForElecMu = copy.deepcopy(ZllHypothesisHistManager)
diTauCandidateZmumuHypothesisHistManagerForElecMu.pluginName = cms.string('diTauCandidateZmumuHypothesisHistManagerForElecMu')
diTauCandidateZmumuHypothesisHistManagerForElecMu.pluginType = cms.string('ZllHypothesisElecMuHistManager')
diTauCandidateZmumuHypothesisHistManagerForElecMu.ZllHypothesisSource = cms.InputTag('elecMuPairZmumuHypotheses')
diTauCandidateZmumuHypothesisHistManagerForElecMu.dqmDirectory_store = cms.string('DiTauCandidateZmumuHypothesisQuantities')

# import config for central jet veto histogram manager
from TauAnalysis.Core.jetHistManager_cfi import *
jetHistManager.centralJetsToBeVetoed.etMin = cms.vdouble()
jetHistManager.centralJetsToBeVetoed.etaMax = cms.vdouble()
jetHistManager.centralJetsToBeVetoed.alphaMin = cms.vdouble()
jetHistManager.jetSource = cms.InputTag('selectedPatJetsAntiOverlapWithLeptonsVetoCumulative')

# import config for missing-Et histogram managers
from TauAnalysis.Core.caloMEtHistManager_cfi import *
caloMEtHistManager.leg1Source = cms.InputTag('selectedPatElectronsIdCumulative')
caloMEtHistManager.leg2Source = cms.InputTag('selectedPatMuonsGlobalCumulative')
caloMEtHistManager.metSource = cms.InputTag("patMETs")
from TauAnalysis.Core.pfMEtHistManager_cfi import *
pfMEtHistManager.leg1Source = cms.InputTag('selectedPatElectronsIdCumulative')
pfMEtHistManager.leg2Source = cms.InputTag('selectedPatMuonsGlobalCumulative')
pfMEtHistManager.metSource = cms.InputTag("patPFMETs")

# import config for particle multiplicity histogram manager
from TauAnalysis.Core.particleMultiplicityHistManager_cfi import *

# import config for primary event vertex histogram manager
from TauAnalysis.Core.vertexHistManager_cfi import *

# import config for L1 & HLT histogram manager
from TauAnalysis.Core.triggerHistManager_cfi import *
triggerHistManagerForElecMu = copy.deepcopy(triggerHistManager)
triggerHistManagerForElecMu.pluginName = cms.string('triggerHistManagerForElecMu')
triggerHistManagerForElecMu.l1Bits = cms.vstring(
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

triggerHistManagerForElecMu.hltPaths = cms.vstring(
    'HLT_Ele15_SW_EleId_L1R',
    'HLT_Mu9',
)

# import config for event weight histogram manager
from TauAnalysis.Core.eventWeightHistManager_cfi import *

elecMuHistManagers = cms.vstring( 'electronHistManager',
                                  'muonHistManager',
                                  'tauHistManager',
                                  'diTauCandidateHistManagerForElecMu',
                                  'diTauCandidateNSVfitHistManagerForElecMu',
                                  'caloMEtHistManager',
                                  'pfMEtHistManager',
                                  'jetHistManager',
                                  'vertexHistManager',
                                  'triggerHistManagerForElecMu'
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
# electron candidate selection
evtSelElectronIdMin = cms.PSet(
    pluginName = cms.string('evtSelElectronIdMin'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronIdMinCut','cumulative'),
    src_individual = cms.InputTag('electronIdMinCut','individual')
)
evtSelElectronIdMax = cms.PSet(
    pluginName = cms.string('evtSelElectronIdMax'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronIdMaxCut','cumulative'),
    src_individual = cms.InputTag('electronIdMaxCut','individual')
)
evtSelElectronAntiCrack = cms.PSet(
    pluginName = cms.string('evtSelElectronAntiCrack'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronAntiCrackCut','cumulative'),
    src_individual = cms.InputTag('electronAntiCrackCut','individual')
)
evtSelElectronEta = cms.PSet(
    pluginName = cms.string('evtSelElectronEta'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronEtaCut','cumulative'),
    src_individual = cms.InputTag('electronEtaCut','individual')
)
evtSelElectronPt = cms.PSet(
    pluginName = cms.string('evtSelElectronPt'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronPtCut','cumulative'),
    src_individual = cms.InputTag('electronPtCut','individual')
)
evtSelElectronIso = cms.PSet(
    pluginName = cms.string('evtSelElectronIso'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronIsoCut','cumulative'),
    src_individual = cms.InputTag('electronIsoCut','individual')
)
evtSelElectronTrk = cms.PSet(
    pluginName = cms.string('evtSelElectronTrk'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronTrkCut','cumulative'),
    src_individual = cms.InputTag('electronTrkCut','individual')
    #src = cms.InputTag('electronTrkCut','individual')
)
evtSelElectronTrkIP = cms.PSet(
    pluginName = cms.string('evtSelElectronTrkIP'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronTrkIPCut','cumulative'),
    src_individual = cms.InputTag('electronTrkIPCut','individual')
    #src = cms.InputTag('electronTrkIPCut','individual')
)
# muon candidate selection
evtSelGlobalMuonMin = cms.PSet(
    pluginName = cms.string('evtSelGlobalMuonMin'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('globalMuonMinCut','cumulative'),
    src_individual = cms.InputTag('globalMuonMinCut','individual')
)
evtSelGlobalMuonMax = cms.PSet(
    pluginName = cms.string('evtSelGlobalMuonMax'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('globalMuonMaxCut','cumulative'),
    src_individual = cms.InputTag('globalMuonMaxCut','individual')
)
evtSelMuonEta = cms.PSet(
    pluginName = cms.string('evtSelMuonEta'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('muonEtaCut','cumulative'),
    src_individual = cms.InputTag('muonEtaCut','individual')
)
evtSelMuonPt = cms.PSet(
    pluginName = cms.string('evtSelMuonPt'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('muonPtCut','cumulative'),
    src_individual = cms.InputTag('muonPtCut','individual')
)
evtSelMuonIso = cms.PSet(
    pluginName = cms.string('evtSelMuonIso'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('muonIsoCut','cumulative'),
    src_individual = cms.InputTag('muonIsoCut','individual')
)
evtSelMuonAntiPion = cms.PSet(
    pluginName = cms.string('evtSelMuonAntiPion'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('muonAntiPionCut','cumulative'),
    src_individual = cms.InputTag('muonAntiPionCut','individual')
)
evtSelMuonTrkIP = cms.PSet(
    pluginName = cms.string('evtSelMuonTrkIP'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('muonTrkIPCut','cumulative'),
    src_individual = cms.InputTag('muonTrkIPCut','individual')
)
# di-tau candidate selection
evtSelDiTauCandidateForElecMuDRmin = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForElecMuDRmin'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForElecMuDRminCut','cumulative'),
    src_individual = cms.InputTag('diTauCandidateForElecMuDRminCut','individual')
)
evtSelDiTauCandidateForElecMuZeroCharge = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForElecMuZeroCharge'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForElecMuZeroChargeCut','cumulative'),
    src_individual = cms.InputTag('diTauCandidateForElecMuZeroChargeCut','individual')
)
evtSelDiTauCandidateForElecMuMt1MET = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForElecMuMt1MET'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForElecMuMt1METCut','cumulative'),
    src_individual = cms.InputTag('diTauCandidateForElecMuMt1METCut','individual')
)
evtSelDiTauCandidateForElecMuMt2MET = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForElecMuMt2MET'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForElecMuMt2METCut','cumulative'),
    src_individual = cms.InputTag('diTauCandidateForElecMuMt2METCut','individual')
)
evtSelDiTauCandidateForElecMuDPhi = cms.PSet(
   pluginName = cms.string('evtSelDiTauCandidateForElecMuDPhi'),
   pluginType = cms.string('BoolEventSelector'),
   src_cumulative = cms.InputTag('diTauCandidateForElecMuDPhiCut','cumulative'),
   src_individual = cms.InputTag('diTauCandidateForElecMuDPhiCut','individual')
)
evtSelDiTauCandidateForElecMuOneLegPt = cms.PSet(
   pluginName = cms.string('evtSelDiTauCandidateForElecMuOneLegPt'),
   pluginType = cms.string('BoolEventSelector'),
   src_cumulative = cms.InputTag('diTauCandidateForElecMuOneLegPtCut','cumulative'),
   src_individual = cms.InputTag('diTauCandidateForElecMuOneLegPtCut','individual')
)
# jet candidate selection
evtSelJetMin = cms.PSet(
    pluginName = cms.string('evtSelJetMin'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('jetMinCut')
)
evtSelJetMax = cms.PSet(
    pluginName = cms.string('evtSelJetMax'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('jetMaxCut')
)
evtSelJetBtagMin = cms.PSet(
    pluginName = cms.string('evtSelJetBtagMin'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('jetBtagMinCut')
)
evtSelJetBtagMax = cms.PSet(
    pluginName = cms.string('evtSelJetBtagMax'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('jetBtagMaxCut')
)
# met selection
evtSelMETMax = cms.PSet(
    pluginName = cms.string('evtSelMETMax'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('metMaxCut')
)

#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------

elecMuEventDump = cms.PSet(
    pluginName = cms.string('elecMuEventDump'),
    pluginType = cms.string('ElecMuEventDump'),

    # L1 trigger bits not contained in AOD;
    # in order to process Monte Carlo samples produced by FastSimulation,
    # disable histogram filling for now
    #l1GtReadoutRecordSource = cms.InputTag('hltGtDigis::HLT'),
    #l1GtObjectMapRecordSource = cms.InputTag('hltL1GtObjectMap::HLT'),
    l1GtReadoutRecordSource = cms.InputTag(''),
    l1GtObjectMapRecordSource = cms.InputTag(''),
    l1BitsToPrint = cms.vstring('L1_SingleEG5', 'L1_SingleEG8', 'L1_SingleEG10', 'L1_SingleEG12', 'L1_SingleEG15',
                                'L1_SingleIsoEG5', 'L1_SingleIsoEG8', 'L1_SingleIsoEG10', 'L1_SingleIsoEG12', 'L1_SingleIsoEG15',
                                'L1_SingleMu3', 'L1_SingleMu5', 'L1_SingleMu7', 'L1_SingleMu10', 'L1_SingleMu14'),

    hltResultsSource = cms.InputTag('TriggerResults::HLT'),
    hltPathsToPrint = cms.vstring('HLT_Ele15_SW_EleId_L1R','HLT_Mu9'),
        
    genParticleSource = cms.InputTag('genParticles'),
    genJetSource = cms.InputTag('ak5GenJets'),
    genTauJetSource = cms.InputTag('tauGenJets'),
    genEventInfoSource = cms.InputTag('generator'),
    
    #electronSource = cms.InputTag('cleanPatElectrons'),
    electronSource = cms.InputTag('selectedPatElectronsIdCumulative'),
    #muonSource = cms.InputTag('cleanPatMuons'),
    muonSource = cms.InputTag('selectedPatMuonsGlobalCumulative'),
    tauSource = cms.InputTag('cleanPatTaus'),
    diTauCandidateSource = cms.InputTag('allElecMuPairs'),
    jetSource = cms.InputTag('selectedPatJetsAntiOverlapWithLeptonsVetoCumulative'),
    caloMEtSource = cms.InputTag('patMETs'),
    pfMEtSource = cms.InputTag('patPFMETs'),
    genMEtSource = cms.InputTag('genMetTrue'),

    #output = cms.string("elecMuEventDump.txt"),
    output = cms.string("std::cout"),

    triggerConditions = cms.vstring("evtSelDiTauCandidateForElecMuMt2MET: passed_cumulative")
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
    # (NOTE: (1) to be used in case of Monte Carlo samples
    #            overlapping in simulated phase-space only !!
    #        (2) genPhaseSpaceCut needs to be **always** the first entry in the list of cuts
    #           - otherwise the script submitToBatch.csh for submission of cmsRun jobs
    #            to the CERN batch system will not work !!)
    cms.PSet(
        filter = cms.string('genPhaseSpaceCut'),
        title = cms.string('gen. Phase-Space'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
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
    #    saveRunLumiSectionEventNumbers = cms.vstring('')
    #),
    #cms.PSet(
    #    filter = cms.string('genMuonCut'),
    #    title = cms.string('gen. Muon'),
    #    saveRunLumiSectionEventNumbers = cms.vstring('')
    #),
    #cms.PSet(
    #    analyzers = cms.vstring(
    #        'genPhaseSpaceEventInfoHistManager',
    #        'electronHistManager',
    #        'muonHistManager',
    #        'caloMEtHistManager',
    #        'pfMEtHistManager',
    #        'vertexHistManager',
    #        'triggerHistManagerForElecMu'
    #    )
    #),

    # trigger selection
    cms.PSet(
        filter = cms.string('evtSelTrigger'),
        title = cms.string('Trigger'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers
    ),

    # primary event vertex selection
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertex'),
        title = cms.string('Vertex'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexQuality'),
        title = cms.string('Vertex quality'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexPosition'),
        title = cms.string('Vertex position'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers
    ),

    #only 1 electron and 1 muon
    cms.PSet(
        filter = cms.string('evtSelElectronIdMin'),
        title = cms.string('electron Id. Min'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsIdCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelElectronIdMax'),
        title = cms.string('electron Id. Max'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsIdCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelGlobalMuonMin'),
        title = cms.string('global Muon min'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsIdCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsGlobalCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelGlobalMuonMax'),
        title = cms.string('global Muon max'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsIdCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsGlobalCumulative')
    ),

    #dR and Zero Charge cuts
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForElecMuDRmin'),
        title = cms.string('DeltaR(Elec,Muon) > 0.5'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsIdCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsGlobalCumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsDRminCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForElecMuZeroCharge'),
        title = cms.string('Charge(Elec+Muon) = 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsPionVetoCumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsZeroChargeCumulative')
    ),

    # kinematic cuts
    cms.PSet(
        filter = cms.string('evtSelElectronAntiCrack'),
        title = cms.string('crack-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsAntiCrackCutCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsGlobalCumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsZeroChargeCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelElectronEta'),
        title = cms.string('-2.4 < eta(Elec) < +2.4'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsEtaCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsGlobalCumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsZeroChargeCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonEta'),
        title = cms.string('-2.1 < eta(Muon) < +2.1'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsEtaCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsEtaCumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsZeroChargeCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelElectronPt'),
        title = cms.string('Pt(Electron) Cut'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsPtCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsEtaCumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsZeroChargeCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonPt'),
        title = cms.string('Pt(Muon) Cut'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsEtaCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsPtCumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsZeroChargeCumulative')
    ),

    #electron isolation
    cms.PSet(
        filter = cms.string('evtSelElectronIso'),
        title = cms.string('Electron iso.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsIsoCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsPtCumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsZeroChargeCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelElectronTrk'),
        title = cms.string('Electron Track find.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsPtCumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsZeroChargeCumulative')
    ),

    # muon isolation
    cms.PSet(
        filter = cms.string('evtSelMuonIso'),
        title = cms.string('Muon iso.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsIsoCumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsZeroChargeCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonAntiPion'),
        title = cms.string('Muon pi-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsPionVetoCumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsZeroChargeCumulative')
    ),

    #selection of electron + muon combinations
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForElecMuMt1MET'),
        title = cms.string('MT(Elec+MET) Cut'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsPionVetoCumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsMt1METcumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForElecMuMt2MET'),
        title = cms.string('MT(Mu+MET) Cut'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsPionVetoCumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsMt2METcumulative')
    ),
    cms.PSet(
       filter = cms.string('evtSelDiTauCandidateForElecMuDPhi'),
       #title = cms.string('DPhi(Elec,Muon) < 3.07'),
       title = cms.string('DPhi(Elec,Muon) > 2.'),
       saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
       analyzers = elecMuHistManagers,
       replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
                             'muonHistManager.muonSource = selectedPatMuonsPionVetoCumulative',
                             'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsDPhiCumulative')
    ),
##     cms.PSet(
##        filter = cms.string('evtSelDiTauCandidateForElecMuOneLegPt'),
##        title = cms.string('leg1.pt > 20 | leg2.pt > 20'),
##        saveRunLumiSectionEventNumbers = cms.vstring('')
##     ),
##     cms.PSet(
##        analyzers = elecMuHistManagers,
##        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
##                              'muonHistManager.muonSource = selectedPatMuonsPionVetoCumulative',
##                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsOneLegPtCumulative')
##     ),
    
    #selection of met
    cms.PSet(
        filter = cms.string('evtSelMETMax'),
        title = cms.string('met < 60 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsPionVetoCumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsDPhiCumulative',
                              'diTauCandidateNSVfitHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsDPhiCumulative',
                              #'jetHistManager.jetSource = selectedPatJetsBtagCumulative',
                              'pfMEtHistManager.metSource = selectedMETMaxCumulative')
    ),
    
    #selection of jets
    cms.PSet(
        filter = cms.string('evtSelJetMin'),
        title = cms.string('Jet min'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsPionVetoCumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsDPhiCumulative',
                              'diTauCandidateNSVfitHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsDPhiCumulative',
                              'pfMEtHistManager.metSource = selectedMETMaxCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelJetMax'),
        title = cms.string('Jet max'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsPionVetoCumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsDPhiCumulative',
                              'diTauCandidateNSVfitHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsDPhiCumulative',
                              'pfMEtHistManager.metSource = selectedMETMaxCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelJetBtagMin'),
        title = cms.string('b-tagging min cut'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsPionVetoCumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsDPhiCumulative',
                              'diTauCandidateNSVfitHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsDPhiCumulative',
                              'pfMEtHistManager.metSource = selectedMETMaxCumulative',
                              'jetHistManager.jetSource = selectedPatJetsBtagCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelJetBtagMax'),
        title = cms.string('b-tagging max cut'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsPionVetoCumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsDPhiCumulative',
                              'diTauCandidateNSVfitHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsDPhiCumulative',
                              'pfMEtHistManager.metSource = selectedMETMaxCumulative',
                              'jetHistManager.jetSource = selectedPatJetsBtagCumulative')
    )

)

