import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
# import configuration parameters of Z --> electron + tau-jet channel
#
# WARNING: definitions from analyzeZtoElecTau_cfi.py need to be imported before any
#          A/H --> electron + tau-jet channel specific configuration parameters are defined;
#          otherwise the import of analyzeZtoElecTau_cfi.py will **overwrite**
#          A/H --> electron + tau-jet channel specific definitions
#         (due to the fact that e.g. histogram managers of 'ZtoElecTau' channel
#          and 'AHtoElecTau' channel have the same name '...ForElecTau')
#
from TauAnalysis.Configuration.analyzeZtoElecTau_cfi import *
#--------------------------------------------------------------------------------

# import config for histogram manager filling information about phase-space simulated in Monte Carlo sample
from TauAnalysis.Core.genPhaseSpaceEventInfoHistManager_cfi import *

# import config for event weight histogram manager
from TauAnalysis.Core.eventWeightHistManager_cfi import *

# import config for electron histogram manager
from TauAnalysis.Core.electronHistManager_cfi import *

# import config for tau histogram manager
from TauAnalysis.Core.pftauHistManager_cfi import *
tauHistManager.src = cms.InputTag('selectedPatTausForElecTauElectronVetoCumulative')
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

from TauAnalysis.Core.diTauCandidateZllHypothesisHistManager_cfi import *
diTauCandidateZeeHypothesisHistManagerForElecTau = copy.deepcopy(ZllHypothesisHistManager)
diTauCandidateZeeHypothesisHistManagerForElecTau.pluginName = cms.string('diTauCandidateZeeHypothesisHistManagerForElecTau')
diTauCandidateZeeHypothesisHistManagerForElecTau.pluginType = cms.string('ZllHypothesisElecTauHistManager')
diTauCandidateZeeHypothesisHistManagerForElecTau.ZllHypothesisSource = cms.InputTag('elecTauPairZeeHypotheses')
diTauCandidateZeeHypothesisHistManagerForElecTau.dqmDirectory_store = cms.string('DiTauCandidateZeeHypothesisQuantities')

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
	'HLT_Ele12_SW_TightEleIdIsol_L1R'
)

# import config for binning results
# used for keeping track of number of events passing all selection criteria
from TauAnalysis.Core.dataBinner_cfi import *

# import config for binning results
# used to estimate systematic uncertainties
from TauAnalysis.Core.sysUncertaintyBinner_cfi import *
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *
sysUncertaintyNames = [ "CENTRAL_VALUE", ]
sysUncertaintyNames.extend(
    getSysUncertaintyNames(
        [ electronSystematics,
          tauSystematics,
          elecTauPairSystematics,
          jetSystematics,
          theorySystematics ]
    )
)
#sysUncertaintyBinnerForElecTauEff = sysUncertaintyBinner.clone(
#    pluginName = cms.string('sysUncertaintyBinnerForElecTauEff'),
#    binnerPlugins = cms.VPSet(
#        dataBinner
#    ),
#    systematics = cms.vstring(sysUncertaintyNames)
#)

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
            config = diTauCandidateSVfitHistManagerForElecTau,
            systematics = cms.PSet(
                diTauCandidateSource = getSysUncertaintyParameterSets(
                    [ elecTauPairSystematics ]
                )
            )
        )
    ),
    dqmDirectory_store = cms.string('sysUncertaintyHistManagerResults')
)
#--------------------------------------------------------------------------------
# define event selection criteria
#--------------------------------------------------------------------------------


# di-tau candidate selection
evtSelDiTauCandidateForAHtoElecTauAntiOverlapVeto = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForAHtoElecTauAntiOverlapVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForAHtoElecTauAntiOverlapVeto', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoElecTauAntiOverlapVeto', 'individual')
)
evtSelDiTauCandidateForAHtoElecTauZeroCharge = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForAHtoElecTauZeroCharge'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForAHtoElecTauZeroChargeCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoElecTauZeroChargeCut', 'individual')
)
evtSelDiTauCandidateForAHtoElecTauAcoplanarity12 = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForAHtoElecTauAcoplanarity12'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForAHtoElecTauAcoplanarity12Cut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoElecTauAcoplanarity12Cut', 'individual')
)
evtSelDiTauCandidateForAHtoElecTauMt1MET = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForAHtoElecTauMt1MET'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForAHtoElecTauMt1METCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoElecTauMt1METCut', 'individual')
)
evtSelDiTauCandidateForAHtoElecTauPzetaDiff = cms.PSet(
    pluginName = cms.string('evtSelDiTauCandidateForAHtoElecTauPzetaDiff'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('diTauCandidateForAHtoElecTauPzetaDiffCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoElecTauPzetaDiffCut', 'individual')
)

# veto events compatible with Z --> e+ e- hypothesis
# (based on reconstructed (visible) invariant mass of e + tau-jet pair)
evtSelElecTauPairZeeHypothesisVeto = cms.PSet(
    pluginName = cms.string('evtSelElecTauPairZeeHypothesisVeto'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('elecTauPairZeeHypothesisVeto')
)

# veto events compatible with Z --> e+ e- hypothesis
# (based on reconstructed presence of a loosely-isolated, opposite-sign electron)
evtSelDiElecPairZeeHypothesisVetoByLooseIsolation = cms.PSet(
    pluginName = cms.string('evtSelDiElecPairZeeHypothesisVetoByLooseIsolation'),
    pluginType = cms.string('BoolEventSelector'),
    src = cms.InputTag('diElecPairZeeHypothesisVetoByLooseIsolation')
)
#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
# define analysis sequence
# (ordered list of event selection criteria and histogram filling)
#--------------------------------------------------------------------------------

elecTauAnalysisSequence = cms.VPSet(
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
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauEta21Cumulative')
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
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauPt15Cumulative')
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
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauPt15Cumulative',
                              'tauHistManager.tauSource = selectedPatTausForElecTauAntiOverlapWithElectronsVetoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauEta'),
        title = cms.string('-2.1 < eta(Tau) < +2.1'),
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
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauPt15Cumulative',
                              'tauHistManager.tauSource = selectedPatTausForElecTauEta23Cumulative')
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
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauPt15Cumulative',
                              'electronHistManager.makeIsoPtConeSizeDepHistograms = True',
                              'tauHistManager.tauSource = selectedPatTausForElecTauPt20Cumulative')
		#'diTauCandidateHistManagerForElecTau.diTauCandidateSource = elecTauPairsAfterTauPt')
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
	#		#'diTauCandidateHistManagerForElecTau',
	#        'electronHistManager'
	#    ),
	#    replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIsoCumulative',
#			#'diTauCandidateHistManagerForElecTau.diTauCandidateSource = elecTauPairsAfterElectronTrkIso',
#							  'electronHistManager.makeIsoPtConeSizeDepHistograms = True')
   # ),
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
			#'diTauCandidateHistManagerForElecTau.diTauCandidateSource = elecTauPairsAfterElectronTrkIP')
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
        title = cms.string('Tau TaNC at 0.5%'),
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
        filter = cms.string('evtSelTauTrkIso'),
        title = cms.string('Tau Track iso.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
			#'diTauCandidateHistManagerForElecTau',
            'tauHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
                              'tauHistManager.tauSource = selectedPatTausForElecTauTrkIsoCumulative',
							  #'diTauCandidateHistManagerForElecTau.diTauCandidateSource = elecTauPairsAfterTauTrkIso',
                              'tauHistManager.makeIsoPtConeSizeDepHistograms = True')
    ),
    cms.PSet(
        filter = cms.string('evtSelTauEcalIso'),
        title = cms.string('Tau ECAL iso.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
			#'diTauCandidateHistManagerForElecTau',
            'tauHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
			#'diTauCandidateHistManagerForElecTau.diTauCandidateSource = elecTauPairsAfterTauEcalIso',
                              'tauHistManager.tauSource = selectedPatTausForElecTauEcalIsoCumulative')
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
        filter = cms.string('evtSelDiTauCandidateForElecTauZeroCharge'),
        title = cms.string('Charge(Electron+Tau) = 0'),
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
                              'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsZeroChargeCumulative')
		),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForElecTauAcoplanarity12'),
        title = cms.string('Acoplanarity(Electron+Tau)'),
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
                              'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsAcoplanarity12Cumulative')
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
            'diTauCandidateZeeHypothesisHistManagerForElecTau'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
                              'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
                              'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsPzetaDiffCumulative',
                              'diTauCandidateHistManagerForElecTau.visMassHypothesisSource = elecTauPairVisMassHypotheses',
                              'diTauCandidateZeeHypothesisHistManagerForElecTau.ZllHypothesisSource = elecTauPairZeeHypotheses')
    ),
	# veto events compatible with Z --> e+ e- hypothesis
    # (based on reconstructed (visible) invariant mass of e + tau-jet pair)
	#cms.PSet(
	#    filter = cms.string('evtSelElecTauPairZeeHypothesisVeto'),
	#    title = cms.string('not 85 < M_{vis} (Electron-Tau) < 100 GeV'),
	#    saveRunLumiSectionEventNumbers = cms.vstring('passed_cumulative')
	#),
	# veto events compatible with Z --> e+ e- hypothesis
	# based on presence of a second, oppositely-charged, loosely isolated electron
	cms.PSet(
	    filter = cms.string('evtSelDiElecPairZeeHypothesisVetoByLooseIsolation'),
	    title = cms.string('no second opposite-sign, loosely-isolated electron'),
	    saveRunLumiSectionEventNumbers = cms.vstring('passed_cumulative')
	),
    cms.PSet(
        analyzers = cms.vstring(
            'genPhaseSpaceEventInfoHistManager',
            'electronHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForElecTau',
            'diTauCandidateSVfitHistManagerForElecTau',
            'diTauCandidateZeeHypothesisHistManagerForElecTau',
            'jetHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'particleMultiplicityHistManager',
            'vertexHistManager',
            'triggerHistManagerForElecTau',
			'sysUncertaintyHistManagerForElecTau'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
                              'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
                              'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsPzetaDiffCumulative',
                              'diTauCandidateHistManagerForElecTau.visMassHypothesisSource = elecTauPairVisMassHypotheses',
                              'diTauCandidateSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsPzetaDiffCumulative',
                              'diTauCandidateZeeHypothesisHistManagerForElecTau.ZllHypothesisSource = selectedElecTauPairZeeHypotheses')
    )
)
