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
diTauCandidateHistManagerForElecTau.visMassHypothesisSource = cms.InputTag('')

from TauAnalysis.Core.diTauCandidateSVfitHistManager_cfi import *
diTauCandidateSVfitHistManagerForElecTau = copy.deepcopy(diTauCandidateSVfitHistManager)
diTauCandidateSVfitHistManagerForElecTau.pluginName = cms.string('diTauCandidateSVfitHistManagerForElecTau')
diTauCandidateSVfitHistManagerForElecTau.pluginType = cms.string('PATElecTauPairSVfitHistManager')
diTauCandidateSVfitHistManagerForElecTau.diTauCandidateSource = cms.InputTag('allElecTauPairs')
diTauCandidateSVfitHistManagerForElecTau.SVfitAlgorithms = cms.VPSet(
		cms.PSet(
			name = cms.string("psKine")
		),
		cms.PSet(
			name = cms.string("psKine_MEt")
		),
		cms.PSet(
			name = cms.string("psKine_MEt_ptBalance")
		),
		cms.PSet(
			name = cms.string("psKine_MEt_ptBalance2")
		),
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
	'HLT_Ele10_SW_L1R',
	'HLT_Ele10_LW_EleId_L1R',
	'HLT_Ele10_SW_EleId_L1R',
	'HLT_Ele12_SW_TightEleIdIsol_L1R',
	'HLT_Ele12_SW_TighterEleIdIsol_L1R_v1',
	'HLT_IsoEle12_PFTau15_v1',
	'HLT_IsoEle12_PFTau15_v2',
	'HLT_IsoEle12_PFTau15_v3'
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

    # L1 trigger bits not contained in AOD;
    # in order to process Monte Carlo samples produced by FastSimulation,
    # disable histogram filling for now
    #l1GtReadoutRecordSource = cms.InputTag('hltGtDigis::HLT'),
    #l1GtObjectMapRecordSource = cms.InputTag('hltL1GtObjectMap::HLT'),
    l1GtReadoutRecordSource = cms.InputTag(''),
    l1GtObjectMapRecordSource = cms.InputTag(''),
    l1BitsToPrint = cms.vstring('L1_SingleEG5', 'L1_SingleEG8', 'L1_SingleEG10', 'L1_SingleEG12', 'L1_SingleEG15',
                                'L1_SingleIsoEG5', 'L1_SingleIsoEG8', 'L1_SingleIsoEG10', 'L1_SingleIsoEG12', 'L1_SingleIsoEG15'),
    
    hltResultsSource = cms.InputTag('TriggerResults::HLT'),
	hltPathsToPrint = cms.vstring(
		'HLT_Ele10_SW_L1R',
		'HLT_Ele10_LW_EleId_L1R',
		'HLT_Ele10_SW_EleId_L1R',
		'HLT_Ele12_SW_TightEleIdIsol_L1R',
		'HLT_Ele12_SW_TighterEleIdIsol_L1R_v1',
		'HLT_IsoEle12_PFTau15_v1',
		'HLT_IsoEle12_PFTau15_v2',
		'HLT_IsoEle12_PFTau15_v3'
	),
    doGenInfo = cms.bool(True),
    genParticleSource = cms.InputTag('genParticles'),
    genJetSource = cms.InputTag('ak5GenJets'),
    genTauJetSource = cms.InputTag('tauGenJets'),
    genEventInfoSource = cms.InputTag('generator'),
    
    electronSource = cms.InputTag('cleanPatElectrons'),
    tauSource = cms.InputTag('selectedPatTausForElecTauPtCumulative'),
    diTauCandidateSource = cms.InputTag('allElecTauPairs'),
	svFitAlgorithms = cms.VPSet(
		cms.PSet(
			name = cms.string("psKine_MEt")
		),
		cms.PSet(
			name = cms.string("psKine_MEt_ptBalance")
		),
		cms.PSet(
			name = cms.string("psKine_MEt_ptBalance2")
		),
	),
	elecTauZeeHypothesisSource = cms.InputTag('elecTauPairZeeHypotheses'), 
	diElecZeeHypothesisSource = cms.InputTag('allDiElecPairZeeHypothesesByLooseIsolation'), 
    jetSource = cms.InputTag('patJets'),
    caloMEtSource = cms.InputTag('patMETs'),
    pfMEtSource = cms.InputTag('patPFMETs'),
    genMEtSource = cms.InputTag('genMetTrue'),

    #output = cms.string("elecTauEventDump.txt"),
    output = cms.string("std::cout"),

    #triggerConditions = cms.vstring("evtSelTightElectronId: rejected_cumulative")
    triggerConditions = cms.vstring("evtSelDiElecPairZeeHypothesisVetoByLooseIsolation: passed_cumulative")
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
            'genPhaseSpaceEventInfoHistManager',
            'electronHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
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
    #        'genPhaseSpaceEventInfoHistManager',
    #        'electronHistManager',
    #        'tauHistManager',
    #        'caloMEtHistManager',
    #        'pfMEtHistManager',
    #        'vertexHistManager',
    #        'triggerHistManagerForElecTau'
    #    )
    #),
  
    # trigger selection
    cms.PSet(
        filter = cms.string('evtSelTrigger'),
        title = cms.string('Trigger'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
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

    # primary event vertex selection
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertex'),
        title = cms.string('Vertex exists'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'vertexHistManager'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexQuality'),
        title = cms.string('p(chi2Vertex) > 0.01'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'vertexHistManager'
        )
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexPosition'),
        title = cms.string('-25 < zVertex < +25 cm'),
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
            'pfMEtHistManager',
            'vertexHistManager'
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
            'electronHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager'
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
            'electronHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauEtaCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelElectronPt'),
        title = cms.string('Pt(Electron) > 15 GeV'),
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
            'pfMEtHistManager',
            'vertexHistManager'
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
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager'
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
			#'diTauCandidateHistManagerForElecTau',
            'triggerHistManagerForElecTau'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauPtCumulative',
                              'electronHistManager.makeIsoPtConeSizeDepHistograms = True',
							  #'diTauCandidateHistManagerForElecTau.diTauCandidateSource = elecTauPairsAfterTauPt',
                              'tauHistManager.tauSource = selectedPatTausForElecTauPtCumulative')
    ),
  
    # selection of electron candidate (isolation & id.)
    # produced in electronic tau decay
	#cms.PSet(
	#    filter = cms.string('evtSelElectronTrkIso'),
	#    title = cms.string('Electron Track iso.'),
	#    saveRunLumiSectionEventNumbers = cms.vstring('')
	#),
	#cms.PSet(
	#    analyzers = cms.vstring(
	#        'electronHistManager'
#			#'diTauCandidateHistManagerForElecTau'
	#    ),
	#    replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIsoCumulative',
#			#'diTauCandidateHistManagerForElecTau.diTauCandidateSource = elecTauPairsAfterElectronTrkIso',
#							  'electronHistManager.makeIsoPtConeSizeDepHistograms = True')
#    ),
	#cms.PSet(
	#    filter = cms.string('evtSelElectronEcalIso'),
	#    title = cms.string('Electron ECAL iso.'),
	#    saveRunLumiSectionEventNumbers = cms.vstring('')
	#),
    cms.PSet(
        filter = cms.string('evtSelElectronIso'),
        title = cms.string('Electron PF iso. (rel.)'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager'
			#'diTauCandidateHistManagerForElecTau'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauIsoCumulative')
			#'diTauCandidateHistManagerForElecTau.diTauCandidateSource = elecTauPairsAfterElectronIso')
    ),
    cms.PSet(
        filter = cms.string('evtSelElectronConversionVeto'),
        title = cms.string('Electron Track conv. veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
            'tauHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
			#'diTauCandidateHistManagerForElecTau',
            'triggerHistManagerForElecTau'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauConversionVetoCumulative')
			#'diTauCandidateHistManagerForElecTau.diTauCandidateSource = elecTauPairsAfterElectronConversionVeto')
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
			#'diTauCandidateHistManagerForElecTau',
            'triggerHistManagerForElecTau'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative')
		#							  'diTauCandidateHistManagerForElecTau.diTauCandidateSource = elecTauPairsAfterElectronTrkIP')
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
            'electronHistManager',
			#'diTauCandidateHistManagerForElecTau',
			'tauHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
			#'diTauCandidateHistManagerForElecTau.diTauCandidateSource = elecTauPairsAfterTauLeadTrkFind',
                              'tauHistManager.tauSource = selectedPatTausForElecTauLeadTrkCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauLeadTrkPt'),
        title = cms.string('Tau lead. Track Pt'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
			#'diTauCandidateHistManagerForElecTau',
            'tauHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
			#'diTauCandidateHistManagerForElecTau.diTauCandidateSource = elecTauPairsAfterTauLeadTrkPt',
                              'tauHistManager.tauSource = selectedPatTausForElecTauLeadTrkPtCumulative',
                              'tauHistManager.makeIsoPtConeSizeDepHistograms = True')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauTaNCdiscr'),
        title = cms.string('Tau HPS loose'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
			#'diTauCandidateHistManagerForElecTau',
            'tauHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
                              'tauHistManager.tauSource = selectedPatTausForElecTauTaNCdiscrCumulative',
							  #'diTauCandidateHistManagerForElecTau.diTauCandidateSource = elecTauPairsAfterTauTaNC',
							  'tauHistManager.makeIsoPtConeSizeDepHistograms = True')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauProng'),
        title = cms.string('Tau 1||3-Prong'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
			#'diTauCandidateHistManagerForElecTau',
            'tauHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
			#'diTauCandidateHistManagerForElecTau.diTauCandidateSource = elecTauPairsAfterTauProng',
                              'tauHistManager.tauSource = selectedPatTausForElecTauProngCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauCharge'),
        title = cms.string('Charge(Tau) = +/-1'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
			#'diTauCandidateHistManagerForElecTau',
            'tauHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
			#'diTauCandidateHistManagerForElecTau.diTauCandidateSource = elecTauPairsAfterTauCharge',
                              'tauHistManager.tauSource = selectedPatTausForElecTauChargeCumulative')
    ),    
    cms.PSet(
        filter = cms.string('evtSelTauElectronVeto'),
        title = cms.string('Tau e-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
			#'diTauCandidateHistManagerForElecTau',
            'tauHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
			#'diTauCandidateHistManagerForElecTau.diTauCandidateSource = elecTauPairsAfterTauElectronVeto',
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
			#'diTauCandidateHistManagerForElecTau',
            'tauHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
			#'diTauCandidateHistManagerForElecTau.diTauCandidateSource = elecTauPairsAfterTauEcalCrackVeto',
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
                              'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative')
    ),
    
    #selection of electron + tau-jet combinations
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForElecTauAntiOverlapVeto'),
        title = cms.string('dR(Electron-Tau) > 0.7'),
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
                              'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
                              'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsAntiOverlapVetoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForElecTauMt1MET'),
        title = cms.string('M_{T}(Electron-MET) < 50 GeV'),
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
                              'diTauCandidateSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsPzetaDiffCumulative',
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
			'diTauCandidateSVfitHistManagerForElecTau',
			'dataBinner',
			'sysUncertaintyBinnerForElecTauEff',
            'diTauCandidateHistManagerForElecTau'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
                              'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
							  'elecPairHistManagerByLooseIsolation.diTauCandidateSource = selectedDiElecPairZeeHypothesesByLooseIsolation',
                              'diTauCandidateSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsPzetaDiffCumulative',
                              'diTauCandidateZeeHypothesisHistManagerForElecTau.ZllHypothesisSource = selectedElecTauPairZeeHypotheses',
                              'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsZeroChargeCumulative'
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
        filter = cms.string('evtSelPrimaryEventVertex'),
        title = cms.string('Vertex'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexQuality'),
        title = cms.string('Vertex quality'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelPrimaryEventVertexPosition'),
        title = cms.string('-24 < zVertex < +24 cm'),
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
        title = cms.string('Pt(Electron) > 15 GeV'),
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
		filter = cms.string('evtSelTauEcalCrackVeto'),
		title = cms.string('Tau ECAL crack-Veto'),
		saveRunLumiSectionEventNumbers = cms.vstring('')
	),
    cms.PSet(
        filter = cms.string('evtSelTauElectronVeto'),
        title = cms.string('Tau e-Veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForElecTauAntiOverlapVeto'),
        title = cms.string('dR(Electron-Tau) > 0.7'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForElecTauMt1MET'),
        title = cms.string('M_{T}(Electron-MET) < 50 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForElecTauPzetaDiff'),
        title = cms.string('P_{#zeta} - 1.5*P_{#zeta}^{vis} > -20 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiElecPairZeeHypothesisVetoByLooseIsolation'),
        title = cms.string('charge(isoElectron+isoElectron) != 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForElecTauNonZeroCharge'),
        title = cms.string('Charge(Muon+Tau) != 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('passed_cumulative')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForElecTau',
            'diTauCandidateSVfitHistManagerForElecTau',
            'diTauCandidateZeeHypothesisHistManagerForElecTau',
            'elecPairHistManagerByLooseIsolation',
            'jetHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForElecTau'
        ),
        replace = cms.vstring(
            'electronHistManager.electronSource = selectedPatMuonsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForElecTauElectronVetoCumulative',
            'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsNonZeroChargeCumulative',
            'diTauCandidateHistManagerForElecTau.visMassHypothesisSource = elecTauPairVisMassHypotheses',
            'diTauCandidateSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsNonZeroChargeCumulative',
            'diTauCandidateZeeHypothesisHistManagerForElecTau.ZllHypothesisSource = elecTauPairZeeHypotheses',
            'elecPairHistManagerByLooseIsolation.diTauCandidateSource = selectedDiMuPairZeeHypothesesByLooseIsolation'
        )
    )
)
