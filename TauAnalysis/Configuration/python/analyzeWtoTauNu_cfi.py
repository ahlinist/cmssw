import FWCore.ParameterSet.Config as cms
import copy

# import config for histogram managers
#from TauAnalysis.Core.genPhaseSpaceEventInfoHistManager_cfi import *
from TauAnalysis.Core.pftauHistManager_cfi import *
tauHistManager.useHPSpTaNCalgorithm = cms.bool(True)

from TauAnalysis.Core.pfMEtHistManager_cfi import *
from TauAnalysis.Core.jetHistManager_cfi import *
from TauAnalysis.Core.vertexHistManager_cfi import *
from TauAnalysis.Core.pftauRecoilEnergyHistManager_cfi import *
from TauAnalysis.Core.metTopologyHistManager_cfi import*
from TauAnalysis.Core.htRatioHistManager_cfi import*
from TauAnalysis.Core.tauNuCandidateHistManager_cfi import*
from TauAnalysis.Core.electronHistManager_cfi import *
from TauAnalysis.Core.muonHistManager_cfi import *

from TauAnalysis.Core.particleMultiplicityHistManager_cfi import *
from TauAnalysis.Core.eventWeightHistManager_cfi import *

from TauAnalysis.Core.dataBinner_cfi import *

# import config for binning results used to estimate systematic uncertainties
from TauAnalysis.Core.sysUncertaintyBinner_cfi import *
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *
sysUncertaintyNames = ["CENTRAL_VALUE"]
sysUncertaintyNames.extend(
    getSysUncertaintyNames(
    [tauSystematics,
     jetSystematics,
     metSystematicsForWtoTauNu,
     htRatioSystematics,
     tauNuPairSystematics,
     metTopologySystematics,
     theorySystematics
     ]
    )
    )

sysUncertaintyBinnerForWTauNuEff = sysUncertaintyBinner.clone(
    pluginName = cms.string('sysUncertaintyBinnerForWTauNuEff'),
    binnerPlugins = cms.VPSet(
         dataBinner
    ),
    systematics = cms.vstring(sysUncertaintyNames)
    )

# import config for binning results used to estimate acceptance of event selection
from TauAnalysis.Core.modelBinner_cfi import *
modelBinnerForWTauNuGenTauNuPairAcc = copy.deepcopy(modelBinner)
modelBinnerForWTauNuGenTauNuPairAcc.pluginName = cms.string('modelBinnerForWTauNuGenTauNuPairAcc')
modelBinnerForWTauNuGenTauNuPairAcc.srcGenFlag = cms.InputTag("isGenWtoTauNu")
modelBinnerForWTauNuGenTauNuPairAcc.srcRecFlag = cms.InputTag("isGenWtoTauNuWithinAcceptance")
modelBinnerForWTauNuGenTauNuPairAcc.dqmDirectory_store = cms.string('modelBinnerForWTauNuGenTauNuPairAcc')

modelBinnerForWTauNuWrtGenTauNuPairAcc = copy.deepcopy(modelBinnerForWTauNuGenTauNuPairAcc)
modelBinnerForWTauNuWrtGenTauNuPairAcc.pluginName = cms.string('modelBinnerForWTauNuWrtGenTauNuPairAcc')
modelBinnerForWTauNuWrtGenTauNuPairAcc.srcGenFlag = cms.InputTag("isGenWtoTauNuWithinAcceptance")
modelBinnerForWTauNuWrtGenTauNuPairAcc.srcRecFlag = cms.InputTag("isRecWtoTauNu")
modelBinnerForWTauNuWrtGenTauNuPairAcc.dqmDirectory_store = cms.string('modelBinnerForWTauNuWrtGenTauNuPairAcc')

sysUncertaintyBinnerForWTauNuAcc = sysUncertaintyBinner.clone(
    pluginName = cms.string('sysUncertaintyBinnerForWTauNuAcc'),
    binnerPlugins = cms.VPSet(
       modelBinnerForWTauNuGenTauNuPairAcc,
       modelBinnerForWTauNuWrtGenTauNuPairAcc
    ),
    systematics = cms.vstring(sysUncertaintyNames)
    )

sysUncertaintyHistManagerForWTauNu = cms.PSet(
    pluginName = cms.string('sysUncertaintyHistManagerForWTauNu'),
    pluginType = cms.string('SysUncertaintyHistManager'),
    histManagers = cms.VPSet(
       cms.PSet(
         config = tauNuCandidateHistManager,
         systematics = cms.PSet(
            tauNuCandidateSource = getSysUncertaintyParameterSets(
               [ tauNuPairSystematics ]
            )
          )
        ),
       cms.PSet(
         config = htRatioHistManager,
         systematics = cms.PSet(
           htRatioSource = getSysUncertaintyParameterSets(
             [ htRatioSystematics]
           )
         )
       ),
       cms.PSet(
         config = pfMEtHistManager,
         systematics = cms.PSet(
           metSource = getSysUncertaintyParameterSets(
            [metSystematicsForWtoTauNu]
           )
         )
       ),
       cms.PSet(
         config = tauHistManager,
         systematics = cms.PSet(
          tauSource = getSysUncertaintyParameterSets(
             [tauSystematics]
           )
         )
       ),
       cms.PSet(
        config = jetHistManager,
        systematics = cms.PSet(
          jetSource = getSysUncertaintyParameterSets(
            [ jetSystematics ]
          )
         )
       ),
       cms.PSet(
        config = metTopologyHistManager,
        systematics = cms.PSet(
          metTopologySource = getSysUncertaintyParameterSets(
            [ metTopologySystematics ]
         )
        )
       )
    ),
    dqmDirectory_store = cms.string('sysUncertaintyHistManagerResults')
 )

wTauNuHistManagers = cms.vstring(
    'tauHistManager',
    'pfMEtHistManager',
    'jetHistManager',
#    'vertexHistManager',
#    'tauRecoilEnergyFromCaloTowersHistManager',
    'metTopologyHistManager',
    'htRatioHistManager',
    'tauNuCandidateHistManager',
    'sysUncertaintyHistManagerForWTauNu',
    'sysUncertaintyBinnerForWTauNuEff'
    #'muonHistManager',
    #'electronHistManager'
)


#--------------------------------------------------------------------------------
# define event selection criteria
#--------------------------------------------------------------------------------

# trigger selection
evtSelTrigger = cms.PSet(
    pluginName = cms.string('evtSelTrigger'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('Trigger'),
    failSilent = cms.bool(False)
    )

evtSelTrigger2 = cms.PSet(
    pluginName = cms.string('evtSelTrigger2'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('PseudoTrigger')
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

evtSelTrkVertex = cms.PSet(
    pluginName = cms.string('evtSelTrkVertex'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauTrkVertex','cumulative'),
    src_individual = cms.InputTag('tauTrkVertex','individual'),
    systematics = cms.vstring(tauSystematics.keys())
    )

evtSelPFMetPt = cms.PSet(
    pluginName = cms.string('evtSelPFMetPt'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('PFmetPtCut'),
    systematics = cms.vstring(metSystematicsForWtoTauNu.keys())
    )

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

evtSelTauIso = cms.PSet(
    pluginName = cms.string('evtSelTauIso'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauIso', 'cumulative'),
    src_individual = cms.InputTag('tauIso', 'individual'),
    systematics = cms.vstring(tauSystematics.keys())
    )

evtSelTauMuonVeto = cms.PSet(
    pluginName = cms.string('evtSelTauMuonVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauMuonVeto', 'cumulative'),
    src_individual = cms.InputTag('tauMuonVeto', 'individual'),
    systematics = cms.vstring(tauSystematics.keys())
    )

evtSelTauElectronVeto = cms.PSet(
    pluginName = cms.string('evtSelTauElectronVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauElectronVeto', 'cumulative'),
    src_individual = cms.InputTag('tauElectronVeto', 'individual'),
    systematics = cms.vstring(tauSystematics.keys())
    )

evtSelTauEmFraction = cms.PSet(
    pluginName = cms.string('evtSelTauEmFraction'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauEmFraction','cumulative'),
    src_individual = cms.InputTag('tauEmFraction','individual'),
    systematics = cms.vstring(tauSystematics.keys())
    )

evtSelTauEcalCrackVeto = cms.PSet(
    pluginName = cms.string('evtSelTauEcalCrackVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauEcalCrackVeto', 'cumulative'),
    src_individual = cms.InputTag('tauEcalCrackVeto', 'individual'),
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

evtSelElectronVeto= cms.PSet(
    pluginName = cms.string('evtSelElectronVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('electronVeto')
    )

evtSelMuonVeto = cms.PSet(
    pluginName = cms.string('evtSelMuonVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('muonVeto')
)

evtSelHtRatio = cms.PSet(
    pluginName = cms.string('evtSelHtRatio'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('htRatio'),
    systematics = cms.vstring(htRatioSystematics.keys())
    )


#evtSelMetTopology = cms.PSet(
#    pluginName = cms.string('evtSelMetTopology'),
#    pluginType = cms.string('BoolEventSelector'),
#    src = cms.InputTag('metTopologyCut'),
#    systematics = cms.vstring(metTopologySystematics.keys())
#    )

#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------

wTauNuEventDump = cms.PSet(
    pluginName = cms.string('wTauNuEventDump'),
    pluginType = cms.string('wTauNuEventDump'),

    genParticleSource = cms.InputTag('genParticles'),
    genJetSource = cms.InputTag('ak5GenJets'),
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
    triggerConditions = cms.vstring("evtSelHtRatio : passed_cumulative")
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
        analyzers = cms.vstring('vertexHistManager',
                                'pfMEtHistManager',
                                'modelBinnerForWTauNuGenTauNuPairAcc',
                                'modelBinnerForWTauNuWrtGenTauNuPairAcc',
                                'sysUncertaintyBinnerForWTauNuAcc'
                                )
        ),
    # trigger selection
    cms.PSet(
       filter = cms.string('evtSelTrigger'),
       title = cms.string('Tau+MET Trigger'),
       saveRunLumiSectionEventNumbers = cms.vstring('')
       ),
    cms.PSet(
       analyzers = cms.vstring('pfMEtHistManager',
                               'sysUncertaintyBinnerForWTauNuEff'
              
            )
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
        filter = cms.string('evtSelTrkVertex'),
        title = cms.string('Track matching vertex'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauLeadTrk'),
        title = cms.string('with leadtrk'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
	),
    #cms.PSet(
    #    analyzers = wTauNuHistManagers,
    #    replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuLeadTrkCumulative')
#	), 
    cms.PSet(
        filter = cms.string('evtSelTauLeadTrkPt'),
        title = cms.string('leadtrk pt > 15 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
	),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuLeadTrkPtCumulative')
	),
    cms.PSet(
        filter = cms.string('evtSelTauMuonVeto'),
        title = cms.string('Tau muon veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
        ),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuMuonVetoCumulative')
        ),
    cms.PSet(
        filter = cms.string('evtSelMuonVeto'),
        title = cms.string('Muon veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
        ),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('')
        ),
    cms.PSet(
        filter = cms.string('evtSelTauElectronVeto'),
        title = cms.string('Tau electron veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
        ),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuElectronVetoCumulative')
        ),
    cms.PSet(
       filter = cms.string('evtSelTauEmFraction'),
       title = cms.string('EM-fraction < 0.95'),
       saveRunLumiSectionEventNumbers = cms.vstring('')
       ),
    cms.PSet(
       analyzers = wTauNuHistManagers,
       replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuEmFractionCumulative')
       ),
    cms.PSet(
       filter = cms.string('evtSelElectronVeto'),
       title = cms.string('Electron veto'),
       saveRunLumiSectionEventNumbers = cms.vstring('')
       ),
    cms.PSet(
       analyzers = wTauNuHistManagers,
       replace = cms.vstring('')
       ),
    cms.PSet(
        filter = cms.string('evtSelTauIso'),
        title = cms.string('Tau isolation'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = wTauNuHistManagers,
        replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuIsoCumulative')
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
        replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuChargeCumulative',
                              'electronHistManager.electronSource = selectedPatElectronsPt15Cumulative',
                              'muonHistManager.muonSource = selectedPatMuonsPFRelIsoCumulative')
	),
    cms.PSet(
        filter = cms.string('evtSelTauEcalCrackVeto'),
        title = cms.string('Ecal crack veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
        ),
    cms.PSet(
        analyzers = cms.vstring(
             'tauHistManager',
             'pfMEtHistManager',
             'jetHistManager',
             'htRatioHistManager',
             'metTopologyHistManager',
             'tauNuCandidateHistManager',
             'sysUncertaintyHistManagerForWTauNu',
             'sysUncertaintyBinnerForWTauNuEff'             
             ),
        replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuEcalCrackVetoCumulative',
                              'jetHistManager.jetSource = selectedPatJetsEt15ForWTauNuCumulative',
                              'tauNuCandidateHistManager.tauNuCandidateSource = allTauNuPairs')	
        ),
    cms.PSet(
        filter = cms.string('evtSelPFMetPt'),
        title = cms.string('PF-MET > 35'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
        ),
    cms.PSet(
       analyzers = wTauNuHistManagers,        
       replace = cms.vstring('jetHistManager.jetSource = selectedPatJetsEt15ForWTauNuCumulative',
                             'tauRecoilEnergyFromCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromCaloTowers',
                             'tauNuCandidateHistManager.tauNuCandidateSource = allTauNuPairs')
       ),
    cms.PSet(
       filter = cms.string('evtSelHtRatio'),
       title = cms.string('HT-ratio > 0.65'),
       saveRunLumiSectionEventNumbers = cms.vstring('passed_cumulative')
       ),
    cms.PSet(
       analyzers = cms.vstring(
               'tauHistManager',
               'jetHistManager',
               'tauNuCandidateHistManager',
               'htRatioHistManager',
               'metTopologyHistManager',
               'pfMEtHistManager',
               'sysUncertaintyHistManagerForWTauNu',
               'dataBinner',
               'sysUncertaintyBinnerForWTauNuEff',
               'vertexHistManager'
       ),
       replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuEcalCrackVetoCumulative',
                             'jetHistManager.jetSource = selectedPatJetsEt15ForWTauNuCumulative'
                             )
       ),
    cms.PSet(
       filter = cms.string('evtSelTrigger2'),
       title = cms.string('trigger'),
       saveRumLumiSectionEventNumbers = cms.vstring('passed_cumulative')
       ),
    cms.PSet(
      analyzers = cms.vstring(
              'tauHistManager',
              'jetHistManager',
              'tauNuCandidateHistManager',
              'htRatioHistManager',
              'metTopologyHistManager',
              'pfMEtHistManager',
              'sysUncertaintyHistManagerForWTauNu',
              'dataBinner',
              'sysUncertaintyBinnerForWTauNuEff',
              'vertexHistManager'
              ),
      replace = cms.vstring('tauHistManager.tauSource = selectedPatTausForWTauNuEcalCrackVetoCumulative',
                          'jetHistManager.jetSource = selectedPatJetsEt15ForWTauNuCumulative'
                          )
    )
#    cms.PSet(
#       filter = cms.string('evtSelMetTopology'),
#       title = cms.string('MET-topology < 0.4'),
#       saveRunLumiSectionEventNumbers = cms.vstring('passed_cumulative')
#       ),
#    cms.PSet(
#       analyzers = wTauNuHistManagers,
#       replace = cms.vstring('jetHistManager.jetSource = selectedPatJetsEt15ForWTauNuCumulative',
#                             'tauRecoilEnergyFromCaloTowersHistManager.leptonRecoilEnergySource = tauRecoilEnergyFromCaloTowers',
#                             'tauNuCandidateHistManager.tauNuCandidateSource = allTauNuPairs',
#                             'electronHistManager.electronSource = selectedPatElectronsPt15Cumulative',
#                             'muonHistManager.muonSource = selectedPatMuonsPFRelIsoCumulative'
#                             )
#       )
      )

