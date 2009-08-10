import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Core.genPhaseSpaceEventInfoHistManager_cfi import genPhaseSpaceEventInfoHistManager
from TauAnalysis.Core.electronHistManager_cfi import electronHistManager
from TauAnalysis.Core.pftauHistManager_cfi import tauHistManager
from TauAnalysis.Core.diTauCandidateHistManager_cfi import diTauCandidateHistManager
from TauAnalysis.Core.metHistManager_cfi import metHistManager
from TauAnalysis.Core.vertexHistManager_cfi import vertexHistManager
from TauAnalysis.Core.triggerHistManager_cfi import triggerHistManager
from TauAnalysis.Core.jetHistManager_cfi import jetHistManager
from TauAnalysis.Core.muonHistManager_cfi import muonHistManager

diTauCandidateHistManagerForElecTau = copy.deepcopy(diTauCandidateHistManager)
diTauCandidateHistManagerForElecTau.pluginName = cms.string('diTauCandidateHistManagerForElecTau')
diTauCandidateHistManagerForElecTau.pluginType = cms.string('PATElecTauPairHistManager')
diTauCandidateHistManagerForElecTau.diTauCandidateSource = cms.InputTag('allElecTauPairs')

diTauCandidateHistManagerForDiTau = copy.deepcopy(diTauCandidateHistManager)
diTauCandidateHistManagerForDiTau.pluginName = cms.string('diTauCandidateHistManagerForDiTau')
diTauCandidateHistManagerForDiTau.pluginType = cms.string('PATDiTauPairHistManager')
diTauCandidateHistManagerForDiTau.diTauCandidateSource = cms.InputTag('allDiTauPairs')

diTauCandidateHistManagerForMuTau = copy.deepcopy(diTauCandidateHistManager)
diTauCandidateHistManagerForMuTau.pluginName = cms.string('diTauCandidateHistManagerForMuTau')
diTauCandidateHistManagerForMuTau.pluginType = cms.string('PATMuTauPairHistManager')
diTauCandidateHistManagerForMuTau.diTauCandidateSource = cms.InputTag('allMuTauPairs')

diTauCandidateHistManagerForElecMu = copy.deepcopy(diTauCandidateHistManager)
diTauCandidateHistManagerForElecMu.pluginName = cms.string('diTauCandidateHistManagerForElecMu')
diTauCandidateHistManagerForElecMu.pluginType = cms.string('PATElecMuPairHistManager')
diTauCandidateHistManagerForElecMu.diTauCandidateSource = cms.InputTag('allElecMuPairs')

metHistManager.metSource = cms.InputTag("layer1PFMETs")

triggerHistManager.l1Bits = cms.vstring('L1_SingleEG5', 'L1_SingleEG8', 'L1_SingleEG10', 'L1_SingleEG12', 'L1_SingleEG15',
                                        'L1_SingleIsoEG5', 'L1_SingleIsoEG8', 'L1_SingleIsoEG10', 'L1_SingleIsoEG12', 'L1_SingleIsoEG15')
triggerHistManager.hltPaths = cms.vstring('HLT_IsoEle15_L1I','HLT_Ele10_SW_L1R')
triggerHistManager.l1GtObjectMapRecordSource = cms.InputTag('hltL1GtObjectMap::HLT')
triggerHistManager.l1GtReadoutRecordSource = cms.InputTag('hltGtDigis::HLT')

elecTauEventDump = cms.PSet(
    pluginName = cms.string('elecTauEventDump'),
    pluginType = cms.string('ElecTauEventDump'),

    l1GtReadoutRecordSource = cms.InputTag('hltGtDigis::HLT'),
    l1GtObjectMapRecordSource = cms.InputTag('hltL1GtObjectMap::HLT'),
    l1BitsToPrint = cms.vstring('L1_SingleEG5', 'L1_SingleEG8', 'L1_SingleEG10', 'L1_SingleEG12', 'L1_SingleEG15',
                                'L1_SingleIsoEG5', 'L1_SingleIsoEG8', 'L1_SingleIsoEG10', 'L1_SingleIsoEG12', 'L1_SingleIsoEG15'),
    
    hltResultsSource = cms.InputTag('TriggerResults::HLT'),
    hltPathsToPrint = cms.vstring('HLT_IsoEle15_L1I'),

    genParticleSource = cms.InputTag('genParticles'),
    genTauJetSource = cms.InputTag('tauGenJets'),
    electronSource = cms.InputTag('$last(electron)'),
    tauSource = cms.InputTag('$last(tau)'),
    diTauCandidateSource = cms.InputTag('$last(elecTauPair)'),
    metSource = cms.InputTag('$last(met)'),
    genMEtSource = cms.InputTag('genMETWithMu'),
    jetSource = cms.InputTag('$last(jet)'),
    recoTrackSource = cms.InputTag('generalTracks'),
    pfChargedHadronSource = cms.InputTag('pfAllChargedHadrons'),
    pfGammaSource = cms.InputTag('pfAllPhotons'),
    pfNeutralHadronSource = cms.InputTag('pfAllNeutralHadrons'),
    output = cms.string("std::cout"),
    triggerConditions = cms.vstring() # This currently has to be set manually, since $last(jet) would give you 'selected...Cumulative', which is wrong here. FIXME.
)
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
    electronSource = cms.InputTag('$last(electron)'),
    muonSource = cms.InputTag('$last(muon)'),
    tauSource = cms.InputTag('$last(tau)'),
    diTauCandidateSource = cms.InputTag('$last(diTauPair)'),
    metSource = cms.InputTag('$last(met)'),
    genMEtSource = cms.InputTag('genMETWithMu'),
    jetSource = cms.InputTag('$last(jet)'),
    recoTrackSource = cms.InputTag('generalTracks'),
    pfChargedHadronSource = cms.InputTag('pfAllChargedHadrons'),
    pfGammaSource = cms.InputTag('pfAllPhotons'),
    pfNeutralHadronSource = cms.InputTag('pfAllNeutralHadrons'),
    
    #output = cms.string("diTauEventDump.txt"),
    output = cms.string("std::cout"),
    
    triggerConditions = cms.vstring()
)
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
    electronSource = cms.InputTag('$last(electron)'),
    muonSource = cms.InputTag('$last(muon)'),
    tauSource = cms.InputTag('$last(tau)'),
    diTauCandidateSource = cms.InputTag('$last(elecMuPair)'),
    metSource = cms.InputTag('$last(met)'),
    genMEtSource = cms.InputTag('genMETWithMu'),
    jetSource = cms.InputTag('$last(jet)'),
    recoTrackSource = cms.InputTag('generalTracks'),
    pfChargedHadronSource = cms.InputTag('pfAllChargedHadrons'),
    pfGammaSource = cms.InputTag('pfAllPhotons'),
    pfNeutralHadronSource = cms.InputTag('pfAllNeutralHadrons'),

    #output = cms.string("elecMuEventDump.txt"),
    output = cms.string("std::cout"),

    triggerConditions = cms.vstring()
)
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
    electronSource = cms.InputTag('$last(electron)'),
    muonSource = cms.InputTag('$last(muon)'),
    tauSource = cms.InputTag('$last(tau)'),
    #tauSource = cms.InputTag('selectedLayer1TausForMuTauMuonVetoCumulative'),
    diTauCandidateSource = cms.InputTag('$last(muTauPair)'),
    metSource = cms.InputTag('$last(met)'),
    genMEtSource = cms.InputTag('genMETWithMu'),
    jetSource = cms.InputTag('$last(jet)'),
    recoTrackSource = cms.InputTag('generalTracks'),
    pfChargedHadronSource = cms.InputTag('pfAllChargedHadrons'),
    pfGammaSource = cms.InputTag('pfAllPhotons'),
    pfNeutralHadronSource = cms.InputTag('pfAllNeutralHadrons'),
    
    #output = cms.string("muTauEventDump.txt"),
    output = cms.string("std::cout"),
    
    #triggerConditions = cms.vstring("evtSelTauTrkIso: rejected_cumulative")
    triggerConditions = cms.vstring()
)
