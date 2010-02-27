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

muonsBgEstQCDenrichedTrkIso = copy.deepcopy(selectedLayer1MuonsTrkIso)
muonsBgEstQCDenrichedTrkIso.sumPtMin = cms.double(4.)
muonsBgEstQCDenrichedTrkIso.sumPtMax = cms.double(8.)

muonsBgEstQCDenrichedEcalIso = copy.deepcopy(selectedLayer1MuonsEcalIso)
muonsBgEstQCDenrichedEcalIso.cut = cms.string('userIsolation("pat::EcalIso") > 4. & userIsolation("pat::EcalIso") < 8.')

muonsBgEstQCDenrichedPionVeto = copy.deepcopy(selectedLayer1MuonsPionVeto)

muonSelConfiguratorBgEstQCDenriched = objSelConfigurator(
    [ muonsBgEstQCDenrichedTrkIso,
      muonsBgEstQCDenrichedEcalIso,
      muonsBgEstQCDenrichedPionVeto ],
    src = "selectedLayer1MuonsPt15Cumulative",
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
tausBgEstQCDenrichedTrkIso = copy.deepcopy(selectedLayer1TausTrkIso)
#tausBgEstQCDenrichedTrkIso.cut = cms.string('tauID("trackIsolation") > 0.5 | chargedHadronIso < 8.')
tausBgEstQCDenrichedTrkIso.cut = cms.string('tauID("trackIsolation") > 0.5')

tausBgEstQCDenrichedEcalIso = copy.deepcopy(selectedLayer1TausEcalIso)
#tausBgEstQCDenrichedEcalIso.cut = cms.string('tauID("ecalIsolation") > 0.5 | photonIso < 8.')
tausBgEstQCDenrichedEcalIso.cut = cms.string('tauID("ecalIsolation") > 0.5')

tausBgEstQCDenrichedMuonVeto = copy.deepcopy(selectedLayer1TausMuonVeto)

tauSelConfiguratorBgEstQCDenriched = objSelConfigurator(
    [ tausBgEstQCDenrichedTrkIso,
      tausBgEstQCDenrichedEcalIso,
      tausBgEstQCDenrichedMuonVeto ],
    src = "selectedLayer1TausForMuTauLeadTrkPtCumulative",
    pyModuleName = __name__,
    doSelIndividual = False
)

selectTausBgEstQCDenriched = tauSelConfiguratorBgEstQCDenriched.configure(pyNameSpace = locals())

#--------------------------------------------------------------------------------  
# produce collection of muon + tau-jet combinations
#--------------------------------------------------------------------------------

muTauPairsBgEstQCDenriched = cms.EDProducer("PATMuTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('muonsBgEstQCDenrichedPionVetoCumulative'),
    srcLeg2 = cms.InputTag('tausBgEstQCDenrichedMuonVetoCumulative'),
    dRmin12 = cms.double(0.7),
    srcMET = cms.InputTag('layer1METs'),
    recoMode = cms.string(""),
    verbosity = cms.untracked.int32(0)
)

selectMuTauPairsBgEstQCDenriched = cms.Sequence(muTauPairsBgEstQCDenriched)

#--------------------------------------------------------------------------------  
# produce boolean event selection flags
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.selectZtoMuTau_cff import *

cfgMuonTrkIsoCutBgEstQCDenriched = copy.deepcopy(cfgMuonTrkIsoCut)
cfgMuonTrkIsoCutBgEstQCDenriched.pluginName = cms.string('muonTrkIsoCutBgEstQCDenriched')
cfgMuonTrkIsoCutBgEstQCDenriched.src_cumulative = cms.InputTag('muonsBgEstQCDenrichedTrkIsoCumulative')
cfgMuonTrkIsoCutBgEstQCDenriched.systematics = cms.vstring()

cfgMuonEcalIsoCutBgEstQCDenriched = copy.deepcopy(cfgMuonEcalIsoCut)
cfgMuonEcalIsoCutBgEstQCDenriched.pluginName = cms.string('muonEcalIsoCutBgEstQCDenriched')
cfgMuonEcalIsoCutBgEstQCDenriched.src_cumulative = cms.InputTag('muonsBgEstQCDenrichedEcalIsoCumulative')
cfgMuonEcalIsoCutBgEstQCDenriched.systematics = cms.vstring()

cfgTauTrkIsoCutBgEstQCDenriched = copy.deepcopy(cfgTauTrkIsoCut)
cfgTauTrkIsoCutBgEstQCDenriched.pluginName = cms.string('tauTrkIsoCutBgEstQCDenriched')
cfgTauTrkIsoCutBgEstQCDenriched.src_cumulative = cms.InputTag('tausBgEstQCDenrichedTrkIsoCumulative')
cfgTauTrkIsoCutBgEstQCDenriched.systematics = cms.vstring()

cfgTauEcalIsoCutBgEstQCDenriched = copy.deepcopy(cfgTauEcalIsoCut)
cfgTauEcalIsoCutBgEstQCDenriched.pluginName = cms.string('tauEcalIsoCutBgEstQCDenriched')
cfgTauEcalIsoCutBgEstQCDenriched.src_cumulative = cms.InputTag('tausBgEstQCDenrichedEcalIsoCumulative')
cfgTauEcalIsoCutBgEstQCDenriched.systematics = cms.vstring()

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

cfgDiMuonVetoBgEstQCDenriched = cms.PSet(
    pluginName = cms.string('diMuonVetoBgEstQCDenriched'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src = cms.InputTag('selectedLayer1MuonsGlobalIndividual'),
    maxNumber = cms.uint32(1)
)

evtSelConfiguratorBgEstQCDenriched = eventSelFlagProdConfigurator(
    [ cfgMuonTrkIsoCutBgEstQCDenriched,
      cfgMuonEcalIsoCutBgEstQCDenriched,
      cfgTauTrkIsoCutBgEstQCDenriched,
      cfgTauEcalIsoCutBgEstQCDenriched,
      cfgTauMuonVetoBgEstQCDenriched,
      cfgMuTauPairBgEstQCDenriched,
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
muonHistManagerBgEstQCDenriched.muonSource = cms.InputTag('muonsBgEstQCDenrichedEcalIsoCumulative')

tauHistManagerBgEstQCDenriched = copy.deepcopy(tauHistManager)
tauHistManagerBgEstQCDenriched.pluginName = cms.string('tauHistManagerBgEstQCDenriched')
tauHistManagerBgEstQCDenriched.tauSource = cms.InputTag('tausBgEstQCDenrichedMuonVetoCumulative')

diTauCandidateHistManagerBgEstQCDenriched = copy.deepcopy(diTauCandidateHistManagerForMuTau)
diTauCandidateHistManagerBgEstQCDenriched.pluginName = cms.string('diTauCandidateHistManagerBgEstQCDenriched')
diTauCandidateHistManagerBgEstQCDenriched.diTauCandidateSource = cms.InputTag('muTauPairsBgEstQCDenriched')
diTauCandidateHistManagerBgEstQCDenriched.visMassHypothesisSource = cms.InputTag('')

from TauAnalysis.BgEstimationTools.tauIdEffZtoMuTauHistManager_cfi import *
tauIdEffHistManagerBgEstQCDenriched = copy.deepcopy(tauIdEffZtoMuTauHistManager)
tauIdEffHistManagerBgEstQCDenriched.pluginName = cms.string('tauIdEffHistManagerBgEstQCDenriched')
tauIdEffHistManagerBgEstQCDenriched.muonSource = cms.InputTag('muonsBgEstQCDenrichedEcalIsoCumulative')
tauIdEffHistManagerBgEstQCDenriched.tauSource = cms.InputTag('tausBgEstQCDenrichedMuonVetoCumulative')
tauIdEffHistManagerBgEstQCDenriched.diTauSource = cms.InputTag('muTauPairsBgEstQCDenriched')
tauIdEffHistManagerBgEstQCDenriched.diTauChargeSignExtractor.src = tauIdEffHistManagerBgEstQCDenriched.diTauSource

dataBinnerBgEstQCDenriched = copy.deepcopy(dataBinner)
dataBinnerBgEstQCDenriched.pluginName = cms.string('dataBinnerBgEstQCDenriched')

analyzeEventsBgEstQCDenriched = cms.EDAnalyzer("GenericAnalyzer",
  
    name = cms.string('BgEstTemplateAnalyzer_QCDenriched'), 
                            
    filters = cms.VPSet(
        genPhaseSpaceCut,
        evtSelTrigger,
        evtSelPrimaryEventVertex,
        evtSelPrimaryEventVertexQuality,
        evtSelPrimaryEventVertexPosition,
        evtSelGlobalMuon,
        evtSelMuonEta,
        evtSelMuonPt,
        cms.PSet(
            pluginName = cms.string('muonTrkIsoCutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('muonTrkIsoCutBgEstQCDenriched', 'cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('muonEcalIsoCutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('muonEcalIsoCutBgEstQCDenriched', 'cumulative')
        ),
        evtSelTauAntiOverlapWithMuonsVeto,
        evtSelTauEta,
        evtSelTauPt,
        evtSelTauLeadTrk,
        evtSelTauLeadTrkPt,
        cms.PSet(
            pluginName = cms.string('tauTrkIsoCutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauTrkIsoCutBgEstQCDenriched', 'cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauEcalIsoCutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauEcalIsoCutBgEstQCDenriched', 'cumulative')
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
            pluginName = cms.string('diMuonVetoBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diMuonVetoBgEstQCDenriched')
        )
    ),
  
    analyzers = cms.VPSet(
        muonHistManagerBgEstQCDenriched,
        tauHistManagerBgEstQCDenriched,
        diTauCandidateHistManagerBgEstQCDenriched,
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
            filter = cms.string('muonTrkIsoCutBgEstQCDenriched'),
            title = cms.string('Muon Track iso.')
        ),
        cms.PSet(
            filter = cms.string('muonEcalIsoCutBgEstQCDenriched'),
            title = cms.string('Muon ECAL iso.')
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
            filter = cms.string('diMuonVetoBgEstQCDenriched'),
            title = cms.string('di-Muon Veto')
        ),
        cms.PSet(
            analyzers = cms.vstring(
                'muonHistManagerBgEstQCDenriched',
                'tauHistManagerBgEstQCDenriched',
                'diTauCandidateHistManagerBgEstQCDenriched',
                'tauIdEffHistManagerBgEstQCDenriched',
                'dataBinnerBgEstQCDenriched'
            )
        )
    )
)

#--------------------------------------------------------------------------------  
# define (final) analysis sequence
#--------------------------------------------------------------------------------

bgEstQCDenrichedAnalysisSequence = cms.Sequence(
    selectMuonsBgEstQCDenriched
   + selectTausBgEstQCDenriched
   + selectMuTauPairsBgEstQCDenriched
   + selectEventsBgEstQCDenriched
   + analyzeEventsBgEstQCDenriched
)
