import FWCore.ParameterSet.Config as cms
import copy

# import config for histogram managers
#from TauAnalysis.Core.genPhaseSpaceEventInfoHistManager_cfi import *
from TauAnalysis.Core.pftauHistManager_cfi import *
from TauAnalysis.Core.caloMEtHistManager_cfi import *
from TauAnalysis.Core.pfMEtHistManager_cfi import *
from TauAnalysis.Core.jetHistManager_cfi import *
from TauAnalysis.Core.vertexHistManager_cfi import *
from TauAnalysis.Core.pftauRecoilEnergyHistManager_cfi import *
from TauAnalysis.Core.metTopologyHistManager_cfi import*
from TauAnalysis.Core.tauNuCandidateHistManager_cfi import*
from TauAnalysis.Core.jetTauCandidateHistManager_cfi import*
from TauAnalysis.Core.electronHistManager_cfi import *
from TauAnalysis.Core.muonHistManager_cfi import *
from TauAnalysis.Core.eventShapeVarsHistManager_cfi import *
caloEventShapeVarsHistManager = copy.deepcopy(eventShapeVarsHistManager)
caloEventShapeVarsHistManager.pluginName = cms.string('caloEventShapeVarsHistManager')
caloEventShapeVarsHistManager.src = cms.InputTag('caloEventShapeVars')
caloEventShapeVarsHistManager.dqmDirectory_store = cms.string('CaloEventShapeVars')
pfEventShapeVarsHistManager = copy.deepcopy(eventShapeVarsHistManager)
pfEventShapeVarsHistManager.pluginName = cms.string('pfEventShapeVarsHistManager')
pfEventShapeVarsHistManager.src = cms.InputTag('pfEventShapeVars')
pfEventShapeVarsHistManager.dqmDirectory_store = cms.string('PFEventShapeVars')

wTauNuHistManagers = cms.vstring(
    'tauHistManager',
    'caloMEtHistManager',
    'pfMEtHistManager',
    'jetHistManager',
    'vertexHistManager',
    'tauRecoilEnergyFromJetsHistManager',
    'tauRecoilEnergyFromCaloTowersHistManager',
    'tauRecoilEnergyFromCentralCaloTowersHistManager',
    'tauRecoilEnergyFromForwardCaloTowersHistManager',
    'metTopologyHistManager',
    'tauNuCandidateHistManager',
    'jetTauCandidateHistManager',
    'caloEventShapeVarsHistManager',
    'pfEventShapeVarsHistManager'
    )


#--------------------------------------------------------------------------------
# define event selection criteria
#--------------------------------------------------------------------------------

# vertex selection
evtSelPrimaryEventVertexBoosted = cms.PSet(
    pluginName = cms.string('evtSelPrimaryEventVertexBoosted'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('primaryEventVertexBoosted')
    )
evtSelPrimaryEventVertexQualityBoosted = cms.PSet(
    pluginName = cms.string('evtSelPrimaryEventVertexQualityBoosted'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('primaryEventVertexQualityBoosted')
    )
evtSelPrimaryEventVertexPositionBoosted = cms.PSet(
    pluginName = cms.string('evtSelPrimaryEventVertexPositionBoosted'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('primaryEventVertexPositionBoosted')
    )

#tau selection
evtSelTauEtaBoosted = cms.PSet(
    pluginName = cms.string('evtSelTauEtaBoosted'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauEtaCutBoosted', 'cumulative'),#from selectWtoTauNu
    src_individual = cms.InputTag('tauEtaCutBoosted', 'individual')
    )
evtSelTauPtBoosted = cms.PSet(
    pluginName = cms.string('evtSelTauPtBoosted'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauPtCutBoosted', 'cumulative'),
    src_individual = cms.InputTag('tauPtCutBoosted', 'individual')
    )

evtSelPFMetPtBoosted = cms.PSet(
    pluginName = cms.string('evtSelPFMetPtBoosted'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('PFmetPtCutBoosted')
    )

evtSelMetPtBoosted = cms.PSet(
    pluginName = cms.string('evtSelMetPtBoosted'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('metPtCutBoosted')
    )

evtSelTauLeadTrkBoosted = cms.PSet(
    pluginName = cms.string('evtSelTauLeadTrkBoosted'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauLeadTrkCutBoosted', 'cumulative'),
    src_individual = cms.InputTag('tauLeadTrkCutBoosted', 'individual')
    )

evtSelTauLeadTrkPtBoosted = cms.PSet(
    pluginName = cms.string('evtSelTauLeadTrkPtBoosted'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauLeadTrkPtCutBoosted', 'cumulative'),
    src_individual = cms.InputTag('tauLeadTrkPtCutBoosted', 'individual')
    )

evtSelTauIsoBoosted = cms.PSet(
    pluginName = cms.string('evtSelTauIsoBoosted'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauEcalIsoBoosted', 'cumulative'),
    src_individual = cms.InputTag('tauEcalIsoBoosted', 'individual')
    )

evtSelTauTaNCBoosted = cms.PSet(
    pluginName = cms.string('evtSelTauTaNCBoosted'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauTrkIsoBoosted', 'cumulative'),
    src_individual = cms.InputTag('tauTrkIsoBoosted', 'individual')
    )

evtSelTauMuonVetoBoosted = cms.PSet(
    pluginName = cms.string('evtSelTauMuonVetoBoosted'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauMuonVetoBoosted', 'cumulative'),
    src_individual = cms.InputTag('tauMuonVetoBoosted', 'individual')
    )

evtSelTauElectronVetoBoosted = cms.PSet(
    pluginName = cms.string('evtSelTauElectronVetoBoosted'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauElectronVetoBoosted', 'cumulative'),
    src_individual = cms.InputTag('tauElectronVetoBoosted', 'individual')
    )

evtSelTauEcalCrackVetoBoosted = cms.PSet(
    pluginName = cms.string('evtSelTauEcalCrackVetoBoosted'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauEcalCrackVetoBoosted', 'cumulative'),
    src_individual = cms.InputTag('tauEcalCrackVetoBoosted', 'individual')
    )

evtSelTauProngBoosted = cms.PSet(
    pluginName = cms.string('evtSelTauProngBoosted'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauProngCutBoosted', 'cumulative'),
    src_individual = cms.InputTag('tauProngCutBoosted', 'individual')
    )

evtSelTauChargeBoosted = cms.PSet(
    pluginName = cms.string('evtSelTauChargeBoosted'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauChargeCutBoosted', 'cumulative'),
    src_individual = cms.InputTag('tauChargeCutBoosted', 'individual')
    )

# require exactly one extra jet with pt>15 GeV in event
evtSelRecoilJetBoosted = cms.PSet(
    pluginName = cms.string('evtSelRecoilJetBoosted'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('recoilJetBoosted')
    )

evtSelCentralJetVetoBoosted = cms.PSet(
    pluginName = cms.string('evtSelCentralJetVetoBoosted'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('centralJetVetoBoosted')
    )

evtSelPhiJetTauBoosted = cms.PSet(
    pluginName = cms.string('evtSelPhiJetTauBoosted'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('phiJetTauBoosted', 'cumulative'),
    src_individual = cms.InputTag('phiJetTauBoosted', 'individual')
    )

evtSelPhiMetTauBoosted = cms.PSet(
    pluginName = cms.string('evtSelPhiMetTauBoosted'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('phiMetTauBoosted', 'cumulative'),
    src_individual = cms.InputTag('phiMetTauBoosted', 'individual')
    )

evtSelPhiJetMetBoosted = cms.PSet(
    pluginName = cms.string('evtSelPhiJetMetBoosted'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('phiJetMetBoosted', 'cumulative'),
    src_individual = cms.InputTag('phiJetMetBoosted', 'individual')
    )

evtSelMetTopologyBoosted = cms.PSet(
    pluginName = cms.string('evtSelMetTopologyBoosted'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('metTopologyCutBoosted')
    )

evtSelMetSignificanceBoosted = cms.PSet(
    pluginName = cms.string('evtSelMetSignificanceBoosted'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('metSignificanceCutBoosted')
    )


#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------

wTauNuBoostedEventDump = cms.PSet(
    pluginName = cms.string('wTauNuEventDump'),
    pluginType = cms.string('wTauNuEventDump'),
    
    genParticleSource = cms.InputTag('genParticles'),
    genJetSource = cms.InputTag('iterativeCone5GenJets'),
    genTauJetSource = cms.InputTag('tauGenJets'),
    genEventInfosource = cms.InputTag('generator'),
    tauSource = cms.InputTag('selectedPatTausForWTauNuEcalCrackVetoCumulative'),
    metSource = cms.InputTag('layer1METs'),
    jetSource = cms.InputTag('selectedPatJetsEt15ForWTauNuCumulative'),
    genMEtSource = cms.InputTag('genMetTrue'),
    recoTrackSource = cms.InputTag('generalTracks'),
    pfChargedHadronSource = cms.InputTag('pfAllChargedHadrons'),
    pfGammaSource = cms.InputTag('pfAllPhotons'),
    pfNeutralHadronSource = cms.InputTag('pfAllNeutralHadrons'),
    output = cms.string("wTauNu_boosted_EventDump.txt"),
    triggerConditions = cms.vstring("evtSelCentralJetVetoBoosted: passed_cumulative")
    )

#replace met with pfmet for met-plots and event dump
#metHistManager.metSource = cms.InputTag("layer1PFMETs")
#wTauNuEventDump.metSource = cms.InputTag('layer1PFMETs')

#--------------------------------------------------------------------------------
# define analysis sequence
# (ordered list of event selection criteria and histogram filling)
#--------------------------------------------------------------------------------

wTauNuBoostedAnalysisSequence = cms.VPSet(
    # fill histograms for full event sample
    cms.PSet(
    analyzers = wTauNuHistManagers
    ),
    
    # vertex selection
    cms.PSet(
    filter = cms.string('evtSelPrimaryEventVertexBoosted'),
    title = cms.string('Vertex'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    analyzers = wTauNuHistManagers
    ),
    cms.PSet(
    filter = cms.string('evtSelPrimaryEventVertexQualityBoosted'),
    title = cms.string('p(chi2Vertex) > 0.01'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    analyzers = wTauNuHistManagers
    ),
    cms.PSet(
    filter = cms.string('evtSelPrimaryEventVertexPositionBoosted'),
    title = cms.string('-50 < Vertex < +50 cm'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    analyzers = wTauNuHistManagers
    ),
    
    #primary tau selection
    cms.PSet(
    filter = cms.string('evtSelTauEtaBoosted'),
    title = cms.string('-2.1 < eta(Tau) < +2.1'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    analyzers = wTauNuHistManagers,
    replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuEta21Cumulative')
    ),
    cms.PSet(
    filter = cms.string('evtSelTauPtBoosted'),
    title = cms.string('25 < Pt(Tau) < 60 GeV'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    analyzers = wTauNuHistManagers,
    replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuPt20Cumulative')
    ),
    cms.PSet(
    filter = cms.string('evtSelPFMetPtBoosted'),
    title = cms.string('PF-MET > 40 GeV'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    analyzers = wTauNuHistManagers,
    replace = cms.vstring('')
    ),
    cms.PSet(
    filter = cms.string('evtSelMetPtBoosted'),
    title = cms.string('MET > 0 GeV'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    analyzers = wTauNuHistManagers,
    replace = cms.vstring('metHistManager.metSource = selectedPatMETsPt15')
    ),
    cms.PSet(
    filter = cms.string('evtSelTauLeadTrkBoosted'),
    title = cms.string('with leadtrk'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    analyzers = wTauNuHistManagers,
    replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuLeadTrkCumulative')
    ),
    cms.PSet(
    filter = cms.string('evtSelTauLeadTrkPtBoosted'),
    title = cms.string('leadtrk pt > 20 GeV'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    analyzers = wTauNuHistManagers,
    replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuLeadTrkPtCumulative')
    ),
    cms.PSet(
    filter = cms.string('evtSelTauIsoBoosted'),
    title = cms.string('TaNC discriminator'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    analyzers = wTauNuHistManagers,
    replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuEcalIsoCumulative')
    ),
    cms.PSet(
    filter = cms.string('evtSelTauTaNCBoosted'),
    title = cms.string('discrimination by isolation'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    analyzers = wTauNuHistManagers,
    replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuTrkIsoCumulative')
    ),
    cms.PSet(
    filter = cms.string('evtSelTauProngBoosted'),
    title = cms.string('Tau 1||3-Prong'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    analyzers = wTauNuHistManagers,
    replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuProngCumulative')
    ),
    cms.PSet(
    filter = cms.string('evtSelTauChargeBoosted'),
    title = cms.string('Charge(Tau) = +/-1'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    analyzers = wTauNuHistManagers,
    replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuChargeCumulative')
    ),
    cms.PSet(
    filter = cms.string('evtSelTauMuonVetoBoosted'),
    title = cms.string('Muon veto'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    analyzers = wTauNuHistManagers,
    replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuMuonVetoCumulative')
    ),
    cms.PSet(
    filter = cms.string('evtSelTauElectronVetoBoosted'),
    title = cms.string('Electron veto'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(

    analyzers = wTauNuHistManagers,
    replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuElectronVetoCumulative')
    ),
    
    cms.PSet(
    filter = cms.string('evtSelTauEcalCrackVetoBoosted'),
    title = cms.string('Ecal Crack veto'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    analyzers = wTauNuHistManagers,
    replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuEcalCrackVetoCumulative',
                          'jetHistManager.jetSource = selectedPatJetsEt15ForWTauNuCumulative',
                          'tauRecoilEnergyFromJetsHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromJets',
                          'tauRecoilEnergyFromCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromCaloTowers',
                          'tauRecoilEnergyFromCentralCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromCentralCaloTowers',
                          'tauRecoilEnergyFromForwardCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromForwardCaloTowers',
                          'jetTauCandidateHistManager.diTauCandidateSource = allJetTauPairs',
                          'tauNuCandidateHistManager.tauNuCandidateSource = allTauNuPairs')
    ),
    cms.PSet(
    filter = cms.string('evtSelRecoilJetBoosted'),
    title = cms.string('recoil jet'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    analyzers = wTauNuHistManagers,
    replace = cms.vstring('jetHistManager.jetSource = selectedPatJetsEt15ForWTauNuCumulative',
                          'tauRecoilEnergyFromJetsHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromJets',
                          'tauRecoilEnergyFromCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromCaloTowersPt10',
                          'tauRecoilEnergyFromCentralCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromCentralCaloTowers',
                          'tauRecoilEnergyFromForwardCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromForwardCaloTowers',
                          'jetTauCandidateHistManager.diTauCandidateSource = allJetTauPairs',
                          'tauNuCandidateHistManager.tauNuCandidateSource = allTauNuPairs')
    ),
    cms.PSet(
    filter = cms.string('evtSelCentralJetVetoBoosted'),
    title = cms.string('central jet veto'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    analyzers = wTauNuHistManagers,
    replace = cms.vstring('jetHistManager.jetSource = selectedPatJetsEt15ForWTauNuCumulative',
                          'tauRecoilEnergyFromJetsHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromJets',
                          'tauRecoilEnergyFromCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromCaloTowersPt10',
                          'tauRecoilEnergyFromCentralCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromCentralCaloTowers',
                          'tauRecoilEnergyFromForwardCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromForwardCaloTowers',
                          'jetTauCandidateHistManager.diTauCandidateSource = allJetTauPairs',
                          'tauNuCandidateHistManager.tauNuCandidateSource = allTauNuPairs')
    ),
  cms.PSet(
        filter = cms.string('evtSelPhiJetTauBoosted'),
        title = cms.string('phi(jet,tau) > 0.1'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
        ),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('jetHistManager.jetSource = selectedPatJetsEt15ForWTauNuCumulative',
                              'tauRecoilEnergyFromJetsHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromJets',
                              'tauRecoilEnergyFromCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromCaloTowersPt10',
                              'tauRecoilEnergyFromCentralCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromCentralCaloTowers',
                              'tauRecoilEnergyFromForwardCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromForwardCaloTowers',
                              'jetTauCandidateHistManager.diTauCandidateSource = allJetTauPairs',
                              'tauNuCandidateHistManager.tauNuCandidateSource = allTauNuPairs')
        ),
    cms.PSet(
        filter = cms.string('evtSelPhiMetTauBoosted'),
        title = cms.string('phi(MET,tau) > 0.3'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
        ),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('jetHistManager.jetSource = selectedPatJetsEt15ForWTauNuCumulative',
                              'tauRecoilEnergyFromJetsHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromJets',
                              'tauRecoilEnergyFromCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromCaloTowersPt10',
                              'tauRecoilEnergyFromCentralCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromCentralCaloTowers',
                              'tauRecoilEnergyFromForwardCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromForwardCaloTowers',
                              'jetTauCandidateHistManager.diTauCandidateSource = allJetTauPairs',
                              'tauNuCandidateHistManager.tauNuCandidateSource = allTauNuPairs')
        ),
  cms.PSet(
        filter = cms.string('evtSelPhiJetMetBoosted'),
        title = cms.string('phi(jet,MET) > 0.3'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
        ),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('jetHistManager.jetSource = selectedPatJetsEt15ForWTauNuCumulative',
                              'tauRecoilEnergyFromJetsHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromJets',
                              'tauRecoilEnergyFromCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromCaloTowersPt10',
                              'tauRecoilEnergyFromCentralCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromCentralCaloTowers',
                              'tauRecoilEnergyFromForwardCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromForwardCaloTowers',
                              'jetTauCandidateHistManager.diTauCandidateSource = allJetTauPairs',
                              'tauNuCandidateHistManager.tauNuCandidateSource = allTauNuPairs')
        ),
    cms.PSet(
        filter = cms.string('evtSelMetTopologyBoosted'),
        title = cms.string('MET-topology < 0.4'),
        saveRunLumiSectionEventNumbers = cms.vstring('passed_cumulative')
        ),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('jetHistManager.jetSource = selectedPatJetsEt15ForWTauNuCumulative',
                              'tauRecoilEnergyFromJetsHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromJets',
                              'tauRecoilEnergyFromCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromCaloTowersPt10',
                              'tauRecoilEnergyFromCentralCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromCentralCaloTowers',
                              'tauRecoilEnergyFromForwardCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromForwardCaloTowers',
                              'jetTauCandidateHistManager.diTauCandidateSource = allJetTauPairs',
                              'tauNuCandidateHistManager.tauNuCandidateSource = allTauNuPairs')
    ),
    cms.PSet(
        filter = cms.string('evtSelMetSignificanceBoosted'),
        title = cms.string('met signif > 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('jetHistManager.jetSource = selectedPatJetsEt15ForWTauNuCumulative',
                              'tauRecoilEnergyFromJetsHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromJets',
                              'tauRecoilEnergyFromCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromCaloTowers',
                              'tauRecoilEnergyFromCentralCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromCentralCaloTowers',
                              'tauRecoilEnergyFromForwardCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromForwardCaloTowers',
                              'jetTauCandidateHistManager.diTauCandidateSource = allJetTauPairs',
                              'tauNuCandidateHistManager.tauNuCandidateSource = allTauNuPairs')
    )
    
    )


