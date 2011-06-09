import FWCore.ParameterSet.Config as cms
import copy

# import config for histogram manager filling information about phase-space simulated in Monte Carlo sample
from TauAnalysis.Core.genPhaseSpaceEventInfoHistManager_cfi import *

# import config for event weight histogram manager
from TauAnalysis.Core.eventWeightHistManager_cfi import *

# import config for electron histogram manager
from TauAnalysis.Core.electronHistManager_cfi import *

# import config for tau histogram manager
from TauAnalysis.Core.pftauHistManager_cfi import *
tauHistManager.useHPSpTaNCalgorithm = cms.bool(True)

# import config for di-tau histogram manager
from TauAnalysis.Core.diTauCandidateHistManager_cfi import *
diTauCandidateHistManagerForElecTau = copy.deepcopy(diTauCandidateHistManager)
diTauCandidateHistManagerForElecTau.pluginName = cms.string('diTauCandidateHistManagerForElecTau')
diTauCandidateHistManagerForElecTau.pluginType = cms.string('PATElecTauPairHistManager')
diTauCandidateHistManagerForElecTau.diTauCandidateSource = cms.InputTag('allElecTauPairs')
diTauCandidateHistManagerForElecTau.visMassHypothesisSource = cms.InputTag('elecTauPairVisMassHypotheses')

from TauAnalysis.Core.diTauCandidateNSVfitHistManager_cfi import *
diTauCandidateNSVfitHistManagerForElecTau = copy.deepcopy(diTauCandidateNSVfitHistManager)
diTauCandidateNSVfitHistManagerForElecTau.pluginName = cms.string('diTauCandidateNSVfitHistManagerForElecTau')
diTauCandidateNSVfitHistManagerForElecTau.pluginType = cms.string('PATElecTauPairNSVfitHistManager')
diTauCandidateNSVfitHistManagerForElecTau.diTauCandidateSource = cms.InputTag('allElecTauPairs')
diTauCandidateNSVfitHistManagerForElecTau.nSVfitEventHypotheses = cms.PSet(
    psKine_MEt_logM_fit = cms.string('psKine_MEt_logM_fit'),
    psKine_MEt_logM_int = cms.string('psKine_MEt_logM_int')
)

from TauAnalysis.Core.diTauCandidateZllHypothesisHistManager_cfi import *
diTauCandidateZeeHypothesisHistManagerForElecTau = copy.deepcopy(ZllHypothesisHistManager)
diTauCandidateZeeHypothesisHistManagerForElecTau.pluginName = cms.string('diTauCandidateZeeHypothesisHistManagerForElecTau')
diTauCandidateZeeHypothesisHistManagerForElecTau.pluginType = cms.string('ZllHypothesisElecTauHistManager')
diTauCandidateZeeHypothesisHistManagerForElecTau.ZllHypothesisSource = cms.InputTag('elecTauPairZeeHypotheses')
diTauCandidateZeeHypothesisHistManagerForElecTau.dqmDirectory_store = cms.string('DiTauCandidateZeeHypothesisQuantities')

# import config for Zee veto histogram manager
elecPairHistManagerByLooseIsolation = diTauCandidateHistManager.clone(
    pluginName = cms.string('elecPairHistManagerByLooseIsolation'),
    pluginType = cms.string('PATDiElecPairHistManager'),
    diTauCandidateSource = cms.InputTag('allDiElecPairZeeHypothesesByLooseIsolation'),
    dqmDirectory_store = cms.string('DiElecZeeHypothesisByLooseIsolationQuantities')
)
# import config for central jet veto histogram manager
from TauAnalysis.Core.jetHistManager_cfi import *

# import config for missing-Et histogram managers
from TauAnalysis.Core.caloMEtHistManager_cfi import *
caloMEtHistManager.leg1Source = cms.InputTag('selectedPatElectronsForElecTauConversionVetoCumulative')
caloMEtHistManager.leg2Source = cms.InputTag('selectedPatTausForElecTauMuonVetoCumulative')
from TauAnalysis.Core.pfMEtHistManager_cfi import *
pfMEtHistManager.leg1Source = cms.InputTag('selectedPatElectronsForElecTauConversionVetoCumulative')
pfMEtHistManager.leg2Source = cms.InputTag('selectedPatTausForElecTauMuonVetoCumulative')

# import config for central jet veto histogram manager
from TauAnalysis.Core.jetHistManager_cfi import *

# import config for particle multiplicity histogram manager
from TauAnalysis.Core.particleMultiplicityHistManager_cfi import *
particleMultiplicityHistManager.histograms[1].src = cms.InputTag("selectedPatElectronsForElecTauIdIndividual")

# import config for primary event vertex histogram manager
from TauAnalysis.Core.vertexHistManager_cfi import *

# import config for L1 & HLT histogram manager
from TauAnalysis.Core.triggerHistManager_cfi import *
triggerHistManagerForElecTau = copy.deepcopy(triggerHistManager)
triggerHistManagerForElecTau.pluginName = cms.string('triggerHistManagerForElecTau')
triggerHistManagerForElecTau.l1Bits = cms.vstring(
    'L1_SingleEG5',
    'L1_SingleEG8',
    'L1_SingleEG10',
    'L1_SingleEG12',
    'L1_SingleEG15',
    'L1_SingleIsoEG5',
    'L1_SingleIsoEG8',
    'L1_SingleIsoEG10',
    'L1_SingleIsoEG12',
    'L1_SingleIsoEG15'
)

triggerHistManagerForElecTau.hltPaths = cms.vstring(    
    'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v1',
    'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v2',
    'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v4',
    'HLT_IsoEle12_PFTau15_v3',
    'HLT_Ele12_SW_TighterEleId_L1R_v2'
)

# import config for binning results
# used to estimate acceptance of event selection
from TauAnalysis.Core.modelBinner_cfi import *
#modelBinnerForElecTauGenTauLeptonPairAcc = copy.deepcopy(modelBinner)
#modelBinnerForElecTauGenTauLeptonPairAcc.pluginName = cms.string('modelBinnerForElecTauGenTauLeptonPairAcc')
#modelBinnerForElecTauGenTauLeptonPairAcc.srcGenFlag = cms.InputTag("isGenZtoElecTau")
#modelBinnerForElecTauGenTauLeptonPairAcc.srcRecFlag = cms.InputTag("isGenZtoElecTauWithinAcceptance")
#modelBinnerForElecTauGenTauLeptonPairAcc.dqmDirectory_store = cms.string('modelBinnerForElecTauGenTauLeptonPairAcc')
#modelBinnerForElecTauWrtGenTauLeptonPairAcc = copy.deepcopy(modelBinner)
#modelBinnerForElecTauWrtGenTauLeptonPairAcc.pluginName = cms.string('modelBinnerForElecTauWrtGenTauLeptonPairAcc')
#modelBinnerForElecTauWrtGenTauLeptonPairAcc.srcGenFlag = cms.InputTag("isGenZtoElecTauWithinAcceptance")
#modelBinnerForElecTauWrtGenTauLeptonPairAcc.srcRecFlag = cms.InputTag("isRecZtoElecTau")
#modelBinnerForElecTauWrtGenTauLeptonPairAcc.dqmDirectory_store = cms.string('modelBinnerForElecTauWrtGenTauLeptonPairAcc')

# import config for binning results
# used for keeping track of number of events passing all selection criteria
from TauAnalysis.Core.dataBinner_cfi import *

# import config for binning results
# used to estimate systematic uncertainties
from TauAnalysis.Core.sysUncertaintyBinner_cfi import *
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *
sysUncertaintyNames = [ "CENTRAL_VALUE" ]
sysUncertaintyNames.extend(
    getSysUncertaintyNames(
        [ electronSystematics,
          tauSystematics,
          elecTauPairSystematics,
          jetSystematics,
          theorySystematics ]
    )
)
sysUncertaintyHistManagerForElecTau = cms.PSet(
    pluginName = cms.string('sysUncertaintyHistManagerForElecTau'),
    pluginType = cms.string('SysUncertaintyHistManager'),
    histManagers = cms.VPSet(
        cms.PSet(
            config = diTauCandidateHistManagerForElecTau,
            systematics = cms.PSet(
                diTauCandidateSource = getSysUncertaintyParameterSets(
                    [ elecTauPairSystematics ]
                )
            )
        ),
        cms.PSet(
            config = diTauCandidateNSVfitHistManagerForElecTau,
            systematics = cms.PSet(
                diTauCandidateSource = getSysUncertaintyParameterSets(
                    [ elecTauPairSystematics ]
                )
            )
        )
    ),
    dqmDirectory_store = cms.string('sysUncertaintyHistManagerResults')
)
#sysUncertaintyBinnerForElecTauAcc = sysUncertaintyBinner.clone(
#    pluginName = cms.string('sysUncertaintyBinnerForElecTauAcc'),
#    binnerPlugins = cms.VPSet(
#        modelBinnerForElecTauGenTauLeptonPairAcc,
#        modelBinnerForElecTauWrtGenTauLeptonPairAcc
#    ),
#    systematics = cms.vstring(sysUncertaintyNames)
#)
sysUncertaintyBinnerForElecTauEff = sysUncertaintyBinner.clone(
    pluginName = cms.string('sysUncertaintyBinnerForElecTauEff'),
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

# generator level selection of Z --> e + tau-jet events
# passing basic acceptance and kinematic cuts
# (NOTE: to be used for efficiency studies only !!)
#genElectronCut = cms.PSet(
#    pluginName = cms.string('genElectronCut'),
#    pluginType = cms.string('BoolEventSelector'),
#    src = cms.InputTag('isGenZtoElecTau')
#)
#genTauCut = cms.PSet(
# (NOTE: to be used in case of Monte Carlo samples
#        overlapping in simulated phase-space only !!)
evtSelGenPhaseSpace = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('genPhaseSpaceCut')
)

# generator level selection of Z --> e + tau-jet events
# passing basic acceptance and kinematic cuts
# (NOTE: to be used for efficiency studies only !!)
#genElectronCut = cms.PSet(
#    pluginName = cms.string('genElectronCut'),
#    pluginType = cms.string('BoolEventSelector'),
#    src = cms.InputTag('isGenZtoElecTau')
#)
#genTauCut = cms.PSet(
#    pluginName = cms.string('genTauCut'),
#    pluginType = cms.string('PATCandViewMinEventSelector'),
#    src = cms.InputTag('selectedGenTauDecaysToHadronsPt20Cumulative'),
#    minNumber = cms.uint32(1)
#)

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

# electron acceptance cuts
evtSelElectronId = cms.PSet(
    pluginName = cms.string('evtSelElectronId'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronIdCut', 'cumulative'),
    src_individual = cms.InputTag('electronIdCut', 'individual'),
	systematics = cms.vstring(electronSystematics.keys())
)
evtSelElectronAntiCrack = cms.PSet(
    pluginName = cms.string('evtSelElectronAntiCrack'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronAntiCrackCut', 'cumulative'),
    src_individual = cms.InputTag('electronAntiCrackCut', 'individual'),
	systematics = cms.vstring(electronSystematics.keys())
)
evtSelElectronEta = cms.PSet(
    pluginName = cms.string('evtSelElectronEta'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronEtaCut', 'cumulative'),
    src_individual = cms.InputTag('electronEtaCut', 'individual'),
	systematics = cms.vstring(electronSystematics.keys())
)
evtSelElectronPt = cms.PSet(
    pluginName = cms.string('evtSelElectronPt'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronPtCut', 'cumulative'),
    src_individual = cms.InputTag('electronPtCut', 'individual'),
	systematics = cms.vstring(electronSystematics.keys())
)

# tau acceptance cuts
evtSelTauAntiOverlapWithElectronsVeto = cms.PSet(
    pluginName = cms.string('evtSelTauAntiOverlapWithElectronsVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauAntiOverlapWithElectronsVeto', 'cumulative'),
    src_individual = cms.InputTag('tauAntiOverlapWithElectronsVeto', 'individual'),
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

evtSelElectronIso = cms.PSet(
    pluginName = cms.string('evtSelElectronIso'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronIsoCut', 'cumulative'),
    src_individual = cms.InputTag('electronIsoCut', 'individual'),
	systematics = cms.vstring(electronSystematics.keys())
)
evtSelElectronConversionVeto = cms.PSet(
    pluginName = cms.string('evtSelElectronConversionVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronConversionVeto', 'cumulative'),
    src_individual = cms.InputTag('electronConversionVeto', 'individual'),
	systematics = cms.vstring(electronSystematics.keys())
)
evtSelElectronTrkIP = cms.PSet(
    pluginName = cms.string('evtSelElectronTrkIP'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('electronTrkIPcut', 'cumulative'),
    src_individual = cms.InputTag('electronTrkIPcut', 'individual'),
	systematics = cms.vstring(electronSystematics.keys())
)

# tau candidate (id.) selection
evtSelTauDecayModeFinding = cms.PSet(
    pluginName = cms.string('evtSelTauDecayModeFinding'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauDecayModeFindingCut', 'cumulative'),
    src_individual = cms.InputTag('tauDecayModeFindingCut', 'individual'),
	systematics = cms.vstring(tauSystematics.keys())
)
evtSelTauLeadTrkPt = cms.PSet(
    pluginName = cms.string('evtSelTauLeadTrkPt'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauLeadTrkPtCut', 'cumulative'),
    src_individual = cms.InputTag('tauLeadTrkPtCut', 'individual'),
	systematics = cms.vstring(tauSystematics.keys())
)
evtSelTauIso = cms.PSet(
    pluginName = cms.string('evtSelTauIso'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauIsoCut', 'cumulative'),
    src_individual = cms.InputTag('tauIsoCut', 'individual'),
	systematics = cms.vstring(tauSystematics.keys())
)
evtSelTauElectronVeto = cms.PSet(
    pluginName = cms.string('evtSelTauElectronVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauElectronVeto', 'cumulative'),
    src_individual = cms.InputTag('tauElectronVeto', 'individual'),
	systematics = cms.vstring(tauSystematics.keys())
)
evtSelTauEcalCrackVeto = cms.PSet(
    pluginName = cms.string('evtSelTauEcalCrackVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauEcalCrackVeto', 'cumulative'),
    src_individual = cms.InputTag('tauEcalCrackVeto', 'individual'),
	systematics = cms.vstring(tauSystematics.keys())
)
evtSelTauMuonVeto = cms.PSet(
    pluginName = cms.string('evtSelTauMuonVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('tauMuonVeto', 'cumulative'),
    src_individual = cms.InputTag('tauMuonVeto', 'individual'),
	systematics = cms.vstring(tauSystematics.keys())
)

# di-tau candidate selection
evtSelDiTauCandidateForElecTauAntiOverlapVeto = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForElecTauAntiOverlapVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForElecTauAntiOverlapVeto', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForElecTauAntiOverlapVeto', 'individual'),
	systematics = cms.vstring(elecTauPairSystematics.keys())
)
evtSelDiTauCandidateForElecTauMt1MET = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForElecTauMt1MET'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForElecTauMt1METCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForElecTauMt1METCut', 'individual'),
	systematics = cms.vstring(elecTauPairSystematics.keys())
)
evtSelDiTauCandidateForElecTauPzetaDiff = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForElecTauPzetaDiff'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForElecTauPzetaDiffCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForElecTauPzetaDiffCut', 'individual'),
	systematics = cms.vstring(elecTauPairSystematics.keys())
)

# primary event vertex selection
evtSelPrimaryEventVertexForElecTau = cms.PSet(
    pluginName = cms.string('evtSelPrimaryEventVertexForElecTau'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('primaryEventVertexForElecTau')
)
evtSelPrimaryEventVertexQualityForElecTau = cms.PSet(
    pluginName = cms.string('evtSelPrimaryEventVertexQualityForElecTau'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('primaryEventVertexQualityForElecTau')
)
evtSelPrimaryEventVertexPositionForElecTau = cms.PSet(
    pluginName = cms.string('evtSelPrimaryEventVertexPositionForElecTau'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('primaryEventVertexPositionForElecTau')
)

#  final selection for opposite-sign (OS) and same-sign (SS) di-tau pairs
evtSelDiTauCandidateForElecTauZeroCharge = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForElecTauZeroCharge'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForElecTauZeroChargeCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForElecTauZeroChargeCut', 'individual'),
	systematics = cms.vstring(elecTauPairSystematics.keys())
)
evtSelDiTauCandidateForElecTauNonZeroCharge = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForElecTauNonZeroCharge'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForElecTauNonZeroChargeCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForElecTauNonZeroChargeCut', 'individual'),
	systematics = cms.vstring(elecTauPairSystematics.keys())
)

# veto events compatible with Z --> e+ e- hypothesis
evtSelDiElecPairZeeHypothesisVetoByLooseIsolation = cms.PSet(
    pluginName = cms.string('evtSelDiElecPairZeeHypothesisVetoByLooseIsolation'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('diElecPairZeeHypothesisVetoByLooseIsolation')
)

#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------

elecTauEventDump = cms.PSet(
    pluginName = cms.string('elecTauEventDump'),
    pluginType = cms.string('ElecTauEventDump'),
    ##  trigger info  ##
    # L1 trigger bits not contained in AOD;
    # in order to process Monte Carlo samples produced by FastSimulation,
    # disable histogram filling for now
    l1GtReadoutRecordSource = cms.InputTag(''),
    l1GtObjectMapRecordSource = cms.InputTag(''),
    #l1BitsToPrint = cms.vstring('L1_SingleEG5', 'L1_SingleEG8', 'L1_SingleEG10', 'L1_SingleEG12', 'L1_SingleEG15',
    #                            'L1_SingleIsoEG5', 'L1_SingleIsoEG8', 'L1_SingleIsoEG10', 'L1_SingleIsoEG12', 'L1_SingleIsoEG15'),
    
    hltResultsSource = cms.InputTag('TriggerResults::HLT'),
	hltPathsToPrint = cms.vstring(
        'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v1',
        'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v2',
        'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v4',
        'HLT_IsoEle12_PFTau15_v3',
        'HLT_Ele12_SW_TighterEleId_L1R_v2'
	),
    ##  gen info  ##
    doGenInfo = cms.bool(True),
    genParticleSource = cms.InputTag('genParticles'),
    genJetSource = cms.InputTag('ak5GenJets'),
    genTauJetSource = cms.InputTag('tauGenJets'),
    genMEtSource = cms.InputTag('genMetTrue'),
    genEventInfoSource = cms.InputTag('generator'),
    ##  reco candidate info  ##
    electronSource = cms.InputTag('cleanPatElectrons'),
    tauSource = cms.InputTag('selectedPatTausForElecTauPtCumulative'),
    diTauCandidateSource = cms.InputTag('allElecTauPairs'),
    ##  PF info  ##
    # un-comment pfCandidateSource to print PF isolation candidate info
    pfCandidateSource = cms.InputTag('pfNoPileUp'),
    # un-comment to print PF-isolation sums
    pfCombIsoExtractor = cms.PSet(
        chargedHadronIso = selectedPatElectronsForElecTauIso.chargedHadronIso,
        neutralHadronIso = selectedPatElectronsForElecTauIso.neutralHadronIso,
        photonIso = selectedPatElectronsForElecTauIso.photonIso
    ),
    pfChargedHadronIsoExtractor = cms.PSet(
        chargedHadronIso = selectedPatElectronsForElecTauIso.chargedHadronIso
    ),
    pfNeutralHadronIsoExtractor = cms.PSet(
        neutralHadronIso = selectedPatElectronsForElecTauIso.neutralHadronIso
    ),
    pfPhotonIsoExtractor = cms.PSet(
        photonIso = selectedPatElectronsForElecTauIso.photonIso
    ),
    ##  di-tau candidate info  ##
    nSVfitAlgorithms = cms.vstring(
        'psKine_MEt_logM_fit',
        'psKine_MEt_logM_int'
    ),
    elecTauZeeHypothesisSource = cms.InputTag('elecTauPairZeeHypotheses'), 
    diElecZeeHypothesisSource = cms.InputTag('allDiElecPairZeeHypothesesByLooseIsolation'), 
    vertexSource = cms.InputTag('selectedPrimaryVertexHighestPtTrackSum'),
    beamSpotSource = cms.InputTag('offlineBeamSpot'),
    ##  jet/MET info  ##
    #jetSource = cms.InputTag('patJets'),
    caloMEtSource = cms.InputTag('patMETs'),
    pfMEtSource = cms.InputTag('patPFMETs'),
    ##  output info  ## 
    output = cms.string("std::cout"),
	triggerConditions = cms.vstring("evtSelDiTauCandidateForElecTauZeroCharge: passed_cumulative"),

    dcsTag = cms.InputTag("scalersRawToDigi")
)

#--------------------------------------------------------------------------------
# define analysis sequences
# (ordered list of event selection criteria and histogram filling)
#--------------------------------------------------------------------------------

#
#  Opposite-sign sequence
#
elecTauAnalysisSequenceOS = cms.VPSet(
    # fill histograms for full event sample
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'electronHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForElecTau'
        )
    ),

    # generator level phase-space selection
    # (NOTE: (1) to be used in case of Monte Carlo samples
    #            overlapping in simulated phase-space only !!
    #        (2) genPhaseSpaceCut needs to be **always** the first entry in the list of cuts
    #           - otherwise the script submitToBatch.csh for submission of cmsRun jobs
    #            to the CERN batch system will not work !!)
    cms.PSet(
        filter = cms.string('genPhaseSpaceCut'),
        title = cms.string('gen. Phase-Space'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'triggerHistManagerForElecTau'
        )
    ),

    # generator level selection of Z --> e + tau-jet events
    # passing basic acceptance and kinematic cuts
    # (NOTE: to be used for efficiency studies only !!)
    #cms.PSet(
    #    filter = cms.string('genElectronCut'),
    #    title = cms.string('gen. Electron'),
    #    saveRunLumiSectionEventNumbers = cms.vstring('')
    #),
    #cms.PSet(
    #    filter = cms.string('genTauCut'),
    #    title = cms.string('gen. Tau'),
    #    saveRunLumiSectionEventNumbers = cms.vstring('')
    #),
    #cms.PSet(
    #    analyzers = cms.vstring(
    #        'genPhaseSpaceEventInfoHistManager'
    #    )
    #),
  
    # trigger selection
    cms.PSet(
        filter = cms.string('evtSelTrigger'),
        title = cms.string('Trigger'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    
	# data quality control
	cms.PSet(
        filter = cms.string('evtSelDataQuality'),
        title = cms.string('Data Quality'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForElecTau'
        )
    ),

    # electron acceptance cuts
	cms.PSet(
        filter = cms.string('evtSelElectronId'),
        title = cms.string('Electron ID'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauIdCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelElectronAntiCrack'),
        title = cms.string('Electron crack-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauAntiCrackCutCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelElectronEta'),
        title = cms.string('-2.1 < eta(Electron) < +2.1'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauEtaCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelElectronPt'),
        title = cms.string('Pt(Electron) > 18 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
            'tauHistManager',
            'vertexHistManager'
           
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauPtCumulative')
    ),

    # tau acceptance cuts
    cms.PSet(
        filter = cms.string('evtSelTauAntiOverlapWithElectronsVeto'),
        title = cms.string('Tau not overlapping with Elec.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauPtCumulative',
                              'tauHistManager.tauSource = selectedPatTausForElecTauAntiOverlapWithElectronsVetoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauEta'),
        title = cms.string('-2.3 < eta(Tau) < +2.3'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauPtCumulative',
                              'tauHistManager.tauSource = selectedPatTausForElecTauEtaCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauPt'),
        title = cms.string('Pt(Tau) > 20 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForElecTau'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauPtCumulative',
			#'electronHistManager.makeIsoPtConeSizeDepHistograms = True',
                              'tauHistManager.tauSource = selectedPatTausForElecTauPtCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelElectronIso'),
        title = cms.string('Electron PF iso. (rel.)'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauIsoCumulative',
			'electronHistManager.makeConversionHistograms = True')
    ),
    cms.PSet(
        filter = cms.string('evtSelElectronConversionVeto'),
        title = cms.string('Electron Track conv. veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauConversionVetoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelElectronTrkIP'),
        title = cms.string('Electron Track IP'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForElecTau'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative')
    ),
  
    # selection of tau-jet candidate (id.)
    # produced in hadronic tau decay
    cms.PSet(
        filter = cms.string('evtSelTauDecayModeFinding'),
        title = cms.string('Tau decay mode find.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
    		'tauHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
                              'tauHistManager.tauSource = selectedPatTausForElecTauDecayModeFindingCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauLeadTrkPt'),
        title = cms.string('Tau lead. Track Pt'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
                              'tauHistManager.tauSource = selectedPatTausForElecTauLeadTrkPtCumulative',
							  'tauHistManager.makeIsoPtConeSizeDepHistograms = True'
							  )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauIso'),
        title = cms.string('Tau ID (HPS loose)'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
                              'tauHistManager.tauSource = selectedPatTausForElecTauIsoCumulative',
							  'tauHistManager.makeIsoPtConeSizeDepHistograms = True'
							  )
    ),
    cms.PSet(
        filter = cms.string('evtSelTauElectronVeto'),
        title = cms.string('Tau e-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
                              'tauHistManager.tauSource = selectedPatTausForElecTauElectronVetoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauEcalCrackVeto'),
        title = cms.string('Tau ECAL crack-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
            'tauHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
                              'tauHistManager.tauSource = selectedPatTausForElecTauEcalCrackVetoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauMuonVeto'),
        title = cms.string('Tau mu-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForElecTau',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForElecTau'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
							  'diTauCandidateHistManagerForElecTau.diTauCandidateSource = allElecTauPairs',
							  'diTauCandidateHistManagerForElecTau.visMassHypothesisSource = elecTauPairVisMassHypotheses',
                              'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative')
    ),
    
    #selection of electron + tau-jet combinations
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForElecTauAntiOverlapVeto'),
        title = cms.string('dR(Electron-Tau) > 0.5'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
            'tauHistManager',
            'vertexHistManager',
            'diTauCandidateHistManagerForElecTau'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
                              'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
							  'diTauCandidateHistManagerForElecTau.visMassHypothesisSource = elecTauPairVisMassHypotheses',
                              'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsAntiOverlapVetoCumulative')
    ),
    
    # primary event vertex selection
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexForElecTau'),
        title = cms.string('Common Electron+Tau Vertex'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
            'tauHistManager',
            'vertexHistManager',
            'diTauCandidateHistManagerForElecTau'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
                              'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
                              'vertexHistManager.vertexSource = selectedPrimaryVertexForElecTau',
                              'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsAntiOverlapVetoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexQualityForElecTau'),
        title = cms.string('p(chi2Vertex) > 0.01'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
            'tauHistManager',
            'vertexHistManager',
            'diTauCandidateHistManagerForElecTau'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
                              'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
                              'vertexHistManager.vertexSource = selectedPrimaryVertexQualityForElecTau',
                              'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsAntiOverlapVetoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexPositionForElecTau'),
        title = cms.string('-25 < zVertex < +25 cm'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
            'tauHistManager',
            'vertexHistManager',
            'diTauCandidateHistManagerForElecTau'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
                              'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
                              'vertexHistManager.vertexSource = selectedPrimaryVertexPositionForElecTau',
                              'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsAntiOverlapVetoCumulative')
    ),
   
    # selection of elec + tau combination (continued)
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForElecTauMt1MET'),
        title = cms.string('M_{T}(Electron-MET) < 40 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForElecTau'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
                              'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
							  'diTauCandidateHistManagerForElecTau.visMassHypothesisSource = elecTauPairVisMassHypotheses',
                              'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsMt1METcumulative')  
    ),
	cms.PSet(
	    filter = cms.string('evtSelDiTauCandidateForElecTauPzetaDiff'),
	    title = cms.string('P_{#zeta} - 1.5*P_{#zeta}^{vis} > -20 GeV'),
	    saveRunLumiSectionEventNumbers = cms.vstring('')
	),
	cms.PSet(
	    analyzers = cms.vstring(
			'electronHistManager',
			'tauHistManager',
			'diTauCandidateHistManagerForElecTau',
			'diTauCandidateZeeHypothesisHistManagerForElecTau',
			'elecPairHistManagerByLooseIsolation'
	    ),
	    replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
	                         'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
							  'elecPairHistManagerByLooseIsolation.diTauCandidateSource = selectedDiElecPairZeeHypothesesByLooseIsolation',
							  'diTauCandidateHistManagerForElecTau.visMassHypothesisSource = elecTauPairVisMassHypotheses',
	                         'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsPzetaDiffCumulative'
		)
	),
	# veto events compatible with Z --> e+ e- hypothesis
    cms.PSet(
        filter = cms.string('evtSelDiElecPairZeeHypothesisVetoByLooseIsolation'),
        title = cms.string('charge(isoElectron + looseIsoElectron) != 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForElecTau',
            'diTauCandidateZeeHypothesisHistManagerForElecTau',
            'elecPairHistManagerByLooseIsolation'
        ),
		replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
                              'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
							  'diTauCandidateHistManagerForElecTau.visMassHypothesisSource = elecTauPairVisMassHypotheses',
							  'elecPairHistManagerByLooseIsolation.diTauCandidateSource = selectedDiElecPairZeeHypothesesByLooseIsolation',
							  'diTauCandidateNSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsPzetaDiffCumulative',
                              'diTauCandidateZeeHypothesisHistManagerForElecTau.ZllHypothesisSource = selectedElecTauPairZeeHypotheses'
		)
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForElecTauZeroCharge'),
        title = cms.string('Charge(Electron+Tau) = 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = cms.vstring(
			'genPhaseSpaceEventInfoHistManager',
			'triggerHistManagerForElecTau',
			'vertexHistManager',
            'electronHistManager',
            'tauHistManager',
			'pfMEtHistManager',
			'caloMEtHistManager',
			'jetHistManager',
			'elecPairHistManagerByLooseIsolation',
			'diTauCandidateZeeHypothesisHistManagerForElecTau',
			'diTauCandidateNSVfitHistManagerForElecTau',
			'dataBinner',
			'sysUncertaintyBinnerForElecTauEff',
            'diTauCandidateHistManagerForElecTau'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
                              'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
							  'elecPairHistManagerByLooseIsolation.diTauCandidateSource = selectedDiElecPairZeeHypothesesByLooseIsolation',
							  'diTauCandidateNSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsPzetaDiffCumulative',
                              'diTauCandidateZeeHypothesisHistManagerForElecTau.ZllHypothesisSource = selectedElecTauPairZeeHypotheses',
                              'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsZeroChargeCumulative',
                              'diTauCandidateHistManagerForElecTau.visMassHypothesisSource = elecTauPairVisMassHypotheses'
		)
	)
)


elecTauAnalysisSequenceSS = cms.VPSet(
    cms.PSet(
        filter = cms.string('genPhaseSpaceCut'),
        title = cms.string('gen. Phase-Space'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTrigger'),
        title = cms.string('Electron Trigger'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDataQuality'),
        title = cms.string('Data Quality'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelElectronId'),
        title = cms.string('Electron ID'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelElectronAntiCrack'),
        title = cms.string('Electron crack veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelElectronEta'),
        title = cms.string('-2.1 < eta(Electron) < +2.1'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelElectronPt'),
        title = cms.string('Pt(Electron) > 18 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauAntiOverlapWithElectronsVeto'),
        title = cms.string('Tau not overlapping w. Electron'),
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
        filter = cms.string('evtSelElectronIso'),
        title = cms.string('Electron iso.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelElectronConversionVeto'),
        title = cms.string('Electron conversion veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelElectronTrkIP'),
        title = cms.string('Electron Track IP'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauDecayModeFinding'),
        title = cms.string('Tau decay mode find.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauLeadTrkPt'),
        title = cms.string('Tau lead. Track Pt'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauIso'),
        title = cms.string('Tau ID (HPS loose)'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauElectronVeto'),
        title = cms.string('Tau e-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
	cms.PSet(
		filter = cms.string('evtSelTauEcalCrackVeto'),
		title = cms.string('Tau ECAL crack-Veto'),
		saveRunLumiSectionEventNumbers = cms.vstring('')
	),
    cms.PSet(
        filter = cms.string('evtSelTauMuonVeto'),
        title = cms.string('Tau mu-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForElecTauAntiOverlapVeto'),
        title = cms.string('dR(Electron-Tau) > 0.5'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexForElecTau'),
        title = cms.string('Common Electron+Tau Vertex'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexQualityForElecTau'),
        title = cms.string('Vertex quality'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexPositionForElecTau'),
        title = cms.string('-24 < zVertex < +24 cm'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForElecTauMt1MET'),
        title = cms.string('M_{T}(Electron-MET) < 40 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
	cms.PSet(
	    filter = cms.string('evtSelDiTauCandidateForElecTauPzetaDiff'),
	    title = cms.string('P_{#zeta} - 1.5*P_{#zeta}^{vis} > -1000 GeV'),
	    saveRunLumiSectionEventNumbers = cms.vstring('')
	),
    cms.PSet(
        filter = cms.string('evtSelDiElecPairZeeHypothesisVetoByLooseIsolation'),
        title = cms.string('charge(isoElectron+isoElectron) != 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForElecTauNonZeroCharge'),
        title = cms.string('Charge(Electron+Tau) != 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForElecTau',
            'diTauCandidateNSVfitHistManagerForElecTau',
            'diTauCandidateZeeHypothesisHistManagerForElecTau',
            'elecPairHistManagerByLooseIsolation',
            'jetHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForElecTau'
        ),
        replace = cms.vstring(
            'electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
			'electronHistManager.makeConversionHistograms = True',
            'tauHistManager.tauSource = selectedPatTausForElecTauElectronVetoCumulative',
            'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsNonZeroChargeCumulative',
            'diTauCandidateHistManagerForElecTau.visMassHypothesisSource = elecTauPairVisMassHypotheses',
            'diTauCandidateNSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsNonZeroChargeCumulative',
            'diTauCandidateZeeHypothesisHistManagerForElecTau.ZllHypothesisSource = elecTauPairZeeHypotheses',
            'elecPairHistManagerByLooseIsolation.diTauCandidateSource = selectedDiElecPairZeeHypothesesByLooseIsolation'
        )
    )
)
