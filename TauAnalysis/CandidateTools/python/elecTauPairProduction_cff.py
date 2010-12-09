import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.svFitAlgorithm_cfi import *

svFitLikelihoodElecTauPairKinematicsPhaseSpace = copy.deepcopy(svFitLikelihoodDiTauKinematicsPhaseSpace)
svFitLikelihoodElecTauPairKinematicsPhaseSpace.pluginType = "SVfitLikelihoodElecTauPairKinematics"
svFitLikelihoodElecTauPairKinematicsPhaseSpace.leg1.pluginType = "SVfitElectronLikelihoodPhaseSpace"
svFitLikelihoodElecTauPairKinematicsPhaseSpace.leg2.pluginType = "SVfitTauLikelihoodPhaseSpace"

svFitLikelihoodElecTauPairMEt = copy.deepcopy(svFitLikelihoodMEt)
svFitLikelihoodElecTauPairMEt.pluginType = cms.string("SVfitLikelihoodMEtElecTau")

svFitLikelihoodElecTauPairTrackInfo = copy.deepcopy(svFitLikelihoodTrackInfo)
svFitLikelihoodElecTauPairTrackInfo.pluginType = "SVfitLikelihoodElecTauPairTrackInfo"
svFitLikelihoodElecTauPairTrackInfo.leg1.pluginType = "SVfitElectronLikelihoodTrackInfo"
svFitLikelihoodElecTauPairTrackInfo.leg2.pluginType = "SVfitTauLikelihoodTrackInfo"

svFitLikelihoodElecTauPairPtBalance = copy.deepcopy(svFitLikelihoodDiTauPtBalance)
svFitLikelihoodElecTauPairPtBalance.pluginType = cms.string("SVfitLikelihoodElecTauPairPtBalance")

#--------------------------------------------------------------------------------
# produce combinations of electron + tau-jet pairs
#--------------------------------------------------------------------------------

elecTauPairsAfterTauPt = cms.EDProducer("PATElecTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
    srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauPt15Cumulative'),
    srcLeg2 = cms.InputTag('selectedPatTausForElecTauPt20Cumulative'),
    dRmin12 = cms.double(0.3),
    srcMET = cms.InputTag('patMETs'),
    srcPrimaryVertex = cms.InputTag("offlinePrimaryVerticesWithBS"),
    srcBeamSpot = cms.InputTag("offlineBeamSpot"),                             
    srcGenParticles = cms.InputTag('genParticles'),                                   
    recoMode = cms.string(""),
    doSVreco = cms.bool(False),                          
    svFit = cms.PSet(
        psKine = cms.PSet(
            likelihoodFunctions = cms.VPSet(
                svFitLikelihoodElecTauPairKinematicsPhaseSpace         
            ),
            estUncertainties = cms.PSet(
                numSamplings = cms.int32(-1)
            )
        ),
        psKine_MEt = cms.PSet(
            likelihoodFunctions = cms.VPSet(
                svFitLikelihoodElecTauPairKinematicsPhaseSpace,
                svFitLikelihoodElecTauPairMEt
            ),
            estUncertainties = cms.PSet(
                numSamplings = cms.int32(-1)
            )
        ),
        psKine_MEt_ptBalance = cms.PSet(
            likelihoodFunctions = cms.VPSet(
                svFitLikelihoodElecTauPairKinematicsPhaseSpace,
                svFitLikelihoodElecTauPairMEt,
                svFitLikelihoodElecTauPairPtBalance
            ),
            estUncertainties = cms.PSet(
                #numSamplings = cms.int32(1000)
                numSamplings = cms.int32(-1)
            )
        )
    ),                                    
    scaleFuncImprovedCollinearApprox = cms.string('1'),                             
    verbosity = cms.untracked.int32(0)
)

elecTauPairsAfterElectronTrkIso = copy.deepcopy(elecTauPairsAfterTauPt)
elecTauPairsAfterElectronTrkIso.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauTrkIsoCumulative')
elecTauPairsAfterElectronTrkIso.srcLeg2 = cms.InputTag('selectedPatTausForElecTauPt20Cumulative')

elecTauPairsAfterElectronEcalIso = copy.deepcopy(elecTauPairsAfterTauPt)
elecTauPairsAfterElectronEcalIso.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauEcalIsoCumulative')
elecTauPairsAfterElectronEcalIso.srcLeg2 = cms.InputTag('selectedPatTausForElecTauPt20Cumulative')

elecTauPairsAfterElectronConversionVeto = copy.deepcopy(elecTauPairsAfterTauPt)
elecTauPairsAfterElectronConversionVeto.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauConversionVetoCumulative')
elecTauPairsAfterElectronConversionVeto.srcLeg2 = cms.InputTag('selectedPatTausForElecTauPt20Cumulative')

elecTauPairsAfterElectronTrkIP = copy.deepcopy(elecTauPairsAfterTauPt)
elecTauPairsAfterElectronTrkIP.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauTrkIPcumulative')
elecTauPairsAfterElectronTrkIP.srcLeg2 = cms.InputTag('selectedPatTausForElecTauPt20Cumulative')

elecTauPairsAfterTauLeadTrkFind = copy.deepcopy(elecTauPairsAfterTauPt)
elecTauPairsAfterTauLeadTrkFind.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauTrkIPcumulative')
elecTauPairsAfterTauLeadTrkFind.srcLeg2 = cms.InputTag('selectedPatTausForElecTauLeadTrkCumulative')

elecTauPairsAfterTauLeadTrkPt = copy.deepcopy(elecTauPairsAfterTauPt)
elecTauPairsAfterTauLeadTrkPt.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauTrkIPcumulative')
elecTauPairsAfterTauLeadTrkPt.srcLeg2 = cms.InputTag('selectedPatTausForElecTauLeadTrkPtCumulative')

elecTauPairsAfterTauTaNC = copy.deepcopy(elecTauPairsAfterTauPt)
elecTauPairsAfterTauTaNC.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauTrkIPcumulative')
elecTauPairsAfterTauTaNC.srcLeg2 = cms.InputTag('selectedPatTausForElecTauTaNCdiscrCumulative')

elecTauPairsAfterTauTrkIso = copy.deepcopy(elecTauPairsAfterTauPt)
elecTauPairsAfterTauTrkIso.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauTrkIPcumulative')
elecTauPairsAfterTauTrkIso.srcLeg2 = cms.InputTag('selectedPatTausForElecTauTrkIsoCumulative')

elecTauPairsAfterTauEcalIso = copy.deepcopy(elecTauPairsAfterTauPt)
elecTauPairsAfterTauEcalIso.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauTrkIPcumulative')
elecTauPairsAfterTauEcalIso.srcLeg2 = cms.InputTag('selectedPatTausForElecTauEcalIsoCumulative')

elecTauPairsAfterTauProng = copy.deepcopy(elecTauPairsAfterTauPt)
elecTauPairsAfterTauProng.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauTrkIPcumulative')
elecTauPairsAfterTauProng.srcLeg2 = cms.InputTag('selectedPatTausForElecTauProngCumulative')

elecTauPairsAfterTauCharge = copy.deepcopy(elecTauPairsAfterTauPt)
elecTauPairsAfterTauCharge.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauTrkIPcumulative')
elecTauPairsAfterTauCharge.srcLeg2 = cms.InputTag('selectedPatTausForElecTauChargeCumulative')

elecTauPairsAfterTauElectronVeto = copy.deepcopy(elecTauPairsAfterTauPt)
elecTauPairsAfterTauElectronVeto.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauTrkIPcumulative')
elecTauPairsAfterTauElectronVeto.srcLeg2 = cms.InputTag('selectedPatTausForElecTauElectronVetoCumulative')

elecTauPairsAfterTauEcalCrackVeto = copy.deepcopy(elecTauPairsAfterTauPt)
elecTauPairsAfterTauEcalCrackVeto.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauTrkIPcumulative')
elecTauPairsAfterTauEcalCrackVeto.srcLeg2 = cms.InputTag('selectedPatTausForElecTauEcalCrackVetoCumulative')

allElecTauPairs = cms.EDProducer("PATElecTauPairProducer",
    useLeadingTausOnly = cms.bool(False),
		srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauTrkIPcumulative'),
		srcLeg2 = cms.InputTag('selectedPatTausForElecTauMuonVetoCumulative'),
    dRmin12 = cms.double(0.3),
    srcMET = cms.InputTag('patMETs'),
    srcPrimaryVertex = cms.InputTag("offlinePrimaryVerticesWithBS"),
	srcBeamSpot = cms.InputTag("offlineBeamSpot"),                             
    srcGenParticles = cms.InputTag('genParticles'),                                   
    recoMode = cms.string(""),
    scaleFuncImprovedCollinearApprox = cms.string('1'),                             
    verbosity = cms.untracked.int32(0),
    doSVreco = cms.bool(True),                          
    svFit = cms.PSet(
        psKine = cms.PSet(
            likelihoodFunctions = cms.VPSet(
                svFitLikelihoodElecTauPairKinematicsPhaseSpace         
            ),
            estUncertainties = cms.PSet(
                numSamplings = cms.int32(-1)
            )
        ),
        psKine_MEt = cms.PSet(
            likelihoodFunctions = cms.VPSet(
                svFitLikelihoodElecTauPairKinematicsPhaseSpace,
                svFitLikelihoodElecTauPairMEt
            ),
            estUncertainties = cms.PSet(
                numSamplings = cms.int32(-1)
            )
        ),
        psKine_MEt_ptBalance = cms.PSet(
            likelihoodFunctions = cms.VPSet(
                svFitLikelihoodElecTauPairKinematicsPhaseSpace,
                svFitLikelihoodElecTauPairMEt,
                svFitLikelihoodElecTauPairPtBalance
            ),
            estUncertainties = cms.PSet(
                #numSamplings = cms.int32(1000)
                numSamplings = cms.int32(-1)
            )
        )
    )
)

produceElecTauPairs = cms.Sequence( elecTauPairsAfterTauPt 
				+ elecTauPairsAfterElectronTrkIso
				+ elecTauPairsAfterElectronEcalIso
				+ elecTauPairsAfterElectronConversionVeto
				+ elecTauPairsAfterElectronTrkIP
				+ elecTauPairsAfterTauLeadTrkFind
				+ elecTauPairsAfterTauLeadTrkPt
				+ elecTauPairsAfterTauTaNC
				+ elecTauPairsAfterTauTrkIso
				+ elecTauPairsAfterTauEcalIso
				+ elecTauPairsAfterTauProng
				+ elecTauPairsAfterTauCharge
				+ elecTauPairsAfterTauElectronVeto
				+ elecTauPairsAfterTauEcalCrackVeto
				+ allElecTauPairs )

# define additional collections of electron + tau-jet candidates
# with loose track and ECAL isolation (and conversion rejection) applied on electron leg
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of electron isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)

elecTauPairsAfterElectronTrkIsoLooseIsolation = copy.deepcopy(elecTauPairsAfterTauPt)
elecTauPairsAfterElectronTrkIsoLooseIsolation.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauTrkIsoLooseIsolationCumulative')
elecTauPairsAfterElectronTrkIsoLooseIsolation.srcLeg2 = cms.InputTag('selectedPatTausForElecTauPt20Cumulative')

elecTauPairsAfterElectronEcalIsoLooseIsolation = copy.deepcopy(elecTauPairsAfterTauPt)
elecTauPairsAfterElectronEcalIsoLooseIsolation.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauEcalIsoLooseIsolationCumulative')
elecTauPairsAfterElectronEcalIsoLooseIsolation.srcLeg2 = cms.InputTag('selectedPatTausForElecTauPt20Cumulative')

elecTauPairsAfterElectronConversionVetoLooseIsolation = copy.deepcopy(elecTauPairsAfterTauPt)
elecTauPairsAfterElectronConversionVetoLooseIsolation.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauConversionVetoLooseIsolationCumulative')
elecTauPairsAfterElectronConversionVetoLooseIsolation.srcLeg2 = cms.InputTag('selectedPatTausForElecTauPt20Cumulative')

elecTauPairsAfterElectronTrkIpLooseIsolation = copy.deepcopy(elecTauPairsAfterTauPt)
elecTauPairsAfterElectronTrkIpLooseIsolation.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauTrkIPlooseIsolationCumulative')
elecTauPairsAfterElectronTrkIpLooseIsolation.srcLeg2 = cms.InputTag('selectedPatTausForElecTauPt20Cumulative')

elecTauPairsAfterTauLeadTrkFindLooseIsolation = copy.deepcopy(elecTauPairsAfterTauPt)
elecTauPairsAfterTauLeadTrkFindLooseIsolation.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauTrkIPlooseIsolationCumulative')
elecTauPairsAfterTauLeadTrkFindLooseIsolation.srcLeg2 = cms.InputTag('selectedPatTausForElecTauLeadTrkCumulative')

elecTauPairsAfterTauLeadTrkPtLooseIsolation = copy.deepcopy(elecTauPairsAfterTauPt)
elecTauPairsAfterTauLeadTrkPtLooseIsolation.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauTrkIPlooseIsolationCumulative')
elecTauPairsAfterTauLeadTrkPtLooseIsolation.srcLeg2 = cms.InputTag('selectedPatTausForElecTauLeadTrkPtCumulative')

elecTauPairsAfterTauTaNCLooseIsolation = copy.deepcopy(elecTauPairsAfterTauPt)
elecTauPairsAfterTauTaNCLooseIsolation.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauTrkIPlooseIsolationCumulative')
elecTauPairsAfterTauTaNCLooseIsolation.srcLeg2 = cms.InputTag('selectedPatTausForElecTauTaNCdiscrCumulative')

elecTauPairsAfterTauTrkIsoLooseIsolation = copy.deepcopy(elecTauPairsAfterTauPt)
elecTauPairsAfterTauTrkIsoLooseIsolation.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauTrkIPlooseIsolationCumulative')
elecTauPairsAfterTauTrkIsoLooseIsolation.srcLeg2 = cms.InputTag('selectedPatTausForElecTauTrkIsoCumulative')

elecTauPairsAfterTauEcalIsoLooseIsolation = copy.deepcopy(elecTauPairsAfterTauPt)
elecTauPairsAfterTauEcalIsoLooseIsolation.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauTrkIPlooseIsolationCumulative')
elecTauPairsAfterTauEcalIsoLooseIsolation.srcLeg2 = cms.InputTag('selectedPatTausForElecTauEcalIsoCumulative')

elecTauPairsAfterTauProngLooseIsolation = copy.deepcopy(elecTauPairsAfterTauPt)
elecTauPairsAfterTauProngLooseIsolation.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauTrkIPlooseIsolationCumulative')
elecTauPairsAfterTauProngLooseIsolation.srcLeg2 = cms.InputTag('selectedPatTausForElecTauProngCumulative')

elecTauPairsAfterTauChargeLooseIsolation = copy.deepcopy(elecTauPairsAfterTauPt)
elecTauPairsAfterTauChargeLooseIsolation.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauTrkIPlooseIsolationCumulative')
elecTauPairsAfterTauChargeLooseIsolation.srcLeg2 = cms.InputTag('selectedPatTausForElecTauChargeCumulative')

elecTauPairsAfterTauElectronVetoLooseIsolation = copy.deepcopy(elecTauPairsAfterTauPt)
elecTauPairsAfterTauElectronVetoLooseIsolation.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauTrkIPlooseIsolationCumulative')
elecTauPairsAfterTauElectronVetoLooseIsolation.srcLeg2 = cms.InputTag('selectedPatTausForElecTauElectronVetoCumulative')

elecTauPairsAfterTauEcalCrackVetoLooseIsolation = copy.deepcopy(elecTauPairsAfterTauPt)
elecTauPairsAfterTauEcalCrackVetoLooseIsolation.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauTrkIPlooseIsolationCumulative')
elecTauPairsAfterTauEcalCrackVetoLooseIsolation.srcLeg2 = cms.InputTag('selectedPatTausForElecTauEcalCrackVetoCumulative')

allElecTauPairsLooseElectronIsolation = allElecTauPairs.clone()
allElecTauPairsLooseElectronIsolation.srcLeg1 = cms.InputTag('selectedPatElectronsForElecTauTrkIPlooseIsolationCumulative')

produceElecTauPairsLooseElectronIsolation = cms.Sequence( elecTauPairsAfterElectronTrkIsoLooseIsolation
						+ elecTauPairsAfterElectronEcalIsoLooseIsolation
						+ elecTauPairsAfterElectronConversionVetoLooseIsolation
						+ elecTauPairsAfterElectronTrkIpLooseIsolation
						+ elecTauPairsAfterTauLeadTrkFindLooseIsolation
						+ elecTauPairsAfterTauLeadTrkPtLooseIsolation
						+ elecTauPairsAfterTauTaNCLooseIsolation
						+ elecTauPairsAfterTauTrkIsoLooseIsolation
						+ elecTauPairsAfterTauEcalIsoLooseIsolation
						+ elecTauPairsAfterTauProngLooseIsolation
						+ elecTauPairsAfterTauChargeLooseIsolation
						+ elecTauPairsAfterTauElectronVetoLooseIsolation
						+ elecTauPairsAfterTauEcalCrackVetoLooseIsolation
						+ allElecTauPairsLooseElectronIsolation )

produceElecTauPairsAll = cms.Sequence( produceElecTauPairs * produceElecTauPairsLooseElectronIsolation )
