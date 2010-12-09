import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *
from TauAnalysis.RecoTools.tools.eventSelFlagProdConfigurator import *

#--------------------------------------------------------------------------------
# select Z --> mu+ mu- background enriched event sample
#--------------------------------------------------------------------------------

muonsBgEstZmumuMuonMisIdEnrichedStandalone = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag('cleanPatMuons'),                                   
    cut = cms.string('isStandAloneMuon()'),
    filter = cms.bool(False)
)

selectMuonsBgEstZmumuEnriched = cms.Sequence(muonsBgEstZmumuMuonMisIdEnrichedStandalone)
    
#--------------------------------------------------------------------------------  
# produce collection of pat::Taus
#--------------------------------------------------------------------------------

from TauAnalysis.RecoTools.patPFTauSelection_cfi import *
from TauAnalysis.RecoTools.patPFTauSelectionForMuTau_cfi import *

tausBgEstZmumuJetMisIdEnrichedTaNCdiscr = copy.deepcopy(selectedPatTausTaNCdiscr)
tausBgEstZmumuJetMisIdEnrichedTaNCdiscr.cut = cms.string('tauID("byTaNCvloose") > 0.5 & tauID("byTaNCloose") < 0.5')

tausBgEstZmumuJetMisIdEnrichedMuonVeto = copy.deepcopy(selectedPatTausMuonVeto)

tauSelConfiguratorBgEstZmumuJetMisIdEnriched = objSelConfigurator(
    [ tausBgEstZmumuJetMisIdEnrichedTaNCdiscr,
      tausBgEstZmumuJetMisIdEnrichedMuonVeto ],
    src = "selectedPatTausForMuTauLeadTrkPtCumulative",
    pyModuleName = __name__,
    doSelIndividual = False
)

selectTausBgEstZmumuJetMisIdEnriched = tauSelConfiguratorBgEstZmumuJetMisIdEnriched.configure(pyNameSpace = locals())

tausBgEstZmumuMuonMisIdEnrichedAntiMuonVeto = cms.EDFilter("PATTauSelector",
    src = cms.InputTag('tausBgEstZmumuJetMisIdEnrichedTaNCdiscrCumulative'),                                              
    cut = cms.string('tauID("againstMuon") < 0.5'),
    filter = cms.bool(False)                                 
)

selectTausBgEstZmumuEnriched = cms.Sequence(selectTausBgEstZmumuJetMisIdEnriched + tausBgEstZmumuMuonMisIdEnrichedAntiMuonVeto)

#--------------------------------------------------------------------------------  
# produce collection of muon + tau-jet combinations
#--------------------------------------------------------------------------------

from TauAnalysis.CandidateTools.muTauPairProduction_cff import *
from TauAnalysis.CandidateTools.muTauPairSelection_cfi import *

muTauPairsBgEstZmumuJetMisIdEnriched = allMuTauPairs.clone(
    srcLeg1 = cms.InputTag('selectedPatMuonsPFRelIsoCumulative'),
    srcLeg2 = cms.InputTag('tausBgEstZmumuJetMisIdEnrichedMuonVetoCumulative'),
    verbosity = cms.untracked.int32(0)
)

muTauPairsBgEstZmumuJetMisIdEnrichedMt1MET = copy.deepcopy(selectedMuTauPairsMt1MET)
muTauPairsBgEstZmumuJetMisIdEnrichedMt1MET.cut = cms.string('mt1MET < 40.')

muTauPairsBgEstZmumuJetMisIdEnrichedPzetaDiff = copy.deepcopy(selectedMuTauPairsPzetaDiff)
muTauPairsBgEstZmumuJetMisIdEnrichedPzetaDiff.cut = cms.string('(pZeta - 1.5*pZetaVis) > -20.')

muTauPairSelConfiguratorBgEstZmumuJetMisIdEnriched = objSelConfigurator(
    [ muTauPairsBgEstZmumuJetMisIdEnrichedMt1MET,
      muTauPairsBgEstZmumuJetMisIdEnrichedPzetaDiff ],
    src = "muTauPairsBgEstZmumuJetMisIdEnriched",
    pyModuleName = __name__,
    doSelIndividual = False
)

selectMuTauPairsBgEstZmumuJetMisIdEnriched = muTauPairSelConfiguratorBgEstZmumuJetMisIdEnriched.configure(pyNameSpace = locals())

#--------------------------------------------------------------------------------

muTauPairsBgEstZmumuMuonMisIdEnriched = allMuTauPairs.clone(
    srcLeg1 = cms.InputTag('selectedPatMuonsPFRelIsoCumulative'),
    srcLeg2 = cms.InputTag('tausBgEstZmumuMuonMisIdEnrichedAntiMuonVeto'),
    verbosity = cms.untracked.int32(0)
)

muTauPairsBgEstZmumuMuonMisIdEnrichedZeroCharge = cms.EDFilter("PATMuTauPairSelector",
    src = cms.InputTag('muTauPairsBgEstZmumuMuonMisIdEnriched'),                                                   
    cut = cms.string('charge = 0'),
    filter = cms.bool(False)
)

#--------------------------------------------------------------------------------

selectMuTauPairsBgEstZmumuEnriched = cms.Sequence(
    muTauPairsBgEstZmumuJetMisIdEnriched + selectMuTauPairsBgEstZmumuJetMisIdEnriched
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

cfgTauTaNCdiscrBgEstZmumuJetMisIdEnriched = copy.deepcopy(cfgTauTaNCdiscrCut)
cfgTauTaNCdiscrBgEstZmumuJetMisIdEnriched.pluginName = cms.string('tauTaNCdiscrBgEstZmumuJetMisIdEnriched')
cfgTauTaNCdiscrBgEstZmumuJetMisIdEnriched.src_cumulative = cms.InputTag('tausBgEstZmumuJetMisIdEnrichedTaNCdiscrCumulative')
cfgTauTaNCdiscrBgEstZmumuJetMisIdEnriched.systematics = cms.vstring()

cfgTauMuonVetoBgEstZmumuJetMisIdEnriched = copy.deepcopy(cfgTauMuonVeto)
cfgTauMuonVetoBgEstZmumuJetMisIdEnriched.pluginName = cms.string('tauMuonVetoBgEstZmumuJetMisIdEnriched')
cfgTauMuonVetoBgEstZmumuJetMisIdEnriched.src_cumulative = cms.InputTag('tausBgEstZmumuJetMisIdEnrichedMuonVetoCumulative')
cfgTauMuonVetoBgEstZmumuJetMisIdEnriched.systematics = cms.vstring()

cfgMuTauPairBgEstZmumuJetMisIdEnriched = cms.PSet(
    pluginName = cms.string('muTauPairBgEstZmumuJetMisIdEnriched'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('muTauPairsBgEstZmumuJetMisIdEnriched'),
    minNumber = cms.uint32(1)
)

cfgMuTauPairMt1METbgEstZmumuJetMisIdEnriched = cms.PSet(
    pluginName = cms.string('muTauPairMt1METbgEstZmumuJetMisIdEnriched'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('muTauPairsBgEstZmumuJetMisIdEnrichedMt1METcumulative'),
    minNumber = cms.uint32(1)
)

cfgMuTauPairPzetaDiffBgEstZmumuJetMisIdEnriched = cms.PSet(
    pluginName = cms.string('muTauPairPzetaDiffBgEstZmumuJetMisIdEnriched'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('muTauPairsBgEstZmumuJetMisIdEnrichedPzetaDiffCumulative'),
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

# NOTE: no anti-overlap with muons veto not applied to tau-jets
cfgTauEtaBgEstZmumuMuonMisIdEnriched = copy.deepcopy(cfgTauEtaCut)
cfgTauEtaBgEstZmumuMuonMisIdEnriched.pluginName = cms.string('tauEtaBgEstZmumuMuonMisIdEnriched')
cfgTauEtaBgEstZmumuMuonMisIdEnriched.src_cumulative = cms.InputTag('selectedPatTausEta23Cumulative')
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

cfgTauTaNCdiscrBgEstZmumuMuonMisIdEnriched = copy.deepcopy(cfgTauTaNCdiscrCut)
cfgTauTaNCdiscrBgEstZmumuMuonMisIdEnriched.pluginName = cms.string('tauTaNCdiscrBgEstZmumuMuonMisIdEnriched')
cfgTauTaNCdiscrBgEstZmumuMuonMisIdEnriched.src_cumulative = cms.InputTag('selectedPatTausTaNCdiscrCumulative')
cfgTauTaNCdiscrBgEstZmumuMuonMisIdEnriched.systematics = cms.vstring()

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
    [ cfgTauTaNCdiscrBgEstZmumuJetMisIdEnriched,
      cfgTauMuonVetoBgEstZmumuJetMisIdEnriched,
      cfgMuTauPairBgEstZmumuJetMisIdEnriched,
      cfgMuTauPairMt1METbgEstZmumuJetMisIdEnriched,
      cfgMuTauPairPzetaDiffBgEstZmumuJetMisIdEnriched,
      cfgDiMuonPairBgEstZmumuJetMisIdEnriched,
      cfgDiMuonPairZeroChargeBgEstZmumuJetMisIdEnriched,
      cfgDiMuonPairInvMassBgEstZmumuJetMisIdEnriched,
      cfgMuonStandaloneZmumuMuonMisIdEnriched,
      cfgTauEtaBgEstZmumuMuonMisIdEnriched,
      cfgTauPtBgEstZmumuMuonMisIdEnriched,
      cfgTauLeadTrkBgEstZmumuMuonMisIdEnriched,
      cfgTauLeadTrkPtBgEstZmumuMuonMisIdEnriched,
      cfgTauTaNCdiscrBgEstZmumuMuonMisIdEnriched,
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
muonHistManagerBgEstZmumuEnriched.muonSource = cms.InputTag('selectedPatMuonsPFRelIsoCumulative')

dataBinnerBgEstZmumuEnriched = copy.deepcopy(dataBinner)
dataBinnerBgEstZmumuEnriched.pluginName = cms.string('dataBinnerBgEstZmumuEnriched')

#--------------------------------------------------------------------------------

tauHistManagerBgEstZmumuJetMisIdEnriched = copy.deepcopy(tauHistManager)
tauHistManagerBgEstZmumuJetMisIdEnriched.pluginName = cms.string('tauHistManagerBgEstZmumuJetMisIdEnriched')
tauHistManagerBgEstZmumuJetMisIdEnriched.tauSource = cms.InputTag('tausBgEstZmumuJetMisIdEnrichedMuonVetoCumulative')

diTauCandidateHistManagerBgEstZmumuJetMisIdEnriched = copy.deepcopy(diTauCandidateHistManagerForMuTau)
diTauCandidateHistManagerBgEstZmumuJetMisIdEnriched.pluginName = cms.string('diTauCandidateHistManagerBgEstZmumuJetMisIdEnriched')
diTauCandidateHistManagerBgEstZmumuJetMisIdEnriched.diTauCandidateSource = cms.InputTag('muTauPairsBgEstZmumuJetMisIdEnrichedPzetaDiffCumulative')
diTauCandidateHistManagerBgEstZmumuJetMisIdEnriched.visMassHypothesisSource = cms.InputTag('')

diTauCandidateSVfitHistManagerBgEstZmumuJetMisIdEnriched = copy.deepcopy(diTauCandidateSVfitHistManagerForMuTau)
diTauCandidateSVfitHistManagerBgEstZmumuJetMisIdEnriched.pluginName = cms.string('diTauCandidateSVfitHistManagerBgEstZmumuJetMisIdEnriched')
diTauCandidateSVfitHistManagerBgEstZmumuJetMisIdEnriched.diTauCandidateSource = diTauCandidateHistManagerBgEstZmumuJetMisIdEnriched.diTauCandidateSource

caloMEtHistManagerBgEstZmumuJetMisIdEnriched = copy.deepcopy(caloMEtHistManager)
caloMEtHistManagerBgEstZmumuJetMisIdEnriched.pluginName = cms.string('caloMEtHistManagerBgEstZmumuJetMisIdEnriched')
caloMEtHistManagerBgEstZmumuJetMisIdEnriched.leg1Source = muonHistManagerBgEstZmumuEnriched.muonSource
caloMEtHistManagerBgEstZmumuJetMisIdEnriched.leg2Source = tauHistManagerBgEstZmumuJetMisIdEnriched.tauSource
pfMEtHistManagerBgEstZmumuJetMisIdEnriched = copy.deepcopy(pfMEtHistManager)
pfMEtHistManagerBgEstZmumuJetMisIdEnriched.pluginName = cms.string('pfMEtHistManagerBgEstZmumuJetMisIdEnriched')
pfMEtHistManagerBgEstZmumuJetMisIdEnriched.leg1Source = muonHistManagerBgEstZmumuEnriched.muonSource
pfMEtHistManagerBgEstZmumuJetMisIdEnriched.leg2Source = tauHistManagerBgEstZmumuJetMisIdEnriched.tauSource

tauIdEffHistManagerBgEstZmumuJetMisIdEnriched = copy.deepcopy(tauIdEffZtoMuTauHistManager)
tauIdEffHistManagerBgEstZmumuJetMisIdEnriched.pluginName = cms.string('tauIdEffHistManagerBgEstZmumuJetMisIdEnriched')
tauIdEffHistManagerBgEstZmumuJetMisIdEnriched.muonSource = cms.InputTag('selectedPatMuonsPFRelIsoCumulative')
tauIdEffHistManagerBgEstZmumuJetMisIdEnriched.tauSource = tauHistManagerBgEstZmumuJetMisIdEnriched.tauSource
tauIdEffHistManagerBgEstZmumuJetMisIdEnriched.diTauSource = diTauCandidateHistManagerBgEstZmumuJetMisIdEnriched.diTauCandidateSource
tauIdEffHistManagerBgEstZmumuJetMisIdEnriched.diTauChargeSignExtractor.src = tauIdEffHistManagerBgEstZmumuJetMisIdEnriched.diTauSource

analyzeEventsBgEstZmumuJetMisIdEnriched = cms.EDAnalyzer("GenericAnalyzer",
  
    name = cms.string('BgEstTemplateAnalyzer_ZmumuJetMisIdEnriched'), 
                            
    filters = cms.VPSet(
        evtSelGenPhaseSpace,
        evtSelTrigger,
        evtSelDataQuality,
        evtSelPrimaryEventVertex,
        evtSelPrimaryEventVertexQuality,
        evtSelPrimaryEventVertexPosition,
        evtSelGlobalMuon,
        evtSelMuonEta,
        evtSelMuonPt,
        evtSelMuonVbTfId,
        evtSelMuonPFRelIso,
        evtSelTauAntiOverlapWithMuonsVeto,
        evtSelTauEta,
        evtSelTauPt,
        evtSelTauLeadTrk,
        evtSelTauLeadTrkPt,
        cms.PSet(
            pluginName = cms.string('tauTaNCdiscrBgEstZmumuJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauTaNCdiscrBgEstZmumuJetMisIdEnriched', 'cumulative')
        ),
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
            pluginName = cms.string('muTauPairMt1METbgEstZmumuJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('muTauPairMt1METbgEstZmumuJetMisIdEnriched')
        ),
        cms.PSet(
            pluginName = cms.string('muTauPairPzetaDiffBgEstZmumuJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('muTauPairPzetaDiffBgEstZmumuJetMisIdEnriched')
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
        diTauCandidateSVfitHistManagerBgEstZmumuJetMisIdEnriched,
	caloMEtHistManagerBgEstZmumuJetMisIdEnriched,
	pfMEtHistManagerBgEstZmumuJetMisIdEnriched,
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
            title = cms.string('Muon Trigger')
        ),
        cms.PSet(
            filter = cms.string('evtSelDataQuality'),
            title = cms.string('Data quality')
        ),
        cms.PSet(
            filter = cms.string('evtSelPrimaryEventVertex'),
            title = cms.string('Vertex')
        ),
        cms.PSet(
            filter = cms.string('evtSelPrimaryEventVertexQuality'),
            title = cms.string('Vertex quality')
        ),
        cms.PSet(
            filter = cms.string('evtSelPrimaryEventVertexPosition'),
            title = cms.string('Vertex position')
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
            filter = cms.string('evtSelMuonVbTfId'),
            title = cms.string('Muon VBTF id.')
        ),
        cms.PSet(
            filter = cms.string('evtSelMuonPFRelIso'),
            title = cms.string('Muon iso.')
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
            filter = cms.string('tauTaNCdiscrBgEstZmumuJetMisIdEnriched'),
            title = cms.string('Tau TaNC discr.')
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
            filter = cms.string('muTauPairMt1METbgEstZmumuJetMisIdEnriched'),
            title = cms.string('M_{T}(Muon-MET) < 50 GeV'),
        ),
        cms.PSet(
            filter = cms.string('muTauPairPzetaDiffBgEstZmumuJetMisIdEnriched'),
            title = cms.string('P_{#zeta} - 1.5*P_{#zeta}^{vis} > -20 GeV'),
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
                'diTauCandidateHistManagerBgEstZmumuJetMisIdEnriched',
                'diTauCandidateSVfitHistManagerBgEstZmumuJetMisIdEnriched'
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
                'diTauCandidateSVfitHistManagerBgEstZmumuJetMisIdEnriched',
                'caloMEtHistManagerBgEstZmumuJetMisIdEnriched',
	        'pfMEtHistManagerBgEstZmumuJetMisIdEnriched',
                'tauIdEffHistManagerBgEstZmumuJetMisIdEnriched',
                'dataBinnerBgEstZmumuEnriched'
            )
        )
    )
)

analysisSequenceBgEstZmumuJetMisIdEnriched = cms.Sequence(analyzeEventsBgEstZmumuJetMisIdEnriched)

#--------------------------------------------------------------------------------

tauHistManagerBgEstZmumuMuonMisIdEnriched = copy.deepcopy(tauHistManager)
tauHistManagerBgEstZmumuMuonMisIdEnriched.pluginName = cms.string('tauHistManagerBgEstZmumuMuonMisIdEnriched')
tauHistManagerBgEstZmumuMuonMisIdEnriched.tauSource = cms.InputTag('tausBgEstZmumuMuonMisIdEnrichedAntiMuonVeto')

diTauCandidateHistManagerBgEstZmumuMuonMisIdEnriched = copy.deepcopy(diTauCandidateHistManagerForMuTau)
diTauCandidateHistManagerBgEstZmumuMuonMisIdEnriched.pluginName = cms.string('diTauCandidateHistManagerBgEstZmumuMuonMisIdEnriched')
diTauCandidateHistManagerBgEstZmumuMuonMisIdEnriched.diTauCandidateSource = cms.InputTag('muTauPairsBgEstZmumuMuonMisIdEnrichedZeroCharge')
diTauCandidateHistManagerBgEstZmumuMuonMisIdEnriched.visMassHypothesisSource = cms.InputTag('')

diTauCandidateSVfitHistManagerBgEstZmumuMuonMisIdEnriched = copy.deepcopy(diTauCandidateSVfitHistManagerForMuTau)
diTauCandidateSVfitHistManagerBgEstZmumuMuonMisIdEnriched.pluginName = cms.string('diTauCandidateSVfitHistManagerBgEstZmumuMuonMisIdEnriched')
diTauCandidateSVfitHistManagerBgEstZmumuMuonMisIdEnriched.diTauCandidateSource = diTauCandidateHistManagerBgEstZmumuMuonMisIdEnriched.diTauCandidateSource

caloMEtHistManagerBgEstZmumuMuonMisIdEnriched = copy.deepcopy(caloMEtHistManager)
caloMEtHistManagerBgEstZmumuMuonMisIdEnriched.pluginName = cms.string('caloMEtHistManagerBgEstZmumuMuonMisIdEnriched')
caloMEtHistManagerBgEstZmumuMuonMisIdEnriched.leg1Source = muonHistManagerBgEstZmumuEnriched.muonSource
caloMEtHistManagerBgEstZmumuMuonMisIdEnriched.leg2Source = tauHistManagerBgEstZmumuMuonMisIdEnriched.tauSource
pfMEtHistManagerBgEstZmumuMuonMisIdEnriched = copy.deepcopy(pfMEtHistManager)
pfMEtHistManagerBgEstZmumuMuonMisIdEnriched.pluginName = cms.string('pfMEtHistManagerBgEstZmumuMuonMisIdEnriched')
pfMEtHistManagerBgEstZmumuMuonMisIdEnriched.leg1Source = muonHistManagerBgEstZmumuEnriched.muonSource
pfMEtHistManagerBgEstZmumuMuonMisIdEnriched.leg2Source = tauHistManagerBgEstZmumuMuonMisIdEnriched.tauSource

tauIdEffHistManagerBgEstZmumuMuonMisIdEnriched = copy.deepcopy(tauIdEffZtoMuTauHistManager)
tauIdEffHistManagerBgEstZmumuMuonMisIdEnriched.pluginName = cms.string('tauIdEffHistManagerBgEstZmumuMuonMisIdEnriched')
tauIdEffHistManagerBgEstZmumuMuonMisIdEnriched.muonSource = cms.InputTag('selectedPatMuonsPFRelIsoCumulative')
tauIdEffHistManagerBgEstZmumuMuonMisIdEnriched.tauSource = tauHistManagerBgEstZmumuMuonMisIdEnriched.tauSource
tauIdEffHistManagerBgEstZmumuMuonMisIdEnriched.diTauSource = diTauCandidateHistManagerBgEstZmumuMuonMisIdEnriched.diTauCandidateSource
tauIdEffHistManagerBgEstZmumuMuonMisIdEnriched.diTauChargeSignExtractor.src = tauIdEffHistManagerBgEstZmumuMuonMisIdEnriched.diTauSource

analyzeEventsBgEstZmumuMuonMisIdEnriched = cms.EDAnalyzer("GenericAnalyzer",
  
    name = cms.string('BgEstTemplateAnalyzer_ZmumuMuonMisIdEnriched'), 
                            
    filters = cms.VPSet(
        evtSelGenPhaseSpace,
        evtSelTrigger,
        evtSelDataQuality,
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
        evtSelMuonVbTfId,
        evtSelMuonPFRelIso,
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
            pluginName = cms.string('tauTaNCdiscrBgEstZmumuMuonMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauTaNCdiscrBgEstZmumuMuonMisIdEnriched', 'cumulative')
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
        diTauCandidateSVfitHistManagerBgEstZmumuMuonMisIdEnriched,
        caloMEtHistManagerBgEstZmumuMuonMisIdEnriched,
	pfMEtHistManagerBgEstZmumuMuonMisIdEnriched,
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
            title = cms.string('Muon Trigger')
        ),
        cms.PSet(
            filter = cms.string('evtSelDataQuality'),
            title = cms.string('Data quality')
        ),
        cms.PSet(
            filter = cms.string('evtSelPrimaryEventVertex'),
            title = cms.string('Vertex')
        ),
        cms.PSet(
            filter = cms.string('evtSelPrimaryEventVertexQuality'),
            title = cms.string('Vertex quality')
        ),
        cms.PSet(
            filter = cms.string('evtSelPrimaryEventVertexPosition'),
            title = cms.string('Vertex position')
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
            filter = cms.string('evtSelMuonVbTfId'),
            title = cms.string('Muon VBTF id.'),
        ),
        cms.PSet(
            filter = cms.string('evtSelMuonPFRelIso'),
            title = cms.string('Muon iso.')
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
            filter = cms.string('tauTaNCdiscrBgEstZmumuMuonMisIdEnriched'),
            title = cms.string('Tau TaNC discr.')
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
                'diTauCandidateSVfitHistManagerBgEstZmumuMuonMisIdEnriched',
                'caloMEtHistManagerBgEstZmumuMuonMisIdEnriched',
	        'pfMEtHistManagerBgEstZmumuMuonMisIdEnriched',
                'tauIdEffHistManagerBgEstZmumuMuonMisIdEnriched',
                'dataBinnerBgEstZmumuEnriched'
            )
        )
    )
)

analysisSequenceBgEstZmumuMuonMisIdEnriched = cms.Sequence(analyzeEventsBgEstZmumuMuonMisIdEnriched)

#--------------------------------------------------------------------------------  
# define (final) analysis sequence
#--------------------------------------------------------------------------------

bgEstZmumuEnrichedAnalysisSequence = cms.Sequence(
    selectMuonsBgEstZmumuEnriched
   + selectTausBgEstZmumuEnriched
   + selectMuTauPairsBgEstZmumuEnriched
   + selectDiMuonPairsBgEstZmumuEnriched
   + selectEventsBgEstZmumuEnriched   
   + analysisSequenceBgEstZmumuJetMisIdEnriched + analysisSequenceBgEstZmumuMuonMisIdEnriched
)
