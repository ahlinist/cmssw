import FWCore.ParameterSet.Config as cms
import copy

# import config for histogram manager filling information about phase-space simulated in Monte Carlo sample
from TauAnalysis.Core.genPhaseSpaceEventInfoHistManager_cfi import *

# import config for event weight histogram manager
from TauAnalysis.Core.eventWeightHistManager_cfi import *

# import config for muon histogram manager
from TauAnalysis.Core.muonHistManager_cfi import *

# import config for tau-jet histogram manager
from TauAnalysis.Core.genTauHistManager_cfi import *
from TauAnalysis.Core.pftauHistManager_cfi import *
tauHistManager.useHPSclassicAlgorithm = cms.bool(True)

# import config for di-tau histogram manager
from TauAnalysis.Core.diTauCandidateHistManager_cfi import *
diTauCandidateHistManagerForMuTau = copy.deepcopy(diTauCandidateHistManager)
diTauCandidateHistManagerForMuTau.pluginName = cms.string('diTauCandidateHistManagerForMuTau')
diTauCandidateHistManagerForMuTau.pluginType = cms.string('PATMuTauPairHistManager')
diTauCandidateHistManagerForMuTau.diTauCandidateSource = cms.InputTag('allMuTauPairs')
diTauCandidateHistManagerForMuTau.visMassHypothesisSource = cms.InputTag('')

diTauCandidateHistManagerForMuTauPFtype1MET = copy.deepcopy(diTauCandidateHistManagerForMuTau)
diTauCandidateHistManagerForMuTauPFtype1MET.pluginName = cms.string('diTauCandidateHistManagerForMuTauPFtype1MET')
diTauCandidateHistManagerForMuTauPFtype1MET.diTauCandidateSource = cms.InputTag('allMuTauPairsPFtype1MET')
diTauCandidateHistManagerForMuTauPFtype1MET.dqmDirectory_store = cms.string('DiTauCandidateQuantitiesPFtype1MET')

from TauAnalysis.Core.diTauCandidateNSVfitHistManager_cfi import *
diTauCandidateNSVfitHistManagerForMuTau = copy.deepcopy(diTauCandidateNSVfitHistManager)
diTauCandidateNSVfitHistManagerForMuTau.pluginName = cms.string('diTauCandidateNSVfitHistManagerForMuTau')
diTauCandidateNSVfitHistManagerForMuTau.pluginType = cms.string('PATMuTauPairNSVfitHistManager')
diTauCandidateNSVfitHistManagerForMuTau.diTauCandidateSource = cms.InputTag('selectedMuTauPairsPzetaDiffCumulative')
diTauCandidateNSVfitHistManagerForMuTau.nSVfitEventHypotheses = cms.PSet(
    psKine_MEt_logM_fit = cms.string('psKine_MEt_logM_fit'),
    psKine_MEt_logM_int = cms.string('psKine_MEt_logM_int')
)

diTauCandidateNSVfitHistManagerForMuTauPFtype1MET = copy.deepcopy(diTauCandidateNSVfitHistManagerForMuTau)
diTauCandidateNSVfitHistManagerForMuTauPFtype1MET.pluginName = cms.string('diTauCandidateNSVfitHistManagerForMuTauPFtype1MET')
diTauCandidateNSVfitHistManagerForMuTauPFtype1MET.diTauCandidateSource = cms.InputTag('allMuTauPairsPFtype1MET')
diTauCandidateNSVfitHistManagerForMuTauPFtype1MET.dqmDirectory_store = cms.string('DiTauCandidateNSVfitQuantitiesPFtype1MET')

diTauCandidateNSVfitVtxMultiplicityBinGridHistManager = cms.PSet(
    pluginName = cms.string('diTauCandidateNSVfitVtxMultiplicityBinGridHistManager'),
    pluginType = cms.string('BinGridHistManager'),
    binning = cms.PSet(
        name = cms.string("vtxMultiplicityTrackPtSumGt10"),
        config = cms.VPSet(
            cms.PSet(
                extractor = cms.PSet(
                    pluginType = cms.string("NumVertexExtractor"),
                    src = cms.InputTag('selectedPrimaryVerticesTrackPtSumGt10')
                ),
                branchName = cms.string('vtxMultiplicityTrackPtSumGt10'),
                binning = cms.PSet(
                    boundaries = cms.vdouble(4.5, 7.5, 10.5),
                    min = cms.double(-0.5),
                    max = cms.double(+20.5)
                )
            )
        )
    ),
    histManagers = cms.VPSet(
        diTauCandidateNSVfitHistManagerForMuTau,
    ),
    dqmDirectory_store = cms.string('vtxMultiplicityBinnedHistograms')
)

from TauAnalysis.Core.diTauCandidateZllHypothesisHistManager_cfi import *
diTauCandidateZmumuHypothesisHistManagerForMuTau = copy.deepcopy(ZllHypothesisHistManager)
diTauCandidateZmumuHypothesisHistManagerForMuTau.pluginName = cms.string('diTauCandidateZmumuHypothesisHistManagerForMuTau')
diTauCandidateZmumuHypothesisHistManagerForMuTau.pluginType = cms.string('ZllHypothesisMuTauHistManager')
diTauCandidateZmumuHypothesisHistManagerForMuTau.ZllHypothesisSource = cms.InputTag('muTauPairZmumuHypotheses')
diTauCandidateZmumuHypothesisHistManagerForMuTau.dqmDirectory_store = cms.string('DiTauCandidateZmumuHypothesisQuantities')

# import config for Z/gamma* --> mu+ mu- veto histogram manager
muPairHistManagerZmumuHypothesesByLooseIsolation = diTauCandidateHistManager.clone(
    pluginName = cms.string('muPairHistManagerZmumuHypothesesByLooseIsolation'),
    pluginType = cms.string('PATDiMuPairHistManager'),
    diTauCandidateSource = cms.InputTag('allDiMuPairZmumuHypothesesByLooseIsolation'),
    dqmDirectory_store = cms.string('DiMuonFromZmumuQuantities')
)
muPairHistManagerDYmumuHypotheses = diTauCandidateHistManager.clone(
    pluginName = cms.string('muPairHistManagerDYmumuHypotheses'),
    pluginType = cms.string('PATDiMuPairHistManager'),
    diTauCandidateSource = cms.InputTag('allDiMuPairDYmumuHypotheses'),
    dqmDirectory_store = cms.string('DiMuonFromDYmumuQuantities')
)

# import config for missing-Et histogram managers
from TauAnalysis.Core.caloMEtHistManager_cfi import *
caloMEtHistManager.leg1Source = cms.InputTag('selectedPatMuonsTrkIPcumulative')
caloMEtHistManager.leg2Source = cms.InputTag('selectedPatTausForMuTauElectronVetoCumulative')
from TauAnalysis.Core.pfMEtHistManager_cfi import *
pfMEtHistManager.leg1Source = cms.InputTag('selectedPatMuonsTrkIPcumulative')
pfMEtHistManager.leg2Source = cms.InputTag('selectedPatTausForMuTauElectronVetoCumulative')

# import config for central jet veto histogram manager
from TauAnalysis.Core.jetHistManager_cfi import *

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
    'L1_SingleMu7'
)

triggerHistManagerForMuTau.hltPaths = cms.vstring(
    'HLT_Mu9',
    'HLT_IsoMu9',
    'HLT_Mu11',
    'HLT_IsoMu13_v3',
    'HLT_IsoMu13_v4',
    'HLT_Mu15_v1',
    'HLT_IsoMu9_PFTau15_v2',
    'HLT_Mu11_PFTau15_v2'
)

# import config for event weight histogram manager
from TauAnalysis.Core.eventWeightHistManager_cfi import *

# import config for cut-flow histogram manager
from TauAnalysis.Core.cutFlowHistManager_cfi import *
cutFlowHistManagerGenMultiplicity = copy.deepcopy(cutFlowHistManager)
cutFlowHistManagerGenMultiplicity.pluginName = cms.string('cutFlowHistManagerGenMultiplicity')
cutFlowHistManagerGenMultiplicity.mode = cms.string("gen")
cutFlowHistManagerGenMultiplicity.dqmDirectory_store = cms.string('CutFlowStatisticsGenMultiplicity')
cutFlowHistManagerRecMultiplicity = copy.deepcopy(cutFlowHistManager)
cutFlowHistManagerRecMultiplicity.pluginName = cms.string('cutFlowHistManagerRecMultiplicity')
cutFlowHistManagerRecMultiplicity.mode = cms.string("rec")
cutFlowHistManagerRecMultiplicity.dqmDirectory_store = cms.string('CutFlowStatisticsRecMultiplicity')

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
modelBinnerForMuTauWrtGenTauLeptonPairAcc = copy.deepcopy(modelBinnerForMuTauGenTauLeptonPairAcc)
modelBinnerForMuTauWrtGenTauLeptonPairAcc.pluginName = cms.string('modelBinnerForMuTauWrtGenTauLeptonPairAcc')
modelBinnerForMuTauWrtGenTauLeptonPairAcc.srcGenFlag = cms.InputTag("isGenZtoMuTauWithinAcceptance")
modelBinnerForMuTauWrtGenTauLeptonPairAcc.srcRecFlag = cms.InputTag("isRecZtoMuTau")
modelBinnerForMuTauWrtGenTauLeptonPairAcc.dqmDirectory_store = cms.string('modelBinnerForMuTauWrtGenTauLeptonPairAcc')

modelBinnerForMuTauGenTauLeptonPairAcc3mZbins = copy.deepcopy(modelBinner3mZbins)
modelBinnerForMuTauGenTauLeptonPairAcc3mZbins.pluginName = cms.string('modelBinnerForMuTauGenTauLeptonPairAcc3mZbins')
modelBinnerForMuTauGenTauLeptonPairAcc3mZbins.binning.config[0].extractor.pluginType = cms.string("PATMuTauPairValExtractor")
modelBinnerForMuTauGenTauLeptonPairAcc3mZbins.binning.config[0].extractor.src = \
  diTauCandidateHistManagerForMuTau.diTauCandidateSource
modelBinnerForMuTauGenTauLeptonPairAcc3mZbins.srcGenFlag = cms.InputTag("isGenZtoMuTau")
modelBinnerForMuTauGenTauLeptonPairAcc3mZbins.srcRecFlag = cms.InputTag("isGenZtoMuTauWithinAcceptance")
modelBinnerForMuTauGenTauLeptonPairAcc3mZbins.dqmDirectory_store = cms.string('modelBinnerForMuTauGenTauLeptonPairAcc3mZbins')
modelBinnerForMuTauWrtGenTauLeptonPairAcc3mZbins = copy.deepcopy(modelBinnerForMuTauGenTauLeptonPairAcc3mZbins)
modelBinnerForMuTauWrtGenTauLeptonPairAcc3mZbins.pluginName = cms.string('modelBinnerForMuTauWrtGenTauLeptonPairAcc3mZbins')
modelBinnerForMuTauWrtGenTauLeptonPairAcc3mZbins.srcGenFlag = cms.InputTag("isGenZtoMuTauWithinAcceptance")
modelBinnerForMuTauWrtGenTauLeptonPairAcc3mZbins.srcRecFlag = cms.InputTag("isRecZtoMuTau")
modelBinnerForMuTauWrtGenTauLeptonPairAcc3mZbins.dqmDirectory_store = cms.string('modelBinnerForMuTauWrtGenTauLeptonPairAcc3mZbins')

# import config for binning results
# used to estimate systematic uncertainties
from TauAnalysis.Core.sysUncertaintyBinner_cfi import *
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *
sysUncertaintyNames = [ "CENTRAL_VALUE" ]
sysUncertaintyNames.extend(
    getSysUncertaintyNames(
        [ muonSystematics,
          tauSystematics,
          muTauPairSystematics,
          jetSystematics,
          theorySystematics ]
    )
)
sysUncertaintyBinnerForMuTauAcc = sysUncertaintyBinner.clone(
    pluginName = cms.string('sysUncertaintyBinnerForMuTauAcc'),
    binnerPlugins = cms.VPSet(
        modelBinnerForMuTauGenTauLeptonPairAcc,
        modelBinnerForMuTauWrtGenTauLeptonPairAcc,
        modelBinnerForMuTauGenTauLeptonPairAcc3mZbins,
        modelBinnerForMuTauWrtGenTauLeptonPairAcc3mZbins
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

# primary event vertex selection
evtSelPrimaryEventVertexForMuTau = cms.PSet(
    pluginName = cms.string('evtSelPrimaryEventVertexForMuTau'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('primaryEventVertexForMuTau')
)
evtSelPrimaryEventVertexQualityForMuTau = cms.PSet(
    pluginName = cms.string('evtSelPrimaryEventVertexQualityForMuTau'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('primaryEventVertexQualityForMuTau')
)
evtSelPrimaryEventVertexPositionForMuTau = cms.PSet(
    pluginName = cms.string('evtSelPrimaryEventVertexPositionForMuTau'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('primaryEventVertexPositionForMuTau')
)

# "final" selection of di-tau candidates for "OppositeSign" signal region
evtSelDiTauCandidateForMuTauZeroCharge = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForMuTauZeroCharge'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForMuTauZeroChargeCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForMuTauZeroChargeCut', 'individual'),
    systematics = cms.vstring(muTauPairSystematics.keys())
)

# "final" selection of di-tau candidates for "SameSign" background dominated control region
evtSelDiTauCandidateForMuTauNonZeroCharge = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForMuTauNonZeroCharge'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForMuTauNonZeroChargeCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForMuTauNonZeroChargeCut', 'individual'),
    systematics = cms.vstring(muTauPairSystematics.keys())
)

# veto events compatible with Z/gamma* --> mu+ mu- hypothesis
evtSelDiMuPairZmumuHypothesisVetoByLooseIsolation = cms.PSet(
    pluginName = cms.string('evtSelDiMuPairZmumuHypothesisVetoByLooseIsolation'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('diMuPairZmumuHypothesisVetoByLooseIsolation')
)
evtSelDiMuPairDYmumuHypothesisVeto = cms.PSet(
    pluginName = cms.string('evtSelDiMuPairDYmumuHypothesisVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('diMuPairDYmumuHypothesisVeto')
)

#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------

from TauAnalysis.RecoTools.patLeptonPFIsolationSelector_cfi import patMuonPFIsolationSelector as patMuonPFIsoExtractorTemplate
from TauAnalysis.RecoTools.patLeptonSelection_cff import selectedPatMuonsPFRelIso as patMuonPFIsoExtractorValues

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
    l1BitsToPrint = cms.vstring(
        'L1_SingleMu3',
        'L1_SingleMu5',
        'L1_SingleMu7'
    ),

    hltResultsSource = cms.InputTag('TriggerResults::HLT'),
    hltPathsToPrint = cms.vstring(
        'HLT_Mu9',
        'HLT_IsoMu9',
        'HLT_Mu11',
        'HLT_IsoMu13_v3',
        'HLT_IsoMu13_v4',
        'HLT_Mu15_v1',
        'HLT_IsoMu9_PFTau15_v2',
        'HLT_Mu11_PFTau15_v2'
    ),

    doGenInfo = cms.bool(True),
    genParticleSource = cms.InputTag('genParticles'),
    genJetSource = cms.InputTag('ak5GenJets'),
    genTauJetSource = cms.InputTag('tauGenJets'),
    genEventInfoSource = cms.InputTag('generator'),    

    electronSource = cms.InputTag('cleanPatElectrons'),
    muonSource = cms.InputTag('cleanPatMuons'),
    muonPFIsoExtractor = patMuonPFIsoExtractorTemplate.clone(
        chargedParticleIso = patMuonPFIsoExtractorValues.chargedParticleIso,
        neutralHadronIso   = patMuonPFIsoExtractorValues.neutralHadronIso,
        photonIso          = patMuonPFIsoExtractorValues.photonIso
    ),
    muonPFIsoCandSource = cms.InputTag('pfNoPileUp'),
    tauSource = cms.InputTag('selectedPatTausPt20Cumulative'),
    printTauIdEfficiencies = cms.bool(False),
    diTauCandidateSource = cms.InputTag('allMuTauPairs'),
	nSVfitAlgorithms = cms.vstring(
	    'psKine_MEt_logM_fit',
	    'psKine_MEt_logM_int'
	),
    muTauZmumuHypothesisSource = cms.InputTag('muTauPairZmumuHypotheses'),
    diMuZmumuHypothesisSource = cms.InputTag('allDiMuPairZmumuHypothesesByLooseIsolation'),
    pfCandidateSource = cms.InputTag('particleFlow'),
    vertexSource = cms.InputTag('selectedPrimaryVertexHighestPtTrackSumForMuTauLooseMuonIsolation'),
    beamSpotSource = cms.InputTag('offlineBeamSpot'),
    jetSource = cms.InputTag('patJets'),
    caloMEtSource = cms.InputTag('patMETs'),
    pfMEtSource = cms.InputTag('patPFMETs'),
    genMEtSource = cms.InputTag('genMetTrue'),

    dcsTag = cms.InputTag("scalersRawToDigi"),

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

muTauAnalysisSequenceOS = cms.VPSet(
    # fill histograms for full event sample
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'muonHistManager',
            'tauHistManager',
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
            'cutFlowHistManagerGenMultiplicity',
            'cutFlowHistManagerRecMultiplicity',
            'modelBinnerForMuTauGenTauLeptonPairAcc',
            'modelBinnerForMuTauWrtGenTauLeptonPairAcc',
			'modelBinnerForMuTauGenTauLeptonPairAcc3mZbins',
			'modelBinnerForMuTauWrtGenTauLeptonPairAcc3mZbins',
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
            'triggerHistManagerForMuTau'
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
            'tauHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsEta21Cumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonPt'),
        title = cms.string('Pt(Muon) > 15 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring('muonHistManager.muonSource = selectedPatMuonsPt15Cumulative')
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
            'tauHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsPt15Cumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauAntiOverlapWithMuonsVetoCumulative'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauEta'),
        title = cms.string('-2.3 < eta(Tau) < +2.3'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring(
           'muonHistManager.muonSource = selectedPatMuonsPt15Cumulative',
           'tauHistManager.tauSource = selectedPatTausForMuTauEta23Cumulative'
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
        filter = cms.string('evtSelMuonTrkIP'),
        title = cms.string('Muon Track IP'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager'
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
            'tauHistManager.tauSource = selectedPatTausForMuTauCaloMuonVetoCumulative'
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
            'diTauCandidateHistManagerForMuTau'
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
            'vertexHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsAntiOverlapVetoCumulative'
        )
    ),
    
    # primary event vertex selection
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexForMuTau'),
        title = cms.string('common Muon+Tau Vertex'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'vertexHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsAntiOverlapVetoCumulative',            
            'vertexHistManager.vertexSource = selectedPrimaryVertexForMuTau'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexQualityForMuTau'),
        title = cms.string('Vertex quality'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'vertexHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsAntiOverlapVetoCumulative',            
            'vertexHistManager.vertexSource = selectedPrimaryVertexQualityForMuTau'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexPositionForMuTau'),
        title = cms.string('-24 < zVertex < +24 cm'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'vertexHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsAntiOverlapVetoCumulative',            
            'vertexHistManager.vertexSource = selectedPrimaryVertexPositionForMuTau'
        )
    ),

    # selection of muon + tau-jet combinations (continued)
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForMuTauMt1MET'),
        title = cms.string('M_{T}(Muon-MET) < 50 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'vertexHistManager'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsMt1METcumulative',
            'vertexHistManager.vertexSource = selectedPrimaryVertexHighestPtTrackSumForMuTau'
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
            'muPairHistManagerZmumuHypothesesByLooseIsolation'           
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsPzetaDiffCumulative',
            'muPairHistManagerZmumuHypothesesByLooseIsolation.diTauCandidateSource = allDiMuPairZmumuHypothesesByLooseIsolation'
        )
    ),

    # veto events compatible with Z/gamma* --> mu+ mu- hypothesis
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
            'diTauCandidateNSVfitHistManagerForMuTau',
            'diTauCandidateNSVfitVtxMultiplicityBinGridHistManager',
            'muPairHistManagerZmumuHypothesesByLooseIsolation',
            'muPairHistManagerDYmumuHypotheses'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsPzetaDiffCumulative',
            'diTauCandidateHistManagerForMuTau.visMassHypothesisSource = muTauPairVisMassHypotheses',
            'muPairHistManagerZmumuHypothesesByLooseIsolation.diTauCandidateSource = selectedDiMuPairZmumuHypothesesByLooseIsolation'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelDiMuPairDYmumuHypothesisVeto'),
        title = cms.string('not Charge(isoMuon+nearbyMuon) = 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateNSVfitHistManagerForMuTau',
            'diTauCandidateNSVfitVtxMultiplicityBinGridHistManager',
            'muPairHistManagerDYmumuHypotheses'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsPzetaDiffCumulative',
            'muPairHistManagerDYmumuHypotheses.diTauCandidateSource = selectedDiMuPairDYmumuHypotheses'            
        )
    ),

    # apply "final" selection of "OS" di-tau candidates
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForMuTauZeroCharge'),
        title = cms.string('Charge(Muon+Tau) = 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'eventWeightHistManager',
            'muonHistManager',
            'genTauHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateHistManagerForMuTauPFtype1MET',
            'diTauCandidateNSVfitHistManagerForMuTau',
            'diTauCandidateNSVfitHistManagerForMuTauPFtype1MET',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau',
            'vertexHistManager',
            'muPairHistManagerZmumuHypothesesByLooseIsolation',
            'muPairHistManagerDYmumuHypotheses',
            'jetHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'particleMultiplicityHistManager',
            'triggerHistManagerForMuTau',
            'dataBinner',
	    'sysUncertaintyBinnerForMuTauEff'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsZeroChargeCumulative',
            'diTauCandidateHistManagerForMuTau.visMassHypothesisSource = muTauPairVisMassHypotheses',
            'diTauCandidateNSVfitHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsZeroChargeCumulative',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau.ZllHypothesisSource = muTauPairZmumuHypotheses',
            'vertexHistManager.vertexSource = selectedPrimaryVertexHighestPtTrackSumForMuTau',
            'muPairHistManagerZmumuHypothesesByLooseIsolation.diTauCandidateSource = selectedDiMuPairZmumuHypothesesByLooseIsolation',
            'muPairHistManagerDYmumuHypotheses.diTauCandidateSource = selectedDiMuPairDYmumuHypotheses'   
        )
    )
)

muTauAnalysisSequenceSS = cms.VPSet(
    cms.PSet(
        filter = cms.string('genPhaseSpaceCut'),
        title = cms.string('gen. Phase-Space'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTrigger'),
        title = cms.string('Muon Trigger'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDataQuality'),
        title = cms.string('Data quality'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),   
    cms.PSet(
        filter = cms.string('evtSelGlobalMuon'),
        title = cms.string('global Muon'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonEta'),
        title = cms.string('-2.1 < eta(Muon) < +2.1'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonPt'),
        title = cms.string('Pt(Muon) > 15 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauAntiOverlapWithMuonsVeto'),
        title = cms.string('Tau not overlapping w. Muon'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauEta'),
        title = cms.string('-2.3 < eta(Tau) < +2.3'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauPt'),
        title = cms.string('Pt(Tau) > 20 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonVbTfId'),
        title = cms.string('Muon VBTF id.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonPFRelIso'),
        title = cms.string('Muon iso.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelMuonTrkIP'),
        title = cms.string('Muon Track IP'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauLeadTrk'),
        title = cms.string('Tau lead. Track find.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauLeadTrkPt'),
        title = cms.string('Tau lead. Track Pt'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauTaNCdiscr'),
        title = cms.string('Tau TaNC discr.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauProng'),
        title = cms.string('Tau 1||3-Prong'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauCharge'),
        title = cms.string('Charge(Tau) = +/-1'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauMuonVeto'),
        title = cms.string('Tau mu-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauElectronVeto'),
        title = cms.string('Tau e-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForMuTauAntiOverlapVeto'),
        title = cms.string('dR(Muon-Tau) > 0.7'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexForMuTau'),
        title = cms.string('common Muon+Tau Vertex'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexQualityForMuTau'),
        title = cms.string('Vertex quality'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexPositionForMuTau'),
        title = cms.string('-24 < zVertex < +24 cm'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForMuTauMt1MET'),
        title = cms.string('M_{T}(Muon-MET) < 50 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForMuTauPzetaDiff'),
        title = cms.string('P_{#zeta} - 1.5*P_{#zeta}^{vis} > -20 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiMuPairZmumuHypothesisVetoByLooseIsolation'),
        title = cms.string('not Charge(isoMuon+isoMuon) = 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiMuPairDYmumuHypothesisVeto'),
        title = cms.string('not Charge(isoMuon+nearbyMuon) = 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForMuTauNonZeroCharge'),
        title = cms.string('Charge(Muon+Tau) != 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'muonHistManager',
            'genTauHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForMuTau',
            'diTauCandidateNSVfitHistManagerForMuTau',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau',
            'vertexHistManager',
            'muPairHistManagerZmumuHypothesesByLooseIsolation',
            'muPairHistManagerDYmumuHypotheses',
            'jetHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'triggerHistManagerForMuTau'
        ),
        replace = cms.vstring(
            'muonHistManager.muonSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForMuTauElectronVetoCumulative',
            'diTauCandidateHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsNonZeroChargeCumulative',
            'diTauCandidateHistManagerForMuTau.visMassHypothesisSource = muTauPairVisMassHypotheses',            
            'diTauCandidateNSVfitHistManagerForMuTau.diTauCandidateSource = selectedMuTauPairsNonZeroChargeCumulative',
            'diTauCandidateZmumuHypothesisHistManagerForMuTau.ZllHypothesisSource = muTauPairZmumuHypotheses',
            'vertexHistManager.vertexSource = selectedPrimaryVertexHighestPtTrackSumForMuTau',
            'muPairHistManagerZmumuHypothesesByLooseIsolation.diTauCandidateSource = selectedDiMuPairZmumuHypothesesByLooseIsolation',
            'muPairHistManagerDYmumuHypotheses.diTauCandidateSource = selectedDiMuPairDYmumuHypotheses'   
        )
    )
)
