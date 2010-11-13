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
diTauCandidateHistManagerForMuTau.diTauCandidateSource = cms.InputTag('allMuTauPairs')
diTauCandidateHistManagerForMuTau.visMassHypothesisSource = cms.InputTag('')

from TauAnalysis.Core.diTauCandidateCollinearApproxHistManager_cfi import *
diTauCandidateCollinearApproxHistManagerBinnedForMuTau = copy.deepcopy(diTauCandidateCollinearApproxHistManagerBinned)
diTauCandidateCollinearApproxHistManagerBinnedForMuTau.pluginName = cms.string('diTauCandidateCollinearApproxHistManagerBinnedForMuTau')
diTauCandidateCollinearApproxHistManagerForMuTau = copy.deepcopy(diTauCandidateCollinearApproxHistManager)
diTauCandidateCollinearApproxHistManagerForMuTau.pluginName = cms.string('diTauCandidateCollinearApproxHistManagerForMuTau')
diTauCandidateCollinearApproxHistManagerForMuTau.pluginType = cms.string('PATMuTauPairCollinearApproxHistManager')
diTauCandidateCollinearApproxHistManagerForMuTau.diTauCandidateSource = cms.InputTag('selectedMuTauPairsPzetaDiffCumulative')
diTauCandidateCollinearApproxHistManagerBinnedForMuTau.histManagers = cms.VPSet(
    diTauCandidateHistManagerForMuTau.clone(
        diTauCandidateSource = cms.InputTag('selectedMuTauPairsPzetaDiffCumulative')
    ),
    diTauCandidateCollinearApproxHistManagerForMuTau,
    pfMEtHistManager,
    caloMEtHistManager,
    genPhaseSpaceEventInfoHistManager
)

from TauAnalysis.Core.diTauCandidateSVfitHistManager_cfi import *
diTauCandidateSVfitHistManagerForMuTau = copy.deepcopy(diTauCandidateSVfitHistManager)
diTauCandidateSVfitHistManagerForMuTau.pluginName = cms.string('diTauCandidateSVfitHistManagerForMuTau')
diTauCandidateSVfitHistManagerForMuTau.pluginType = cms.string('PATMuTauPairSVfitHistManager')
diTauCandidateSVfitHistManagerForMuTau.diTauCandidateSource = cms.InputTag('selectedMuTauPairsPzetaDiffCumulative')

from TauAnalysis.Core.diTauCandidateEventActivityHistManager_cfi import *
diTauCandidateEventActivityHistManagerForMuTau = copy.deepcopy(diTauCandidateEventActivityHistManager)
diTauCandidateEventActivityHistManagerForMuTau.pluginName = cms.string('diTauCandidateEventActivityHistManagerForMuTau')
diTauCandidateEventActivityHistManagerForMuTau.pluginType = cms.string('PATMuTauPairEventActivityHistManager')
diTauCandidateEventActivityHistManagerForMuTau.diTauCandidateSource = cms.InputTag('selectedMuTauPairsPzetaDiffCumulative')

from TauAnalysis.Core.diTauCandidateZllHypothesisHistManager_cfi import *
diTauCandidateZmumuHypothesisHistManagerForMuTau = copy.deepcopy(ZllHypothesisHistManager)
diTauCandidateZmumuHypothesisHistManagerForMuTau.pluginName = cms.string('diTauCandidateZmumuHypothesisHistManagerForMuTau')
diTauCandidateZmumuHypothesisHistManagerForMuTau.pluginType = cms.string('ZllHypothesisMuTauHistManager')
diTauCandidateZmumuHypothesisHistManagerForMuTau.ZllHypothesisSource = cms.InputTag('muTauPairZmumuHypotheses')
diTauCandidateZmumuHypothesisHistManagerForMuTau.dqmDirectory_store = cms.string('DiTauCandidateZmumuHypothesisQuantities')

# import config for Zmumu veto histogram manager
muPairHistManagerByLooseIsolation = diTauCandidateHistManager.clone(
    pluginName = cms.string('muPairHistManagerByLooseIsolation'),
    pluginType = cms.string('PATDiMuPairHistManager'),
    diTauCandidateSource = cms.InputTag('allDiMuPairZmumuHypothesesByLooseIsolation'),
    dqmDirectory_store = cms.string('DiMuZmumuHypothesisByLooseIsolationQuantities')
)
muPairHistManagerByMass = diTauCandidateHistManager.clone(
    pluginName = cms.string('muPairHistManagerByMass'),
    pluginType = cms.string('PATDiMuPairHistManager'),
    diTauCandidateSource = cms.InputTag('allDiMuPairZmumuHypothesesByMass'),
    dqmDirectory_store = cms.string('DiMuZmumuHypothesisByMassQuantities')
)

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

diTauCandidateCollinearApproxBinnerForMuTau = cms.PSet(
    pluginName = cms.string('diTauCandidateCollinearApproxBinnerForMuTau'),
    pluginType = cms.string('DataBinner'),
    binning = collinearApproxBinning,
    binningService = cms.PSet(
        pluginType = cms.string("DataBinningService")
    ),
    dqmDirectory_store = cms.string('collinearApproxBinningResults4regions')
)

# import config for binning results
# used for keeping track of number of events passing all selection criteria
from TauAnalysis.Core.dataBinner_cfi import *

# import config for binning results
# used to estimate acceptance of event selection
from TauAnalysis.Core.modelBinner_cfi import *
modelBinnerForMuTauGenTauLeptonPairAcc = copy.deepcopy(modelBinner)
modelBinnerForMuTauGenTauLeptonPairAcc.pluginName = cms.string('modelBinnerForMuTauGenTauLeptonPairAcc')
modelBinnerForMuTauGenTauLeptonPairAcc.srcGenFlag = cms.InputTag("isGenZtoMuTau")
modelBinnerForMuTauGenTauLeptonPairAcc.srcRecFlag = cms.InputTag("isGenZtoMuTauWithinAcceptance")
modelBinnerForMuTauGenTauLeptonPairAcc.dqmDirectory_store = cms.string('modelBinnerForMuTauGenTauLeptonPairAcc')
modelBinnerForMuTauWrtGenTauLeptonPairAcc = copy.deepcopy(modelBinner)
modelBinnerForMuTauWrtGenTauLeptonPairAcc.pluginName = cms.string('modelBinnerForMuTauWrtGenTauLeptonPairAcc')
modelBinnerForMuTauWrtGenTauLeptonPairAcc.srcGenFlag = cms.InputTag("isGenZtoMuTauWithinAcceptance")
modelBinnerForMuTauWrtGenTauLeptonPairAcc.srcRecFlag = cms.InputTag("isRecZtoMuTau")
modelBinnerForMuTauWrtGenTauLeptonPairAcc.dqmDirectory_store = cms.string('modelBinnerForMuTauWrtGenTauLeptonPairAcc')

# import config for binning results
# used to estimate systematic uncertainties
from TauAnalysis.Core.sysUncertaintyBinner_cfi import *
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *
sysUncertaintyNames = [ "CENTRAL_VALUE", ]
sysUncertaintyNames.extend(
    getSysUncertaintyNames(
        [ muonSystematics,
          tauSystematics,
          jetSystematics,
          theorySystematics ]
    )
)
sysUncertaintyBinnerForMuTauAcc = sysUncertaintyBinner.clone(
    pluginName = cms.string('sysUncertaintyBinnerForMuTauAcc'),
    binnerPlugins = cms.VPSet(
        modelBinnerForMuTauGenTauLeptonPairAcc,
        modelBinnerForMuTauWrtGenTauLeptonPairAcc
    ),
    systematics = cms.vstring(sysUncertaintyNames)
)
sysUncertaintyBinnerForMuTauEff = sysUncertaintyBinner.clone(
    pluginName = cms.string('sysUncertaintyBinnerForMuTauEff'),
    binnerPlugins = cms.VPSet(
        dataBinner
    ),
    systematics = cms.vstring(sysUncertaintyNames)
)

#--------------------------------------------------------------------------------
# define event selection criteria
#--------------------------------------------------------------------------------

# generator level phase-space selection
# (NOTE: to be used in case of Monte Carlo samples
#        overlapping in simulated phase-space only !!)
evtSelGenPhaseSpace = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('genPhaseSpaceCut')
)

# trigger selection
evtSelTrigger = cms.PSet(
    pluginName = cms.string('evtSelTrigger'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('Trigger')
)

# data-quality selection
evtSelDataQuality = cms.PSet(
    pluginName = cms.string('evtSelDataQuality'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('dataQualityCutsPassed'),
    failSilent = cms.bool(True)
)

# primary event vertex selection
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

# muon acceptance cuts
evtSelGlobalMuon = cms.PSet(
    pluginName = cms.string('evtSelGlobalMuon'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('globalMuonCut', 'cumulative'),
    src_individual = cms.InputTag('globalMuonCut', 'individual'),
    systematics = cms.vstring(muonSystematics.keys())
)
evtSelMuonEta = cms.PSet(
    pluginName = cms.string('evtSelMuonEta'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('muonEtaCut', 'cumulative'),
    src_individual = cms.InputTag('muonEtaCut', 'individual'),
    systematics = cms.vstring(muonSystematics.keys())
)
evtSelMuonPt = cms.PSet(
    pluginName = cms.string('evtSelMuonPt'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('muonPtCut', 'cumulative'),
    src_individual = cms.InputTag('muonPtCut', 'individual'),
    systematics = cms.vstring(muonSystematics.keys())
)

# tau acceptance cuts
evtSelTauAntiOverlapWithMuonsVeto = cms.PSet(
    pluginName = cms.string('evtSelTauAntiOverlapWithMuonsVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauAntiOverlapWithMuonsVeto', 'cumulative'),
    src_individual = cms.InputTag('tauAntiOverlapWithMuonsVeto', 'individual'),
    systematics = cms.vstring(tauSystematics.keys())
)
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

# muon candidate (isolation & id.) selection
evtSelMuonVbTfId = cms.PSet(
    pluginName = cms.string('evtSelMuonVbTfId'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('muonVbTfIdCut', 'cumulative'),
    src_individual = cms.InputTag('muonVbTfIdCut', 'individual'),
    systematics = cms.vstring(muonSystematics.keys())
)
evtSelMuonPFRelIso = cms.PSet(
    pluginName = cms.string('evtSelMuonPFRelIso'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('muonPFRelIsoCut', 'cumulative'),
    src_individual = cms.InputTag('muonPFRelIsoCut', 'individual'),
    systematics = cms.vstring(muonSystematics.keys())
)
evtSelMuonAntiPion = cms.PSet(
    pluginName = cms.string('evtSelMuonAntiPion'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('muonAntiPionCut', 'cumulative'),
    src_individual = cms.InputTag('muonAntiPionCut', 'individual'),
    systematics = cms.vstring(muonSystematics.keys())
)
evtSelMuonTrkIP = cms.PSet(
    pluginName = cms.string('evtSelMuonTrkIP'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('muonTrkIPcut', 'cumulative'),
    src_individual = cms.InputTag('muonTrkIPcut', 'individual'),
    systematics = cms.vstring(muonSystematics.keys())
)

# tau candidate (id.) selection
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
evtSelTauTaNCdiscr = cms.PSet(
    pluginName = cms.string('evtSelTauTaNCdiscr'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauTaNCdiscrCut', 'cumulative'),
    src_individual = cms.InputTag('tauTaNCdiscrCut', 'individual'),
    systematics = cms.vstring(tauSystematics.keys())
)
evtSelTauTrkIso = cms.PSet(
    pluginName = cms.string('evtSelTauTrkIso'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauTrkIsoCut', 'cumulative'),
    src_individual = cms.InputTag('tauTrkIsoCut', 'individual'),
    systematics = cms.vstring(tauSystematics.keys())
)
evtSelTauEcalIso = cms.PSet(
    pluginName = cms.string('evtSelTauEcalIso'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauEcalIsoCut', 'cumulative'),
    src_individual = cms.InputTag('tauEcalIsoCut', 'individual'),
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

# di-tau candidate selection
evtSelDiTauCandidateForMuTauAntiOverlapVeto = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForMuTauAntiOverlapVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForMuTauAntiOverlapVeto', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForMuTauAntiOverlapVeto', 'individual'),
    systematics = cms.vstring(muTauPairSystematics.keys())
)
evtSelDiTauCandidateForMuTauZeroCharge = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForMuTauZeroCharge'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForMuTauZeroChargeCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForMuTauZeroChargeCut', 'individual'),
    systematics = cms.vstring(muTauPairSystematics.keys())
)
evtSelDiTauCandidateForMuTauAcoplanarity12 = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForMuTauAcoplanarity12'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForMuTauAcoplanarity12Cut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForMuTauAcoplanarity12Cut', 'individual'),
    systematics = cms.vstring(muTauPairSystematics.keys())
)
evtSelDiTauCandidateForMuTauMt1MET = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForMuTauMt1MET'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForMuTauMt1METcut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForMuTauMt1METcut', 'individual'),
    systematics = cms.vstring(muTauPairSystematics.keys())
)
evtSelDiTauCandidateForMuTauPzetaDiff = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForMuTauPzetaDiff'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForMuTauPzetaDiffCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForMuTauPzetaDiffCut', 'individual'),
    systematics = cms.vstring(muTauPairSystematics.keys())
)

# veto events compatible with Z --> mu+ mu- hypothesis
evtSelDiMuPairZmumuHypothesisVetoByLooseIsolation = cms.PSet(
    pluginName = cms.string('evtSelDiMuPairZmumuHypothesisVetoByLooseIsolation'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('diMuPairZmumuHypothesisVetoByLooseIsolation')
)
evtSelDiMuPairZmumuHypothesisVetoByMass = cms.PSet(
    pluginName = cms.string('evtSelDiMuPairZmumuHypothesisVetoByMass'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('diMuPairZmumuHypothesisVetoByMass')
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

    doGenInfo = cms.bool(True),
    genParticleSource = cms.InputTag('genParticles'),
    genJetSource = cms.InputTag('ak5GenJets'),
    genTauJetSource = cms.InputTag('tauGenJets'),
    genEventInfoSource = cms.InputTag('generator'),

    electronSource = cms.InputTag('cleanPatElectrons'),
    muonSource = cms.InputTag('cleanPatMuons'),
    tauSource = cms.InputTag('selectedPatTausPt20Cumulative'),
    printTauIdEfficiencies = cms.bool(False),
    diTauCandidateSource = cms.InputTag('allMuTauPairs'),
    svFitAlgorithms = cms.VPSet(
        cms.PSet(
            name = cms.string("psKine_MEt_ptBalance")
        )
    ),
    muTauZmumuHypothesisSource = cms.InputTag('muTauPairZmumuHypotheses'),
    diMuZmumuHypothesisSource = cms.InputTag('allDiMuPairZmumuHypothesesByMass'),
    jetSource = cms.InputTag('patJets'),
    caloMEtSource = cms.InputTag('patMETs'),
    pfMEtSource = cms.InputTag('patPFMETs'),
    genMEtSource = cms.InputTag('genMetTrue'),

    #output = cms.string("muTauEventDump.txt"),
    output = cms.string("std::cout"),

    #triggerConditions = cms.vstring("evtSelTauTrkIso: rejected_cumulative")
    triggerConditions = cms.vstring("evtSelDiMuPairZmumuHypothesisVetoByLooseIsolation: passed_cumulative")
    #triggerConditions = cms.vstring("genPhaseSpaceCut: always")
)

#--------------------------------------------------------------------------------
# define analysis sequence
# (ordered list of event selection criteria and histogram filling)
#--------------------------------------------------------------------------------

muTauAnalysisSequence = cms.VPSet(
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
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'muonHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau',
            'modelBinnerForMuTauGenTauLeptonPairAcc',
            'modelBinnerForMuTauWrtGenTauLeptonPairAcc',
            'sysUncertaintyBinnerForMuTauAcc'
        )
    ),

    # trigger selection
    cms.PSet(
        filter = cms.string('evtSelTrigger'),
        title = cms.string('Muon Trigger'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
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

    # data-quality selection
    cms.PSet(
        filter = cms.string('evtSelDataQuality'),
        title = cms.string('Data quality'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
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

    # primary event vertex selection
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertex'),
        title = cms.string('Vertex'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'vertexHistManager'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexQuality'),
        title = cms.string('Vertex quality'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
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
        title = cms.string('-24 < zVertex < +24 cm'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
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
        saveRunLumiSectionEventNumbers = cms.vstring('')
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
        saveRunLumiSectionEventNumbers = cms.vstring('')
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
        title = cms.string('Pt(Muon) > 10 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
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
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsPt10Cumulative')
    ),

    # tau acceptance cuts
    cms.PSet(
        filter = cms.string('evtSelTauAntiOverlapWithMuonsVeto'),
        title = cms.string('Tau not overlapping w. Muon'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
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
            'muonHistManager.muonSource = selectedPatMuonsPt10Cumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauAntiOverlapWithMuonsVetoCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauEta'),
        title = cms.string('-2.1 < eta(Tau) < +2.1'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
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
           'muonHistManager.muonSource = selectedPatMuonsPt10Cumulative',
           'tauHistManager.tauSource = selectedPatTausForMuTauEta21Cumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauPt'),
        title = cms.string('Pt(Tau) > 20 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
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
            'muonHistManager.muonSource = selectedPatMuonsPt10Cumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauPt20Cumulative'
        )
    ),

    # selection of muon candidate (isolation & id.)
    # produced in muonic tau decay
    cms.PSet(
        filter = cms.string('evtSelMuonVbTfId'),
        title = cms.string('Muon VBTF id.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsVbTfIdCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonPFRelIso'),
        title = cms.string('Muon iso.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsPFRelIsoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonAntiPion'),
        title = cms.string('Muon pi-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
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
        saveRunLumiSectionEventNumbers = cms.vstring('')
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
        saveRunLumiSectionEventNumbers = cms.vstring('')
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
        saveRunLumiSectionEventNumbers = cms.vstring('')
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
        saveRunLumiSectionEventNumbers = cms.vstring('')
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
        saveRunLumiSectionEventNumbers = cms.vstring('')
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
        saveRunLumiSectionEventNumbers = cms.vstring('')
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
        saveRunLumiSectionEventNumbers = cms.vstring('')
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
        saveRunLumiSectionEventNumbers = cms.vstring('')
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
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauMuonVetoCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauElectronVeto'),
        title = cms.string('Tau e-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
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
        filter = cms.string('evtSelDiTauCandidateForMuTauAntiOverlapVeto'),
        title = cms.string('dR(Muon-Tau) > 0.7'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
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
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsAntiOverlapVetoCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForMuTauZeroCharge'),
        title = cms.string('Charge(Muon+Tau) = 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
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
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsZeroChargeCumulative'
        ),
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForMuTauAcoplanarity12'),
        title = cms.string('Acoplanarity(Muon+Tau)'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
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
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsAcoplanarity12Cumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForMuTauMt1MET'),
        title = cms.string('M_{T}(Muon-MET) < 50 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
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
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsMt1METcumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForMuTauPzetaDiff'),
        title = cms.string('P_{#zeta} - 1.5*P_{#zeta}^{vis} > -20 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau',
            'muPairHistManagerByLooseIsolation'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsPzetaDiffCumulative',
            'muPairHistManagerByLooseIsolation.diTauCandidateSource = allDiMuPairZmumuHypothesesByLooseIsolation'
        )
    ),

    # veto events compatible with Z --> mu+ mu- hypothesis
    cms.PSet(
        filter = cms.string('evtSelDiMuPairZmumuHypothesisVetoByLooseIsolation'),
        title = cms.string('not Charge(isoMuon+isoMuon) = 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau',
            'muPairHistManagerByLooseIsolation',
            'muPairHistManagerByMass'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsPzetaDiffCumulative',
            'muPairHistManagerByLooseIsolation.diTauCandidateSource = selectedDiMuPairZmumuHypothesesByLooseIsolation',
            'muPairHistManagerByMass.diTauCandidateSource = allDiMuPairZmumuHypothesesByMass'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelDiMuPairZmumuHypothesisVetoByMass'),
        title = cms.string('not 80 < M(Muon-Muon) < 100 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'eventWeightHistManager',
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateCollinearApproxHistManagerForMuTau',
            'diTauCandidateCollinearApproxHistManagerBinnedForMuTau',
            'diTauCandidateCollinearApproxBinnerForMuTau',
            'diTauCandidateSVfitHistManagerForMuTau',
            'diTauCandidateEventActivityHistManagerForMuTau',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau',
            'muPairHistManagerByMass',
            'jetHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'particleMultiplicityHistManager',
            'vertexHistManager',
            'triggerHistManagerForMuTau',
            'dataBinner',
	    'sysUncertaintyBinnerForMuTauEff'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsPzetaDiffCumulative',
            'diTauCandidateHistManagerForMuTau.visMassHypothesisSource = muTauPairVisMassHypotheses',
            'diTauCandidateCollinearApproxHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsPzetaDiffCumulative',
            'diTauCandidateSVfitHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsPzetaDiffCumulative',
            'diTauCandidateEventActivityHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsPzetaDiffCumulative',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau.ZllHypothesisSource = muTauPairZmumuHypotheses',
            'muPairHistManagerByMass.diTauCandidateSource = selectedDiMuPairZmumuHypothesesByMass'
        )
    )
)
