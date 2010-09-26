import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *
from TauAnalysis.RecoTools.tools.eventSelFlagProdConfigurator import *

#--------------------------------------------------------------------------------
# select ttbar + jets background enriched event sample
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------  
# produce collection of pat::Muons
#--------------------------------------------------------------------------------

from TauAnalysis.RecoTools.patMuonSelection_cfi import *

muonsBgEstTTplusJetsEnrichedTrkIso = copy.deepcopy(selectedPatMuonsTrkIso)
muonsBgEstTTplusJetsEnrichedTrkIso.sumPtMax = cms.double(0.10)
muonsBgEstTTplusJetsEnrichedTrkIso.sumPtMethod = cms.string("relative")

muonsBgEstTTplusJetsEnrichedEcalIso = copy.deepcopy(selectedPatMuonsEcalIso)
muonsBgEstTTplusJetsEnrichedEcalIso.sumPtMax = cms.double(0.10)
muonsBgEstTTplusJetsEnrichedEcalIso.sumPtMethod = cms.string("relative")

muonsBgEstTTplusJetsEnrichedCombIso = copy.deepcopy(selectedPatMuonsCombIso)
muonsBgEstTTplusJetsEnrichedCombIso.sumPtMax = cms.double(0.10)
muonsBgEstTTplusJetsEnrichedCombIso.sumPtMethod = cms.string("relative")

muonsBgEstTTplusJetsEnrichedPionVeto = copy.deepcopy(selectedPatMuonsPionVeto)

muonSelConfiguratorBgEstTTplusJetsEnriched = objSelConfigurator(
    [ muonsBgEstTTplusJetsEnrichedTrkIso,
      muonsBgEstTTplusJetsEnrichedEcalIso,
      muonsBgEstTTplusJetsEnrichedCombIso,
      muonsBgEstTTplusJetsEnrichedPionVeto ],
    src = "selectedPatMuonsPt10Cumulative",
    pyModuleName = __name__,
    doSelIndividual = False
)

selectMuonsBgEstTTplusJetsEnriched = muonSelConfiguratorBgEstTTplusJetsEnriched.configure(pyNameSpace = locals())

#--------------------------------------------------------------------------------  
# produce collection of muon + tau-jet combinations
#--------------------------------------------------------------------------------

muTauPairsBgEstTTplusJetsEnriched = cms.EDProducer("PATMuTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('muonsBgEstTTplusJetsEnrichedPionVetoCumulative'),
    srcLeg2 = cms.InputTag('selectedPatTausForMuTauMuonVetoCumulative'),
    dRmin12 = cms.double(0.7),
    srcMET = cms.InputTag('patMETs'),
    recoMode = cms.string(""),
    scaleFuncImprovedCollinearApprox = cms.string('1'),                                               
    verbosity = cms.untracked.int32(0)
)

muTauPairsBgEstTTplusJetsEnrichedZeroCharge = cms.EDFilter("PATMuTauPairSelector",
    src = cms.InputTag('muTauPairsBgEstTTplusJetsEnriched'),                                                   
    cut = cms.string('abs(charge) < 0.5'),
    filter = cms.bool(False)
)

selectMuTauPairsBgEstTTplusJetsEnriched = cms.Sequence(muTauPairsBgEstTTplusJetsEnriched + muTauPairsBgEstTTplusJetsEnrichedZeroCharge)

#--------------------------------------------------------------------------------  
# produce collection of pat::Jets used for central jet veto
# (in order to reject QCD di-jet events)
#--------------------------------------------------------------------------------

jetsBgEstTTplusJetsEnrichedAntiOverlapWithLeptonsVeto = cms.EDFilter("PATJetAntiOverlapSelector",
    src = cms.InputTag("selectedPatJetsEt20Cumulative"),                                                                  
    srcNotToBeFiltered = cms.VInputTag(
        "selectedPatElectronsTrkCumulative",
        "muonsBgEstTTplusJetsEnrichedPionVetoCumulative",
        "selectedPatTausForMuTauMuonVetoCumulative"
    ),
    dRmin = cms.double(0.7),
    filter = cms.bool(False)                                           
)

jetsBgEstTTplusJetsEnrichedEt40 = cms.EDFilter("PATJetSelector",
    src = cms.InputTag("jetsBgEstTTplusJetsEnrichedAntiOverlapWithLeptonsVeto"),
    cut = cms.string('et > 40.'),
    filter = cms.bool(False)
)

jetsBgEstTTplusJetsEnrichedEt40bTag = cms.EDFilter("PATJetSelector",
    src = cms.InputTag("jetsBgEstTTplusJetsEnrichedEt40"),
    cut = cms.string('bDiscriminator("trackCountingHighEffBJetTags") > 4.5'),
    filter = cms.bool(False)
)

jetsBgEstTTplusJetsEnrichedEt60 = cms.EDFilter("PATJetSelector",
    src = cms.InputTag("jetsBgEstTTplusJetsEnrichedAntiOverlapWithLeptonsVeto"),
    cut = cms.string('et > 60.'),
    filter = cms.bool(False)
)

selectJetsBgEstTTplusJetsEnriched = cms.Sequence(
    jetsBgEstTTplusJetsEnrichedAntiOverlapWithLeptonsVeto
   + jetsBgEstTTplusJetsEnrichedEt40 + jetsBgEstTTplusJetsEnrichedEt40bTag
   + jetsBgEstTTplusJetsEnrichedEt60
)

#--------------------------------------------------------------------------------  
# produce boolean event selection flags
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.selectZtoMuTau_cff import *

cfgMuonTrkIsoCutBgEstTTplusJetsEnriched = copy.deepcopy(cfgMuonTrkIsoCut)
cfgMuonTrkIsoCutBgEstTTplusJetsEnriched.pluginName = cms.string('muonTrkIsoCutBgEstTTplusJetsEnriched')
cfgMuonTrkIsoCutBgEstTTplusJetsEnriched.src_cumulative = cms.InputTag('muonsBgEstTTplusJetsEnrichedTrkIsoCumulative')
cfgMuonTrkIsoCutBgEstTTplusJetsEnriched.systematics = cms.vstring()

cfgMuonEcalIsoCutBgEstTTplusJetsEnriched = copy.deepcopy(cfgMuonEcalIsoCut)
cfgMuonEcalIsoCutBgEstTTplusJetsEnriched.pluginName = cms.string('muonEcalIsoCutBgEstTTplusJetsEnriched')
cfgMuonEcalIsoCutBgEstTTplusJetsEnriched.src_cumulative = cms.InputTag('muonsBgEstTTplusJetsEnrichedEcalIsoCumulative')
cfgMuonEcalIsoCutBgEstTTplusJetsEnriched.systematics = cms.vstring()

cfgMuonCombIsoCutBgEstTTplusJetsEnriched = copy.deepcopy(cfgMuonCombIsoCut)
cfgMuonCombIsoCutBgEstTTplusJetsEnriched.pluginName = cms.string('muonCombIsoCutBgEstTTplusJetsEnriched')
cfgMuonCombIsoCutBgEstTTplusJetsEnriched.src_cumulative = cms.InputTag('muonsBgEstTTplusJetsEnrichedCombIsoCumulative')
cfgMuonCombIsoCutBgEstTTplusJetsEnriched.systematics = cms.vstring()

cfgMuonAntiPionCutBgEstTTplusJetsEnriched = copy.deepcopy(cfgMuonAntiPionCut)
cfgMuonAntiPionCutBgEstTTplusJetsEnriched.pluginName = cms.string('muonAntiPionCutBgEstTTplusJetsEnriched')
cfgMuonAntiPionCutBgEstTTplusJetsEnriched.src_cumulative = cms.InputTag('muonsBgEstTTplusJetsEnrichedPionVetoCumulative')
cfgMuonAntiPionCutBgEstTTplusJetsEnriched.systematics = cms.vstring()

cfgMuTauPairBgEstTTplusJetsEnriched = cms.PSet(
    pluginName = cms.string('muTauPairBgEstTTplusJetsEnriched'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('muTauPairsBgEstTTplusJetsEnriched'),
    minNumber = cms.uint32(1)
)

cfgMuTauPairZeroChargeBgEstTTplusJetsEnriched = cms.PSet(
    pluginName = cms.string('muTauPairZeroChargeBgEstTTplusJetsEnriched'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('muTauPairsBgEstTTplusJetsEnrichedZeroCharge'),
    minNumber = cms.uint32(1)
)

cfgJetsEt40BgEstTTplusJetsEnriched = cms.PSet(
    pluginName = cms.string('jetsEt40BgEstTTplusJetsEnriched'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('jetsBgEstTTplusJetsEnrichedEt40'),
    minNumber = cms.uint32(2)
)

cfgJetEt40bTagBgEstTTplusJetsEnriched = cms.PSet(
    pluginName = cms.string('jetEt40bTagBgEstTTplusJetsEnriched'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('jetsBgEstTTplusJetsEnrichedEt40bTag'),
    minNumber = cms.uint32(1)
)

cfgJetEt60BgEstTTplusJetsEnriched = cms.PSet(
    pluginName = cms.string('jetEt60BgEstTTplusJetsEnriched'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('jetsBgEstTTplusJetsEnrichedEt60'),
    minNumber = cms.uint32(1)
)

evtSelConfiguratorBgEstTTplusJetsEnriched = eventSelFlagProdConfigurator(
    [ cfgMuonTrkIsoCutBgEstTTplusJetsEnriched,
      cfgMuonEcalIsoCutBgEstTTplusJetsEnriched,
      cfgMuonCombIsoCutBgEstTTplusJetsEnriched,
      cfgMuonAntiPionCutBgEstTTplusJetsEnriched,
      cfgMuTauPairBgEstTTplusJetsEnriched,
      cfgMuTauPairZeroChargeBgEstTTplusJetsEnriched,
      cfgJetsEt40BgEstTTplusJetsEnriched,
      cfgJetEt40bTagBgEstTTplusJetsEnriched,
      cfgJetEt60BgEstTTplusJetsEnriched ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

selectEventsBgEstTTplusJetsEnriched = evtSelConfiguratorBgEstTTplusJetsEnriched.configure()

#--------------------------------------------------------------------------------  
# apply event selection criteria; fill histograms
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.analyzeZtoMuTau_cfi import *

muonHistManagerBgEstTTplusJetsEnriched = copy.deepcopy(muonHistManager)
muonHistManagerBgEstTTplusJetsEnriched.pluginName = cms.string('muonHistManagerBgEstTTplusJetsEnriched')
muonHistManagerBgEstTTplusJetsEnriched.muonSource = cms.InputTag('muonsBgEstTTplusJetsEnrichedCombIsoCumulative')

tauHistManagerBgEstTTplusJetsEnriched = copy.deepcopy(tauHistManager)
tauHistManagerBgEstTTplusJetsEnriched.pluginName = cms.string('tauHistManagerBgEstTTplusJetsEnriched')
tauHistManagerBgEstTTplusJetsEnriched.tauSource = cms.InputTag('selectedPatTausForMuTauMuonVetoCumulative')

diTauCandidateHistManagerBgEstTTplusJetsEnriched = copy.deepcopy(diTauCandidateHistManagerForMuTau)
diTauCandidateHistManagerBgEstTTplusJetsEnriched.pluginName = cms.string('diTauCandidateHistManagerBgEstTTplusJetsEnriched')
diTauCandidateHistManagerBgEstTTplusJetsEnriched.diTauCandidateSource = cms.InputTag('muTauPairsBgEstTTplusJetsEnrichedZeroCharge')
diTauCandidateHistManagerBgEstTTplusJetsEnriched.visMassHypothesisSource = cms.InputTag('')

jetHistManagerBgEstTTplusJetsEnriched = copy.deepcopy(jetHistManager)
jetHistManagerBgEstTTplusJetsEnriched.pluginName = cms.string('jetHistManagerBgEstTTplusJetsEnriched')
jetHistManagerBgEstTTplusJetsEnriched.jetSource = cms.InputTag('jetsBgEstTTplusJetsEnrichedEt40')

from TauAnalysis.BgEstimationTools.tauIdEffZtoMuTauHistManager_cfi import *
tauIdEffHistManagerBgEstTTplusJetsEnriched = copy.deepcopy(tauIdEffZtoMuTauHistManager)
tauIdEffHistManagerBgEstTTplusJetsEnriched.pluginName = cms.string('tauIdEffHistManagerBgEstTTplusJetsEnriched')
tauIdEffHistManagerBgEstTTplusJetsEnriched.muonSource = cms.InputTag('muonsBgEstTTplusJetsEnrichedCombIsoCumulative')
tauIdEffHistManagerBgEstTTplusJetsEnriched.tauSource = cms.InputTag('selectedPatTausForMuTauMuonVetoCumulative')
tauIdEffHistManagerBgEstTTplusJetsEnriched.diTauSource = cms.InputTag('muTauPairsBgEstTTplusJetsEnrichedZeroCharge')
tauIdEffHistManagerBgEstTTplusJetsEnriched.diTauChargeSignExtractor.src = tauIdEffHistManagerBgEstTTplusJetsEnriched.diTauSource

dataBinnerBgEstTTplusJetsEnriched = copy.deepcopy(dataBinner)
dataBinnerBgEstTTplusJetsEnriched.pluginName = cms.string('dataBinnerBgEstTTplusJetsEnriched')

analyzeEventsBgEstTTplusJetsEnriched = cms.EDAnalyzer("GenericAnalyzer",
  
    name = cms.string('BgEstTemplateAnalyzer_TTplusJetsEnriched'), 
                            
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
            pluginName = cms.string('muonTrkIsoCutBgEstTTplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('muonTrkIsoCutBgEstTTplusJetsEnriched', 'cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('muonEcalIsoCutBgEstTTplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('muonEcalIsoCutBgEstTTplusJetsEnriched', 'cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('muonCombIsoCutBgEstTTplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('muonCombIsoCutBgEstTTplusJetsEnriched', 'cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('muonAntiPionCutBgEstTTplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('muonAntiPionCutBgEstTTplusJetsEnriched', 'cumulative'),
        ),
        evtSelTauAntiOverlapWithMuonsVeto,
        evtSelTauEta,
        evtSelTauPt,
        evtSelTauLeadTrk,
        evtSelTauLeadTrkPt,
        evtSelTauTrkIso,
        evtSelTauEcalIso,        
        evtSelTauProng,
        evtSelTauCharge,
        evtSelTauMuonVeto,
        cms.PSet(
            pluginName = cms.string('muTauPairBgEstTTplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('muTauPairBgEstTTplusJetsEnriched')
        ),
        cms.PSet(
            pluginName = cms.string('muTauPairZeroChargeBgEstTTplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('muTauPairZeroChargeBgEstTTplusJetsEnriched')
        ),
        cms.PSet(
            pluginName = cms.string('jetEt40BgEstTTplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('jetsEt40BgEstTTplusJetsEnriched')
        ),
        cms.PSet(
            pluginName = cms.string('jetEt40bTagBgEstTTplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('jetEt40bTagBgEstTTplusJetsEnriched')
        ),
        cms.PSet(
            pluginName = cms.string('jetEt60BgEstTTplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('jetEt60BgEstTTplusJetsEnriched')
        )
    ),
  
    analyzers = cms.VPSet(
        muonHistManagerBgEstTTplusJetsEnriched,
        tauHistManagerBgEstTTplusJetsEnriched,
        diTauCandidateHistManagerBgEstTTplusJetsEnriched,
        jetHistManagerBgEstTTplusJetsEnriched,
        tauIdEffHistManagerBgEstTTplusJetsEnriched,
        dataBinnerBgEstTTplusJetsEnriched
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
            filter = cms.string('muonTrkIsoCutBgEstTTplusJetsEnriched'),
            title = cms.string('Muon Track iso.')
        ),
        cms.PSet(
            filter = cms.string('muonEcalIsoCutBgEstTTplusJetsEnriched'),
            title = cms.string('Muon ECAL iso.')
        ),
        cms.PSet(
            filter = cms.string('muonCombIsoCutBgEstTTplusJetsEnriched'),
            title = cms.string('Muon Combined iso.')
        ),
        cms.PSet(
            filter = cms.string('muonAntiPionCutBgEstTTplusJetsEnriched'),
            title = cms.string('Muon pi-Veto')
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
            filter = cms.string('evtSelTauTrkIso'),
            title = cms.string('Tau Track iso.'),
            saveRunEventNumbers = cms.vstring('')
        ),
        cms.PSet(
            filter = cms.string('evtSelTauEcalIso'),
            title = cms.string('Tau ECAL iso.'),
            saveRunEventNumbers = cms.vstring('')
        ),
        cms.PSet(
            filter = cms.string('evtSelTauProng'),
            title = cms.string('Tau 1||3-Prong'),
            saveRunEventNumbers = cms.vstring('')
        ),
        cms.PSet(
            filter = cms.string('evtSelTauCharge'),
            title = cms.string('Charge(Tau) = +/-1'),
            saveRunEventNumbers = cms.vstring('')
        ),
        cms.PSet(
            filter = cms.string('evtSelTauMuonVeto'),
            title = cms.string('Tau mu-Veto'),
            saveRunEventNumbers = cms.vstring('')
        ),
        cms.PSet(
            filter = cms.string('muTauPairBgEstTTplusJetsEnriched'),
            title = cms.string('dR(Muon-Tau) > 0.7')
        ),
        cms.PSet(
            filter = cms.string('muTauPairZeroChargeBgEstTTplusJetsEnriched'),
            title = cms.string('Charge(Muon+Tau) = 0')
        ),
        cms.PSet(
            filter = cms.string('jetEt40BgEstTTplusJetsEnriched'),
            title = cms.string('two E_{T} > 40 GeV Jets')
        ),
        cms.PSet(
            filter = cms.string('jetEt40bTagBgEstTTplusJetsEnriched'),
            title = cms.string('one E_{T} > 40 GeV Jet with b-Tag')
        ),
        cms.PSet(
            filter = cms.string('jetEt60BgEstTTplusJetsEnriched'),
            title = cms.string('one E_{T} > 60 GeV Jet')
        ),
        cms.PSet(
            analyzers = cms.vstring(
                'muonHistManagerBgEstTTplusJetsEnriched',
                'tauHistManagerBgEstTTplusJetsEnriched',
                'diTauCandidateHistManagerBgEstTTplusJetsEnriched',
                'jetHistManagerBgEstTTplusJetsEnriched',
                'tauIdEffHistManagerBgEstTTplusJetsEnriched',
                'dataBinnerBgEstTTplusJetsEnriched'
            )
        )
    )
)

#--------------------------------------------------------------------------------  
# define (final) analysis sequence
#--------------------------------------------------------------------------------

bgEstTTplusJetsEnrichedAnalysisSequence = cms.Sequence(
    selectMuonsBgEstTTplusJetsEnriched
   + selectMuTauPairsBgEstTTplusJetsEnriched
   + selectJetsBgEstTTplusJetsEnriched 
   + selectEventsBgEstTTplusJetsEnriched
   + analyzeEventsBgEstTTplusJetsEnriched
)
