import FWCore.ParameterSet.Config as cms
import copy

# import config for electron histogram manager
from TauAnalysis.Core.electronHistManager_cfi import *
electronHistManager.electronSource = cms.InputTag('selectedPatElectronsIdCumulative') #no crappy electrons in plots!

# import config for muon histogram manager
from TauAnalysis.Core.muonHistManager_cfi import *
muonHistManager.muonSource = cms.InputTag('selectedPatMuonsGlobalCumulative') #no crappy muons in plots!

# import config for di-tau histogram manager
from TauAnalysis.Core.diTauCandidateHistManager_cfi import *
diTauCandidateHistManagerForElecMu = copy.deepcopy(diTauCandidateHistManager)
diTauCandidateHistManagerForElecMu.pluginName = cms.string('diTauCandidateHistManagerForElecMu')
diTauCandidateHistManagerForElecMu.pluginType = cms.string('PATElecMuPairHistManager')
diTauCandidateHistManagerForElecMu.diTauCandidateSource = cms.InputTag('allElecMuPairs')

# import config for central jet veto histogram manager
from TauAnalysis.Core.jetHistManager_cfi import *
jetHistManager.centralJetsToBeVetoed.etMin = cms.vdouble()
jetHistManager.centralJetsToBeVetoed.etaMax = cms.vdouble()
jetHistManager.centralJetsToBeVetoed.alphaMin = cms.vdouble()
#jetHistManager.jetSource = cms.InputTag('selectedPatJetsEtaCumulative')
jetHistManager.jetSource = cms.InputTag('cleanPatJets')

# import config for missing-Et histogram managers
from TauAnalysis.Core.caloMEtHistManager_cfi import *
from TauAnalysis.Core.pfMEtHistManager_cfi import *
caloMEtHistManager.metSource = cms.InputTag("layer1METs")

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
                                  'caloMEtHistManager',
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
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

# passing basic acceptance and kinematic cuts
# (NOTE: to be used for efficiency studies only !!)
#genElectronCut = cms.PSet(
#    pluginName = cms.string('genElectronCut'),
#    pluginType = cms.string('TauGenJetMinEventSelector'),
#    src = cms.InputTag('selectedGenTauDecaysToElectronPtCumulative'),
#    minNumber = cms.uint32(1)
#)
#genMuonCut = cms.PSet(
#    pluginName = cms.string('genMuonCut'),
#    pluginType = cms.string('TauGenJetMinEventSelector'),
#    src = cms.InputTag('selectedGenTauDecaysToMuonPtCumulative'),
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
#evtSelElectronTrkIso = cms.PSet(
#    pluginName = cms.string('evtSelElectronTrkIso'),
#    pluginType = cms.string('BoolEventSelector'),
#    src_cumulative = cms.InputTag('electronTrkIsoCut','cumulative'),
#    src_individual = cms.InputTag('electronTrkIsoCut','individual')
#)
#evtSelElectronEcalIso = cms.PSet(
#    pluginName = cms.string('evtSelElectronEcalIso'),
#    pluginType = cms.string('BoolEventSelector'),
#    src_cumulative = cms.InputTag('electronEcalIsoCut','cumulative'),
#    src_individual = cms.InputTag('electronEcalIsoCut','individual')
#)
#evtSelElectronHcalIso = cms.PSet(
#    pluginName = cms.string('evtSelElectronHcalIso'),
#    pluginType = cms.string('BoolEventSelector'),
#    src_cumulative = cms.InputTag('electronHcalIsoCut','cumulative'),
#    src_individual = cms.InputTag('electronHcalIsoCut','individual')
#)
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
#evtSelMuonTrkIso = cms.PSet(
#    pluginName = cms.string('evtSelMuonTrkIso'),
#    pluginType = cms.string('BoolEventSelector'),
#    src_cumulative = cms.InputTag('muonTrkIsoCut','cumulative'),
#    src_individual = cms.InputTag('muonTrkIsoCut','individual')
#)
#evtSelMuonEcalIso = cms.PSet(
#    pluginName = cms.string('evtSelMuonEcalIso'),
#    pluginType = cms.string('BoolEventSelector'),
#    src_cumulative = cms.InputTag('muonEcalIsoCut','cumulative'),
#    src_individual = cms.InputTag('muonEcalIsoCut','individual')
#)
#evtSelMuonHcalIso = cms.PSet(
#    pluginName = cms.string('evtSelMuonHcalIso'),
#    pluginType = cms.string('BoolEventSelector'),
#    src_cumulative = cms.InputTag('muonHcalIsoCut','cumulative'),
#    src_individual = cms.InputTag('muonHcalIsoCut','individual')
#)
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
evtSelDiTauCandidateForElecMuZeroCharge = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForElecMuZeroCharge'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForElecMuZeroChargeCut','cumulative'),
    src_individual = cms.InputTag('diTauCandidateForElecMuZeroChargeCut','individual')
)
## evtSelDiTauCandidateForElecMuAcoplanarity = cms.PSet(
##     pluginName = cms.string('evtSelDiTauCandidateForElecMuAcoplanarity'),
##     pluginType = cms.string('BoolEventSelector'),
##     src_cumulative = cms.InputTag('diTauCandidateForElecMuAcoplanarityCut','cumulative'),
##     src_individual = cms.InputTag('diTauCandidateForElecMuAcoplanarityCut','individual')
## )
evtSelDiTauCandidateForElecMuDPhi = cms.PSet(
   pluginName = cms.string('evtSelDiTauCandidateForElecMuDPhi'),
   pluginType = cms.string('BoolEventSelector'),
   src_cumulative = cms.InputTag('diTauCandidateForElecMuDPhiCut','cumulative'),
   src_individual = cms.InputTag('diTauCandidateForElecMuDPhiCut','individual')
)
#evtSelDiTauCandidateForElecMuImpParamSig = cms.PSet(
#    pluginName = cms.string('evtSelDiTauCandidateForElecMuImpParamSig'),
#    pluginType = cms.string('BoolEventSelector'),
#    src_cumulative = cms.InputTag('diTauCandidateForElecMuImpParamSigCut','cumulative'),
#    src_individual = cms.InputTag('diTauCandidateForElecMuImpParamSigCut','individual')
#)
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

    l1GtReadoutRecordSource = cms.InputTag('hltGtDigis::HLT'),
    l1GtObjectMapRecordSource = cms.InputTag('hltL1GtObjectMap::HLT'),
    l1BitsToPrint = cms.vstring('L1_SingleEG5', 'L1_SingleEG8', 'L1_SingleEG10', 'L1_SingleEG12', 'L1_SingleEG15',
                                'L1_SingleIsoEG5', 'L1_SingleIsoEG8', 'L1_SingleIsoEG10', 'L1_SingleIsoEG12', 'L1_SingleIsoEG15',
                                'L1_SingleMu3', 'L1_SingleMu5', 'L1_SingleMu7', 'L1_SingleMu10', 'L1_SingleMu14'),
    
    hltResultsSource = cms.InputTag('TriggerResults::HLT'),
    hltPathsToPrint = cms.vstring('HLT_IsoEle15_L1I', 'HLT_Mu15', 'HLT_IsoMu11', 'HLT_IsoEle8_IsoMu7'),

    genParticleSource = cms.InputTag('genParticles'),
    genTauJetSource = cms.InputTag('tauGenJets'),
    electronSource = cms.InputTag('cleanPatElectrons'),
    #electronSource = cms.InputTag('selectedPatElectronsTrkIPcumulative'),
    muonSource = cms.InputTag('cleanPatMuons'),
    #muonSource = cms.InputTag('selectedPatMuonsTrkIPcumulative'),
    #tauSource = cms.InputTag('allLayer1Taus'),
    diTauCandidateSource = cms.InputTag('allElecMuPairs'),
    metSource = cms.InputTag('layer1METs'),
    genMEtSource = cms.InputTag('genMetTrue'),
    #jetSource = cms.InputTag('selectedPatJetsEtaCumulative'),
    recoTrackSource = cms.InputTag('generalTracks'),
    pfChargedHadronSource = cms.InputTag('pfAllChargedHadrons'),
    pfGammaSource = cms.InputTag('pfAllPhotons'),
    pfNeutralHadronSource = cms.InputTag('pfAllNeutralHadrons'),

    #output = cms.string("elecMuEventDump.txt"),
    output = cms.string("std::cout"),
    
    triggerConditions = cms.vstring("evtSelDiTauCandidateForElecMuZeroCharge: passed_cumulative")
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

    # generator level selection of Z --> e + mu events
    # passing basic acceptance and kinematic cuts
    # (NOTE: to be used for efficiency studies only !!)
    #cms.PSet(
    #    filter = cms.string('genElectronCut'),
    #    title = cms.string('gen. Electron'),
    #),
    #cms.PSet(
    #    filter = cms.string('genMuonCut'),
    #    title = cms.string('gen. Muon'),
    #),
    #cms.PSet(
    #    analyzers = elecMuHistManagers
    #),
    
    # trigger selection
    cms.PSet(
        filter = cms.string('evtSelTrigger'),
        title = cms.string('Elec || Muon Triggers'),
        saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers
    ),

    # primary event vertex selection
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertex'),
        title = cms.string('Vertex'),
        saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexQuality'),
        title = cms.string('p(chi2Vertex) > 0.01'),
        saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexPosition'),
        title = cms.string('-50 < zVertex < +50 cm'),
        saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers
    ),

    #only 1 electron and 1 muon
    cms.PSet(
        filter = cms.string('evtSelElectronIdMin'),
        title = cms.string('electron Id. Min'),
        saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsIdCumulative')
    ),
    #cms.PSet(
    #    filter = cms.string('evtSelElectronIdMax'),
    #    title = cms.string('electron Id. Max'),
    #    saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    #),
    #cms.PSet(
    #    analyzers = elecMuHistManagers,
    #    replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsIdCumulative')
    #),
    cms.PSet(
        filter = cms.string('evtSelGlobalMuonMin'),
        title = cms.string('global Muon min'),
        saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsIdCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsGlobalCumulative')
    ),
    #cms.PSet(
    #    filter = cms.string('evtSelGlobalMuonMax'),
    #    title = cms.string('global Muon max'),
    #    saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    #),
    #cms.PSet(
    #    analyzers = elecMuHistManagers,
    #    replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsIdCumulative',
    #                          'muonHistManager.muonSource = selectedPatMuonsGlobalCumulative')
    #),

    # kinematic cuts
    cms.PSet(
        filter = cms.string('evtSelElectronAntiCrack'),
        title = cms.string('crack-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsAntiCrackCutCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsGlobalCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelElectronEta'),
        title = cms.string('-2.4 < eta(Elec) < +2.4'),
        saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsEtaCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsGlobalCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonEta'),
        title = cms.string('-2.4 < eta(Muon) < +2.4'),
        saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsEtaCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsEtaCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelElectronPt'),
        title = cms.string('Pt(Electron) Cut'),
        saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsPtCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsEtaCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonPt'),
        title = cms.string('Pt(Muon) Cut'),
        saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsEtaCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsPtCumulative')
    ),

    #electron isolation
    cms.PSet(
        filter = cms.string('evtSelElectronIso'),
        title = cms.string('Electron iso.'),
        saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsIsoCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsPtCumulative')
    ),
    #cms.PSet(
    #    filter = cms.string('evtSelElectronTrkIso'),
    #    title = cms.string('Electron Track iso.'),
    #    saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    #),
    #cms.PSet(
    #    analyzers = elecMuHistManagers,
    #    replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkIsoCumulative',
    #                          'muonHistManager.muonSource = selectedPatMuonsPtCumulative')
    #),
    #cms.PSet(
    #    filter = cms.string('evtSelElectronEcalIso'),
    #    title = cms.string('Electron ECAL iso.'),
    #    saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    #),
    #cms.PSet(
    #    analyzers = elecMuHistManagers,
    #    replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsEcalIsoCumulative',
    #                          'muonHistManager.muonSource = selectedPatMuonsPtCumulative')
    #),
    #cms.PSet(
    #    filter = cms.string('evtSelElectronHcalIso'),
    #    title = cms.string('Electron HCAL iso.'),
    #    saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    #),
    #cms.PSet(
    #    analyzers = elecMuHistManagers,
    #    replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsHcalIsoCumulative',
    #                          'muonHistManager.muonSource = selectedPatMuonsPtCumulative')
    #),
    cms.PSet(
        filter = cms.string('evtSelElectronTrk'),
        title = cms.string('Electron Track find.'),
        saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsPtCumulative')
    ),
    #cms.PSet(
    #    filter = cms.string('evtSelElectronTrkIP'),
    #    title = cms.string('Electron Track IP cut.'),
    #    saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    #),
    #cms.PSet(
    #    analyzers = elecMuHistManagers,
    #    replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkIPcumulative',
    #                          'muonHistManager.muonSource = selectedPatMuonsPtCumulative')
    #),

    # muon isolation
    cms.PSet(
        filter = cms.string('evtSelMuonIso'),
        title = cms.string('Muon iso.'),
        saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsIsoCumulative')
    ),
    #cms.PSet(
    #    filter = cms.string('evtSelMuonTrkIso'),
    #    title = cms.string('Muon Track iso.'),
    #    saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    #),
    #cms.PSet(
    #    analyzers = elecMuHistManagers,
    #    replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
    #                          'muonHistManager.muonSource = selectedPatMuonsTrkIsoCumulative')
    #),
    #cms.PSet(
    #    filter = cms.string('evtSelMuonEcalIso'),
    #    title = cms.string('Muon ECAL iso.'),
    #    saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    #),
    #cms.PSet(
    #    analyzers = elecMuHistManagers,
    #    replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
    #                          'muonHistManager.muonSource = selectedPatMuonsEcalIsoCumulative')
    #),
    #cms.PSet(
    #    filter = cms.string('evtSelMuonHcalIso'),
    #    title = cms.string('Muon HCAL iso.'),
    #    saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    #),
    #cms.PSet(
    #    analyzers = elecMuHistManagers,
    #    replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
    #                          'muonHistManager.muonSource = selectedPatMuonsHcalIsoCumulative')
    #),
    cms.PSet(
        filter = cms.string('evtSelMuonAntiPion'),
        title = cms.string('Muon pi-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsPionVetoCumulative')
    ),
    #cms.PSet(
    #    filter = cms.string('evtSelMuonTrkIP'),
    #    title = cms.string('Muon Track IP cut.'),
    #    saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    #),
    #cms.PSet(
    #    analyzers = elecMuHistManagers,
    #    replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
    #                          'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative')
    #),

    #selection of electron + muon combinations
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForElecMuZeroCharge'),
        title = cms.string('Charge(Elec+Muon) = 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsPionVetoCumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsZeroChargeCumulative')
    ),
##     cms.PSet(
##         filter = cms.string('evtSelDiTauCandidateForElecMuAcoplanarity'),
##         title = cms.string('cos(dPhi(lepton-MET)) > 0.5'),
##         saveRunLumiSectionEventNumbers = cms.vstring('')
##     ),
##     cms.PSet(
##         analyzers = elecMuHistManagers,
##         replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
##                               'muonHistManager.muonSource = selectedPatMuonsPionVetoCumulative',
##                               'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsAcoplanarityCumulative')
##     ),
    cms.PSet(
       filter = cms.string('evtSelDiTauCandidateForElecMuDPhi'),
       #title = cms.string('DPhi(Elec,Muon) < 3.07'),
       title = cms.string('DPhi(Elec,Muon) > 2.'),
       saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    ),
    cms.PSet(
       analyzers = elecMuHistManagers,
       replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
                             'muonHistManager.muonSource = selectedPatMuonsPionVetoCumulative',
                             'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsDPhiCumulative')
    ),
    #cms.PSet(
    #    filter = cms.string('evtSeliTauCandidateForElecMuImpParamSig'),
    #    title = cms.string('Imp.Param. Significance'),
    #    saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    #),
    #cms.PSet(
    #    analyzers = elecMuHistManagers,
    #    replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
    #                          'muonHistManager.muonSource = selectedPatMuonsPionVetoCumulative',
    #                          'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsImpParamSigCumulative')
    #),
    cms.PSet(
       filter = cms.string('evtSelDiTauCandidateForElecMuOneLegPt'),
       title = cms.string('leg1.pt > 20 | leg2.pt > 20'),
       saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    ),
    cms.PSet(
       analyzers = elecMuHistManagers,
       replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
                             'muonHistManager.muonSource = selectedPatMuonsPionVetoCumulative',
                             'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsOneLegPtCumulative')
    ),
    

    #selection of met
    cms.PSet(
        filter = cms.string('evtSelMETMax'),
        title = cms.string('met < 60 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsPionVetoCumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsOneLegPtCumulative',
                              #'jetHistManager.jetSource = selectedPatJetsBtagCumulative',
                              'metHistManager.metSource = selectedMETMaxCumulative')
    ),

    #selection of jets
    cms.PSet(
        filter = cms.string('evtSelJetMin'),
        title = cms.string('Jet min'),
        saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsPionVetoCumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsOneLegPtCumulative',
                              'jetHistManager.jetSource = selectedPatJetsEt20Cumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelJetMax'),
        title = cms.string('Jet max'),
        saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsPionVetoCumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsOneLegPtCumulative',
                              'metHistManager.metSource = selectedMETMaxCumulative',
                              'jetHistManager.jetSource = selectedPatJetsEt20Cumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelJetBtagMin'),
        title = cms.string('b-tagging min cut'),
        saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsPionVetoCumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsOneLegPtCumulative',
                              'metHistManager.metSource = selectedMETMaxCumulative',
                              'jetHistManager.jetSource = selectedPatJetsBtagCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelJetBtagMax'),
        title = cms.string('b-tagging max cut'),
        saveRunLumiSectionEventNumbers = cms.vstring('exclRejected', 'passed_cumulative')
    ),
    cms.PSet(
        analyzers = elecMuHistManagers,
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsTrkCumulative',
                              'muonHistManager.muonSource = selectedPatMuonsPionVetoCumulative',
                              'diTauCandidateHistManagerForElecMu.diTauCandidateSource = selectedElecMuPairsOneLegPtCumulative',
                              'metHistManager.metSource = selectedMETMaxCumulative',
                              'jetHistManager.jetSource = selectedPatJetsBtagCumulative')
    )

)

