import FWCore.ParameterSet.Config as cms
import copy

# import config for histogram managers
#from TauAnalysis.Core.genPhaseSpaceEventInfoHistManager_cfi import *
from TauAnalysis.Core.pftauHistManager_cfi import *
from TauAnalysis.Core.metHistManager_cfi import *
from TauAnalysis.Core.jetHistManager_cfi import *
from TauAnalysis.Core.vertexHistManager_cfi import *
from TauAnalysis.Core.pftauRecoilEnergyHistManager_cfi import *
from TauAnalysis.Core.metTopologyHistManager_cfi import*

#to make plots for pfmet
#metHistManager.metSource = cms.InputTag("layer1PFMETs") 

wTauNuHistManagers = cms.vstring(
    'tauHistManager',
    'metHistManager',
    'jetHistManager',
    'vertexHistManager',
    'tauRecoilEnergyFromJetsHistManager',
    'metTopologyHistManager'
)

#--------------------------------------------------------------------------------
# define event selection criteria
#--------------------------------------------------------------------------------

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

evtSelTauProng = cms.PSet(
    pluginName = cms.string('evtSelTauProng'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauProngCut', 'cumulative'),
    src_individual = cms.InputTag('tauProngCut', 'individual')
)

evtSelRecoilEnergy = cms.PSet(
    pluginName = cms.string('evtSelRecoilEnergy'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('recoilEnergyCut')
)

# veto events containing additional central jets with Et > 20 GeV
evtSelCentralJetVeto = cms.PSet(
    pluginName = cms.string('evtSelCentralJetVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('centralJetVeto')
    )


#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------

wTauNuEventDump = cms.PSet(
    pluginName = cms.string('wTauNuEventDump'),
    pluginType = cms.string('MuTauEventDump'),

    genParticleSource = cms.InputTag('genParticles'),
    genTauJetSource = cms.InputTag('tauGenJets'),
    tauSource = cms.InputTag('cleanLayer1TausSel'),
    metSource = cms.InputTag('layer1METs'),
    genMEtSource = cms.InputTag('genMETWithMu'),
    jetSource = cms.InputTag('selectedLayer1JetsEt20Cumulative'),
    recoTrackSource = cms.InputTag('generalTracks'),
    pfChargedHadronSource = cms.InputTag('pfAllChargedHadrons'),
    pfGammaSource = cms.InputTag('pfAllPhotons'),
    pfNeutralHadronSource = cms.InputTag('pfAllNeutralHadrons'),    
    output = cms.string("muTauEventDump.txt"),
    #output = cms.string("std::cout"),    
    triggerConditions = cms.vstring("evtSelTauTaNC: passed_cumulative")
    #triggerConditions = cms.vstring("evtSelDiTauCandidateForMuTauMt1MET: passed_cumulative")
)
wTauNuEventDump.metSource = cms.InputTag('layer1PFMETs')

#--------------------------------------------------------------------------------
# define analysis sequence
# (ordered list of event selection criteria and histogram filling)
#--------------------------------------------------------------------------------

wTauNuAnalysisSequence = cms.VPSet(
    # fill histograms for full event sample
    cms.PSet(
        analyzers = wTauNuHistManagers
    ),

    # vertex selection
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertex'),
        title = cms.string('Vertex'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = wTauNuHistManagers
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexQuality'),
        title = cms.string('p(chi2Vertex) > 0.01'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = wTauNuHistManagers
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexPosition'),
        title = cms.string('-50 < Vertex < +50 cm'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = wTauNuHistManagers
    ),

    #primary tau selection
    cms.PSet(
        filter = cms.string('evtSelTauEta'),
        title = cms.string('-2.1 < eta(Tau) < +2.1'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('tauHistManager.tauSource = selectedLayer1TausForWTauNuEta21Cumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauPt'),
        title = cms.string('Pt(Tau) > 15 GeV'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('tauHistManager.tauSource = selectedLayer1TausForWTauNuPt20Cumulative')
	), 
    cms.PSet(
        filter = cms.string('evtSelMetPt'),
        title = cms.string('MET > 15 GeV'),
        saveRunEventNumbers = cms.vstring('')
	),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('metHistManager.metSource = selectedLayer1METsPt15')
	), 
    cms.PSet(
        filter = cms.string('evtSelTauLeadTrk'),
        title = cms.string('with leadtrk'),
        saveRunEventNumbers = cms.vstring('')
	),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('tauHistManager.tauSource = selectedLayer1TausForWTauNuLeadTrkCumulative')
	), 

    cms.PSet(
        filter = cms.string('evtSelTauLeadTrkPt'),
        title = cms.string('leadtrk pt > 15 GeV'),
        saveRunEventNumbers = cms.vstring('')
	),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('tauHistManager.tauSource = selectedLayer1TausForWTauNuLeadTrkPtCumulative')
	), 


    cms.PSet(
        filter = cms.string('evtSelTauTaNC'),
        title = cms.string('TaNC discriminator'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('tauHistManager.tauSource = selectedLayer1TausForWTauNuTrkIsoCumulative')
	),
    cms.PSet(
        filter = cms.string('evtSelTauMuonVeto'),
        title = cms.string('Muon veto'),
        saveRunEventNumbers = cms.vstring('')
	),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('tauHistManager.tauSource = selectedLayer1TausForWTauNuMuonVetoCumulative')	
	),
    cms.PSet(
        filter = cms.string('evtSelTauElectronVeto'),
        title = cms.string('Electron veto'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('tauHistManager.tauSource = selectedLayer1TausForWTauNuElectronVetoCumulative')	
	),
    cms.PSet(
        filter = cms.string('evtSelTauProng'),
        title = cms.string('Tau 1||3-Prong'),
        saveRunEventNumbers = cms.vstring('')
	),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('tauHistManager.tauSource = selectedLayer1TausForWTauNuProngCumulative')
	),
    cms.PSet(
	filter = cms.string('evtSelRecoilEnergy'),
	title = cms.string('recoil energy < 15'),
	saveRunEventNumbers = cms.vstring('')
	),
    cms.PSet(
	analyzers = wTauNuHistManagers,
	#replace = cms.vstring('jetHistManager.jetSource = selectedLayer1JetsEt20Cumulative')
	),
    cms.PSet(
	filter = cms.string('evtSelCentralJetVeto'),
	title = cms.string('central jet veto'),
	saveRunEventNumbers = cms.vstring('')
	),
    cms.PSet(
	analyzers = wTauNuHistManagers,	
	)
    )
