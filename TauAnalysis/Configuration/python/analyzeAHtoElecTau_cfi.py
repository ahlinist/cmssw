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
electronHistManager.src = cms.InputTag("selectedPatElectronsTrkIPcumulative")

# import config for tau histogram manager
from TauAnalysis.Core.pftauHistManager_cfi import *
tauHistManager.src = cms.InputTag('selectedPatTausForElecTauElectronVetoCumulative')
tauHistManager.useHPSpTaNCalgorithm = cms.bool(True)

# import config for di-tau histogram manager
from TauAnalysis.Core.diTauCandidateHistManager_cfi import *
diTauCandidateHistManagerForElecTau = copy.deepcopy(diTauCandidateHistManager)
diTauCandidateHistManagerForElecTau.pluginName = cms.string('diTauCandidateHistManagerForElecTau')
diTauCandidateHistManagerForElecTau.pluginType = cms.string('PATElecTauPairHistManager')
diTauCandidateHistManagerForElecTau.diTauCandidateSource = cms.InputTag('selectedElecTauPairsForAHtoElecTauZeroChargeCumulative')
diTauCandidateHistManagerForElecTau.visMassHypothesisSource = cms.InputTag('')

from TauAnalysis.Core.diTauCandidateSVfitHistManager_cfi import *
diTauCandidateSVfitHistManagerForElecTau = copy.deepcopy(diTauCandidateSVfitHistManager)
diTauCandidateSVfitHistManagerForElecTau.pluginName = cms.string('diTauCandidateSVfitHistManagerForElecTau')
diTauCandidateSVfitHistManagerForElecTau.pluginType = cms.string('PATElecTauPairSVfitHistManager')
diTauCandidateSVfitHistManagerForElecTau.diTauCandidateSource = cms.InputTag('selectedElecTauPairsForAHtoElecTauZeroChargeCumulative')

from TauAnalysis.Core.diTauCandidateZllHypothesisHistManager_cfi import *
diTauCandidateZeeHypothesisHistManagerForElecTau = copy.deepcopy(ZllHypothesisHistManager)
diTauCandidateZeeHypothesisHistManagerForElecTau.pluginName = cms.string('diTauCandidateZeeHypothesisHistManagerForElecTau')
diTauCandidateZeeHypothesisHistManagerForElecTau.pluginType = cms.string('ZllHypothesisElecTauHistManager')
diTauCandidateZeeHypothesisHistManagerForElecTau.ZllHypothesisSource = cms.InputTag('elecTauPairZeeHypothesesForAHtoElecTau')
diTauCandidateZeeHypothesisHistManagerForElecTau.dqmDirectory_store = cms.string('DiTauCandidateZeeHypothesisQuantities')

# import config for Zmumu veto histogram manager
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
sysUncertaintyBinnerForElecTauEff = sysUncertaintyBinner.clone(
    pluginName = cms.string('sysUncertaintyBinnerForElecTauEff'),
    binnerPlugins = cms.VPSet(
        dataBinner
    ),
    systematics = cms.vstring(sysUncertaintyNames)
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

diTauLeg1ChargeBinGridHistManager = cms.PSet(
    pluginName = cms.string('diTauLeg1ChargeBinGridHistManager'),
    pluginType = cms.string('BinGridHistManager'),
    binning = cms.PSet(
        name = cms.string("diTauLeg1ChargeBinning"),
        config = cms.VPSet(
            cms.PSet(
                extractor = cms.PSet(
                    pluginType = cms.string("PATElecTauPairValExtractor"),
                    src = cms.InputTag('selectedElecTauPairsForAHtoElecTauZeroChargeCumulative'),
                    value = cms.string('leg1.charge')
                ),
                branchName = cms.string('diTauLeg1Charge'),
                binning = cms.PSet(
                    boundaries = cms.vdouble(0.),
                    min = cms.double(-2.),
                    max = cms.double(+2.)
                )
            )
        )
    ),
    histManagers = cms.VPSet(
        diTauCandidateHistManagerForElecTau,
        diTauCandidateSVfitHistManagerForElecTau
    ),
    dqmDirectory_store = cms.string('diTauLeg1ChargeBinnedHistograms')
)
#--------------------------------------------------------------------------------
# define event selection criteria
#--------------------------------------------------------------------------------


# di-tau candidate selection
evtSelDiTauCandidateForAHtoElecTauAntiOverlapVeto = evtSelDiTauCandidateForElecTauAntiOverlapVeto.clone(
    pluginName = cms.string('evtSelDiTauCandidateForAHtoElecTauAntiOverlapVeto'),
    src_cumulative = cms.InputTag('diTauCandidateForAHtoElecTauAntiOverlapVeto', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoElecTauAntiOverlapVeto', 'individual')
)
evtSelDiTauCandidateForAHtoElecTauMt1MET = evtSelDiTauCandidateForElecTauMt1MET.clone(
    pluginName = cms.string('evtSelDiTauCandidateForAHtoElecTauMt1MET'),
    src_cumulative = cms.InputTag('diTauCandidateForAHtoElecTauMt1METcut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoElecTauMt1METcut', 'individual')
)
evtSelDiTauCandidateForAHtoElecTauPzetaDiff = evtSelDiTauCandidateForElecTauPzetaDiff.clone(
    pluginName = cms.string('evtSelDiTauCandidateForAHtoElecTauPzetaDiff'),
    src_cumulative = cms.InputTag('diTauCandidateForAHtoElecTauPzetaDiffCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoElecTauPzetaDiffCut', 'individual')
)

# opposite-sign pairs
evtSelDiTauCandidateForAHtoElecTauZeroCharge = evtSelDiTauCandidateForElecTauZeroCharge.clone(
    pluginName = cms.string('evtSelDiTauCandidateForAHtoElecTauZeroCharge'),
    src_cumulative = cms.InputTag('diTauCandidateForAHtoElecTauZeroChargeCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoElecTauZeroChargeCut', 'individual')
)

# same-sign pairs
evtSelDiTauCandidateForAHtoElecTauNonZeroCharge = evtSelDiTauCandidateForElecTauNonZeroCharge.clone(
    pluginName = cms.string('evtSelDiTauCandidateForAHtoElecTauNonZeroCharge'),
    src_cumulative = cms.InputTag('diTauCandidateForAHtoElecTauNonZeroChargeCut', 'cumulative'),
    src_individual = cms.InputTag('diTauCandidateForAHtoElecTauNonZeroChargeCut', 'individual')
)

# central jet veto/b-jet candidate selection
evtSelNonCentralJetEt20bTag = cms.PSet(
    pluginName = cms.string('evtSelNonCentralJetEt20bTag'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('centralJetEt20bTagVeto', 'cumulative'),
    src_individual = cms.InputTag('centralJetEt20bTagVeto', 'individual'),
    systematics = cms.vstring(jetSystematics.keys())
)
evtSelCentralJetEt20 = cms.PSet(
    pluginName = cms.string('evtSelCentralJetEt20'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('centralJetEt20Cut', 'cumulative'),
    src_individual = cms.InputTag('centralJetEt20Cut', 'individual'),
    systematics = cms.vstring(jetSystematics.keys())
)
evtSelCentralJetEt20bTag = cms.PSet(
    pluginName = cms.string('evtSelCentralJetEt20bTag'),
    pluginType = cms.string('BoolEventSelector'),
    src_cumulative = cms.InputTag('centralJetEt20bTagCut', 'cumulative'),
    src_individual = cms.InputTag('centralJetEt20bTagCut', 'individual'),
    systematics = cms.vstring(jetSystematics.keys())
)

#--------------------------------------------------------------------------------
# define event print-out
#--------------------------------------------------------------------------------
elecTauEventDump_woBtag = elecTauEventDump.clone(
	diTauCandidateSource = cms.InputTag('selectedElecTauPairsForAHtoElecTauPzetaDiffCumulative'),
	elecTauZeeHypothesisSource = cms.InputTag('elecTauPairZeeHypothesesForAHtoElecTau'),
	triggerConditions = cms.vstring("evtSelNonCentralJetEt20bTag: passed_cumulative")
)

elecTauEventDump_woBtag = elecTauEventDump.clone(
	diTauCandidateSource = cms.InputTag('selectedElecTauPairsForAHtoElecTauPzetaDiffCumulative'),
	elecTauZeeHypothesisSource = cms.InputTag('elecTauPairZeeHypothesesForAHtoElecTau'),
	triggerConditions = cms.vstring("evtSelCentralJetEt20bTag: passed_cumulative")
)

#--------------------------------------------------------------------------------
# define analysis sequence
# (ordered list of event selection criteria and histogram filling)
#--------------------------------------------------------------------------------

elecTauAnalysisSequenceOS_woBtag = cms.VPSet(
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
                              'electronHistManager.makeIsoPtConeSizeDepHistograms = True',
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
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauIsoCumulative')
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
            'triggerHistManagerForElecTau'
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
        filter = cms.string('evtSelTauLeadTrk'),
        title = cms.string('Tau lead. Track find.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
			'tauHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
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
            'tauHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
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
            'tauHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
                              'tauHistManager.tauSource = selectedPatTausForElecTauTaNCdiscrCumulative',
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
            'tauHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
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
            'tauHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
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
        replace = cms.vstring(
			'electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
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
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForElecTau'
        ),
        replace = cms.vstring(
			'electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
			'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative')
    ),
    
    #selection of electron + tau-jet combinations
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoElecTauAntiOverlapVeto'),
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
                              'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauAntiOverlapVetoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoElecTauMt1MET'),
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
                              'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauMt1METcumulative'
		)  
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoElecTauPzetaDiff'),
        title = cms.string('P_{#zeta} - 1.5*P_{#zeta}^{vis} > -20 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForElecTau',
			'diTauCandidateSVfitHistManagerForElecTau',
            'elecPairHistManagerByLooseIsolation'
        ),
		replace = cms.vstring(
			'electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
			'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
			'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauPzetaDiffCumulative',
			'diTauCandidateSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauPzetaDiffCumulative',
			'elecPairHistManagerByLooseIsolation.diTauCandidateSource = allDiElecPairZeeHypothesesByLooseIsolation'
		)
    ),
	# veto events compatible with Z --> e+ e- hypothesis
	# based on presence of a second, oppositely-charged, loosely isolated electron
	cms.PSet(
	    filter = cms.string('evtSelDiElecPairZeeHypothesisVetoByLooseIsolation'),
	    title = cms.string('charge(isoelectron + looseIsoElectron) != 0'),
	    saveRunLumiSectionEventNumbers = cms.vstring('')
	),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForElecTau',
            'diTauCandidateSVfitHistManagerForElecTau',
            'diTauCandidateZeeHypothesisHistManagerForElecTau',
            'elecPairHistManagerByLooseIsolation',
            'jetHistManager'
        ),
		replace = cms.vstring(
			'electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
			'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
			'jetHistManager.jetSource = selectedPatJetsForAHtoElecTauAntiOverlapWithLeptonsVetoCumulative',
			'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauPzetaDiffCumulative',
			'diTauCandidateSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauPzetaDiffCumulative',
			'elecPairHistManagerByLooseIsolation.diTauCandidateSource = selectedDiElecPairZeeHypothesesByLooseIsolation'
			#'diTauCandidateZeeHypothesisHistManagerForElecTau.ZllHypothesisSource = selectedElecTauPairZeeHypotheses'
		)
    ),
    # veto events containing b-tagged jets with Et > 20
    # in order to avoid overlap with "b-tag" analysis path
	cms.PSet(
		filter = cms.string('evtSelNonCentralJetEt20bTag'),
		title = cms.string('no E_{T} > 20 GeV central Jet with b-Tag'),
		saveRunLumiSectionEventNumbers = cms.vstring('')
	),
	cms.PSet(
		analyzers = cms.vstring(
			'electronHistManager',
			'tauHistManager',
			'diTauCandidateHistManagerForElecTau',
			'diTauCandidateSVfitHistManagerForElecTau',
			'jetHistManager'
		),
		replace = cms.vstring(
			'electronHistManager.electronSource = selectedPatElectronsTrkIPcumulative',
			'tauHistManager.tauSource = selectedPatTausForElecTauElectronVetoCumulative',
			'jetHistManager.jetSource = selectedPatJetsForAHtoElecTauAntiOverlapWithLeptonsVetoCumulative',
			'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauPzetaDiffCumulative',
			'diTauCandidateHistManagerForElecTau.visMassHypothesisSource = elecTauPairVisMassHypothesesForAHtoElecTau',
			'diTauCandidateSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauPzetaDiffCumulative',
			'jetHistManager.jetSource = selectedPatJetsForAHtoElecTauAntiOverlapWithLeptonsVetoCumulative'
		)
	),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoElecTauZeroCharge'),
        title = cms.string('Charge(Electron+Tau) = 0'),
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
			'diTauLeg1ChargeBinGridHistManager',
			'jetHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'particleMultiplicityHistManager',
            'vertexHistManager',
            'triggerHistManagerForElecTau',
			'sysUncertaintyHistManagerForElecTau',
			'dataBinner',
			'sysUncertaintyBinnerForElecTauEff'
        ),
		replace = cms.vstring(
			'electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
			'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
			'jetHistManager.jetSource = selectedPatJetsForAHtoElecTauAntiOverlapWithLeptonsVetoCumulative',
            'diTauCandidateZeeHypothesisHistManagerForElecTau.ZllHypothesisSource = elecTauPairZeeHypothesesForAHtoElecTau',
			'diTauCandidateSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauZeroChargeCumulative',
			'diTauCandidateHistManagerForElecTau.visMassHypothesisSource = elecTauPairVisMassHypothesesForAHtoElecTau',
			'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauZeroChargeCumulative'
		)
	)
)
elecTauAnalysisSequenceOS_wBtag = cms.VPSet(
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
                              'electronHistManager.makeIsoPtConeSizeDepHistograms = True',
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
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauIsoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelElectronConversionVeto'),
        title = cms.string('Electron Track conv. veto'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
            'tauHistManager'
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
        filter = cms.string('evtSelTauLeadTrk'),
        title = cms.string('Tau lead. Track find.'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
			'tauHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
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
            'tauHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
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
            'tauHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
                              'tauHistManager.tauSource = selectedPatTausForElecTauTaNCdiscrCumulative',
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
            'tauHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
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
            'tauHistManager'
        ),
        replace = cms.vstring('electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
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
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForElecTau'
        ),
        replace = cms.vstring(
			'electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
			'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative')
    ),
    
    #selection of electron + tau-jet combinations
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoElecTauAntiOverlapVeto'),
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
                              'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauAntiOverlapVetoCumulative')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoElecTauMt1MET'),
        title = cms.string('M_{T}(Electron-MET) < 50 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForElecTau'
        ),
        replace = cms.vstring(
			'electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
            'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauMt1METcumulative'
		)  
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoElecTauPzetaDiff'),
        title = cms.string('P_{#zeta} - 1.5*P_{#zeta}^{vis} > -20 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForElecTau',
            'elecPairHistManagerByLooseIsolation'
        ),
		replace = cms.vstring(
			'electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
			'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
			'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauPzetaDiffCumulative',
			'diTauCandidateSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauPzetaDiffCumulative',
			'elecPairHistManagerByLooseIsolation.diTauCandidateSource = allDiElecPairZeeHypothesesByLooseIsolation'
		)
    ),
	# veto events compatible with Z --> e+ e- hypothesis
	# based on presence of a second, oppositely-charged, loosely isolated electron
	cms.PSet(
	    filter = cms.string('evtSelDiElecPairZeeHypothesisVetoByLooseIsolation'),
	    title = cms.string('charge(isoelectron + looseIsoElectron) != 0'),
	    saveRunLumiSectionEventNumbers = cms.vstring('')
	),
    cms.PSet(
        analyzers = cms.vstring(
            'electronHistManager',
            'tauHistManager',
            'diTauCandidateHistManagerForElecTau',
            'diTauCandidateSVfitHistManagerForElecTau',
            'diTauCandidateZeeHypothesisHistManagerForElecTau',
            'elecPairHistManagerByLooseIsolation',
            'jetHistManager'
        ),
		replace = cms.vstring(
			'electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
			'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
			'jetHistManager.jetSource = selectedPatJetsForAHtoElecTauAntiOverlapWithLeptonsVetoCumulative',
			'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauPzetaDiffCumulative',
			'diTauCandidateSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauPzetaDiffCumulative',
			'elecPairHistManagerByLooseIsolation.diTauCandidateSource = selectedDiElecPairZeeHypothesesByLooseIsolation'
		)
    ),
    # require at least one b-tagged jet with Et > 20 in the event
	cms.PSet(
		filter = cms.string('evtSelCentralJetEt20'),
		title = cms.string('E_{T} > 20 GeV central jet'),
		saveRunLumiSectionEventNumbers = cms.vstring('')
	),
	cms.PSet(
		analyzers = cms.vstring(
			'electronHistManager',
			'tauHistManager',
			'diTauCandidateHistManagerForElecTau',
			'jetHistManager'
			),
		replace = cms.vstring(
			'electronHistManager.electronSource = selectedPatElectronsTrkIPcumulative',
			'tauHistManager.tauSource = selectedPatTausForElecTauElectronVetoCumulative',
			'jetHistManager.jetSource = selectedPatJetsForAHtoElecTauAntiOverlapWithLeptonsVetoCumulative',
			'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauPzetaDiffCumulative',
			'diTauCandidateHistManagerForElecTau.visMassHypothesisSource = elecTauPairVisMassHypothesesForAHtoElecTau'
		)
	),
	cms.PSet(
		filter = cms.string('evtSelCentralJetEt20bTag'),
		title = cms.string('E_{T} > 20 GeV central Jet with b-Tag'),
		saveRunLumiSectionEventNumbers = cms.vstring('')
	),
	cms.PSet(
		analyzers = cms.vstring(
			'electronHistManager',
			'tauHistManager',
			'diTauCandidateHistManagerForElecTau',
			'diTauCandidateSVfitHistManagerForElecTau',
			'jetHistManager'
			),
		replace = cms.vstring(
			'electronHistManager.electronSource = selectedPatElectronsTrkIPcumulative',
			'tauHistManager.tauSource = selectedPatTausForElecTauElectronVetoCumulative',
			'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauPzetaDiffCumulative',
			'diTauCandidateHistManagerForElecTau.visMassHypothesisSource = elecTauPairVisMassHypothesesForAHtoElecTau',
			'diTauCandidateSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauPzetaDiffCumulative',
			'jetHistManager.jetSource = selectedPatJetsForAHtoElecTauBtagCumulative'
		)
	),
	#  finally apply opposite-sign selection
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoElecTauZeroCharge'),
        title = cms.string('Charge(Electron+Tau) = 0'),
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
			'diTauLeg1ChargeBinGridHistManager',
			'jetHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'particleMultiplicityHistManager',
            'vertexHistManager',
            'triggerHistManagerForElecTau',
			'sysUncertaintyHistManagerForElecTau',
			'dataBinner',
			'sysUncertaintyBinnerForElecTauEff'
        ),
		replace = cms.vstring(
			'electronHistManager.electronSource = selectedPatElectronsForElecTauTrkIPcumulative',
			'tauHistManager.tauSource = selectedPatTausForElecTauMuonVetoCumulative',
			'jetHistManager.jetSource = selectedPatJetsForAHtoElecTauBtagCumulative',
            'diTauCandidateZeeHypothesisHistManagerForElecTau.ZllHypothesisSource = elecTauPairZeeHypothesesForAHtoElecTau',
			'diTauCandidateSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauZeroChargeCumulative',
			'diTauCandidateHistManagerForElecTau.visMassHypothesisSource = elecTauPairVisMassHypothesesForAHtoElecTau',
			'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauZeroChargeCumulative'
		)
	)
)
elecTauAnalysisSequenceSS_woBtag = cms.VPSet(
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
        filter = cms.string('evtSelDiTauCandidateForAHtoElecTauAntiOverlapVeto'),
        title = cms.string('dR(Electron-Tau) > 0.7'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoElecTauMt1MET'),
        title = cms.string('M_{T}(Electron-MET) < 50 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoElecTauPzetaDiff'),
        title = cms.string('P_{#zeta} - 1.5*P_{#zeta}^{vis} > -20 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiElecPairZeeHypothesisVetoByLooseIsolation'),
        title = cms.string('charge(isoElectron+looseIsoElectron) != 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
	cms.PSet(
		filter = cms.string('evtSelNonCentralJetEt20bTag'),
		title = cms.string('no E_{T} > 20 GeV central Jet with b-Tag'),
		saveRunLumiSectionEventNumbers = cms.vstring('')
	),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoElecTauNonZeroCharge'),
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
            'jetHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForElecTau'
        ),
        replace = cms.vstring(
            'electronHistManager.electronSource = selectedPatElectronsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForElecTauElectronVetoCumulative',
            'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauNonZeroChargeCumulative',
            'diTauCandidateHistManagerForElecTau.visMassHypothesisSource = elecTauPairVisMassHypothesesForAHtoElecTau',
            'diTauCandidateSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauNonZeroChargeCumulative',
            'diTauCandidateZeeHypothesisHistManagerForElecTau.ZllHypothesisSource = elecTauPairZeeHypothesesForAHtoElecTau',
			'jetHistManager.jetSource = selectedPatJetsForAHtoElecTauAntiOverlapWithLeptonsVetoCumulative'
        )
    )
)

elecTauAnalysisSequenceSS_wBtag = cms.VPSet(
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
        filter = cms.string('evtSelDiTauCandidateForAHtoElecTauAntiOverlapVeto'),
        title = cms.string('dR(Electron-Tau) > 0.7'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoElecTauMt1MET'),
        title = cms.string('M_{T}(Electron-MET) < 50 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoElecTauPzetaDiff'),
        title = cms.string('P_{#zeta} - 1.5*P_{#zeta}^{vis} > -20 GeV'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
    cms.PSet(
        filter = cms.string('evtSelDiElecPairZeeHypothesisVetoByLooseIsolation'),
        title = cms.string('charge(isoElectron+looseIsoElectron) != 0'),
        saveRunLumiSectionEventNumbers = cms.vstring('')
    ),
	cms.PSet(
		filter = cms.string('evtSelCentralJetEt20'),
		title = cms.string('E_{T} > 20 GeV central Jet'),
		saveRunLumiSectionEventNumbers = cms.vstring('')
	),
	cms.PSet(
		filter = cms.string('evtSelCentralJetEt20bTag'),
		title = cms.string('E_{T} > 20 GeV central Jet with b-Tag'),
		saveRunLumiSectionEventNumbers = cms.vstring('')
	),
    cms.PSet(
        filter = cms.string('evtSelDiTauCandidateForAHtoElecTauNonZeroCharge'),
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
            'jetHistManager',
            'caloMEtHistManager',
            'pfMEtHistManager',
            'vertexHistManager',
            'triggerHistManagerForElecTau'
        ),
        replace = cms.vstring(
            'electronHistManager.electronSource = selectedPatElectronsTrkIPcumulative',
            'tauHistManager.tauSource = selectedPatTausForElecTauElectronVetoCumulative',
            'diTauCandidateHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauNonZeroChargeCumulative',
            'diTauCandidateHistManagerForElecTau.visMassHypothesisSource = elecTauPairVisMassHypothesesForAHtoElecTau',
            'diTauCandidateSVfitHistManagerForElecTau.diTauCandidateSource = selectedElecTauPairsForAHtoElecTauNonZeroChargeCumulative',
            'diTauCandidateZeeHypothesisHistManagerForElecTau.ZllHypothesisSource = elecTauPairZeeHypothesesForAHtoElecTau',
            'jetHistManager.jetSource = selectedPatJetsForAHtoElecTauBtagCumulative'
        )
    )
)
