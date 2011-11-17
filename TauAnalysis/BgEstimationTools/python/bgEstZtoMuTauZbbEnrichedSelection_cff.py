import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *
from TauAnalysis.RecoTools.tools.eventSelFlagProdConfigurator import *

#--------------------------------------------------------------------------------
# select b bbar Z --> mu+ mu- background enriched event sample
#--------------------------------------------------------------------------------

from TauAnalysis.Skimming.goldenZmmSelectionVBTFnoMuonIsolation_cfi import *

goodMuonsBgEstZbbEnriched = goodMuons.clone(
    filter = cms.bool(False)
)

goodIsoMuonsBgEstZbbEnriched = goodIsoMuons.clone(
    filter = cms.bool(False)
)

goodIsoMuPlusBgEstZbbEnriched = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag("goodIsoMuonsBgEstZbbEnriched"),
    cut = cms.string('charge > +0.5'),
    filter = cms.bool(False)
)

goodIsoMuMinusBgEstZbbEnriched = goodIsoMuPlusBgEstZbbEnriched.clone(
    cut = cms.string('charge < -0.5'),
)


selectMuonsBgEstZbbEnriched = cms.Sequence(
    goodMuonsBgEstZbbEnriched + goodIsoMuonsBgEstZbbEnriched
   + goodIsoMuPlusBgEstZbbEnriched + goodIsoMuMinusBgEstZbbEnriched
)

goldenZmumuCandidatesBgEstZbbEnriched = goldenZmumuCandidatesGe2IsoMuons.clone(
    decay = cms.string("goodIsoMuonsBgEstZbbEnriched@+ goodIsoMuonsBgEstZbbEnriched@-")
)

selectDiMuPairsBgEstZbbEnriched = cms.Sequence(goldenZmumuCandidatesBgEstZbbEnriched)

#--------------------------------------------------------------------------------
# produce collection of pat::Jets used for central jet veto
# (in order to reject QCD di-jet events)
#--------------------------------------------------------------------------------

jetsBgEstZbbEnrichedAntiOverlapWithLeptonsVeto = cms.EDFilter("PATJetAntiOverlapSelector",
    src = cms.InputTag("patJets"),
    srcNotToBeFiltered = cms.VInputTag(
        "selectedPatElectronsTrkCumulative",
        "goodIsoMuonsBgEstZbbEnriched"
    ),
    dRmin = cms.double(0.7),
    filter = cms.bool(False)
)

jetsBgEstZbbEnrichedEta24 = cms.EDFilter("PATJetSelector",
    src = cms.InputTag("jetsBgEstZbbEnrichedAntiOverlapWithLeptonsVeto"),
    cut = cms.string('abs(eta) < 2.4'),
    filter = cms.bool(False)
)

jetsBgEstZbbEnrichedEt20 = cms.EDFilter("PATJetSelector",
    src = cms.InputTag("jetsBgEstZbbEnrichedEta24"),
    cut = cms.string('et > 20.'),
    filter = cms.bool(False)
)

jetsBgEstZbbEnrichedEt20bTag = cms.EDFilter("PATJetSelector",
    src = cms.InputTag("jetsBgEstZbbEnrichedEt20"),
    cut = cms.string('bDiscriminator("trackCountingHighEffBJetTags") > 2.5'),
    filter = cms.bool(False)
)

selectJetsBgEstZbbEnriched = cms.Sequence(
    jetsBgEstZbbEnrichedAntiOverlapWithLeptonsVeto
   + jetsBgEstZbbEnrichedEta24 + jetsBgEstZbbEnrichedEt20
   + jetsBgEstZbbEnrichedEt20bTag
)

#--------------------------------------------------------------------------------
# produce boolean event selection flags
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.selectZtoMuTau_cff import *

cfgGoodIsoMuPlusBgEstZbbEnriched = cms.PSet(
    pluginName = cms.string('goodIsoMuPlusBgEstZbbEnriched'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('goodIsoMuPlusBgEstZbbEnriched'),
    minNumber = cms.uint32(1)
)

cfgGoodIsoMuMinusBgEstZbbEnriched = cfgGoodIsoMuPlusBgEstZbbEnriched.clone(
    pluginName = cms.string('goodIsoMuMinusBgEstZbbEnriched'),
    src = cms.InputTag('goodIsoMuMinusBgEstZbbEnriched')
)

cfgGoldenZmumuCandidatesBgEstZbbEnriched = cms.PSet(
    pluginName = cms.string('goldenZmumuCandidatesBgEstZbbEnriched'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('goldenZmumuCandidatesBgEstZbbEnriched'),
    minNumber = cms.uint32(1)
)

cfgJetsBgEstZbbEnrichedAntiOverlapWithLeptonsVeto = cms.PSet(
    pluginName = cms.string('jetsBgEstZbbEnrichedAntiOverlapWithLeptonsVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('jetsBgEstZbbEnrichedAntiOverlapWithLeptonsVeto'),
    minNumber = cms.uint32(1)
)

cfgJetsBgEstZbbEnrichedEta24 = cfgJetsBgEstZbbEnrichedAntiOverlapWithLeptonsVeto.clone(
    pluginName = cms.string('jetsBgEstZbbEnrichedEta24'),
    src = cms.InputTag('jetsBgEstZbbEnrichedEta24')
)

cfgJetsBgEstZbbEnrichedEt20 = cfgJetsBgEstZbbEnrichedAntiOverlapWithLeptonsVeto.clone(
    pluginName = cms.string('jetsBgEstZbbEnrichedEt20'),
    src = cms.InputTag('jetsBgEstZbbEnrichedEt20')
)

cfgJetsBgEstZbbEnrichedEt20bTag = cfgJetsBgEstZbbEnrichedAntiOverlapWithLeptonsVeto.clone(
    pluginName = cms.string('jetsBgEstZbbEnrichedEt20bTag'),
    src = cms.InputTag('jetsBgEstZbbEnrichedEt20bTag')
)

evtSelConfiguratorBgEstZbbEnriched = eventSelFlagProdConfigurator(
    [ cfgGoodIsoMuPlusBgEstZbbEnriched,
      cfgGoodIsoMuMinusBgEstZbbEnriched,
      cfgGoldenZmumuCandidatesBgEstZbbEnriched,
      cfgJetsBgEstZbbEnrichedAntiOverlapWithLeptonsVeto,
      cfgJetsBgEstZbbEnrichedEta24,
      cfgJetsBgEstZbbEnrichedEt20,
      cfgJetsBgEstZbbEnrichedEt20bTag ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

selectEventsBgEstZbbEnriched = evtSelConfiguratorBgEstZbbEnriched.configure()

#--------------------------------------------------------------------------------
# apply event selection criteria; fill histograms
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.analyzeZtoMuTau_cfi import *
from TauAnalysis.BgEstimationTools.selectZtoMuTauEventVertex_cff import *
from TauAnalysis.Core.candidateHistManager_cfi import *

muPlusHistManagerBgEstZbbEnriched = muonHistManager.clone(
    pluginName = cms.string('muPlusHistManagerBgEstZbbEnriched'),
    muonSource = cms.InputTag('goodIsoMuPlusBgEstZbbEnriched')
)

muMinusHistManagerBgEstZbbEnriched = muonHistManager.clone(
    pluginName = cms.string('muMinusHistManagerBgEstZbbEnriched'),
    muonSource = cms.InputTag('goodIsoMuMinusBgEstZbbEnriched')
)

diMuPairHistManager = candidateHistManager.clone(
    pluginName = cms.string('diMuPairHistManager'),
    candidateSource = cms.InputTag('goldenZmumuCandidatesBgEstZbbEnriched'),
    dqmDirectory_store = cms.string('DiMuPairQuantities'),
)

jetHistManagerBgEstZbbEnriched = cms.PSet(
    pluginName = cms.string('jetHistManagerBgEstZbbEnriched'),
    jetSource = cms.InputTag('jetsBgEstZbbEnrichedAntiOverlapWithLeptonsVeto')
)

dataBinnerBgEstZbbEnriched = dataBinner.clone(
    pluginName = cms.string('dataBinnerBgEstZbbEnriched')
)

analyzeEventsBgEstZbbEnriched = cms.EDAnalyzer("GenericAnalyzer",

    name = cms.string('BgEstTemplateAnalyzer_ZmumuJetMisIdEnriched'),

    filters = cms.VPSet(
        evtSelGenPhaseSpace,
        evtSelTrigger,
        evtSelDataQuality,
        evtSelPrimaryEventVertexForMuTau,
        evtSelPrimaryEventVertexQualityForMuTau,
        evtSelPrimaryEventVertexPositionForMuTau,
        cms.PSet(
            pluginName = cms.string('goodIsoMuPlusBgEstZbbEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('goodIsoMuPlusBgEstZbbEnriched')
        ),
        cms.PSet(
            pluginName = cms.string('goodIsoMuMinusBgEstZbbEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('goodIsoMuMinusBgEstZbbEnriched')
        ),
        cms.PSet(
            pluginName = cms.string('goldenZmumuCandidatesBgEstZbbEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('goldenZmumuCandidatesBgEstZbbEnriched')
        ),
        cms.PSet(
            pluginName = cms.string('jetsBgEstZbbEnrichedAntiOverlapWithLeptonsVeto'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('jetsBgEstZbbEnrichedAntiOverlapWithLeptonsVeto')
        ),
        cms.PSet(
            pluginName = cms.string('jetsBgEstZbbEnrichedEta24'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('jetsBgEstZbbEnrichedEta24')
        ),
        cms.PSet(
            pluginName = cms.string('jetsBgEstZbbEnrichedEt20'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('jetsBgEstZbbEnrichedEt20')
        ),
        cms.PSet(
            pluginName = cms.string('jetsBgEstZbbEnrichedEt20bTag'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('jetsBgEstZbbEnrichedEt20bTag')
        )
    ),

    analyzers = cms.VPSet(
        muPlusHistManagerBgEstZbbEnriched,
        muMinusHistManagerBgEstZbbEnriched,
        diMuPairHistManager,
        jetHistManagerBgEstZbbEnriched,
        caloMEtHistManager,
	pfMEtHistManager,
        dataBinnerBgEstZbbEnriched
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
            filter = cms.string('goodIsoMuPlusBgEstZbbEnriched'),
            title = cms.string('good iso. Muon+')
        ),
        cms.PSet(
            filter = cms.string('goodIsoMuMinusBgEstZbbEnriched'),
            title = cms.string('good iso. Muon-')
        ),
        cms.PSet(
            filter = cms.string('goldenZmumuCandidatesBgEstZbbEnriched'),
            title = cms.string('60 < M (Muon-Pair) < 120 GeV')
        ),
        cms.PSet(
            analyzers = cms.vstring(
                'muPlusHistManagerBgEstZbbEnriched',
                'muMinusHistManagerBgEstZbbEnriched',
                'diMuPairHistManager',
                'jetHistManagerBgEstZbbEnriched'
            )
        ),
        cms.PSet(
            filter = cms.string('jetsBgEstZbbEnrichedEt20'),
            title = cms.string('one E_{T} > 20 GeV Jet')
        ),
        cms.PSet(
            filter = cms.string('jetsBgEstZbbEnrichedEt20bTag'),
            title = cms.string('one E_{T} > 20 GeV Jet with b-Tag')
        ),
        cms.PSet(
            analyzers = cms.vstring(
                'muPlusHistManagerBgEstZbbEnriched',
                'muMinusHistManagerBgEstZbbEnriched',
                'diMuPairHistManager',
                'jetHistManagerBgEstZbbEnriched',
                'caloMEtHistManager',
	        'pfMEtHistManager',
                'dataBinnerBgEstZbbEnriched'
            ),
            replace = cms.vstring(
                'jetHistManagerBgEstZbbEnriched.jetSource = jetsBgEstZbbEnrichedEt20bTag'
            )
        )
    )
)

analysisSequenceBgEstZbbEnriched = cms.Sequence(analyzeEventsBgEstZbbEnriched)

#--------------------------------------------------------------------------------
# define (final) analysis sequence
#--------------------------------------------------------------------------------

bgEstZbbEnrichedAnalysisSequence = cms.Sequence(
    selectMuonsBgEstZbbEnriched
   + selectDiMuPairsBgEstZbbEnriched
   + selectJetsBgEstZbbEnriched
   + selectEventsBgEstZbbEnriched
   + analysisSequenceBgEstZbbEnriched
)
