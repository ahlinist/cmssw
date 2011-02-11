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
    'tauRecoilEnergyFromCaloTowersHistManager',
    'metTopologyHistManager',
    'tauNuCandidateHistManager',
    'caloEventShapeVarsHistManager',
    'pfEventShapeVarsHistManager'
)


#--------------------------------------------------------------------------------
# define event selection criteria
#--------------------------------------------------------------------------------

# trigger selection
evtSelTrigger = cms.PSet(
    pluginName = cms.string('evtSelTrigger'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('Trigger')
    )

# vertex selection
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

#tau selection
evtSelTauEta = cms.PSet(
    pluginName = cms.string('evtSelTauEta'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauEtaCut', 'cumulative'),#from selectWtoTauNu
    src_individual = cms.InputTag('tauEtaCut', 'individual')
)
evtSelTauPt = cms.PSet(
    pluginName = cms.string('evtSelTauPt'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauPtCut', 'cumulative'),
    src_individual = cms.InputTag('tauPtCut', 'individual')
)

evtSelPFMetPt = cms.PSet(
    pluginName = cms.string('evtSelPFMetPt'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('PFmetPtCut')
    )

evtSelMetPt = cms.PSet(
    pluginName = cms.string('evtSelMetPt'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('metPtCut')
    )

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

evtSelTauIso = cms.PSet(
    pluginName = cms.string('evtSelTauIso'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauEcalIso', 'cumulative'),
    src_individual = cms.InputTag('tauEcalIso', 'individual')
    )

evtSelTauTaNC = cms.PSet(
    pluginName = cms.string('evtSelTauTaNC'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauTrkIso', 'cumulative'),
    src_individual = cms.InputTag('tauTrkIso', 'individual')
    )

evtSelTauMuonVeto = cms.PSet(
    pluginName = cms.string('evtSelTauMuonVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauMuonVeto', 'cumulative'),
    src_individual = cms.InputTag('tauMuonVeto', 'individual')
    )

evtSelTauElectronVeto = cms.PSet(
    pluginName = cms.string('evtSelTauElectronVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauElectronVeto', 'cumulative'),
    src_individual = cms.InputTag('tauElectronVeto', 'individual')
    )

evtSelTauEcalCrackVeto = cms.PSet(
    pluginName = cms.string('evtSelTauEcalCrackVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauEcalCrackVeto', 'cumulative'),
    src_individual = cms.InputTag('tauEcalCrackVeto', 'individual')
    )

evtSelTauProng = cms.PSet(
    pluginName = cms.string('evtSelTauProng'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauProngCut', 'cumulative'),
    src_individual = cms.InputTag('tauProngCut', 'individual')
)

evtSelTauCharge = cms.PSet(
    pluginName = cms.string('evtSelTauCharge'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauChargeCut', 'cumulative'),
    src_individual = cms.InputTag('tauChargeCut', 'individual')
)

# veto events containing additional central jets with Et > 20 GeV
evtSelCentralJetVeto = cms.PSet(
    pluginName = cms.string('evtSelCentralJetVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('centralJetVeto')
    )

evtSelRecoilEnergyFromCaloTowers = cms.PSet(
    pluginName = cms.string('evtSelRecoilEnergyFromCaloTowers'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('recoilEnergyFromCaloTowersCut')
)

evtSelMetTopology = cms.PSet(
    pluginName = cms.string('evtSelMetTopology'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('metTopologyCut')
)

#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------

wTauNuEventDump = cms.PSet(
    pluginName = cms.string('wTauNuEventDump'),
    pluginType = cms.string('wTauNuEventDump'),

    genParticleSource = cms.InputTag('genParticles'),
    genJetSource = cms.InputTag('iterativeCone5GenJets'),
    genTauJetSource = cms.InputTag('tauGenJets'),
    genEventInfoSource = cms.InputTag('generator'),
    
    tauSource = cms.InputTag('selectedPatTausForWTauNuEcalCrackVetoCumulative'),
    metSource = cms.InputTag('patMETs'),
    jetSource = cms.InputTag('selectedPatJetsEt20ForWTauNuCumulative'),
    genMEtSource = cms.InputTag('genMetTrue'),
    recoTrackSource = cms.InputTag('generalTracks'),
    pfChargedHadronSource = cms.InputTag('pfAllChargedHadrons'),
    pfGammaSource = cms.InputTag('pfAllPhotons'),
    pfNeutralHadronSource = cms.InputTag('pfAllNeutralHadrons'),    
    output = cms.string("wTauNuEventDump.txt"),
    triggerConditions = cms.vstring("evtSelRecoilEnergyFromCaloTowers: passed_cumulative")
)

#replace met with pfmet for met-plots and event dump
#metHistManager.metSource = cms.InputTag("patPFMETs")
#wTauNuEventDump.metSource = cms.InputTag('patPFMETs')

#--------------------------------------------------------------------------------
# define analysis sequence
# (ordered list of event selection criteria and histogram filling)
#--------------------------------------------------------------------------------

wTauNuAnalysisSequence = cms.VPSet(
    # fill histograms for full event sample
    cms.PSet(
        analyzers = wTauNuHistManagers
    ),
    # trigger selection
    cms.PSet(
    filter = cms.string('evtSelTrigger'),
    title = cms.string('Tau+MET Trigger'),
    saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
    analyzers = wTauNuHistManagers
    ),
    #vertex selection
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertex'),
        title = cms.string('Vertex'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = wTauNuHistManagers
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexQuality'),
        title = cms.string('Vertex quality'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = wTauNuHistManagers
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexPosition'),
        title = cms.string('Vertex position'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = wTauNuHistManagers
    ),

    #primary tau selection
    cms.PSet(
        filter = cms.string('evtSelTauEta'),
        title = cms.string('|eta(Tau)| < +2.3'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuEta21Cumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauPt'),
        title = cms.string('Pt(Tau) > 30 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuPt20Cumulative')
	), 
    cms.PSet(
        filter = cms.string('evtSelPFMetPt'),
        title = cms.string('PF-MET > 30 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
	),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('')
	), 
    cms.PSet(
        filter = cms.string('evtSelMetPt'),
        title = cms.string('calo-MET cut (switch off)'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
	),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('metHistManager.metSource = selectedPatMETsPt15')
	), 
    cms.PSet(
        filter = cms.string('evtSelTauLeadTrk'),
        title = cms.string('with leadtrk'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
	),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuLeadTrkCumulative')
	), 
    cms.PSet(
        filter = cms.string('evtSelTauLeadTrkPt'),
        title = cms.string('leadtrk pt > 20 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
	),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuLeadTrkPtCumulative')
	), 
    cms.PSet(
        filter = cms.string('evtSelTauIso'),
        title = cms.string('TaNC disctiminator'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuEcalIsoCumulative')
	),
    cms.PSet(
        filter = cms.string('evtSelTauTaNC'),
        title = cms.string('Isolation (switch off)'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuTrkIsoCumulative')
	),
    cms.PSet(
        filter = cms.string('evtSelTauProng'),
        title = cms.string('Tau 1||3-Prong'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
	),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuProngCumulative')
	),
    cms.PSet(
        filter = cms.string('evtSelTauCharge'),
        title = cms.string('Charge(Tau) = +/-1'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
	),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuChargeCumulative')
	),
    cms.PSet(
        filter = cms.string('evtSelTauMuonVeto'),
        title = cms.string('Muon veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
	),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuMuonVetoCumulative')	
	),
    cms.PSet(
        filter = cms.string('evtSelTauElectronVeto'),
        title = cms.string('Electron veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuElectronVetoCumulative')	
	),
   
    cms.PSet(
        filter = cms.string('evtSelTauEcalCrackVeto'),
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
                              'tauNuCandidateHistManager.tauNuCandidateSource = allTauNuPairs')	
        ),
    cms.PSet(
        filter = cms.string('evtSelCentralJetVeto'),
	title = cms.string('central jet veto'),
	saveRunLumiSectionEventNumbers = cms.vstring('')
	),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('tauRecoilEnergyFromJetsHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromJets', 
                              'jetHistManager.jetSource = selectedPatJetsEt15ForWTauNuCumulative',
                              'tauRecoilEnergyFromCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromCaloTowers',
                              'tauRecoilEnergyFromCentralCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromCentralCaloTowers',
                              'tauRecoilEnergyFromForwardCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromForwardCaloTowers',
                              'tauNuCandidateHistManager.tauNuCandidateSource = allTauNuPairs')	
        ),
    cms.PSet(
	filter = cms.string('evtSelRecoilEnergyFromCaloTowers'),
	title = cms.string('recoil energy (switch off)'),
	saveRunLumiSectionEventNumbers = cms.vstring('')
	),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('tauRecoilEnergyFromJetsHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromJets', 
                              'jetHistManager.jetSource = selectedPatJetsEt15ForWTauNuCumulative',
                              'tauRecoilEnergyFromCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromCaloTowers',
                              'tauRecoilEnergyFromCentralCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromCentralCaloTowers',
                              'tauRecoilEnergyFromForwardCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromForwardCaloTowers',
                              'tauNuCandidateHistManager.tauNuCandidateSource = allTauNuPairs')	
        ),
 cms.PSet(
        filter = cms.string('evtSelMetTopology'),
        title = cms.string('MET-topology < 0.25'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
        ),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('jetHistManager.jetSource = selectedPatJetsEt15ForWTauNuCumulative',
                              'tauRecoilEnergyFromJetsHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromJets',
                              'tauRecoilEnergyFromCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromCaloTowers',
                              'tauRecoilEnergyFromCentralCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromCentralCaloTowers',
                              'tauRecoilEnergyFromForwardCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromForwardCaloTowers',
                              'tauNuCandidateHistManager.tauNuCandidateSource = allTauNuPairs')
    )

    )

