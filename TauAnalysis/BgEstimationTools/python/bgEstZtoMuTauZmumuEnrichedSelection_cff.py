import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *
from TauAnalysis.RecoTools.tools.eventSelFlagProdConfigurator import *

#--------------------------------------------------------------------------------
# select Z --> mu+ mu- background enriched event sample
#------------------------------------sstand--------------------------------------------

#--------------------------------------------------------------------------------  
# produce collection of pat::Muons
#--------------------------------------------------------------------------------

from TauAnalysis.RecoTools.patMuonSelection_cfi import *

muonsBgEstZmumuEnrichedTrkIso = copy.deepcopy(selectedPatMuonsTrkIso)
muonsBgEstZmumuEnrichedTrkIso.sumPtMax = cms.double(0.10)
muonsBgEstZmumuEnrichedTrkIso.sumPtMethod = cms.string("relative")

muonsBgEstZmumuEnrichedEcalIso = copy.deepcopy(selectedPatMuonsEcalIso)
muonsBgEstZmumuEnrichedEcalIso.sumPtMax = cms.double(0.10)
muonsBgEstZmumuEnrichedEcalIso.sumPtMethod = cms.string("relative")

muonsBgEstZmumuEnrichedCombIso = copy.deepcopy(selectedPatMuonsCombIso)
muonsBgEstZmumuEnrichedCombIso.sumPtMax = cms.double(0.10)
muonsBgEstZmumuEnrichedCombIso.sumPtMethod = cms.string("relative")

muonsBgEstZmumuEnrichedPionVeto = copy.deepcopy(selectedPatMuonsPionVeto)

muonsBgEstZmumuEnrichedTrk = copy.deepcopy(selectedPatMuonsTrk)

muonsBgEstZmumuEnrichedTrkIP = copy.deepcopy(selectedPatMuonsTrkIP)

muonSelConfiguratorBgEstZmumuEnriched = objSelConfigurator(
    [ muonsBgEstZmumuEnrichedTrkIso,
      muonsBgEstZmumuEnrichedEcalIso,
      muonsBgEstZmumuEnrichedCombIso,
      muonsBgEstZmumuEnrichedPionVeto,
      muonsBgEstZmumuEnrichedTrk,
      muonsBgEstZmumuEnrichedTrkIP ],
    src = "selectedPatMuonsPt10Cumulative",
    pyModuleName = __name__,
    doSelIndividual = False
)

selectMuonsBgEstZmumuEnriched = muonSelConfiguratorBgEstZmumuEnriched.configure(pyNameSpace = locals())

#--------------------------------------------------------------------------------

muonsBgEstZmumuMuonMisIdEnrichedStandalone = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag('cleanPatMuons'),                                   
    cut = cms.string('isStandAloneMuon()'),
    filter = cms.bool(False)
)

selectMuonsBgEstZmumuEnriched._seq = selectMuonsBgEstZmumuEnriched._seq * muonsBgEstZmumuMuonMisIdEnrichedStandalone
    
#--------------------------------------------------------------------------------  
# produce collection of pat::Taus
#--------------------------------------------------------------------------------

from TauAnalysis.RecoTools.patPFTauSelection_cfi import *
from TauAnalysis.RecoTools.patPFTauSelectionForMuTau_cfi import *

tausBgEstZmumuJetMisIdEnrichedMuonVeto = copy.deepcopy(selectedPatTausMuonVeto)
tausBgEstZmumuJetMisIdEnrichedMuonVeto.src = cms.InputTag('selectedPatTausForMuTauEcalIsoCumulative')

tausBgEstZmumuMuonMisIdEnrichedAntiMuonVeto = cms.EDFilter("PATTauSelector",
    src = cms.InputTag('selectedPatTausEcalIsoCumulative'),                                              
    cut = cms.string('tauID("againstMuon") < 0.5'),
    filter = cms.bool(False)                                 
)

selectTausBgEstZmumuEnriched = cms.Sequence(tausBgEstZmumuJetMisIdEnrichedMuonVeto + tausBgEstZmumuMuonMisIdEnrichedAntiMuonVeto)

#--------------------------------------------------------------------------------  
# produce collection of muon + tau-jet combinations
#--------------------------------------------------------------------------------

muTauPairsBgEstZmumuJetMisIdEnriched = cms.EDProducer("PATMuTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedPatMuonsPionVetoCumulative'),
    srcLeg2 = cms.InputTag('tausBgEstZmumuJetMisIdEnrichedMuonVeto'),
    dRmin12 = cms.double(0.7),
    srcMET = cms.InputTag('patMETs'),
    recoMode = cms.string(""),
    scaleFuncImprovedCollinearApprox = cms.string('1'),                                                  
    verbosity = cms.untracked.int32(0)
)

#--------------------------------------------------------------------------------

muTauPairsBgEstZmumuMuonMisIdEnriched = cms.EDProducer("PATMuTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedPatMuonsPionVetoCumulative'),
    srcLeg2 = cms.InputTag('tausBgEstZmumuMuonMisIdEnrichedAntiMuonVeto'),
    dRmin12 = cms.double(0.7),
    srcMET = cms.InputTag('patMETs'),
    recoMode = cms.string(""),
    scaleFuncImprovedCollinearApprox = cms.string('1'),                                                   
    verbosity = cms.untracked.int32(0)
)

muTauPairsBgEstZmumuMuonMisIdEnrichedZeroCharge = cms.EDFilter("PATMuTauPairSelector",
    src = cms.InputTag('muTauPairsBgEstZmumuMuonMisIdEnriched'),                                                   
    cut = cms.string('charge = 0'),
    filter = cms.bool(False)
)

#--------------------------------------------------------------------------------

selectMuTauPairsBgEstZmumuEnriched = cms.Sequence(
    muTauPairsBgEstZmumuJetMisIdEnriched
   + muTauPairsBgEstZmumuMuonMisIdEnriched + muTauPairsBgEstZmumuMuonMisIdEnrichedZeroCharge
)

#--------------------------------------------------------------------------------  
# produce collection of muon + muon combinations
#--------------------------------------------------------------------------------

diMuonPairsBgEstZmumuJetMisIdEnriched = cms.EDProducer("DiCandidatePairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedPatMuonsTrkIPcumulative'),
    srcLeg2 = cms.InputTag('selectedPatMuonsGlobalIndividual'),
    dRmin12 = cms.double(0.5),
    srcMET = cms.InputTag(''),
    recoMode = cms.string(""),
    scaleFuncImprovedCollinearApprox = cms.string('1'),                                                   
    verbosity = cms.untracked.int32(0)
)

diMuonPairsZeroChargeBgEstZmumuJetMisIdEnriched = cms.EDFilter("DiCandidatePairSelector",
    src = cms.InputTag('diMuonPairsBgEstZmumuJetMisIdEnriched'),                                   
    cut = cms.string('charge = 0'),
    filter = cms.bool(False)
)
#
# Note: do not apply tight cut on M(muon + muon) invariant mass
#       as it would bias the muon + tau-jet visible invariant mass distribution
#   
diMuonPairsInvMassBgEstZmumuJetMisIdEnriched = cms.EDFilter("DiCandidatePairSelector",
    src = cms.InputTag('diMuonPairsZeroChargeBgEstZmumuJetMisIdEnriched'),                                   
    cut = cms.string('p4Vis.mass > 40. & p4Vis.mass < 100.'),
    filter = cms.bool(False)
)

#--------------------------------------------------------------------------------

diMuonPairsBgEstZmumuMuonMisIdEnriched = cms.EDProducer("DiCandidatePairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedPatMuonsTrkIPcumulative'),
    srcLeg2 = cms.InputTag('muonsBgEstZmumuMuonMisIdEnrichedStandalone'),
    dRmin12 = cms.double(0.5),
    srcMET = cms.InputTag(''),
    recoMode = cms.string(""),
    scaleFuncImprovedCollinearApprox = cms.string('1'),                                                    
    verbosity = cms.untracked.int32(0)
)

#--------------------------------------------------------------------------------

selectDiMuonPairsBgEstZmumuEnriched = cms.Sequence(
    diMuonPairsBgEstZmumuJetMisIdEnriched + diMuonPairsZeroChargeBgEstZmumuJetMisIdEnriched + diMuonPairsInvMassBgEstZmumuJetMisIdEnriched
   + diMuonPairsBgEstZmumuMuonMisIdEnriched
)

#--------------------------------------------------------------------------------  
# produce boolean event selection flags
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.selectZtoMuTau_cff import *

cfgTauMuonVetoBgEstZmumuJetMisIdEnriched = copy.deepcopy(cfgTauMuonVeto)
cfgTauMuonVetoBgEstZmumuJetMisIdEnriched.pluginName = cms.string('tauMuonVetoBgEstZmumuJetMisIdEnriched')
cfgTauMuonVetoBgEstZmumuJetMisIdEnriched.src_cumulative = cms.InputTag('tausBgEstZmumuJetMisIdEnrichedMuonVeto')
cfgTauMuonVetoBgEstZmumuJetMisIdEnriched.systematics = cms.vstring()

cfgMuTauPairBgEstZmumuJetMisIdEnriched = cms.PSet(
    pluginName = cms.string('muTauPairBgEstZmumuJetMisIdEnriched'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('muTauPairsBgEstZmumuJetMisIdEnriched'),
    minNumber = cms.uint32(1)
)

cfgDiMuonPairBgEstZmumuJetMisIdEnriched = cms.PSet(
    pluginName = cms.string('diMuonPairBgEstZmumuJetMisIdEnriched'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('diMuonPairsBgEstZmumuJetMisIdEnriched'),
    minNumber = cms.uint32(1)
)

cfgDiMuonPairZeroChargeBgEstZmumuJetMisIdEnriched = cms.PSet(
    pluginName = cms.string('diMuonPairZeroChargeBgEstZmumuJetMisIdEnriched'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('diMuonPairsZeroChargeBgEstZmumuJetMisIdEnriched'),
    minNumber = cms.uint32(1)
)

cfgDiMuonPairInvMassBgEstZmumuJetMisIdEnriched = cms.PSet(
    pluginName = cms.string('diMuonPairInvMassBgEstZmumuJetMisIdEnriched'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('diMuonPairsInvMassBgEstZmumuJetMisIdEnriched'),
    minNumber = cms.uint32(1)
)

#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.selectZtoMuTau_cff import *

cfgMuonStandaloneZmumuMuonMisIdEnriched = cms.PSet(
    pluginName = cms.string('muonStandaloneZmumuMuonMisIdEnriched'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('muonsBgEstZmumuMuonMisIdEnrichedStandalone'),
    minNumber = cms.uint32(2)
)

cfgTauEtaBgEstZmumuMuonMisIdEnriched = copy.deepcopy(cfgTauEtaCut)
cfgTauEtaBgEstZmumuMuonMisIdEnriched.pluginName = cms.string('tauEtaBgEstZmumuMuonMisIdEnriched')
cfgTauEtaBgEstZmumuMuonMisIdEnriched.src_cumulative = cms.InputTag('selectedPatTausEta21Cumulative')
cfgTauEtaBgEstZmumuMuonMisIdEnriched.systematics = cms.vstring()

cfgTauPtBgEstZmumuMuonMisIdEnriched = copy.deepcopy(cfgTauPtCut)
cfgTauPtBgEstZmumuMuonMisIdEnriched.pluginName = cms.string('tauPtBgEstZmumuMuonMisIdEnriched')
cfgTauPtBgEstZmumuMuonMisIdEnriched.src_cumulative = cms.InputTag('selectedPatTausPt20Cumulative')
cfgTauPtBgEstZmumuMuonMisIdEnriched.systematics = cms.vstring()

cfgTauLeadTrkBgEstZmumuMuonMisIdEnriched = copy.deepcopy(cfgTauLeadTrkCut)
cfgTauLeadTrkBgEstZmumuMuonMisIdEnriched.pluginName = cms.string('tauLeadTrkBgEstZmumuMuonMisIdEnriched')
cfgTauLeadTrkBgEstZmumuMuonMisIdEnriched.src_cumulative = cms.InputTag('selectedPatTausLeadTrkCumulative')
cfgTauLeadTrkBgEstZmumuMuonMisIdEnriched.systematics = cms.vstring()

cfgTauLeadTrkPtBgEstZmumuMuonMisIdEnriched = copy.deepcopy(cfgTauLeadTrkPtCut)
cfgTauLeadTrkPtBgEstZmumuMuonMisIdEnriched.pluginName = cms.string('tauLeadTrkPtBgEstZmumuMuonMisIdEnriched')
cfgTauLeadTrkPtBgEstZmumuMuonMisIdEnriched.src_cumulative = cms.InputTag('selectedPatTausLeadTrkPtCumulative')
cfgTauLeadTrkPtBgEstZmumuMuonMisIdEnriched.systematics = cms.vstring()

cfgTauTrkIsoBgEstZmumuMuonMisIdEnriched = copy.deepcopy(cfgTauTrkIsoCut)
cfgTauTrkIsoBgEstZmumuMuonMisIdEnriched.pluginName = cms.string('tauTrkIsoBgEstZmumuMuonMisIdEnriched')
cfgTauTrkIsoBgEstZmumuMuonMisIdEnriched.src_cumulative = cms.InputTag('selectedPatTausTrkIsoCumulative')
cfgTauTrkIsoBgEstZmumuMuonMisIdEnriched.systematics = cms.vstring()

cfgTauEcalIsoBgEstZmumuMuonMisIdEnriched = copy.deepcopy(cfgTauEcalIsoCut)
cfgTauEcalIsoBgEstZmumuMuonMisIdEnriched.pluginName = cms.string('tauEcalIsoBgEstZmumuMuonMisIdEnriched')
cfgTauEcalIsoBgEstZmumuMuonMisIdEnriched.src_cumulative = cms.InputTag('selectedPatTausEcalIsoCumulative')
cfgTauEcalIsoBgEstZmumuMuonMisIdEnriched.systematics = cms.vstring()

cfgTauProngBgEstZmumuMuonMisIdEnriched = copy.deepcopy(cfgTauProngCut)
cfgTauProngBgEstZmumuMuonMisIdEnriched.pluginName = cms.string('tauProngBgEstZmumuMuonMisIdEnriched')
cfgTauProngBgEstZmumuMuonMisIdEnriched.src_cumulative = cms.InputTag('selectedPatTausProngCumulative')
cfgTauProngBgEstZmumuMuonMisIdEnriched.systematics = cms.vstring()

cfgTauChargeBgEstZmumuMuonMisIdEnriched = copy.deepcopy(cfgTauChargeCut)
cfgTauChargeBgEstZmumuMuonMisIdEnriched.pluginName = cms.string('tauChargeBgEstZmumuMuonMisIdEnriched')
cfgTauChargeBgEstZmumuMuonMisIdEnriched.src_cumulative = cms.InputTag('selectedPatTausChargeCumulative')
cfgTauChargeBgEstZmumuMuonMisIdEnriched.systematics = cms.vstring()

cfgTauAntiMuonVetoBgEstZmumuMuonMisIdEnriched = copy.deepcopy(cfgTauMuonVeto)
cfgTauAntiMuonVetoBgEstZmumuMuonMisIdEnriched.pluginName = cms.string('tauAntiMuonVetoBgEstZmumuMuonMisIdEnriched')
cfgTauAntiMuonVetoBgEstZmumuMuonMisIdEnriched.src_cumulative = cms.InputTag('tausBgEstZmumuMuonMisIdEnrichedAntiMuonVeto')
cfgTauAntiMuonVetoBgEstZmumuMuonMisIdEnriched.systematics = cms.vstring()

cfgMuTauPairBgEstZmumuMuonMisIdEnriched = cms.PSet(
    pluginName = cms.string('muTauPairBgEstZmumuMuonMisIdEnriched'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('muTauPairsBgEstZmumuMuonMisIdEnriched'),
    minNumber = cms.uint32(1)
)

cfgMuTauPairZeroChargeBgEstZmumuMuonMisIdEnriched = cms.PSet(
    pluginName = cms.string('muTauPairZeroChargeBgEstZmumuMuonMisIdEnriched'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('muTauPairsBgEstZmumuMuonMisIdEnrichedZeroCharge'),
    minNumber = cms.uint32(1)
)

cfgDiMuonPairBgEstZmumuMuonMisIdEnriched = cms.PSet(
    pluginName = cms.string('diMuonPairBgEstZmumuMuonMisIdEnriched'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('diMuonPairsBgEstZmumuMuonMisIdEnriched'),
    minNumber = cms.uint32(1)
)

#--------------------------------------------------------------------------------

evtSelConfiguratorBgEstZmumuEnriched = eventSelFlagProdConfigurator(
    [ cfgTauMuonVetoBgEstZmumuJetMisIdEnriched,
      cfgMuTauPairBgEstZmumuJetMisIdEnriched,
      cfgDiMuonPairBgEstZmumuJetMisIdEnriched,
      cfgDiMuonPairZeroChargeBgEstZmumuJetMisIdEnriched,
      cfgDiMuonPairInvMassBgEstZmumuJetMisIdEnriched,
      cfgMuonStandaloneZmumuMuonMisIdEnriched,      
      cfgTauEtaBgEstZmumuMuonMisIdEnriched,
      cfgTauPtBgEstZmumuMuonMisIdEnriched,
      cfgTauLeadTrkBgEstZmumuMuonMisIdEnriched,
      cfgTauLeadTrkPtBgEstZmumuMuonMisIdEnriched,
      cfgTauTrkIsoBgEstZmumuMuonMisIdEnriched,
      cfgTauEcalIsoBgEstZmumuMuonMisIdEnriched,
      cfgTauProngBgEstZmumuMuonMisIdEnriched,
      cfgTauChargeBgEstZmumuMuonMisIdEnriched,
      cfgTauAntiMuonVetoBgEstZmumuMuonMisIdEnriched,      
      cfgMuTauPairBgEstZmumuMuonMisIdEnriched,
      cfgMuTauPairZeroChargeBgEstZmumuMuonMisIdEnriched,
      cfgDiMuonPairBgEstZmumuMuonMisIdEnriched ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

selectEventsBgEstZmumuEnriched = evtSelConfiguratorBgEstZmumuEnriched.configure()

#--------------------------------------------------------------------------------  
# apply event selection criteria; fill histograms
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.analyzeZtoMuTau_cfi import *
from TauAnalysis.BgEstimationTools.tauIdEffZtoMuTauHistManager_cfi import *

muonHistManagerBgEstZmumuEnriched = copy.deepcopy(muonHistManager)
muonHistManagerBgEstZmumuEnriched.pluginName = cms.string('muonHistManagerBgEstZmumuEnriched')
muonHistManagerBgEstZmumuEnriched.muonSource = cms.InputTag('selectedPatMuonsPionVetoCumulative')

dataBinnerBgEstZmumuEnriched = copy.deepcopy(dataBinner)
dataBinnerBgEstZmumuEnriched.pluginName = cms.string('dataBinnerBgEstZmumuEnriched')

#--------------------------------------------------------------------------------

tauHistManagerBgEstZmumuJetMisIdEnriched = copy.deepcopy(tauHistManager)
tauHistManagerBgEstZmumuJetMisIdEnriched.pluginName = cms.string('tauHistManagerBgEstZmumuJetMisIdEnriched')
tauHistManagerBgEstZmumuJetMisIdEnriched.tauSource = cms.InputTag('tausBgEstZmumuJetMisIdEnrichedMuonVeto')

diTauCandidateHistManagerBgEstZmumuJetMisIdEnriched = copy.deepcopy(diTauCandidateHistManagerForMuTau)
diTauCandidateHistManagerBgEstZmumuJetMisIdEnriched.pluginName = cms.string('diTauCandidateHistManagerBgEstZmumuJetMisIdEnriched')
diTauCandidateHistManagerBgEstZmumuJetMisIdEnriched.diTauCandidateSource = cms.InputTag('muTauPairsBgEstZmumuJetMisIdEnriched')
diTauCandidateHistManagerBgEstZmumuJetMisIdEnriched.visMassHypothesisSource = cms.InputTag('')

tauIdEffHistManagerBgEstZmumuJetMisIdEnriched = copy.deepcopy(tauIdEffZtoMuTauHistManager)
tauIdEffHistManagerBgEstZmumuJetMisIdEnriched.pluginName = cms.string('tauIdEffHistManagerBgEstZmumuJetMisIdEnriched')
tauIdEffHistManagerBgEstZmumuJetMisIdEnriched.muonSource = cms.InputTag('selectedPatMuonsPionVetoCumulative')
tauIdEffHistManagerBgEstZmumuJetMisIdEnriched.tauSource = cms.InputTag('tausBgEstZmumuJetMisIdEnrichedMuonVeto')
tauIdEffHistManagerBgEstZmumuJetMisIdEnriched.diTauSource = cms.InputTag('muTauPairsBgEstZmumuJetMisIdEnriched')
tauIdEffHistManagerBgEstZmumuJetMisIdEnriched.diTauChargeSignExtractor.src = tauIdEffHistManagerBgEstZmumuJetMisIdEnriched.diTauSource

analyzeEventsBgEstZmumuJetMisIdEnriched = cms.EDAnalyzer("GenericAnalyzer",
  
    name = cms.string('BgEstTemplateAnalyzer_ZmumuJetMisIdEnriched'), 
                            
    filters = cms.VPSet(
        genPhaseSpaceCut,
        evtSelTrigger,
        evtSelPrimaryEventVertex,
        evtSelPrimaryEventVertexQuality,
        evtSelPrimaryEventVertexPosition,
        evtSelGlobalMuon,
        evtSelMuonEta,
        evtSelMuonPt,
        evtSelMuonTrkIso,
        evtSelMuonEcalIso,
        evtSelMuonCombIso,
        evtSelMuonAntiPion,
        evtSelTauAntiOverlapWithMuonsVeto,
        evtSelTauEta,
        evtSelTauPt,
        evtSelTauLeadTrk,
        evtSelTauLeadTrkPt,
        evtSelTauTrkIso,
        evtSelTauEcalIso,
        cms.PSet(
            pluginName = cms.string('tauMuonVetoBgEstZmumuJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauMuonVetoBgEstZmumuJetMisIdEnriched', 'cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('muTauPairBgEstZmumuJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('muTauPairBgEstZmumuJetMisIdEnriched')
        ),
        cms.PSet(
            pluginName = cms.string('diMuonPairBgEstZmumuJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diMuonPairBgEstZmumuJetMisIdEnriched')
        ),
        cms.PSet(
            pluginName = cms.string('diMuonPairZeroChargeBgEstZmumuJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diMuonPairZeroChargeBgEstZmumuJetMisIdEnriched')
        ),
        cms.PSet(
            pluginName = cms.string('diMuonPairInvMassBgEstZmumuJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diMuonPairInvMassBgEstZmumuJetMisIdEnriched')
        )        
    ),
  
    analyzers = cms.VPSet(
        muonHistManagerBgEstZmumuEnriched,
        tauHistManagerBgEstZmumuJetMisIdEnriched,
        diTauCandidateHistManagerBgEstZmumuJetMisIdEnriched,
        tauIdEffHistManagerBgEstZmumuJetMisIdEnriched,
        dataBinnerBgEstZmumuEnriched
    ),

    eventDumps = cms.VPSet(),
   
    analysisSequence = cms.VPSet(
    
        # generator level phase-space selection
        # (NOTE: (1) to be used in case of Monte Carlo samples
        #            overlapping in simulated phase-space only !!
        #        (2) genPhaseSpaceCut needs to be **always** the first entry in the list of cuts
        #           - otherwise the script submitToBatch.csh for submission of cmsRun jobs
        #            to the CERN batch system will not work !!)
        cms.PSet(
            filter = cms.string('genPhaseSpaceCut'),
            title = cms.string('gen. Phase-Space')
        ),
        cms.PSet(
            filter = cms.string('evtSelTrigger'),
            title = cms.string('mu15 || isoMu11 Trigger')
        ),
        cms.PSet(
            filter = cms.string('evtSelPrimaryEventVertex'),
            title = cms.string('Vertex')
        ),
        cms.PSet(
            filter = cms.string('evtSelPrimaryEventVertexQuality'),
            title = cms.string('p(chi2Vertex) > 0.01')
        ),
        cms.PSet(
            filter = cms.string('evtSelPrimaryEventVertexPosition'),
            title = cms.string('-25 < zVertex < +25 cm')
        ),
        cms.PSet(
            filter = cms.string('evtSelGlobalMuon'),
            title = cms.string('global Muon')
        ),
        cms.PSet(
            filter = cms.string('evtSelMuonEta'),
            title = cms.string('-2.1 < eta(Muon) < +2.1')
        ),
        cms.PSet(
            filter = cms.string('evtSelMuonPt'),
            title = cms.string('Pt(Muon) > 15 GeV')
        ),
        cms.PSet(
            filter = cms.string('evtSelTauAntiOverlapWithMuonsVeto'),
            title = cms.string('Tau not overlapping w. Muon')
        ),
        cms.PSet(
            filter = cms.string('evtSelTauEta'),
            title = cms.string('-2.1 < eta(Tau) < +2.1')
        ),
        cms.PSet(
            filter = cms.string('evtSelTauPt'),
            title = cms.string('Pt(Tau) > 20 GeV')
        ),
        cms.PSet(
            filter = cms.string('evtSelMuonTrkIso'),
            title = cms.string('Muon Track iso.')
        ),
        cms.PSet(
            filter = cms.string('evtSelMuonEcalIso'),
            title = cms.string('Muon ECAL iso.')
        ),
        cms.PSet(
            filter = cms.string('evtSelMuonCombIso'),
            title = cms.string('Muon Combined iso.')
        ),
        cms.PSet(
            filter = cms.string('evtSelMuonAntiPion'),
            title = cms.string('Muon pi-Veto')
        ),
        cms.PSet(
            filter = cms.string('evtSelTauLeadTrk'),
            title = cms.string('Tau lead. Track find.')
        ),
        cms.PSet(
            filter = cms.string('evtSelTauLeadTrkPt'),
            title = cms.string('Tau lead. Track Pt')
        ),
        cms.PSet(
            filter = cms.string('evtSelTauTrkIso'),
            title = cms.string('Tau Track iso.')
        ),
        cms.PSet(
            filter = cms.string('evtSelTauEcalIso'),
            title = cms.string('Tau ECAL iso.')
        ),
        cms.PSet(
            filter = cms.string('tauMuonVetoBgEstZmumuJetMisIdEnriched'),
            title = cms.string('Tau mu-Veto')
        ),
        cms.PSet(
            filter = cms.string('muTauPairBgEstZmumuJetMisIdEnriched'),
            title = cms.string('dR(Muon-Tau) > 0.7')
        ),
        cms.PSet(
            filter = cms.string('diMuonPairBgEstZmumuJetMisIdEnriched'),
            title = cms.string('Muon-Pair')
        ),
        cms.PSet(
            filter = cms.string('diMuonPairZeroChargeBgEstZmumuJetMisIdEnriched'),
            title = cms.string('Charge(Muon-Pair) = 0')
        ),
        cms.PSet(
            analyzers = cms.vstring(
                'muonHistManagerBgEstZmumuEnriched',
                'tauHistManagerBgEstZmumuJetMisIdEnriched',
                'diTauCandidateHistManagerBgEstZmumuJetMisIdEnriched'
            )
        ),
        cms.PSet(
            filter = cms.string('diMuonPairInvMassBgEstZmumuJetMisIdEnriched'),
            title = cms.string('40 < M (Muon-Pair) < 100 GeV')
        ),
        cms.PSet(
            analyzers = cms.vstring(
                'muonHistManagerBgEstZmumuEnriched',
                'tauHistManagerBgEstZmumuJetMisIdEnriched',
                'diTauCandidateHistManagerBgEstZmumuJetMisIdEnriched',
                'tauIdEffHistManagerBgEstZmumuJetMisIdEnriched',
                'dataBinnerBgEstZmumuEnriched'
            )
        )
    )
)

#--------------------------------------------------------------------------------

tauHistManagerBgEstZmumuMuonMisIdEnriched = copy.deepcopy(tauHistManager)
tauHistManagerBgEstZmumuMuonMisIdEnriched.pluginName = cms.string('tauHistManagerBgEstZmumuMuonMisIdEnriched')
tauHistManagerBgEstZmumuMuonMisIdEnriched.tauSource = cms.InputTag('tausBgEstZmumuMuonMisIdEnrichedAntiMuonVeto')

diTauCandidateHistManagerBgEstZmumuMuonMisIdEnriched = copy.deepcopy(diTauCandidateHistManagerForMuTau)
diTauCandidateHistManagerBgEstZmumuMuonMisIdEnriched.pluginName = cms.string('diTauCandidateHistManagerBgEstZmumuMuonMisIdEnriched')
diTauCandidateHistManagerBgEstZmumuMuonMisIdEnriched.diTauCandidateSource = cms.InputTag('muTauPairsBgEstZmumuMuonMisIdEnrichedZeroCharge')
diTauCandidateHistManagerBgEstZmumuMuonMisIdEnriched.visMassHypothesisSource = cms.InputTag('')

tauIdEffHistManagerBgEstZmumuMuonMisIdEnriched = copy.deepcopy(tauIdEffZtoMuTauHistManager)
tauIdEffHistManagerBgEstZmumuMuonMisIdEnriched.pluginName = cms.string('tauIdEffHistManagerBgEstZmumuMuonMisIdEnriched')
tauIdEffHistManagerBgEstZmumuMuonMisIdEnriched.muonSource = cms.InputTag('selectedPatMuonsPionVetoCumulative')
tauIdEffHistManagerBgEstZmumuMuonMisIdEnriched.tauSource = cms.InputTag('tausBgEstZmumuMuonMisIdEnrichedAntiMuonVeto')
tauIdEffHistManagerBgEstZmumuMuonMisIdEnriched.diTauSource = cms.InputTag('muTauPairsBgEstZmumuMuonMisIdEnrichedZeroCharge')
tauIdEffHistManagerBgEstZmumuMuonMisIdEnriched.diTauChargeSignExtractor.src = tauIdEffHistManagerBgEstZmumuMuonMisIdEnriched.diTauSource

analyzeEventsBgEstZmumuMuonMisIdEnriched = cms.EDAnalyzer("GenericAnalyzer",
  
    name = cms.string('BgEstTemplateAnalyzer_ZmumuMuonMisIdEnriched'), 
                            
    filters = cms.VPSet(
        genPhaseSpaceCut,
        evtSelTrigger,
        evtSelPrimaryEventVertex,
        evtSelPrimaryEventVertexQuality,
        evtSelPrimaryEventVertexPosition,
        cms.PSet(
            pluginName = cms.string('muonStandaloneZmumuMuonMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('muonStandaloneZmumuMuonMisIdEnriched')
        ),
        evtSelGlobalMuon,
        evtSelMuonEta,
        evtSelMuonPt,
        evtSelMuonTrkIso,
        evtSelMuonEcalIso,
        evtSelMuonCombIso,
        evtSelMuonAntiPion,
        cms.PSet(
            pluginName = cms.string('tauEtaBgEstZmumuMuonMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauEtaBgEstZmumuMuonMisIdEnriched', 'cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauPtBgEstZmumuMuonMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauPtBgEstZmumuMuonMisIdEnriched', 'cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauLeadTrkBgEstZmumuMuonMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauLeadTrkBgEstZmumuMuonMisIdEnriched', 'cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauLeadTrkPtBgEstZmumuMuonMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauLeadTrkPtBgEstZmumuMuonMisIdEnriched', 'cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauTrkIsoBgEstZmumuMuonMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauTrkIsoBgEstZmumuMuonMisIdEnriched', 'cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauEcalIsoBgEstZmumuMuonMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauEcalIsoBgEstZmumuMuonMisIdEnriched', 'cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauProngBgEstZmumuMuonMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauProngBgEstZmumuMuonMisIdEnriched', 'cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauChargeBgEstZmumuMuonMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauChargeBgEstZmumuMuonMisIdEnriched', 'cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauAntiMuonVetoBgEstZmumuMuonMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauAntiMuonVetoBgEstZmumuMuonMisIdEnriched', 'cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('muTauPairBgEstZmumuMuonMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('muTauPairBgEstZmumuMuonMisIdEnriched')
        ),
        cms.PSet(
            pluginName = cms.string('muTauPairZeroChargeBgEstZmumuMuonMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('muTauPairZeroChargeBgEstZmumuMuonMisIdEnriched')
        ),
        cms.PSet(
            pluginName = cms.string('diMuonPairBgEstZmumuMuonMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diMuonPairBgEstZmumuMuonMisIdEnriched')
        )
    ),
  
    analyzers = cms.VPSet(
        muonHistManagerBgEstZmumuEnriched,
        tauHistManagerBgEstZmumuMuonMisIdEnriched,
        diTauCandidateHistManagerBgEstZmumuMuonMisIdEnriched,
        tauIdEffHistManagerBgEstZmumuMuonMisIdEnriched,
        dataBinnerBgEstZmumuEnriched
    ),

    eventDumps = cms.VPSet(),
   
    analysisSequence = cms.VPSet(
    
        # generator level phase-space selection
        # (NOTE: (1) to be used in case of Monte Carlo samples
        #            overlapping in simulated phase-space only !!
        #        (2) genPhaseSpaceCut needs to be **always** the first entry in the list of cuts
        #           - otherwise the script submitToBatch.csh for submission of cmsRun jobs
        #            to the CERN batch system will not work !!)
        cms.PSet(
            filter = cms.string('genPhaseSpaceCut'),
            title = cms.string('gen. Phase-Space')
        ),
        cms.PSet(
            filter = cms.string('evtSelTrigger'),
            title = cms.string('mu15 || isoMu11 Trigger')
        ),
        cms.PSet(
            filter = cms.string('evtSelPrimaryEventVertex'),
            title = cms.string('Vertex')
        ),
        cms.PSet(
            filter = cms.string('evtSelPrimaryEventVertexQuality'),
            title = cms.string('p(chi2Vertex) > 0.01')
        ),
        cms.PSet(
            filter = cms.string('evtSelPrimaryEventVertexPosition'),
            title = cms.string('-25 < zVertex < +25 cm')
        ),
        cms.PSet(
            filter = cms.string('muonStandaloneZmumuMuonMisIdEnriched'),
            title = cms.string('at least two stand-alone Muons')
        ),
        cms.PSet(
            filter = cms.string('evtSelGlobalMuon'),
            title = cms.string('global Muon')
        ),
        cms.PSet(
            filter = cms.string('evtSelMuonEta'),
            title = cms.string('-2.1 < eta(Muon) < +2.1')
        ),
        cms.PSet(
            filter = cms.string('evtSelMuonPt'),
            title = cms.string('Pt(Muon) > 15 GeV')
        ),
        cms.PSet(
            filter = cms.string('tauEtaBgEstZmumuMuonMisIdEnriched'),
            title = cms.string('-2.1 < eta(Tau) < +2.1')
        ),
        cms.PSet(
            filter = cms.string('tauPtBgEstZmumuMuonMisIdEnriched'),
            title = cms.string('Pt(Tau) > 20 GeV')
        ),
        cms.PSet(
            filter = cms.string('evtSelMuonTrkIso'),
            title = cms.string('Muon Track iso.')
        ),
        cms.PSet(
            filter = cms.string('evtSelMuonEcalIso'),
            title = cms.string('Muon ECAL iso.')
        ),
        cms.PSet(
            filter = cms.string('evtSelMuonCombIso'),
            title = cms.string('Muon Combined iso.')
        ),
        cms.PSet(
            filter = cms.string('evtSelMuonAntiPion'),
            title = cms.string('Muon pi-Veto')
        ),
        cms.PSet(
            filter = cms.string('tauLeadTrkBgEstZmumuMuonMisIdEnriched'),
            title = cms.string('Tau lead. Track find.')
        ),
        cms.PSet(
            filter = cms.string('tauLeadTrkPtBgEstZmumuMuonMisIdEnriched'),
            title = cms.string('Tau lead. Track Pt')
        ),
        cms.PSet(
            filter = cms.string('tauTrkIsoBgEstZmumuMuonMisIdEnriched'),
            title = cms.string('Tau Track iso.')
        ),
        cms.PSet(
            filter = cms.string('tauEcalIsoBgEstZmumuMuonMisIdEnriched'),
            title = cms.string('Tau ECAL iso.')
        ),
        cms.PSet(
            filter = cms.string('tauProngBgEstZmumuMuonMisIdEnriched'),
            title = cms.string('Tau 1||3-Prong')
        ),
        cms.PSet(
            filter = cms.string('tauChargeBgEstZmumuMuonMisIdEnriched'),
            title = cms.string('Charge(Tau) = +/-1')
        ),
        cms.PSet(
            filter = cms.string('tauAntiMuonVetoBgEstZmumuMuonMisIdEnriched'),
            title = cms.string('Tau anti-mu-Veto')
        ),
        cms.PSet(
            filter = cms.string('muTauPairBgEstZmumuMuonMisIdEnriched'),
            title = cms.string('dR(Muon-Tau) > 0.7')
        ),
        cms.PSet(
            filter = cms.string('muTauPairZeroChargeBgEstZmumuMuonMisIdEnriched'),
            title = cms.string('Charge(Muon+Tau) = 0')
        ),
        cms.PSet(
            filter = cms.string('diMuonPairBgEstZmumuMuonMisIdEnriched'),
            title = cms.string('Muon-Pair')
        ),
        cms.PSet(
            analyzers = cms.vstring(
                'muonHistManagerBgEstZmumuEnriched',
                'tauHistManagerBgEstZmumuMuonMisIdEnriched',
                'diTauCandidateHistManagerBgEstZmumuMuonMisIdEnriched',
                'tauIdEffHistManagerBgEstZmumuMuonMisIdEnriched',
                'dataBinnerBgEstZmumuEnriched'
            )
        )
    )
)

#--------------------------------------------------------------------------------  
# define (final) analysis sequence
#--------------------------------------------------------------------------------

bgEstZmumuEnrichedAnalysisSequence = cms.Sequence(
    selectMuonsBgEstZmumuEnriched
   + selectTausBgEstZmumuEnriched
   + selectMuTauPairsBgEstZmumuEnriched
   + selectDiMuonPairsBgEstZmumuEnriched
   + selectEventsBgEstZmumuEnriched   
   + analyzeEventsBgEstZmumuJetMisIdEnriched + analyzeEventsBgEstZmumuMuonMisIdEnriched
)
