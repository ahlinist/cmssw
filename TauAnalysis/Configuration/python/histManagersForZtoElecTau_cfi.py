import FWCore.ParameterSet.Config as cms
import copy


# import config for histogram manager filling information about phase-space simulated in Monte Carlo sample
from TauAnalysis.Core.genPhaseSpaceEventInfoHistManager_cfi import *

# import config for event weight histogram manager
from TauAnalysis.Core.eventWeightHistManager_cfi import *

# import config for electron histogram manager
from TauAnalysis.Core.electronHistManager_cfi import *
electronHistManager.src = cms.InputTag("selectedPatElectronsForElecTauTrkIPcumulative")

# import config for tau histogram manager
from TauAnalysis.Core.pftauHistManager_cfi import *
tauHistManager.src = cms.InputTag('selectedPatTausForElecTauElectronVetoCumulative')
tauHistManager.useHPSclassicAlgorithm = cms.bool(True)

# import config for di-tau histogram manager
from TauAnalysis.Core.diTauCandidateHistManager_cfi import *
diTauCandidateHistManagerForElecTau = copy.deepcopy(diTauCandidateHistManager)
diTauCandidateHistManagerForElecTau.pluginName = cms.string('diTauCandidateHistManagerForElecTau')
diTauCandidateHistManagerForElecTau.pluginType = cms.string('PATElecTauPairHistManager')
diTauCandidateHistManagerForElecTau.diTauCandidateSource = cms.InputTag('allElecTauPairs')
diTauCandidateHistManagerForElecTau.visMassHypothesisSource = cms.InputTag('')

from TauAnalysis.Core.diTauCandidateNSVfitHistManager_cfi import *
diTauCandidateNSVfitHistManagerForElecTau = copy.deepcopy(diTauCandidateNSVfitHistManager)
diTauCandidateNSVfitHistManagerForElecTau.pluginName = cms.string('diTauCandidateNSVfitHistManagerForElecTau')
diTauCandidateNSVfitHistManagerForElecTau.pluginType = cms.string('PATElecTauPairNSVfitHistManager')
diTauCandidateNSVfitHistManagerForElecTau.diTauCandidateSource = cms.InputTag('allElecTauPairs')
diTauCandidateNSVfitHistManagerForElecTau.nSVfitEventHypotheses = cms.PSet(
    psKine_MEt_logM_fit = cms.string('psKine_MEt_logM_fit')
    #    psKine_MEt_logM_int = cms.string('psKine_MEt_logM_int')
)

from TauAnalysis.Core.diTauCandidateZllHypothesisHistManager_cfi import *
diTauCandidateZeeHypothesisHistManagerForElecTau = copy.deepcopy(ZllHypothesisHistManager)
diTauCandidateZeeHypothesisHistManagerForElecTau.pluginName = cms.string('diTauCandidateZeeHypothesisHistManagerForElecTau')
diTauCandidateZeeHypothesisHistManagerForElecTau.pluginType = cms.string('ZllHypothesisElecTauHistManager')
diTauCandidateZeeHypothesisHistManagerForElecTau.ZllHypothesisSource = cms.InputTag('elecTauPairZeeHypothesesForAHtoElecTau')
diTauCandidateZeeHypothesisHistManagerForElecTau.dqmDirectory_store = cms.string('DiTauCandidateZeeHypothesisQuantities')

# import config for Zee veto histogram manager
elecPairHistManagerByLooseIsolation = diTauCandidateHistManager.clone(
	pluginName = cms.string('elecPairHistManagerByLooseIsolation'),
	pluginType = cms.string('PATDiElecPairHistManager'),
	diTauCandidateSource = cms.InputTag('allDiElecPairZeeHypothesesByLooseIsolation'),
	dqmDirectory_store = cms.string('DiElecZeeHypothesisByLooseIsolationQuantities')
)

# import config for VBF event hypothesis histogram manager
from TauAnalysis.Core.vbfDiTauCandidateEventHistManager_cfi import *
vbfEventHistManagerForElecTau = copy.deepcopy(vbfDiTauCandidateEventHistManager)
vbfEventHistManagerForElecTau.pluginName = cms.string('vbfEventHistManagerForElecTau')
vbfEventHistManagerForElecTau.pluginType = cms.string('PATElecTauPairVBFEventHistManager')
vbfEventHistManagerForElecTau.vbfEventSource = cms.InputTag('allVBFEventHypothesesForAHtoElecTau')

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
vertexHistManager.vertexSource = cms.InputTag('offlinePrimaryVerticesWithBS')

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
    'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v2',
    'HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1',
    'HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1'
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
        diTauCandidateNSVfitHistManagerForElecTau
    ),
    dqmDirectory_store = cms.string('diTauLeg1ChargeBinnedHistograms')
)
