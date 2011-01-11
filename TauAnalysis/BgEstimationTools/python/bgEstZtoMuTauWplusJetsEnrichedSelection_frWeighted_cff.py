import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.BgEstimationTools.bgEstZtoMuTauWplusJetsEnrichedSelection_cff import *
from TauAnalysis.Configuration.tools.factorizationTools import replaceEventSelections, replaceAnalyzerModules

#--------------------------------------------------------------------------------
# select W + jets background enriched event sample with no tau id. cuts applied,
# but events weighted by fake-rates instead
#--------------------------------------------------------------------------------

tausBgEstWplusJetsEnrichedFRweightedTaNCdiscrNotApplied = copy.deepcopy(selectedPatTausTaNCdiscr)
tausBgEstWplusJetsEnrichedFRweightedTaNCdiscrNotApplied.cut = cms.string('tauID("byTaNCvloose") > -1000. & tauID("byTaNCmedium") < +1000.')

tausBgEstWplusJetsEnrichedFRweightedMuonVeto = copy.deepcopy(selectedPatTausMuonVeto)

tauSelConfiguratorBgEstWplusJetsEnrichedFRweighted = objSelConfigurator(
    [ tausBgEstWplusJetsEnrichedFRweightedTaNCdiscrNotApplied,
      tausBgEstWplusJetsEnrichedFRweightedMuonVeto ],
    src = "selectedPatTausForMuTauLeadTrkPtCumulative",
    pyModuleName = __name__,
    doSelIndividual = False
)

selectTausBgEstWplusJetsEnrichedFRweighted = tauSelConfiguratorBgEstWplusJetsEnrichedFRweighted.configure(pyNameSpace = locals())

#--------------------------------------------------------------------------------  
# produce collection of muon + tau-jet combinations
#--------------------------------------------------------------------------------

from TauAnalysis.CandidateTools.muTauPairProduction_cff import *

muTauPairsBgEstWplusJetsEnrichedFRweighted = allMuTauPairs.clone(
    srcLeg1 = cms.InputTag('muonsBgEstWplusJetsEnrichedPFRelIsoCumulative'),
    srcLeg2 = cms.InputTag('tausBgEstWplusJetsEnrichedFRweightedMuonVetoCumulative'),
    verbosity = cms.untracked.int32(0)
)

produceMuTauPairsBgEstWplusJetsEnrichedFRweighted = cms.Sequence(muTauPairsBgEstWplusJetsEnrichedFRweighted)

muTauPairsBgEstWplusJetsEnrichedFRweightedMt1MET = cms.EDFilter("PATMuTauPairSelector",
    src = cms.InputTag('muTauPairsBgEstWplusJetsEnrichedFRweighted'),                                                   
    cut = cms.string('mt1MET > 30.'),
    filter = cms.bool(False)
)

selectMuTauPairsBgEstWplusJetsEnrichedFRweighted = cms.Sequence(muTauPairsBgEstWplusJetsEnrichedFRweightedMt1MET)

#--------------------------------------------------------------------------------  
# produce collection of pat::Jets used for central jet veto
# (in order to reject QCD di-jet events)
#--------------------------------------------------------------------------------

jetsBgEstWplusJetsEnrichedFRweightedAntiOverlapWithLeptonsVeto = cms.EDFilter("PATJetAntiOverlapSelector",
    src = cms.InputTag("selectedPatJetsEt20Cumulative"),                                                                  
    srcNotToBeFiltered = cms.VInputTag(
        "selectedPatElectronsTrkIPcumulative",
        "muonsBgEstWplusJetsEnrichedPFRelIsoCumulative",
        "tausBgEstWplusJetsEnrichedFRweightedMuonVetoCumulative"
    ),
    dRmin = cms.double(0.7),
    filter = cms.bool(False)                                           
)

jetsBgEstWplusJetsEnrichedFRweightedAlpha0point1 = cms.EDFilter("PATJetAlphaSelector",
    src = cms.InputTag("jetsBgEstWplusJetsEnrichedFRweightedAntiOverlapWithLeptonsVeto"),
    alphaMin = cms.double(0.1),
    filter = cms.bool(False)
)

selectJetsBgEstWplusJetsEnrichedFRweighted = cms.Sequence(
    jetsBgEstWplusJetsEnrichedFRweightedAntiOverlapWithLeptonsVeto + jetsBgEstWplusJetsEnrichedFRweightedAlpha0point1
)

#--------------------------------------------------------------------------------  
# produce boolean event selection flags
#--------------------------------------------------------------------------------

cfgTauTaNCdiscrCutNotAppliedBgEstWplusJetsEnrichedFRweighted = copy.deepcopy(cfgTauTaNCdiscrCut)
cfgTauTaNCdiscrCutNotAppliedBgEstWplusJetsEnrichedFRweighted.pluginName = cms.string('tauTaNCdiscrCutNotAppliedBgEstWplusJetsEnrichedFRweighted')
cfgTauTaNCdiscrCutNotAppliedBgEstWplusJetsEnrichedFRweighted.src_cumulative = cms.InputTag('tausBgEstWplusJetsEnrichedFRweightedTaNCdiscrNotAppliedCumulative')
cfgTauTaNCdiscrCutNotAppliedBgEstWplusJetsEnrichedFRweighted.systematics = cms.vstring()

cfgTauMuonVetoBgEstWplusJetsEnrichedFRweighted = copy.deepcopy(cfgTauMuonVeto)
cfgTauMuonVetoBgEstWplusJetsEnrichedFRweighted.pluginName = cms.string('tauMuonVetoBgEstWplusJetsEnrichedFRweighted')
cfgTauMuonVetoBgEstWplusJetsEnrichedFRweighted.src_cumulative = cms.InputTag('tausBgEstWplusJetsEnrichedFRweightedMuonVetoCumulative')
cfgTauMuonVetoBgEstWplusJetsEnrichedFRweighted.systematics = cms.vstring()

cfgMuTauPairBgEstWplusJetsEnrichedFRweighted = cms.PSet(
    pluginName = cms.string('muTauPairBgEstWplusJetsEnrichedFRweighted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('muTauPairsBgEstWplusJetsEnrichedFRweighted'),
    minNumber = cms.uint32(1)
)

cfgMuTauPairMt1MEtBgEstWplusJetsEnrichedFRweighted = cms.PSet(
    pluginName = cms.string('muTauPairMt1MEtBgEstWplusJetsEnrichedFRweighted'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('muTauPairsBgEstWplusJetsEnrichedFRweightedMt1MET'),
    minNumber = cms.uint32(1)
)

cfgCentralJetVetoBgEstWplusJetsEnrichedFRweighted = cms.PSet(
    pluginName = cms.string('centralJetVetoBgEstWplusJetsEnrichedFRweighted'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src = cms.InputTag('jetsBgEstWplusJetsEnrichedFRweightedAlpha0point1'),
    maxNumber = cms.uint32(0)
)

evtSelConfiguratorBgEstWplusJetsEnrichedFRweighted = eventSelFlagProdConfigurator(
    [ cfgTauTaNCdiscrCutNotAppliedBgEstWplusJetsEnrichedFRweighted,
      cfgTauMuonVetoBgEstWplusJetsEnrichedFRweighted,
      cfgMuTauPairBgEstWplusJetsEnrichedFRweighted,
      cfgMuTauPairMt1MEtBgEstWplusJetsEnrichedFRweighted,
      cfgCentralJetVetoBgEstWplusJetsEnrichedFRweighted ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

selectEventsBgEstWplusJetsEnrichedFRweighted = evtSelConfiguratorBgEstWplusJetsEnrichedFRweighted.configure()

#--------------------------------------------------------------------------------  
# apply event selection criteria; fill histograms
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.analyzeZtoMuTau_cfi import *

tauHistManagerBgEstWplusJetsEnrichedFRweighted = copy.deepcopy(tauHistManagerBgEstWplusJetsEnriched)
tauHistManagerBgEstWplusJetsEnrichedFRweighted.tauSource = muTauPairsBgEstWplusJetsEnrichedFRweighted.srcLeg2
tauHistManagerBgEstWplusJetsEnrichedFRmcWeighted = copy.deepcopy(tauHistManagerBgEstWplusJetsEnrichedFRweighted)
tauHistManagerBgEstWplusJetsEnrichedFRmcWeighted.pluginName = cms.string('tauHistManagerBgEstWplusJetsEnrichedFRmcWeighted')
tauHistManagerBgEstWplusJetsEnrichedFRmcWeighted.tauJetWeightSource = cms.vstring('bgEstFakeRateJetWeight_WplusJetsSim')
tauHistManagerBgEstWplusJetsEnrichedFRdataWeighted = copy.deepcopy(tauHistManagerBgEstWplusJetsEnrichedFRweighted)
tauHistManagerBgEstWplusJetsEnrichedFRdataWeighted.pluginName = cms.string('tauHistManagerBgEstWplusJetsEnrichedFRdataWeighted')
tauHistManagerBgEstWplusJetsEnrichedFRdataWeighted.tauJetWeightSource = cms.vstring('bgEstFakeRateJetWeight_WplusJetsData')

diTauCandidateHistManagerBgEstWplusJetsEnrichedFRweighted = copy.deepcopy(diTauCandidateHistManagerBgEstWplusJetsEnriched)
diTauCandidateHistManagerBgEstWplusJetsEnrichedFRweighted.diTauCandidateSource = cms.InputTag('muTauPairsBgEstWplusJetsEnrichedFRweightedMt1MET')
diTauCandidateHistManagerBgEstWplusJetsEnrichedFRweighted.visMassHypothesisSource = cms.InputTag('')
diTauCandidateHistManagerBgEstWplusJetsEnrichedFRmcWeighted = copy.deepcopy(diTauCandidateHistManagerBgEstWplusJetsEnrichedFRweighted)
diTauCandidateHistManagerBgEstWplusJetsEnrichedFRmcWeighted.pluginName = cms.string('diTauCandidateHistManagerBgEstWplusJetsEnrichedFRmcWeighted')
diTauCandidateHistManagerBgEstWplusJetsEnrichedFRmcWeighted.diTauLeg2WeightSource = cms.vstring('bgEstFakeRateJetWeight_WplusJetsSim')
diTauCandidateHistManagerBgEstWplusJetsEnrichedFRdataWeighted = copy.deepcopy(diTauCandidateHistManagerBgEstWplusJetsEnrichedFRweighted)
diTauCandidateHistManagerBgEstWplusJetsEnrichedFRdataWeighted.pluginName = cms.string('diTauCandidateHistManagerBgEstWplusJetsEnrichedFRdataWeighted')
diTauCandidateHistManagerBgEstWplusJetsEnrichedFRdataWeighted.diTauLeg2WeightSource = cms.vstring('bgEstFakeRateJetWeight_WplusJetsData')

diTauCandidateSVfitHistManagerBgEstWplusJetsEnrichedFRweighted = copy.deepcopy(diTauCandidateSVfitHistManagerBgEstWplusJetsEnriched)
diTauCandidateSVfitHistManagerBgEstWplusJetsEnrichedFRweighted.diTauCandidateSource = diTauCandidateHistManagerBgEstWplusJetsEnrichedFRweighted.diTauCandidateSource
diTauCandidateSVfitHistManagerBgEstWplusJetsEnrichedFRmcWeighted = copy.deepcopy(diTauCandidateSVfitHistManagerBgEstWplusJetsEnrichedFRweighted)
diTauCandidateSVfitHistManagerBgEstWplusJetsEnrichedFRmcWeighted.pluginName = cms.string('diTauCandidateSVfitHistManagerBgEstWplusJetsEnrichedFRmcWeighted')
diTauCandidateSVfitHistManagerBgEstWplusJetsEnrichedFRmcWeighted.diTauLeg2WeightSource = cms.vstring('bgEstFakeRateJetWeight_WplusJetsSim')
diTauCandidateSVfitHistManagerBgEstWplusJetsEnrichedFRdataWeighted = copy.deepcopy(diTauCandidateSVfitHistManagerBgEstWplusJetsEnrichedFRweighted)
diTauCandidateSVfitHistManagerBgEstWplusJetsEnrichedFRdataWeighted.pluginName = cms.string('diTauCandidateSVfitHistManagerBgEstWplusJetsEnrichedFRdataWeighted')
diTauCandidateSVfitHistManagerBgEstWplusJetsEnrichedFRdataWeighted.diTauLeg2WeightSource = cms.vstring('bgEstFakeRateJetWeight_WplusJetsData')

caloMEtHistManagerBgEstWplusJetsEnrichedFRweighted = copy.deepcopy(caloMEtHistManagerBgEstWplusJetsEnriched)
caloMEtHistManagerBgEstWplusJetsEnrichedFRweighted.pluginName = cms.string('caloMEtHistManagerBgEstWplusJetsEnrichedFRweighted')
caloMEtHistManagerBgEstWplusJetsEnrichedFRweighted.leg2Source = tauHistManagerBgEstWplusJetsEnrichedFRweighted.tauSource
pfMEtHistManagerBgEstWplusJetsEnrichedFRweighted = copy.deepcopy(pfMEtHistManagerBgEstWplusJetsEnriched)
pfMEtHistManagerBgEstWplusJetsEnrichedFRweighted.pluginName = cms.string('pfMEtHistManagerBgEstWplusJetsEnrichedFRweighted')
pfMEtHistManagerBgEstWplusJetsEnrichedFRweighted.leg2Source = tauHistManagerBgEstWplusJetsEnrichedFRweighted.tauSource

jetHistManagerBgEstWplusJetsEnrichedFRweighted = copy.deepcopy(jetHistManagerBgEstWplusJetsEnriched)
jetHistManagerBgEstWplusJetsEnrichedFRweighted.pluginName = cms.string('jetHistManagerBgEstWplusJetsEnrichedFRweighted')
jetHistManagerBgEstWplusJetsEnrichedFRweighted.jetSource = cms.InputTag('jetsBgEstWplusJetsEnrichedFRweightedAntiOverlapWithLeptonsVeto')

eventSelectionReplacementsWplusJetsEnrichedFRweighted = [
    [ cfgTauTaNCdiscrCutBgEstWplusJetsEnriched, cfgTauTaNCdiscrCutNotAppliedBgEstWplusJetsEnrichedFRweighted ],
    [ cfgTauMuonVetoBgEstWplusJetsEnriched, cfgTauMuonVetoBgEstWplusJetsEnrichedFRweighted],
    [ cfgMuTauPairBgEstWplusJetsEnriched, cfgMuTauPairBgEstWplusJetsEnrichedFRweighted],
    [ cfgMuTauPairMt1MEtBgEstWplusJetsEnriched, cfgMuTauPairMt1MEtBgEstWplusJetsEnrichedFRweighted],
    [ cfgCentralJetVetoBgEstWplusJetsEnriched, cfgCentralJetVetoBgEstWplusJetsEnrichedFRweighted]
]

analyzerModuleReplacementsWplusJetsEnrichedFRweighted = [
    [ caloMEtHistManagerBgEstWplusJetsEnriched, caloMEtHistManagerBgEstWplusJetsEnrichedFRweighted ],
    [ pfMEtHistManagerBgEstWplusJetsEnriched, pfMEtHistManagerBgEstWplusJetsEnrichedFRweighted ],
    [ jetHistManagerBgEstWplusJetsEnriched, jetHistManagerBgEstWplusJetsEnrichedFRweighted ]
]

analyzeEventsBgEstWplusJetsEnrichedFRmcWeighted = analyzeEventsBgEstWplusJetsEnriched.clone(
  name = cms.string('BgEstTemplateAnalyzer_WplusJetsEnrichedFRmcWeighted')
)
replaceEventSelections(analyzeEventsBgEstWplusJetsEnrichedFRmcWeighted, eventSelectionReplacementsWplusJetsEnrichedFRweighted)
analyzerModuleReplacementsWplusJetsEnrichedFRmcWeighted = copy.deepcopy(analyzerModuleReplacementsWplusJetsEnrichedFRweighted)
analyzerModuleReplacementsWplusJetsEnrichedFRmcWeighted.extend([
    [ tauHistManagerBgEstWplusJetsEnriched, tauHistManagerBgEstWplusJetsEnrichedFRmcWeighted ],
    [ diTauCandidateHistManagerBgEstWplusJetsEnriched, diTauCandidateHistManagerBgEstWplusJetsEnrichedFRmcWeighted ],    
    [ diTauCandidateSVfitHistManagerBgEstWplusJetsEnriched, diTauCandidateSVfitHistManagerBgEstWplusJetsEnrichedFRmcWeighted ]
])
replaceAnalyzerModules(analyzeEventsBgEstWplusJetsEnrichedFRmcWeighted, analyzerModuleReplacementsWplusJetsEnrichedFRmcWeighted)
psetFRmcEventWeightWplusJetsEnriched = cms.PSet(
    src = cms.InputTag('bgEstFakeRateEventWeights', 'WplusJetsSim'),
    applyAfterFilter = cms.string("tauTaNCdiscrCutNotAppliedBgEstWplusJetsEnrichedFRweighted")
)
if hasattr(analyzeEventsBgEstWplusJetsEnrichedFRmcWeighted, "eventWeights"):
    getattr(analyzeEventsBgEstWplusJetsEnrichedFRmcWeighted, "eventWeights").append(psetFRmcEventWeightWplusJetsEnriched)
else:
    setattr(analyzeEventsBgEstWplusJetsEnrichedFRmcWeighted, "eventWeights", cms.VPSet(psetFRmcEventWeightWplusJetsEnriched))
setattr(tauHistManagerBgEstWplusJetsEnrichedFRmcWeighted, "checkWeightConsistency", cms.bool(True))

analyzeEventsBgEstWplusJetsEnrichedFRdataWeighted = analyzeEventsBgEstWplusJetsEnriched.clone(
  name = cms.string('BgEstTemplateAnalyzer_WplusJetsEnrichedFRmdataWeighted')
)
replaceEventSelections(analyzeEventsBgEstWplusJetsEnrichedFRdataWeighted, eventSelectionReplacementsWplusJetsEnrichedFRweighted)
analyzerModuleReplacementsWplusJetsEnrichedFRdataWeighted = copy.deepcopy(analyzerModuleReplacementsWplusJetsEnrichedFRweighted)
analyzerModuleReplacementsWplusJetsEnrichedFRdataWeighted.extend([
    [ tauHistManagerBgEstWplusJetsEnriched, tauHistManagerBgEstWplusJetsEnrichedFRdataWeighted ],
    [ diTauCandidateHistManagerBgEstWplusJetsEnriched, diTauCandidateHistManagerBgEstWplusJetsEnrichedFRdataWeighted ],    
    [ diTauCandidateSVfitHistManagerBgEstWplusJetsEnriched, diTauCandidateSVfitHistManagerBgEstWplusJetsEnrichedFRdataWeighted ]
])
replaceAnalyzerModules(analyzeEventsBgEstWplusJetsEnrichedFRdataWeighted, analyzerModuleReplacementsWplusJetsEnrichedFRdataWeighted)
psetFRdataEventWeightWplusJetsEnriched = cms.PSet(
    src = cms.InputTag('bgEstFakeRateEventWeights', 'WplusJetsData'),
    applyAfterFilter = cms.string("tauTaNCdiscrCutNotAppliedBgEstWplusJetsEnrichedFRweighted")
)
if hasattr(analyzeEventsBgEstWplusJetsEnrichedFRdataWeighted, "eventWeights"):
    getattr(analyzeEventsBgEstWplusJetsEnrichedFRdataWeighted, "eventWeights").append(psetFRdataEventWeightWplusJetsEnriched)
else:
    setattr(analyzeEventsBgEstWplusJetsEnrichedFRdataWeighted, "eventWeights", cms.VPSet(psetFRdataEventWeightWplusJetsEnriched))
setattr(tauHistManagerBgEstWplusJetsEnrichedFRdataWeighted, "checkWeightConsistency", cms.bool(True))

analysisSequenceBgEstWplusJetsEnrichedFRweighted = cms.Sequence(analyzeEventsBgEstWplusJetsEnrichedFRmcWeighted * analyzeEventsBgEstWplusJetsEnrichedFRdataWeighted)

#--------------------------------------------------------------------------------  
# define (final) analysis sequence
#--------------------------------------------------------------------------------

bgEstWplusJetsEnrichedFRweightedAnalysisSequence = cms.Sequence(
    selectTausBgEstWplusJetsEnrichedFRweighted
   + produceMuTauPairsBgEstWplusJetsEnrichedFRweighted + selectMuTauPairsBgEstWplusJetsEnrichedFRweighted
   + selectJetsBgEstWplusJetsEnrichedFRweighted 
   + selectEventsBgEstWplusJetsEnrichedFRweighted
   + analysisSequenceBgEstWplusJetsEnrichedFRweighted
)
