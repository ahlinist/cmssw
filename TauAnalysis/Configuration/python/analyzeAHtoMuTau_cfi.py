import FWCore.ParameterSet.Config as cms
import copy

# import config for histogram manager filling information about phase-space simulated in Monte Carlo sample
from TauAnalysis.Core.genPhaseSpaceEventInfoHistManager_cfi import *

# import config for event weight histogram manager
from TauAnalysis.Core.eventWeightHistManager_cfi import *

# import config for muon histogram manager
from TauAnalysis.Core.muonHistManager_cfi import *

# import config for tau-jet histogram manager
from TauAnalysis.Core.pftauHistManager_cfi import *

# import config for di-tau histogram manager
from TauAnalysis.Core.diTauCandidateHistManager_cfi import *
diTauCandidateHistManagerForMuTau = copy.deepcopy(diTauCandidateHistManager)
diTauCandidateHistManagerForMuTau.pluginName = cms.string('diTauCandidateHistManagerForMuTau')
diTauCandidateHistManagerForMuTau.pluginType = cms.string('PATMuTauPairHistManager')
diTauCandidateHistManagerForMuTau.diTauCandidateSource = cms.InputTag('selectedMuTauPairsForAHtoMuTauCollinearApproxZmassVetoCumulative')
diTauCandidateHistManagerForMuTau.visMassHypothesisSource = cms.InputTag('')

from TauAnalysis.Core.diTauCandidateCollinearApproxHistManager_cfi import *
diTauCandidateCollinearApproxHistManagerForMuTau = copy.deepcopy(diTauCandidateCollinearApproxHistManager)
diTauCandidateCollinearApproxHistManagerForMuTau.pluginName = cms.string('diTauCandidateCollinearApproxHistManagerForMuTau')
diTauCandidateCollinearApproxHistManagerForMuTau.pluginType = cms.string('PATMuTauPairCollinearApproxHistManager')
diTauCandidateCollinearApproxHistManagerForMuTau.diTauCandidateSource = cms.InputTag('selectedMuTauPairsForAHtoMuTauCollinearApproxZmassVetoCumulative')

from TauAnalysis.Core.diTauCandidateSVfitHistManager_cfi import *
diTauCandidateSVfitHistManagerForMuTau = copy.deepcopy(diTauCandidateSVfitHistManager)
diTauCandidateSVfitHistManagerForMuTau.pluginName = cms.string('diTauCandidateSVfitHistManagerForMuTau')
diTauCandidateSVfitHistManagerForMuTau.pluginType = cms.string('PATMuTauPairSVfitHistManager')
diTauCandidateSVfitHistManagerForMuTau.diTauCandidateSource = cms.InputTag('selectedMuTauPairsForAHtoMuTauCollinearApproxZmassVetoCumulative')

from TauAnalysis.Core.diTauCandidateZllHypothesisHistManager_cfi import *
diTauCandidateZmumuHypothesisHistManagerForMuTau = copy.deepcopy(ZllHypothesisHistManager)
diTauCandidateZmumuHypothesisHistManagerForMuTau.pluginName = cms.string('diTauCandidateZmumuHypothesisHistManagerForMuTau')
diTauCandidateZmumuHypothesisHistManagerForMuTau.pluginType = cms.string('ZllHypothesisMuTauHistManager')
diTauCandidateZmumuHypothesisHistManagerForMuTau.ZllHypothesisSource = cms.InputTag('')
diTauCandidateZmumuHypothesisHistManagerForMuTau.dqmDirectory_store = cms.string('DiTauCandidateZmumuHypothesisQuantities')

# import config for Zmumu veto histogram manager
muPairHistManager = copy.deepcopy(diTauCandidateHistManager)
muPairHistManager.pluginName = cms.string('muPairHistManager')
muPairHistManager.pluginType = cms.string('DiCandidatePairHistManager')
muPairHistManager.diTauCandidateSource = cms.InputTag('allDiMuPairZmumuHypotheses')
muPairHistManager.dqmDirectory_store = cms.string('DiMuZmumuHypothesisQuantities')

# import config for central jet veto histogram manager
from TauAnalysis.Core.jetHistManager_cfi import *

# import config for missing-Et histogram managers
from TauAnalysis.Core.caloMEtHistManager_cfi import *
caloMEtHistManager.leg1Source = cms.InputTag('selectedPatMuonsTrkIPcumulative')
caloMEtHistManager.leg2Source = cms.InputTag('selectedPatTausForMuTauElectronVetoCumulative')
from TauAnalysis.Core.pfMEtHistManager_cfi import *
pfMEtHistManager.leg1Source = cms.InputTag('selectedPatMuonsTrkIPcumulative')
pfMEtHistManager.leg2Source = cms.InputTag('selectedPatTausForMuTauElectronVetoCumulative')

# import config for particle multiplicity histogram manager
from TauAnalysis.Core.particleMultiplicityHistManager_cfi import *

# import config for primary event vertex histogram manager
from TauAnalysis.Core.vertexHistManager_cfi import *

# import config for L1 & HLT histogram manager
from TauAnalysis.Core.triggerHistManager_cfi import *
triggerHistManagerForMuTau = copy.deepcopy(triggerHistManager)
triggerHistManagerForMuTau.pluginName = cms.string('triggerHistManagerForMuTau')
triggerHistManagerForMuTau.l1Bits = cms.vstring(
    'L1_SingleMu3',
    'L1_SingleMu5',
    'L1_SingleMu7',
    'L1_SingleMu10',
    'L1_SingleMu14'
)

triggerHistManagerForMuTau.hltPaths = cms.vstring(
    'HLT_Mu9',
    'HLT_IsoMu9',
    'HLT_Mu11',
    'HLT_Mu15'
)

# import config for event weight histogram manager
from TauAnalysis.Core.eventWeightHistManager_cfi import *

# import config for analyzer modules used for debugging purposes
from TauAnalysis.Core.svFitLikelihoodAnalyzer_cfi import *

# import config for binning results
# used for keeping track of number of events passing all selection criteria
from TauAnalysis.Core.dataBinner_cfi import *

chi2mZbinGridHistManager = cms.PSet(
    pluginName = cms.string('chi2mZbinGridHistManager'),
    pluginType = cms.string('BinGridHistManager'),
    binning = cms.PSet(
        name = cms.string("chi2mZbinning"),
        config = cms.VPSet(
            cms.PSet( 
                extractor = cms.PSet(
                    pluginType = cms.string("PATMuTauPairValExtractor"),
                    src = cms.InputTag('selectedMuTauPairsForAHtoMuTauCollinearApproxZmassVetoCumulative'),
                    value = cms.string('collinearApproxCompatibility("mZ").minuitFitChi2')
                ),
                branchName = cms.string('chi2mZ'),
                binning = cms.PSet(
                    boundaries = cms.vdouble(0.5, 2.5),
                    min = cms.double(-0.01),
                    max = cms.double(1.e+3)
                )
            )
        )
    ),
    histManagers = cms.VPSet(
        diTauCandidateHistManagerForMuTau,
        diTauCandidateCollinearApproxHistManagerForMuTau,
        caloMEtHistManager,
        pfMEtHistManager
    ),
    dqmDirectory_store = cms.string('chi2mZbinnedHistograms')
)

#--------------------------------------------------------------------------------
# define event selection criteria
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.analyzeZtoMuTau_cfi import *

# tau candidate (id.) selection
evtSelTauTaNCdiscr = cms.PSet(
    pluginName = cms.string('evtSelTauTaNCdiscr'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauTaNCdiscrCut', 'cumulative'),
    src_individual = cms.InputTag('tauTaNCdiscrCut', 'individual'),
    systematics = cms.vstring(tauSystematics.keys())
)

# di-tau candidate selection
evtSelDiTauCandidateForAHtoMuTauAntiOverlapVeto = evtSelDiTauCandidateForMuTauAntiOverlapVeto.clone(
    pluginName = cms.string('evtSelDiTauCandidateForAHtoMuTauAntiOverlapVeto'),
    src_cumulative = cms.InputTag('diTauCandidateForAHtoMuTauAntiOverlapVeto', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoMuTauAntiOverlapVeto', 'individual')
)
evtSelDiTauCandidateForAHtoMuTauZeroCharge = evtSelDiTauCandidateForMuTauZeroCharge.clone(
    pluginName = cms.string('evtSelDiTauCandidateForAHtoMuTauZeroCharge'),
    src_cumulative = cms.InputTag('diTauCandidateForAHtoMuTauZeroChargeCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoMuTauZeroChargeCut', 'individual')
)
evtSelDiTauCandidateForAHtoMuTauMt1MET = evtSelDiTauCandidateForMuTauMt1MET.clone(
    pluginName = cms.string('evtSelDiTauCandidateForAHtoMuTauMt1MET'),
    src_cumulative = cms.InputTag('diTauCandidateForAHtoMuTauMt1METcut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoMuTauMt1METcut', 'individual')
)
evtSelDiTauCandidateForAHtoMuTauPzetaDiff = evtSelDiTauCandidateForMuTauPzetaDiff.clone(
    pluginName = cms.string('evtSelDiTauCandidateForAHtoMuTauPzetaDiff'),
    src_cumulative = cms.InputTag('diTauCandidateForAHtoMuTauPzetaDiffCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoMuTauPzetaDiffCut', 'individual')
)
evtSelDiTauCandidateForAHtoMuTauCollinearApproxZmassVeto = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForAHtoMuTauCollinearApproxZmassVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForAHtoMuTauCollinearApproxZmassVeto', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoMuTauCollinearApproxZmassVeto', 'individual'),
    systematics = cms.vstring(muTauPairSystematics.keys())
)

# central jet veto/b-jet candidate selection
evtSelNonCentralJetEt20bTag = cms.PSet(
    pluginName = cms.string('evtSelNonCentralJetEt20bTag'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('centralJetEt20bTagVeto', 'cumulative'),
    src_individual = cms.InputTag('centralJetEt20bTagVeto', 'individual')
)
evtSelCentralJetEt20 = cms.PSet(
    pluginName = cms.string('evtSelCentralJetEt20'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('centralJetEt20Cut', 'cumulative'),
    src_individual = cms.InputTag('centralJetEt20Cut', 'individual')
)
evtSelCentralJetEt20bTag = cms.PSet(
    pluginName = cms.string('evtSelCentralJetEt20bTag'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('centralJetEt20bTagCut', 'cumulative'),
    src_individual = cms.InputTag('centralJetEt20bTagCut', 'individual')
)

#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------

muTauEventDump = cms.PSet(
    pluginName = cms.string('muTauEventDump'),
    pluginType = cms.string('MuTauEventDump'),

    # L1 trigger bits not contained in AOD;
    # in order to process Monte Carlo samples produced by FastSimulation,
    # disable histogram filling for now
    #l1GtReadoutRecordSource = cms.InputTag('hltGtDigis::HLT'),
    #l1GtObjectMapRecordSource = cms.InputTag('hltL1GtObjectMap::HLT'),
    l1GtReadoutRecordSource = cms.InputTag(''),
    l1GtObjectMapRecordSource = cms.InputTag(''),
    l1BitsToPrint = cms.vstring('L1_SingleMu3', 'L1_SingleMu5', 'L1_SingleMu7', 'L1_SingleMu10', 'L1_SingleMu14'),
    
    hltResultsSource = cms.InputTag('TriggerResults::HLT'),
    hltPathsToPrint = cms.vstring('HLT_Mu9', 'HLT_IsoMu9', 'HLT_Mu11', 'HLT_Mu15'),
        
    genParticleSource = cms.InputTag('genParticles'),
    genJetSource = cms.InputTag('iterativeCone5GenJets'),
    genTauJetSource = cms.InputTag('tauGenJets'),
    genEventInfoSource = cms.InputTag('generator'),
    
    electronSource = cms.InputTag('cleanPatElectrons'),
    muonSource = cms.InputTag('selectedPatMuonsTrkIPlooseIsolationCumulative'),
    tauSource = cms.InputTag('selectedPatTausForMuTauElectronVetoCumulative'),
    printTauIdEfficiencies = cms.bool(False),
    diTauCandidateSource = cms.InputTag(''),
    svFitAlgorithms = cms.VPSet(
        cms.PSet(
            name = cms.string("psKine")
        ),
        ##cms.PSet(
        ##    name = cms.string("psKine_MEt")
        ##),
        ##cms.PSet(
        ##    name = cms.string("psKine_MEt_ptBalance")
        ##)
        cms.PSet(
            name = cms.string("polKine"),
            polarizationHypotheses = cms.vstring("LL", "LR", "RL", "RR")
        )
    ),
    muTauZmumuHypothesisSource = cms.InputTag(''),
    diMuZmumuHypothesisSource = cms.InputTag('allDiMuPairZmumuHypotheses'),
    jetSource = cms.InputTag('selectedPatJetsForAHtoMuTauAntiOverlapWithLeptonsVetoCumulative'),
    caloMEtSource = cms.InputTag('patMETs'),
    pfMEtSource = cms.InputTag('patPFMETs'),
    genMEtSource = cms.InputTag('genMetTrue'),
    
    output = cms.string("std::cout"),

    triggerConditions = cms.vstring()
)

#--------------------------------------------------------------------------------
# define analysis sequences
# (ordered list of event selection criteria and histogram filling)
#--------------------------------------------------------------------------------

muTauAnalysisSequence_woBtag = cms.VPSet(
    # fill histograms for full event sample
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        )
    ),

    # generator level phase-space selection
    #
    # NOTE:
    #     (1) to be used in case of Monte Carlo samples
    #         overlapping in simulated phase-space only !!
    #     (2) binning objects for computation of signal acceptance and systematic uncertainties
    #         need to be filled at this stage of the analysis sequence,
    #         so that the number of generator level events within detector acceptance get counted
    #         regardless of whether the event passes or fails the final event selection on reconstruction level !!
    #
    cms.PSet(
        filter = cms.string('genPhaseSpaceCut'),
        title = cms.string('gen. Phase-Space'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        )
    ),
    
    # trigger selection
    cms.PSet(
        filter = cms.string('evtSelTrigger'),
        title = cms.string('Muon Trigger'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        )
    ),

    # primary event vertex selection
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertex'),
        title = cms.string('Vertex'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'vertexHistManager'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexQuality'),
        title = cms.string('p(chi2Vertex) > 0.01'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexPosition'),
        title = cms.string('-25 < zVertex < +25 cm'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager'
        ),
    ),

    # muon acceptance cuts
    cms.PSet(
        filter = cms.string('evtSelGlobalMuon'),
        title = cms.string('global Muon'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsGlobalCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonEta'),
        title = cms.string('-2.1 < eta(Muon) < +2.1'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsEta21Cumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonPt'),
        title = cms.string('Pt(Muon) > 15 GeV'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsPt15Cumulative')
    ),

    # tau acceptance cuts
    cms.PSet(
        filter = cms.string('evtSelTauAntiOverlapWithMuonsVeto'),
        title = cms.string('Tau not overlapping w. Muon'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsPt15Cumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauAntiOverlapWithMuonsVetoCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauEta'),
        title = cms.string('-2.1 < eta(Tau) < +2.1'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager'
        ),
        replace = cms.vstring(
           'muonHistManager.muonSource = selectedPatMuonsPt15Cumulative',
           'tauHistManager.tauSource = selectedPatTausForMuTauEta21Cumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauPt'),
        title = cms.string('Pt(Tau) > 20 GeV'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsPt15Cumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauPt20Cumulative'
        )
    ),
    
    # selection of muon candidate (isolation & id.)
    # produced in muonic tau decay
    cms.PSet(
        filter = cms.string('evtSelMuonTrkIso'),
        title = cms.string('Muon Track iso.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsTrkIsoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonEcalIso'),
        title = cms.string('Muon ECAL iso.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsEcalIsoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonAntiPion'),
        title = cms.string('Muon pi-Veto'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsPionVetoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonTrkIP'),
        title = cms.string('Muon Track IP'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative')
    ),
    
    # selection of tau-jet candidate (id.)
    # produced in hadronic tau decay
    cms.PSet(
        filter = cms.string('evtSelTauLeadTrk'),
        title = cms.string('Tau lead. Track find.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauLeadTrkCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauLeadTrkPt'),
        title = cms.string('Tau lead. Track Pt'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauLeadTrkPtCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauTaNCdiscr'),
        title = cms.string('Tau TaNC discr.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauTaNCdiscrCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauTrkIso'),
        title = cms.string('Tau Track iso.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauTrkIsoCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauEcalIso'),
        title = cms.string('Tau ECAL iso.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauEcalIsoCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauProng'),
        title = cms.string('Tau 1||3-Prong'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauProngCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauCharge'),
        title = cms.string('Charge(Tau) = +/-1'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauChargeCumulative'
        )
    ),    
    cms.PSet(
        filter = cms.string('evtSelTauMuonVeto'),
        title = cms.string('Tau mu-Veto'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauChargeCumulative'
        )
    ),  
    cms.PSet(
        filter = cms.string('evtSelTauElectronVeto'),
        title = cms.string('Tau e-Veto'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative'
        )
    ),

    # selection of muon + tau-jet combinations
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauAntiOverlapVeto'),
        title = cms.string('dR(Muon-Tau) > 0.7'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauAntiOverlapVetoCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauZeroCharge'),
        title = cms.string('Charge(Muon+Tau) = 0'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauZeroChargeCumulative'
        ),
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauMt1MET'),
        title = cms.string('M_{T}(Muon-MET) < 50 GeV'),
        saveRunEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauMt1METcumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauPzetaDiff'),
        title = cms.string('P_{#zeta} - 1.5*P_{#zeta}^{vis} > -20 GeV'),
        saveRunEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateCollinearApproxHistManagerForMuTau',
            'diTauCandidateSVfitHistManagerForMuTau'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative',
            'diTauCandidateCollinearApproxHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative',
            'diTauCandidateSVfitHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative'
        )
    ),

    # veto events compatible with Z --> tau+ tau- hypothesis
    # (based on invariant mass of di-tau pair reconstructed by collinear approximation fit)
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauCollinearApproxZmassVeto'),
        title = cms.string('#Chi^{2}( M(Muon-Tau) == M_{Z} ) > 1.'),
        saveRunEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateCollinearApproxHistManagerForMuTau',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau',
            'muPairHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauCollinearApproxZmassVetoCumulative',
            'diTauCandidateCollinearApproxHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauCollinearApproxZmassVetoCumulative',
            'diTauCandidateSVfitHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauCollinearApproxZmassVetoCumulative',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau.ZllHypothesisSource = muTauPairZmumuHypothesesForAHtoMuTau'
        )
    ),
 
    # veto events compatible with Z --> mu+ mu- hypothesis
    # (based on reconstructed invariant mass of di-muon pair)
    cms.PSet(
        filter = cms.string('evtSelDiMuPairZmumuHypothesisVeto'),
        title = cms.string('not 80 < M (Muon-Muon) < 100 GeV'),
        saveRunEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateCollinearApproxHistManagerForMuTau',
            'diTauCandidateSVfitHistManagerForMuTau',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau',
            'muPairHistManager',
            'jetHistManager',
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauCollinearApproxZmassVetoCumulative',
            'diTauCandidateCollinearApproxHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauCollinearApproxZmassVetoCumulative',
            'diTauCandidateSVfitHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauCollinearApproxZmassVetoCumulative',
            'jetHistManager.jetSource = selectedPatJetsForAHtoMuTauAntiOverlapWithLeptonsVetoCumulative'
        )
    ),

    # veto events containing b-tagged jets with Et > 20
    # in order to avoid overlap with "b-tag" analysis path
    cms.PSet(
        filter = cms.string('evtSelNonCentralJetEt20bTag'),
        title = cms.string('no E_{T} > 20 GeV central Jet with b-Tag'),
        saveRunEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'eventWeightHistManager',
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateCollinearApproxHistManagerForMuTau',
            'diTauCandidateSVfitHistManagerForMuTau',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau',
            'muPairHistManager',
            'jetHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'particleMultiplicityHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau',
            'dataBinner',
            'chi2mZbinGridHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauCollinearApproxZmassVetoCumulative',
            'diTauCandidateHistManagerForMuTau.visMassHypothesisSource = muTauPairVisMassHypothesesForAHtoMuTau',
            'diTauCandidateCollinearApproxHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauCollinearApproxZmassVetoCumulative',
            'diTauCandidateSVfitHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauCollinearApproxZmassVetoCumulative',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau.ZllHypothesisSource = muTauPairZmumuHypothesesForAHtoMuTau',
            'jetHistManager.jetSource = selectedPatJetsForAHtoMuTauAntiOverlapWithLeptonsVetoCumulative'
        )
    )
)

muTauAnalysisSequence_wBtag = cms.VPSet(
    # fill histograms for full event sample
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        )
    ),

    # generator level phase-space selection
    #
    # NOTE:
    #     (1) to be used in case of Monte Carlo samples
    #         overlapping in simulated phase-space only !!
    #     (2) binning objects for computation of signal acceptance and systematic uncertainties
    #         need to be filled at this stage of the analysis sequence,
    #         so that the number of generator level events within detector acceptance get counted
    #         regardless of whether the event passes or fails the final event selection on reconstruction level !!
    #
    cms.PSet(
        filter = cms.string('genPhaseSpaceCut'),
        title = cms.string('gen. Phase-Space'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        )
    ),
    
    # trigger selection
    cms.PSet(
        filter = cms.string('evtSelTrigger'),
        title = cms.string('Muon Trigger'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        )
    ),

    # primary event vertex selection
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertex'),
        title = cms.string('Vertex'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'vertexHistManager'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexQuality'),
        title = cms.string('p(chi2Vertex) > 0.01'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexPosition'),
        title = cms.string('-25 < zVertex < +25 cm'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager'
        ),
    ),

    # muon acceptance cuts
    cms.PSet(
        filter = cms.string('evtSelGlobalMuon'),
        title = cms.string('global Muon'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsGlobalCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonEta'),
        title = cms.string('-2.1 < eta(Muon) < +2.1'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsEta21Cumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonPt'),
        title = cms.string('Pt(Muon) > 15 GeV'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsPt15Cumulative')
    ),

    # tau acceptance cuts
    cms.PSet(
        filter = cms.string('evtSelTauAntiOverlapWithMuonsVeto'),
        title = cms.string('Tau not overlapping w. Muon'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsPt15Cumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauAntiOverlapWithMuonsVetoCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauEta'),
        title = cms.string('-2.1 < eta(Tau) < +2.1'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager'
        ),
        replace = cms.vstring(
           'muonHistManager.muonSource = selectedPatMuonsPt15Cumulative',
           'tauHistManager.tauSource = selectedPatTausForMuTauEta21Cumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauPt'),
        title = cms.string('Pt(Tau) > 20 GeV'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsPt15Cumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauPt20Cumulative'
        )
    ),
    
    # selection of muon candidate (isolation & id.)
    # produced in muonic tau decay    
    cms.PSet(
        filter = cms.string('evtSelMuonTrkIso'),
        title = cms.string('Muon Track iso.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsTrkIsoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonEcalIso'),
        title = cms.string('Muon ECAL iso.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsEcalIsoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonAntiPion'),
        title = cms.string('Muon pi-Veto'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsPionVetoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonTrkIP'),
        title = cms.string('Muon Track IP'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative')
    ),
    
    # selection of tau-jet candidate (id.)
    # produced in hadronic tau decay
    cms.PSet(
        filter = cms.string('evtSelTauLeadTrk'),
        title = cms.string('Tau lead. Track find.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauLeadTrkCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauLeadTrkPt'),
        title = cms.string('Tau lead. Track Pt'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauLeadTrkPtCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauTaNCdiscr'),
        title = cms.string('Tau TaNC discr.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauTaNCdiscrCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauTrkIso'),
        title = cms.string('Tau Track iso.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauTrkIsoCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauEcalIso'),
        title = cms.string('Tau ECAL iso.'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauEcalIsoCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauProng'),
        title = cms.string('Tau 1||3-Prong'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauProngCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauCharge'),
        title = cms.string('Charge(Tau) = +/-1'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauChargeCumulative'
        )
    ),    
    cms.PSet(
        filter = cms.string('evtSelTauMuonVeto'),
        title = cms.string('Tau mu-Veto'),
        saveRunEventNumbers = cms.vstring('')
    ),
     cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauChargeCumulative'
        )
    ),  
    cms.PSet(
        filter = cms.string('evtSelTauElectronVeto'),
        title = cms.string('Tau e-Veto'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative'
        )
    ),
    
    # selection of muon + tau-jet combinations
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauAntiOverlapVeto'),
        title = cms.string('dR(Muon-Tau) > 0.7'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauAntiOverlapVetoCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauZeroCharge'),
        title = cms.string('Charge(Muon+Tau) = 0'),
        saveRunEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauZeroChargeCumulative'
        ),
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauMt1MET'),
        title = cms.string('M_{T}(Muon-MET) < 50 GeV'),
        saveRunEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauMt1METcumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauPzetaDiff'),
        title = cms.string('P_{#zeta} - 1.5*P_{#zeta}^{vis} > -20 GeV'),
        saveRunEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateCollinearApproxHistManagerForMuTau',
            'diTauCandidateSVfitHistManagerForMuTau'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative',
            'diTauCandidateCollinearApproxHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative',
            'diTauCandidateSVfitHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative',            
        )
    ),

    # veto events compatible with Z --> tau+ tau- hypothesis
    # (based on invariant mass of di-tau pair reconstructed by collinear approximation fit)
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoMuTauCollinearApproxZmassVeto'),
        title = cms.string('#Chi^{2}( M(Muon-Tau) == M_{Z} ) > 1.'),
        saveRunEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau',
            'muPairHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauCollinearApproxZmassVetoCumulative',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau.ZllHypothesisSource = muTauPairZmumuHypothesesForAHtoMuTau'
        )
    ),
 
    # veto events compatible with Z --> mu+ mu- hypothesis
    # (based on reconstructed invariant mass of di-muon pair)
    cms.PSet(
        filter = cms.string('evtSelDiMuPairZmumuHypothesisVeto'),
        title = cms.string('not 80 < M (Muon-Muon) < 100 GeV'),
        saveRunEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau',
            'muPairHistManager',
            'jetHistManager',
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauCollinearApproxZmassVetoCumulative',
            'jetHistManager.jetSource = selectedPatJetsForAHtoMuTauAntiOverlapWithLeptonsVetoCumulative'
        )
    ),

    # require at least one b-tagged jet with Et > 20 in the event
    cms.PSet(
        filter = cms.string('evtSelCentralJetEt20'),
        title = cms.string('E_{T} > 20 GeV central Jet'),
        saveRunEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'jetHistManager',
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauCollinearApproxZmassVetoCumulative',
            'jetHistManager.jetSource = selectedPatJetsForAHtoMuTauAntiOverlapWithLeptonsVetoCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelCentralJetEt20bTag'),
        title = cms.string('E_{T} > 20 GeV central Jet with b-Tag'),
        saveRunEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'eventWeightHistManager',
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateCollinearApproxHistManagerForMuTau',
            'diTauCandidateSVfitHistManagerForMuTau',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau',
            'muPairHistManager',
            'jetHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'particleMultiplicityHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau',
            'svFitLikelihoodAnalyzer',
            'dataBinner',
            'chi2mZbinGridHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauCollinearApproxZmassVetoCumulative',
            'diTauCandidateHistManagerForMuTau.visMassHypothesisSource = muTauPairVisMassHypothesesForAHtoMuTau',
            'diTauCandidateCollinearApproxHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauCollinearApproxZmassVetoCumulative',
            'diTauCandidateSVfitHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsForAHtoMuTauCollinearApproxZmassVetoCumulative',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau.ZllHypothesisSource = muTauPairZmumuHypothesesForAHtoMuTau',
            'jetHistManager.jetSource = selectedPatJetsForAHtoMuTauBtagCumulative'
        )
    )
)

