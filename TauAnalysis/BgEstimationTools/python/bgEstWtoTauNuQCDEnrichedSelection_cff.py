import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *
from TauAnalysis.RecoTools.tools.eventSelFlagProdConfigurator import *

#--------------------------------------------------------------------------------
# produce collection of good vertices
#--------------------------------------------------------------------------------
from TauAnalysis.RecoTools.recoVertexSelection_cff import *
selectPrimaryVertexBgEst = cms.Sequence(selectPrimaryVertex)

#--------------------------------------------------------------------------------
# produce collection of pat::Taus
#--------------------------------------------------------------------------------
from TauAnalysis.RecoTools.patLeptonSelection_cff import *

tausBgEstQCDEnrichedLeadTrk = copy.deepcopy(selectedPatTausForWTauNuLeadTrk)
tausBgEstQCDEnrichedLeadTrk.cut = cms.string('pt() > 30') 
tausBgEstQCDEnrichedLeadTrkPt = copy.deepcopy(selectedPatTausForWTauNuLeadTrkPt)
tausBgEstQCDEnrichedLeadTrkPt.cut = cms.string('leadPFChargedHadrCand().isNonnull() & leadPFChargedHadrCand().pt() > 15.')
tausBgEstQCDEnrichedIso = copy.deepcopy(selectedPatTausForWTauNuIso)
tausBgEstQCDEnrichedIso.cut = cms.string('tauID("byHPSmedium") < 0.5')
tausBgEstQCDEnrichedProng = copy.deepcopy(selectedPatTausForWTauNuProng)
tausBgEstQCDEnrichedProng.cut = cms.string('tauID("byHPSmedium") < 0.5')
tausBgEstQCDEnrichedCharge = copy.deepcopy(selectedPatTausForWTauNuCharge)
tausBgEstQCDEnrichedCharge.cut = cms.string('tauID("byHPSmedium") < 0.5')

tausBgEstQCDEnrichedMuonVeto = copy.deepcopy(selectedPatTausForWTauNuMuonVeto)
tausBgEstQCDEnrichedElectronVeto = copy.deepcopy(selectedPatTausForWTauNuElectronVeto)
tausBgEstQCDEnrichedEmFraction = copy.deepcopy(selectedPatTausForWTauNuEmFraction)
tausBgEstQCDEnrichedEcalCrackVeto = copy.deepcopy(selectedPatTausForWTauNuEcalCrackVeto)

tauSelConfiguratorBgEstQCDEnriched = objSelConfigurator(
    [
        tausBgEstQCDEnrichedLeadTrk,
        tausBgEstQCDEnrichedLeadTrkPt,
        tausBgEstQCDEnrichedMuonVeto,
        tausBgEstQCDEnrichedElectronVeto,
        tausBgEstQCDEnrichedEmFraction,
        tausBgEstQCDEnrichedIso,
        tausBgEstQCDEnrichedProng,
        tausBgEstQCDEnrichedCharge,
        tausBgEstQCDEnrichedEcalCrackVeto
        ],
    src = "selectedPatTausForWTauNuPt20Cumulative",
    pyModuleName = __name__,
    doSelIndividual = False
    )
selectTausBgEstQCDEnriched = tauSelConfiguratorBgEstQCDEnriched.configure(pyNameSpace = locals())
#------------------------------------------------------------------------------
# Produce collection of tau-nu pairs
#-----------------------------------------------------------------------------
from TauAnalysis.CandidateTools.tauNuPairProduction_cff import *

tauNuPairsBgEstQCDEnriched = copy.deepcopy(allTauNuPairs)
tauNuPairsBgEstQCDEnriched.srcVisDecayProducts = cms.InputTag('tausBgEstQCDEnrichedEcalCrackVetoCumulative')

produceTauNuPairsBgEstQCDEnriched = cms.Sequence(tauNuPairsBgEstQCDEnriched)

#-----------------------------------------------------------------------------------
# modify MET-topology collection
#----------------------------------------------------------------------------------
MEtTopologyBgEstQCDEnriched = cms.EDFilter("MEtTopologySelector",
                                             src = cms.InputTag('metTopologies'),
                                             cut = cms.string('Vanti()/Vparallel() < 0.6'),
                                             filter = cms.bool(False)
                                             )
selectMEtTopologyBgEstQCDEnriched = cms.Sequence(MEtTopologyBgEstQCDEnriched)

#----------------------------------------------------------------------
# modify HT-ratio collection
#----------------------------------------------------------------------
htRatioBgEstQCDEnriched = cms.EDProducer("HtRatioProducer",
    srcJet = cms.InputTag('selectedPatJetsEt15ForWTauNuCumulative'),
    srcTau = cms.InputTag('tausBgEstQCDEnrichedEcalCrackVetoCumulative'),
    verbosity = cms.untracked.int32(0)
)

selHtRatioBgEstQCDEnriched = cms.EDFilter("HtRatioSelector",
    src = cms.InputTag('htRatioBgEstQCDEnriched'),
    cut = cms.string('Ratio() > 0.65'),
    filter = cms.bool(False)
)


produceHtRatioBgEstQCDEnriched = cms.Sequence(htRatioBgEstQCDEnriched)

selectHtRatioBgEstQCDEnriched =   cms.Sequence(selHtRatioBgEstQCDEnriched)

#---------------------------------------------------------------------------------
# produce boolean event selection flags
#----------------------------------------------------------------------------------
from TauAnalysis.Configuration.selectWtoTauNu_cff import *
# trigger selection
cfgTriggerBgEst = copy.deepcopy(cfgTrigger)

# primary event vertex selection
cfgPrimaryEventVertexBgEst = copy.deepcopy(cfgPrimaryEventVertex)
cfgPrimaryEventVertexQualityBgEst = copy.deepcopy(cfgPrimaryEventVertexQuality)
cfgPrimaryEventVertexPositionBgEst = copy.deepcopy(cfgPrimaryEventVertexPosition)

# analysis selection
cfgElectronVetoBgEstQCDEnriched = copy.deepcopy(cfgElectronVeto)
cfgMuonVetoBgEstQCDEnriched = copy.deepcopy(cfgMuonVeto)

#cfgHtRatioCutBgEstQCDEnriched = copy.deepcopy(cfgHtRatioCut)
cfgHtRatioCutBgEstQCDEnriched = cms.PSet(
    pluginName = cms.string('htRatioCutBgEstQCDEnriched'),
    pluginType = cms.string('HtRatioMinEventSelector'),
    src = cms.InputTag('selHtRatioBgEstQCDEnriched'),
    minNumber = cms.uint32(1)
    )

cfgPFMetPtBgEstQCDEnriched = copy.deepcopy(cfgPFMetPt)
cfgPFMetPtBgEstQCDEnriched.systematics = cms.vstring()

cfgMetTopologyCutBgEstQCDEnriched = cms.PSet(
    pluginName = cms.string('metTopologyCutBgEstQCDEnriched'),
    pluginType = cms.string('MEtTopologyMinEventSelector'),
    src = cms.InputTag('MEtTopologyBgEstQCDEnriched'),
    minNumber = cms.uint32(1)
    )

cfgSingleTauCutBgEstQCDEnriched = cms.PSet(
    pluginName = cms.string('singleTauCutBgEstQCDEnriched'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src = cms.InputTag('tausBgEstQCDEnrichedEcalCrackVetoCumulative'),
    maxNumber = cms.uint32(1)
    )

#tau selection
cfgTauEtaCutBgEstQCDEnriched = copy.deepcopy(cfgTauEtaCut)
cfgTauPtCutBgEstQCDEnriched = copy.deepcopy(cfgTauPtCut)
cfgTauPtCutBgEstQCDEnriched.systematics = cms.vstring()

cfgTauLeadTrkCutBgEstQCDEnriched = copy.deepcopy(cfgTauLeadTrk)
cfgTauLeadTrkCutBgEstQCDEnriched.pluginName = cms.string('tauLeadTrkCutBgEstQCDEnriched')
cfgTauLeadTrkCutBgEstQCDEnriched.src_cumulative = cms.InputTag('tausBgEstQCDEnrichedLeadTrkCumulative')
cfgTauLeadTrkCutBgEstQCDEnriched.systematics = cms.vstring()

cfgTauLeadTrkPtCutBgEstQCDEnriched = copy.deepcopy(cfgTauLeadTrkPt)
cfgTauLeadTrkPtCutBgEstQCDEnriched.pluginName = cms.string('tauLeadTrkPtCutBgEstQCDEnriched')
cfgTauLeadTrkPtCutBgEstQCDEnriched.src_cumulative = cms.InputTag('tausBgEstQCDEnrichedLeadTrkPtCumulative')
cfgTauLeadTrkPtCutBgEstQCDEnriched.systematics = cms.vstring()

cfgTauMuonVetoBgEstQCDEnriched = copy.deepcopy(cfgTauMuonVeto)
cfgTauMuonVetoBgEstQCDEnriched.pluginName = cms.string('tauMuonVetoBgEstQCDEnriched')
cfgTauMuonVetoBgEstQCDEnriched.src_cumulative = cms.InputTag('tausBgEstQCDEnrichedMuonVetoCumulative')
cfgTauMuonVetoBgEstQCDEnriched.systematics = cms.vstring()

cfgTauElectronVetoBgEstQCDEnriched = copy.deepcopy(cfgTauElectronVeto)
cfgTauElectronVetoBgEstQCDEnriched.pluginName = cms.string('tauElectronVetoBgEstQCDEnriched')
cfgTauElectronVetoBgEstQCDEnriched.src_cumulative = cms.InputTag('tausBgEstQCDEnrichedElectronVetoCumulative')
cfgTauElectronVetoBgEstQCDEnriched.systematics = cms.vstring()

cfgTauEmFractionBgEstQCDEnriched = copy.deepcopy(cfgTauEmFraction)
cfgTauEmFractionBgEstQCDEnriched.pluginName = cms.string('tauEmFractionBgEstQCDEnriched')
cfgTauEmFractionBgEstQCDEnriched.src_cumulative = cms.InputTag('tausBgEstQCDEnrichedEmFractionCumulative')
cfgTauEmFractionBgEstQCDEnriched.systematics = cms.vstring()

cfgTauIsoBgEstQCDEnriched = copy.deepcopy(cfgTauIso)
cfgTauIsoBgEstQCDEnriched.pluginName = cms.string('tauIsoBgEstQCDEnriched')
cfgTauIsoBgEstQCDEnriched.src_cumulative = cms.InputTag('tausBgEstQCDEnrichedIsoCumulative')
cfgTauIsoBgEstQCDEnriched.systematics = cms.vstring()

cfgTauProngBgEstQCDEnriched = copy.deepcopy(cfgTauProngCut)
cfgTauProngBgEstQCDEnriched.pluginName = cms.string('tauProngBgEstQCDEnriched')
cfgTauProngBgEstQCDEnriched.src_cumulative = cms.InputTag('tausBgEstQCDEnrichedProngCumulative')
cfgTauProngBgEstQCDEnriched.systematics = cms.vstring()

cfgTauChargeBgEstQCDEnriched = copy.deepcopy(cfgTauChargeCut)
cfgTauChargeBgEstQCDEnriched.pluginName = cms.string('tauChargeBgEstQCDEnriched')
cfgTauChargeBgEstQCDEnriched.src_cumulative = cms.InputTag('tausBgEstQCDEnrichedChargeCumulative')
cfgTauChargeBgEstQCDEnriched.systematics = cms.vstring()

cfgTauEcalCrackVetoBgEstQCDEnriched = copy.deepcopy(cfgTauEcalCrackVeto)
cfgTauEcalCrackVetoBgEstQCDEnriched.pluginName = cms.string('tauEcalCrackVetoBgEstQCDEnriched')
cfgTauEcalCrackVetoBgEstQCDEnriched.src_cumulative = cms.InputTag('tausBgEstQCDEnrichedEcalCrackVetoCumulative')
cfgTauEcalCrackVetoBgEstQCDEnriched.systematics = cms.vstring()

evtSelConfiguratorBgEstQCDEnriched = eventSelFlagProdConfigurator(
    [ cfgTriggerBgEst,
      cfgPrimaryEventVertexBgEst,
      cfgPrimaryEventVertexQualityBgEst,
      cfgPrimaryEventVertexPositionBgEst,
      cfgTauEtaCutBgEstQCDEnriched,
      cfgTauPtCutBgEstQCDEnriched,
      cfgTauLeadTrkCutBgEstQCDEnriched,
      cfgTauLeadTrkPtCutBgEstQCDEnriched,
      cfgTauMuonVetoBgEstQCDEnriched,
      cfgMuonVetoBgEstQCDEnriched,
      cfgTauElectronVetoBgEstQCDEnriched,
      cfgTauEmFractionBgEstQCDEnriched,
      cfgElectronVetoBgEstQCDEnriched,
      cfgTauIsoBgEstQCDEnriched,
      cfgTauProngBgEstQCDEnriched,
      cfgTauChargeBgEstQCDEnriched,
      cfgTauEcalCrackVetoBgEstQCDEnriched,
      cfgPFMetPtBgEstQCDEnriched,
      cfgHtRatioCutBgEstQCDEnriched,
      cfgSingleTauCutBgEstQCDEnriched,
      cfgMetTopologyCutBgEstQCDEnriched
      ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
    )

selectEventsBgEstQCDEnriched = evtSelConfiguratorBgEstQCDEnriched.configure()

#--------------------------------------------------------------------------------  
# apply event selection criteria; fill histograms
#--------------------------------------------------------------------------------
from TauAnalysis.Configuration.analyzeWtoTauNu_cff import *
from TauAnalysis.Configuration.analyzeWtoTauNu_cfi import *

tauHistManagerBgEstQCDEnriched = copy.deepcopy(tauHistManager)
tauHistManagerBgEstQCDEnriched.pluginName = cms.string('tauHistManagerBgEstQCDEnriched')
tauHistManagerBgEstQCDEnriched.tauSource = cms.InputTag('tausBgEstWtoTauNuQCDEnrichedEcalCrackVetoCumulative')

tauNuCandidateHistManagerBgEstQCDEnriched = copy.deepcopy(tauNuCandidateHistManager)
tauNuCandidateHistManagerBgEstQCDEnriched.pluginName = cms.string('tauNuCandidateHistManagerBgEstQCDEnriched')
tauNuCandidateHistManagerBgEstQCDEnriched.tauNuCandidateSource = cms.InputTag('tauNuPairsBgEstQCDEnriched')

htRatioHistManagerBgEstQCDEnriched = copy.deepcopy(htRatioHistManager)
htRatioHistManagerBgEstQCDEnriched.pluginName = cms.string('htRatioHistManagerBgEstQCDEnriched')
htRatioHistManagerBgEstQCDEnriched.htRatioSource = cms.InputTag('htRatioBgEstQCDEnriched')

pfMEtHistManagerBgEstQCDEnriched = copy.deepcopy(pfMEtHistManager)
pfMEtHistManagerBgEstQCDEnriched.pluginName = cms.string('pfMEtHistManagerBgEstQCDEnriched')

metTopologyHistManagerBgEstQCDEnriched = copy.deepcopy(metTopologyHistManager)
metTopologyHistManagerBgEstQCDEnriched.pluginName = cms.string('metTopologyHistManagerBgEstQCDEnriched')

analyzeEventsBgEstQCDEnriched = cms.EDAnalyzer("GenericAnalyzer",
                                                       name = cms.string('BgEstTemplateAnalyzer_QCDEnriched'),
                                                       filters = cms.VPSet(
        evtSelTrigger,
        evtSelPrimaryEventVertex,
        evtSelPrimaryEventVertexQuality,
        evtSelPrimaryEventVertexPosition,
        evtSelTauEta,
        evtSelTauPt,
        cms.PSet(
            pluginName = cms.string('tauLeadTrkCutBgEstQCDEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauLeadTrkCutBgEstQCDEnriched','cumulative')
            ),
        cms.PSet(
            pluginName = cms.string('tauLeadTrkPtCutBgEstQCDEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauLeadTrkPtCutBgEstQCDEnriched','cumulative')
            ),
        cms.PSet(
            pluginName = cms.string('tauMuonVetoBgEstQCDEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauMuonVetoBgEstQCDEnriched','cumulative')
            ),
        evtSelMuonVeto,
        cms.PSet(
            pluginName = cms.string('tauElectronVetoBgEstQCDEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauElectronVetoBgEstQCDEnriched','cumulative')
            ),
        cms.PSet(
            pluginName = cms.string('tauEmFractionBgEstQCDEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauEmFractionBgEstQCDEnriched','cumulative')
            ),
        evtSelElectronVeto,
        cms.PSet(
            pluginName = cms.string('tauIsoBgEstQCDEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauIsoBgEstQCDEnriched','cumulative')
            ),
        cms.PSet(
            pluginName = cms.string('tauProngBgEstQCDEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauProngBgEstQCDEnriched','cumulative')
            ),
        cms.PSet(
            pluginName = cms.string('tauChargeBgEstQCDEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauChargeBgEstQCDEnriched','cumulative')
            ),
        cms.PSet(
            pluginName = cms.string('tauEcalCrackVetoBgEstQCDEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauEcalCrackVetoBgEstQCDEnriched','cumulative')
            ),
        evtSelPFMetPt,
        cms.PSet(
            pluginName = cms.string('htRatioCutBgEstQCDEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('htRatioCutBgEstQCDEnriched')
            ),
        cms.PSet(
            pluginName = cms.string('singleTauCutBgEstQCDEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('singleTauCutBgEstQCDEnriched')
            ),
        cms.PSet(
            pluginName = cms.string('metTopologyCutBgEstQCDEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('metTopologyCutBgEstQCDEnriched')
            )
       ),
             
       analyzers = cms.VPSet(
        tauHistManagerBgEstQCDEnriched,
        tauNuCandidateHistManagerBgEstQCDEnriched,
        htRatioHistManagerBgEstQCDEnriched,
        pfMEtHistManagerBgEstQCDEnriched,
        metTopologyHistManagerBgEstQCDEnriched
        ),
        eventDumps = cms.VPSet(),
        analysisSequence = cms.VPSet(
        cms.PSet(
            analyzers = cms.vstring('tauHistManagerBgEstQCDEnriched',
                                    'tauNuCandidateHistManagerBgEstQCDEnriched',
                                    'pfMEtHistManagerBgEstQCDEnriched',
                                    'htRatioHistManagerBgEstQCDEnriched',
                                    'metTopologyHistManagerBgEstQCDEnriched'
                                    ),
            replace = cms.vstring('tauHistManagerBgEstQCDEnriched.tauSource = cleanPatTaus'),
            ),
        cms.PSet(
            filter = cms.string('evtSelTrigger'),
            title = cms.string('Trigger')
            ),
        cms.PSet(
             analyzers = cms.vstring('tauHistManagerBgEstQCDEnriched',
                                     'pfMEtHistManagerBgEstQCDEnriched',
                                     'tauNuCandidateHistManagerBgEstQCDEnriched',
                                     'htRatioHistManagerBgEstQCDEnriched',
                                     'metTopologyHistManagerBgEstQCDEnriched'
                                     ),
             replace = cms.vstring('tauHistManagerBgEstQCDEnriched.tauSource = cleanPatTaus'),
            ),
        cms.PSet(
            filter = cms.string('evtSelPrimaryEventVertex'),
            title = cms.string('Vertex exists')
            ),
        cms.PSet(
            filter = cms.string('evtSelPrimaryEventVertexQuality'),
            title = cms.string('p(chi2Vertex) > 0.01')
            ),
        cms.PSet(
            filter =cms.string('evtSelPrimaryEventVertexPosition'),
            title = cms.string('-25 < zVertex < +25')
            ),
        cms.PSet(
            filter = cms.string('evtSelTauEta'),
            title = cms.string('|eta| < 2.3')             
            ),
        cms.PSet(
            filter = cms.string('evtSelTauPt'),
            title = cms.string('pt > 30')
            ),
        cms.PSet(
           analyzers = cms.vstring('tauHistManagerBgEstQCDEnriched',
                                   'pfMEtHistManagerBgEstQCDEnriched',
                                   'tauNuCandidateHistManagerBgEstQCDEnriched',
                                   'htRatioHistManagerBgEstQCDEnriched',
                                   'metTopologyHistManagerBgEstQCDEnriched'
                                   ),
           replace = cms.vstring('tauHistManagerBgEstQCDEnriched.tauSource = selectedPatTausForWTauNuPt20Cumulative'),
           ),
        cms.PSet(
            filter = cms.string('tauLeadTrkCutBgEstQCDEnriched'),
            title = cms.string('lead trk inactive')
            ),
        cms.PSet(
            filter = cms.string('tauLeadTrkPtCutBgEstQCDEnriched'),
            title = cms.string('lead trk pt inactive')
            ),
        cms.PSet(
            filter = cms.string('tauMuonVetoBgEstQCDEnriched'),
            title = cms.string('tau Muon veto')
            ),
        cms.PSet(
            filter = cms.string('evtSelMuonVeto'),
            title = cms.string('Muon veto')
            ),
        cms.PSet(
            filter = cms.string('tauElectronVetoBgEstQCDEnriched'),
            title = cms.string('tau Electron veto')
            ),
        cms.PSet(
            analyzers = cms.vstring('tauHistManagerBgEstQCDEnriched',
                                    'pfMEtHistManagerBgEstQCDEnriched',
                                    'tauNuCandidateHistManagerBgEstQCDEnriched',
                                    'htRatioHistManagerBgEstQCDEnriched',
                                    'metTopologyHistManagerBgEstQCDEnriched'
                                    ),
            replace = cms.vstring('tauHistManagerBgEstQCDEnriched.tauSource = tausBgEstQCDEnrichedElectronVetoCumulative'),
             ),
        cms.PSet(
            filter = cms.string('tauEmFractionBgEstQCDEnriched'),
            title = cms.string('EM-fraction < 0.9')
            ),
        cms.PSet(
            filter = cms.string('evtSelElectronVeto'),
            title = cms.string('Electron veto')
            ),
        cms.PSet(
            filter = cms.string('tauIsoBgEstQCDEnriched'),
            title = cms.string('inverse isolation')
            ),
        cms.PSet(
            analyzers = cms.vstring('tauHistManagerBgEstQCDEnriched',
                                    'pfMEtHistManagerBgEstQCDEnriched',
                                    'tauNuCandidateHistManagerBgEstQCDEnriched',
                                    'htRatioHistManagerBgEstQCDEnriched',
                                    'metTopologyHistManagerBgEstQCDEnriched'
                                    ),
            replace = cms.vstring('tauHistManagerBgEstQCDEnriched.tauSource = tausBgEstQCDEnrichedIsoCumulative'),
            ),
        cms.PSet(
            filter = cms.string('tauEcalCrackVetoBgEstQCDEnriched'),
            title = cms.string('Ecal crack veto')
            ),
        cms.PSet(
            filter = cms.string('evtSelPFMetPt'),
            title = cms.string('PF-MET > 35')
            ),
        cms.PSet(
            analyzers = cms.vstring('tauHistManagerBgEstQCDEnriched',
                                    'pfMEtHistManagerBgEstQCDEnriched',
                                    'tauNuCandidateHistManagerBgEstQCDEnriched',
                                    'htRatioHistManagerBgEstQCDEnriched',
                                    'metTopologyHistManagerBgEstQCDEnriched'
                                    ),
            replace = cms.vstring(
                'tauHistManagerBgEstQCDEnriched.tauSource = tausBgEstQCDEnrichedEcalCrackVetoCumulative',
                'htRatioHistManagerBgEstQCDEnriched.htRatioSource = htRatioBgEstQCDEnriched'),
            ),
        cms.PSet(
            filter = cms.string('htRatioCutBgEstQCDEnriched'),
            title = cms.string('ht-ratio > 0.45')
            ),
        cms.PSet(
            analyzers = cms.vstring('tauHistManagerBgEstQCDEnriched',
                                    'pfMEtHistManagerBgEstQCDEnriched',
                                    'tauNuCandidateHistManagerBgEstQCDEnriched',
                                    'htRatioHistManagerBgEstQCDEnriched',
                                    'metTopologyHistManagerBgEstQCDEnriched'
                                    ),
            replace = cms.vstring(
                'tauHistManagerBgEstQCDEnriched.tauSource = tausBgEstQCDEnrichedEcalCrackVetoCumulative',
                'htRatioHistManagerBgEstQCDEnriched.htRatioSource = selHtRatioBgEstQCDEnriched'),
            ),
        cms.PSet(
            filter = cms.string('singleTauCutBgEstQCDEnriched'),
            title = cms.string('one tau candidate')
            ),
        cms.PSet(
             analyzers = cms.vstring('tauHistManagerBgEstQCDEnriched',
                                     'pfMEtHistManagerBgEstQCDEnriched',
                                     'tauNuCandidateHistManagerBgEstQCDEnriched',
                                     'htRatioHistManagerBgEstQCDEnriched',
                                     'metTopologyHistManagerBgEstQCDEnriched'
                                     ),
             replace = cms.vstring(
                'tauHistManagerBgEstQCDEnriched.tauSource = tausBgEstQCDEnrichedEcalCrackVetoCumulative',
                'htRatioHistManagerBgEstQCDEnriched.htRatioSource = selHtRatioBgEstQCDEnriched'),
             )
        )
                                               )

bgEstQCDEnrichedAnalysisSequence = cms.Sequence(
    selectPrimaryVertexBgEst
    +selectTausBgEstQCDEnriched
    +selectMEtTopologyBgEstQCDEnriched
    +produceTauNuPairsBgEstQCDEnriched
    +produceHtRatioBgEstQCDEnriched
    +selectHtRatioBgEstQCDEnriched
    +selectEventsBgEstQCDEnriched
    +analyzeEventsBgEstQCDEnriched
)
