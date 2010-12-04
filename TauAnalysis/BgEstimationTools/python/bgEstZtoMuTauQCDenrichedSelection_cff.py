import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *
from TauAnalysis.RecoTools.tools.eventSelFlagProdConfigurator import *

#--------------------------------------------------------------------------------
# select QCD background enriched event sample
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------  
# produce collection of pat::Muons
#--------------------------------------------------------------------------------

from TauAnalysis.RecoTools.patMuonSelection_cfi import *

muonsBgEstQCDenrichedPFRelIso = copy.deepcopy(selectedPatMuonsPFRelIso)
muonsBgEstQCDenrichedPFRelIso.sumPtMin = cms.double(0.10)
muonsBgEstQCDenrichedPFRelIso.sumPtMax = cms.double(0.30)

muonSelConfiguratorBgEstQCDenriched = objSelConfigurator(
    [ muonsBgEstQCDenrichedPFRelIso ],
    src = "selectedPatMuonsPt15Cumulative",
    pyModuleName = __name__,
    doSelIndividual = False
)

selectMuonsBgEstQCDenriched = muonSelConfiguratorBgEstQCDenriched.configure(pyNameSpace = locals())

#--------------------------------------------------------------------------------  
# produce collection of pat::Taus
#--------------------------------------------------------------------------------

from TauAnalysis.RecoTools.patPFTauSelection_cfi import *
from TauAnalysis.RecoTools.patPFTauSelectionForMuTau_cfi import *
#
# Note: probability for quark/gluon jets to pass tau track and ECAL isolation criteria
#       is higher for low Pt than for high Pt jets; the consequence is that muon + tau-jet visible invariant mass distribution
#       gets shifted towards higher values in case tau track and ECAL isolation criteria are not applied.
#       For this reason, either need to apply tau track and ECAL isolation criteria in selection of QCD background enriched sample
#       or correct for template shape distortion by reweighting
#      (--> see TauAnalysis/BgEstimationTools/python/bgEstZtoMuTauWplusJetsEnrichedSelection_cff.py also)
#
tausBgEstQCDenrichedTaNCdiscr = copy.deepcopy(selectedPatTausTaNCdiscr)
tausBgEstQCDenrichedTaNCdiscr.cut = cms.string('tauID("byTaNCvloose") > 0.5 & tauID("byTaNCloose") < 0.5')

tausBgEstQCDenrichedMuonVeto = copy.deepcopy(selectedPatTausMuonVeto)

tauSelConfiguratorBgEstQCDenriched = objSelConfigurator(
    [ tausBgEstQCDenrichedTaNCdiscr,
      tausBgEstQCDenrichedMuonVeto ],
    src = "selectedPatTausForMuTauLeadTrkPtCumulative",
    pyModuleName = __name__,
    doSelIndividual = False
)

selectTausBgEstQCDenriched = tauSelConfiguratorBgEstQCDenriched.configure(pyNameSpace = locals())

#--------------------------------------------------------------------------------  
# produce collection of muon + tau-jet combinations
#--------------------------------------------------------------------------------

from TauAnalysis.CandidateTools.muTauPairProduction_cff import *
from TauAnalysis.CandidateTools.muTauPairSelection_cfi import *

muTauPairsBgEstQCDenriched = allMuTauPairs.clone(
    srcLeg1 = cms.InputTag('muonsBgEstQCDenrichedPFRelIsoCumulative'),
    srcLeg2 = cms.InputTag('tausBgEstQCDenrichedMuonVetoCumulative'),
    verbosity = cms.untracked.int32(0)
)

muTauPairsBgEstQCDenrichedMt1MET = copy.deepcopy(selectedMuTauPairsMt1MET)
muTauPairsBgEstQCDenrichedMt1MET.cut = cms.string('mt1MET < 40.')

muTauPairsBgEstQCDenrichedPzetaDiff = copy.deepcopy(selectedMuTauPairsPzetaDiff)
muTauPairsBgEstQCDenrichedPzetaDiff.cut = cms.string('(pZeta - 1.5*pZetaVis) > -20.')

muTauPairSelConfiguratorBgEstQCDenriched = objSelConfigurator(
    [ muTauPairsBgEstQCDenrichedMt1MET,
      muTauPairsBgEstQCDenrichedPzetaDiff ],
    src = "muTauPairsBgEstQCDenriched",
    pyModuleName = __name__,
    doSelIndividual = False
)

selectMuTauPairsBgEstQCDenriched = muTauPairSelConfiguratorBgEstQCDenriched.configure(pyNameSpace = locals())

#--------------------------------------------------------------------------------  
# produce boolean event selection flags
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.selectZtoMuTau_cff import *

cfgMuonPFRelIsoCutBgEstQCDenriched = copy.deepcopy(cfgMuonPFRelIsoCut)
cfgMuonPFRelIsoCutBgEstQCDenriched.pluginName = cms.string('muonPFRelIsoCutBgEstQCDenriched')
cfgMuonPFRelIsoCutBgEstQCDenriched.src_cumulative = cms.InputTag('muonsBgEstQCDenrichedPFRelIsoCumulative')
cfgMuonPFRelIsoCutBgEstQCDenriched.systematics = cms.vstring()

cfgTauTaNCdiscrCutBgEstQCDenriched = copy.deepcopy(cfgTauTaNCdiscrCut)
cfgTauTaNCdiscrCutBgEstQCDenriched.pluginName = cms.string('tauTaNCdiscrCutBgEstQCDenriched')
cfgTauTaNCdiscrCutBgEstQCDenriched.src_cumulative = cms.InputTag('tausBgEstQCDenrichedTaNCdiscrCumulative')
cfgTauTaNCdiscrCutBgEstQCDenriched.systematics = cms.vstring()

cfgTauMuonVetoBgEstQCDenriched = copy.deepcopy(cfgTauMuonVeto)
cfgTauMuonVetoBgEstQCDenriched.pluginName = cms.string('tauMuonVetoBgEstQCDenriched')
cfgTauMuonVetoBgEstQCDenriched.src_cumulative = cms.InputTag('tausBgEstQCDenrichedMuonVetoCumulative')
cfgTauMuonVetoBgEstQCDenriched.systematics = cms.vstring()

cfgMuTauPairBgEstQCDenriched = cms.PSet(
    pluginName = cms.string('muTauPairBgEstQCDenriched'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('muTauPairsBgEstQCDenriched'),
    minNumber = cms.uint32(1)
)

cfgMuTauPairMt1METbgEstQCDenriched = cms.PSet(
    pluginName = cms.string('muTauPairMt1METbgEstQCDenriched'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('muTauPairsBgEstQCDenrichedMt1METcumulative'),
    minNumber = cms.uint32(1)
)

cfgMuTauPairPzetaDiffBgEstQCDenriched = cms.PSet(
    pluginName = cms.string('muTauPairPzetaDiffBgEstQCDenriched'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('muTauPairsBgEstQCDenrichedPzetaDiffCumulative'),
    minNumber = cms.uint32(1)
)

cfgDiMuonVetoBgEstQCDenriched = cms.PSet(
    pluginName = cms.string('diMuonVetoBgEstQCDenriched'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src = cms.InputTag('selectedPatMuonsGlobalIndividual'),
    maxNumber = cms.uint32(1)
)

evtSelConfiguratorBgEstQCDenriched = eventSelFlagProdConfigurator(
    [ cfgMuonPFRelIsoCutBgEstQCDenriched,
      cfgTauTaNCdiscrCutBgEstQCDenriched,
      cfgTauMuonVetoBgEstQCDenriched,
      cfgMuTauPairBgEstQCDenriched,
      cfgMuTauPairMt1METbgEstQCDenriched,
      cfgMuTauPairPzetaDiffBgEstQCDenriched,
      cfgDiMuonVetoBgEstQCDenriched ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

selectEventsBgEstQCDenriched = evtSelConfiguratorBgEstQCDenriched.configure()

#--------------------------------------------------------------------------------  
# apply event selection criteria; fill histograms
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.analyzeZtoMuTau_cfi import *

muonHistManagerBgEstQCDenriched = copy.deepcopy(muonHistManager)
muonHistManagerBgEstQCDenriched.pluginName = cms.string('muonHistManagerBgEstQCDenriched')
muonHistManagerBgEstQCDenriched.muonSource = cms.InputTag('muonsBgEstQCDenrichedPFRelIsoCumulative')

tauHistManagerBgEstQCDenriched = copy.deepcopy(tauHistManager)
tauHistManagerBgEstQCDenriched.pluginName = cms.string('tauHistManagerBgEstQCDenriched')
tauHistManagerBgEstQCDenriched.tauSource = cms.InputTag('tausBgEstQCDenrichedMuonVetoCumulative')

diTauCandidateHistManagerBgEstQCDenriched = copy.deepcopy(diTauCandidateHistManagerForMuTau)
diTauCandidateHistManagerBgEstQCDenriched.pluginName = cms.string('diTauCandidateHistManagerBgEstQCDenriched')
diTauCandidateHistManagerBgEstQCDenriched.diTauCandidateSource = cms.InputTag('muTauPairsBgEstQCDenrichedPzetaDiffCumulative')
diTauCandidateHistManagerBgEstQCDenriched.visMassHypothesisSource = cms.InputTag('')

diTauCandidateSVfitHistManagerBgEstQCDenriched = copy.deepcopy(diTauCandidateSVfitHistManagerForMuTau)
diTauCandidateSVfitHistManagerBgEstQCDenriched.pluginName = cms.string('diTauCandidateSVfitHistManagerBgEstQCDenriched')
diTauCandidateSVfitHistManagerBgEstQCDenriched.diTauCandidateSource = diTauCandidateHistManagerBgEstQCDenriched.diTauCandidateSource

from TauAnalysis.BgEstimationTools.tauIdEffZtoMuTauHistManager_cfi import *
tauIdEffHistManagerBgEstQCDenriched = copy.deepcopy(tauIdEffZtoMuTauHistManager)
tauIdEffHistManagerBgEstQCDenriched.pluginName = cms.string('tauIdEffHistManagerBgEstQCDenriched')
tauIdEffHistManagerBgEstQCDenriched.muonSource = muonHistManagerBgEstQCDenriched.muonSource
tauIdEffHistManagerBgEstQCDenriched.tauSource = tauHistManagerBgEstQCDenriched.tauSource
tauIdEffHistManagerBgEstQCDenriched.diTauSource = diTauCandidateHistManagerBgEstQCDenriched.diTauCandidateSource
tauIdEffHistManagerBgEstQCDenriched.diTauChargeSignExtractor.src = tauIdEffHistManagerBgEstQCDenriched.diTauSource

dataBinnerBgEstQCDenriched = copy.deepcopy(dataBinner)
dataBinnerBgEstQCDenriched.pluginName = cms.string('dataBinnerBgEstQCDenriched')

analyzeEventsBgEstQCDenriched = cms.EDAnalyzer("GenericAnalyzer",
  
    name = cms.string('BgEstTemplateAnalyzer_QCDenriched'), 
                            
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
        cms.PSet(
            pluginName = cms.string('muonPFRelIsoCutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('muonPFRelIsoCutBgEstQCDenriched', 'cumulative')
        ),
        evtSelTauAntiOverlapWithMuonsVeto,
        evtSelTauEta,
        evtSelTauPt,
        evtSelTauLeadTrk,
        evtSelTauLeadTrkPt,
        cms.PSet(
            pluginName = cms.string('tauTaNCdiscrCutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauTaNCdiscrCutBgEstQCDenriched', 'cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauMuonVetoBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauMuonVetoBgEstQCDenriched', 'cumulative')
        ),      
        cms.PSet(
            pluginName = cms.string('muTauPairBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('muTauPairBgEstQCDenriched')
        ),
        cms.PSet(
            pluginName = cms.string('muTauPairMt1METbgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('muTauPairMt1METbgEstQCDenriched')
        ),
        cms.PSet(
            pluginName = cms.string('muTauPairPzetaDiffBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('muTauPairPzetaDiffBgEstQCDenriched')
        ),
        cms.PSet(
            pluginName = cms.string('diMuonVetoBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diMuonVetoBgEstQCDenriched')
        )
    ),
  
    analyzers = cms.VPSet(
        muonHistManagerBgEstQCDenriched,
        tauHistManagerBgEstQCDenriched,
        diTauCandidateHistManagerBgEstQCDenriched,
        diTauCandidateSVfitHistManagerBgEstQCDenriched,
        tauIdEffHistManagerBgEstQCDenriched,
        dataBinnerBgEstQCDenriched
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
            title = cms.string('Tau not overlapping w. Muon'),
        ),
        cms.PSet(
            filter = cms.string('evtSelTauEta'),
            title = cms.string('-2.1 < eta(Tau) < +2.1'),
        ),
        cms.PSet(
            filter = cms.string('evtSelTauPt'),
            title = cms.string('Pt(Tau) > 20 GeV'),
        ),
        cms.PSet(
            filter = cms.string('muonPFRelIsoCutBgEstQCDenriched'),
            title = cms.string('Muon loose iso.')
        ),
        cms.PSet(
            filter = cms.string('evtSelTauLeadTrk'),
            title = cms.string('Tau lead. Track find.'),
        ),
        cms.PSet(
            filter = cms.string('evtSelTauLeadTrkPt'),
            title = cms.string('Tau lead. Track Pt'),
        ),
        cms.PSet(
            filter = cms.string('tauTaNCdiscrCutBgEstQCDenriched'),
            title = cms.string('Tau TaNC discr.')
        ),
        cms.PSet(
            filter = cms.string('tauTrkIsoCutBgEstQCDenriched'),
            title = cms.string('Tau Track iso.')
        ),
        cms.PSet(
            filter = cms.string('tauEcalIsoCutBgEstQCDenriched'),
            title = cms.string('Tau ECAL iso.')
        ),
        cms.PSet(
            filter = cms.string('tauMuonVetoBgEstQCDenriched'),
            title = cms.string('Tau mu-Veto')
        ),
        cms.PSet(
            filter = cms.string('muTauPairBgEstQCDenriched'),
            title = cms.string('dR(Muon-Tau) > 0.7')
        ),
        cms.PSet(
            filter = cms.string('muTauPairMt1METbgEstQCDenriched'),
            title = cms.string('M_{T}(Muon-MET) < 40 GeV'),
        ),
        cms.PSet(
            filter = cms.string('muTauPairPzetaDiffBgEstQCDenriched'),
            title = cms.string('P_{#zeta} - 1.5*P_{#zeta}^{vis} > -20 GeV'),
        ),
        cms.PSet(
            filter = cms.string('diMuonVetoBgEstQCDenriched'),
            title = cms.string('di-Muon Veto')
        ),
        cms.PSet(
            analyzers = cms.vstring(
                'muonHistManagerBgEstQCDenriched',
                'tauHistManagerBgEstQCDenriched',
                'diTauCandidateHistManagerBgEstQCDenriched',
                'diTauCandidateSVfitHistManagerBgEstQCDenriched',
                'tauIdEffHistManagerBgEstQCDenriched',
                'dataBinnerBgEstQCDenriched'
            )
        )
    )
)

analysisSequenceBgEstQCDenriched = cms.Sequence(analyzeEventsBgEstQCDenriched)

#--------------------------------------------------------------------------------  
# define (final) analysis sequence
#--------------------------------------------------------------------------------

bgEstQCDenrichedAnalysisSequence = cms.Sequence(
    selectMuonsBgEstQCDenriched
   + selectTausBgEstQCDenriched
   + muTauPairsBgEstQCDenriched + selectMuTauPairsBgEstQCDenriched
   + selectEventsBgEstQCDenriched
   + analysisSequenceBgEstQCDenriched
)
