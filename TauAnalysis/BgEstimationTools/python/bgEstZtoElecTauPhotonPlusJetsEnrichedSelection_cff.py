import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *
from TauAnalysis.RecoTools.tools.eventSelFlagProdConfigurator import *

#--------------------------------------------------------------------------------
# select photon + jet  background enriched event sample
#--------------------------------------------------------------------------------

from TauAnalysis.BgEstimationTools.bgEstZtoElecTauZtautauEnrichedSelection_cff import *

#--------------------------------------------------------------------------------  
# produce collection of pat::Electrons
#--------------------------------------------------------------------------------

# require electron candidate to pass the eidRobustTight electron id. criteria
electronsBgEstPhotonPlusJetsEnrichedId = copy.deepcopy(electronsBgEstZtautauEnrichedId)

# require electron candidate to not be within eta-crack
# between Barrel and Encap ECAL calorimeter
electronsBgEstPhotonPlusJetsEnrichedAntiCrackCut = copy.deepcopy(electronsBgEstZtautauEnrichedAntiCrackCut)

# require electron candidate to be within geometric acceptance of electron trigger
electronsBgEstPhotonPlusJetsEnrichedEta = copy.deepcopy(electronsBgEstZtautauEnrichedEta)

# require electron candidate to have transverse momentum above threshold
electronsBgEstPhotonPlusJetsEnrichedPt = copy.deepcopy(electronsBgEstZtautauEnrichedPt)


electronsBgEstPhotonPlusJetsEnrichedIso = copy.deepcopy(electronsBgEstZtautauEnrichedIso)
electronsBgEstPhotonPlusJetsEnrichedIso.sumPtMinEB = cms.double(0.01)
electronsBgEstPhotonPlusJetsEnrichedIso.sumPtMaxEB = cms.double(0.05)
electronsBgEstPhotonPlusJetsEnrichedIso.sumPtMinEE = cms.double(0.01)
electronsBgEstPhotonPlusJetsEnrichedIso.sumPtMaxEE = cms.double(0.05)



# require electron to not be from a photon conversion
electronsBgEstPhotonPlusJetsEnrichedConversionVeto = copy.deepcopy(electronsBgEstZtautauEnrichedConversionVeto)
electronsBgEstPhotonPlusJetsEnrichedConversionVeto.src = cms.InputTag('electronsBgEstPhotonPlusJetsEnrichedIsoCumulative')

produceNonConversionElectronsBgEstZeeEnriched =  cms.Sequence(electronsBgEstPhotonPlusJetsEnrichedConversionVeto)

electronsBgEstPhotonPlusJetsEnrichedTrkIP = copy.deepcopy(electronsBgEstZtautauEnrichedTrkIP)

electronSelConfiguratorBgEstPhotonPlusJetsEnriched = objSelConfigurator(
    [ electronsBgEstPhotonPlusJetsEnrichedId,
      electronsBgEstPhotonPlusJetsEnrichedAntiCrackCut,
      electronsBgEstPhotonPlusJetsEnrichedEta,
      electronsBgEstPhotonPlusJetsEnrichedPt,
      electronsBgEstPhotonPlusJetsEnrichedIso,
      electronsBgEstPhotonPlusJetsEnrichedTrkIP
      #electronsBgEstPhotonPlusJetsEnrichedConversionVeto
      ],
    src = "cleanPatElectrons",
    pyModuleName = __name__,
    doSelIndividual = False
)


selectElectronsBgEstPhotonPlusJetsEnriched = electronSelConfiguratorBgEstPhotonPlusJetsEnriched.configure(pyNameSpace = locals())

   
#--------------------------------------------------------------------------------  
# produce collection of pat::Taus
#--------------------------------------------------------------------------------

# require tau candidate not to overlap with selected electrons
# (in order to avoid double-counting one and the same physical particle
#  as electron and as tau candidate)
tausBgEstPhotonPlusJetsEnrichedAntiOverlapWithElectronsVeto = copy.deepcopy(tausBgEstZtautauEnrichedAntiOverlapWithElectronsVeto)
tausBgEstPhotonPlusJetsEnrichedAntiOverlapWithElectronsVeto.srcNotToBeFiltered = cms.VInputTag("electronsBgEstPhotonPlusJetsEnrichedPtCumulative")

# require tau candidate to be within geometric acceptance of Pixel + SiTracker detectors
tausBgEstPhotonPlusJetsEnrichedEta = copy.deepcopy(tausBgEstZtautauEnrichedEta)

# require tau candidate to have transverse energy above threshold
tausBgEstPhotonPlusJetsEnrichedPt = copy.deepcopy(tausBgEstZtautauEnrichedPt)

# require leading track of tau candidate to have Pt > 5. GeV
tausBgEstPhotonPlusJetsEnrichedLeadTrkPt = copy.deepcopy(tausBgEstZtautauEnrichedLeadTrkPt)

# require tau candidate to pass TaNC discriminator
tausBgEstPhotonPlusJetsEnrichedTaNCdiscr = copy.deepcopy(tausBgEstZtautauEnrichedTaNCdiscr)
#tausBgEstPhotonPlusJetsEnrichedTaNCdiscr.cut = cms.string('tauID("byLooseIsolation") > 0.5')
#tausBgEstPhotonPlusJetsEnrichedTaNCdiscr.cut = cms.string('tauID("byTaNCfrOnePercent") > 0.5')

# require tau candidate to have either one or three tracks within signal cone
tausBgEstPhotonPlusJetsEnrichedProng = copy.deepcopy(tausBgEstZtautauEnrichedProng)
tausBgEstPhotonPlusJetsEnrichedProng.cut = cms.string("signalPFChargedHadrCands.size() != -1")

# require tau candidate to have charge either +1 or -1
# (computed as sum of charges of tracks within signal cone)
tausBgEstPhotonPlusJetsEnrichedCharge = copy.deepcopy(tausBgEstZtautauEnrichedCharge)
tausBgEstPhotonPlusJetsEnrichedCharge.cut = cms.string('abs(charge) > -1')

# require tau candidate to pass electron veto
tausBgEstPhotonPlusJetsEnrichedElectronVeto = copy.deepcopy(tausBgEstZtautauEnrichedElectronVeto)

# require tau candidate not to be in ECAL barrel/endcap crack
tausBgEstPhotonPlusJetsEnrichedEcalCrackVeto = copy.deepcopy(tausBgEstZtautauEnrichedEcalCrackVeto)

# require tau candidate to pass muon veto
tausBgEstPhotonPlusJetsEnrichedMuonVeto = copy.deepcopy(tausBgEstZtautauEnrichedMuonVeto)



tauSelConfiguratorBgEstPhotonPlusJetsEnriched = objSelConfigurator(
    [ tausBgEstPhotonPlusJetsEnrichedAntiOverlapWithElectronsVeto,
      tausBgEstPhotonPlusJetsEnrichedEta,
      tausBgEstPhotonPlusJetsEnrichedPt,
      tausBgEstPhotonPlusJetsEnrichedLeadTrkPt,
      tausBgEstPhotonPlusJetsEnrichedTaNCdiscr,
      tausBgEstPhotonPlusJetsEnrichedProng,
      tausBgEstPhotonPlusJetsEnrichedCharge,
      tausBgEstPhotonPlusJetsEnrichedElectronVeto,
      tausBgEstPhotonPlusJetsEnrichedEcalCrackVeto,
      tausBgEstPhotonPlusJetsEnrichedMuonVeto ],
    src = "cleanPatTaus",
    pyModuleName = __name__,
    doSelIndividual = False
)

selectTausBgEstPhotonPlusJetsEnriched = tauSelConfiguratorBgEstPhotonPlusJetsEnriched.configure(pyNameSpace = locals())


#--------------------------------------------------------------------------------  
# produce collection of electron + tau-jet combinations
#--------------------------------------------------------------------------------

### production


elecTauPairsBgEstPhotonPlusJetsEnriched = copy.deepcopy(elecTauPairsBgEstZtautauEnriched)
elecTauPairsBgEstPhotonPlusJetsEnriched.srcLeg1 = cms.InputTag('electronsBgEstPhotonPlusJetsEnrichedTrkIPcumulative')
elecTauPairsBgEstPhotonPlusJetsEnriched.srcLeg2 = cms.InputTag('tausBgEstPhotonPlusJetsEnrichedMuonVetoCumulative')

produceElecTauPairsBgEstPhotonPlusJetsEnriched = cms.Sequence(elecTauPairsBgEstPhotonPlusJetsEnriched)

### selection

elecTauPairsBgEstPhotonPlusJetsEnrichedAntiOverlapVeto = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedAntiOverlapVeto)
elecTauPairsBgEstPhotonPlusJetsEnrichedMt1MET = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedMt1MET)
elecTauPairsBgEstPhotonPlusJetsEnrichedMt1MET.cut = cms.string('mt1MET < 500.')
elecTauPairsBgEstPhotonPlusJetsEnrichedPzetaDiff = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedPzetaDiff)
elecTauPairsBgEstPhotonPlusJetsEnrichedPzetaDiff.cut = cms.string('(pZeta - 1.5*pZetaVis) > -2000.')
elecTauPairsBgEstPhotonPlusJetsEnrichedZeroCharge = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedZeroCharge)
elecTauPairsBgEstPhotonPlusJetsEnrichedZeroCharge.cut = cms.string('charge != 10')

elecTauPairSelConfiguratorBgEstPhotonPlusJetsEnriched = objSelConfigurator(
    [ elecTauPairsBgEstPhotonPlusJetsEnrichedAntiOverlapVeto,
      elecTauPairsBgEstPhotonPlusJetsEnrichedMt1MET,
      elecTauPairsBgEstPhotonPlusJetsEnrichedPzetaDiff,
      elecTauPairsBgEstPhotonPlusJetsEnrichedZeroCharge
      ],
    src = "elecTauPairsBgEstPhotonPlusJetsEnriched",
    pyModuleName = __name__,
    doSelIndividual = False
)

selectElecTauPairsBgEstPhotonPlusJetsEnriched = elecTauPairSelConfiguratorBgEstPhotonPlusJetsEnriched.configure(pyNameSpace = locals())

####### anti Zee Cut #######

### production

allDiElecPairBgEstPhotonPlusJetsEnrichedZeeHypothesesByLooseIsolation = copy.deepcopy(allDiElecPairBgEstZtautauEnrichedZeeHypothesesByLooseIsolation)
allDiElecPairBgEstPhotonPlusJetsEnrichedZeeHypothesesByLooseIsolation.srcLeg1 = cms.InputTag("electronsBgEstPhotonPlusJetsEnrichedTrkIPcumulative")

### selection

selectedDiElecPairBgEstPhotonPlusJetsEnrichedZeeHypothesesByLooseIsolation = copy.deepcopy(selectedDiElecPairBgEstZtautauEnrichedZeeHypothesesByLooseIsolation)
selectedDiElecPairBgEstPhotonPlusJetsEnrichedZeeHypothesesByLooseIsolation.src = cms.InputTag("allDiElecPairBgEstPhotonPlusJetsEnrichedZeeHypothesesByLooseIsolation")

produceElecTauPairZeeHypothesesBgEstPhotonPlusJetsEnriched = cms.Sequence(
	selectedPatElectronsForZeeHypotheses * 
	allDiElecPairBgEstPhotonPlusJetsEnrichedZeeHypothesesByLooseIsolation *
	selectedDiElecPairBgEstPhotonPlusJetsEnrichedZeeHypothesesByLooseIsolation
)


#--------------------------------------------------------------------------------  
# produce boolean event selection flags
#--------------------------------------------------------------------------------


# electron cuts

cfgElectronIdCutBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgElectronIdCutBgEstZtautauEnriched)
cfgElectronIdCutBgEstPhotonPlusJetsEnriched.pluginName = cms.string('electronIdCutBgEstPhotonPlusJetsEnriched')
cfgElectronIdCutBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('electronsBgEstPhotonPlusJetsEnrichedIdCumulative')

cfgElectronAntiCrackCutBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgElectronAntiCrackCutBgEstZtautauEnriched)
cfgElectronAntiCrackCutBgEstPhotonPlusJetsEnriched.pluginName = cms.string('electronAntiCrackCutBgEstPhotonPlusJetsEnriched')
cfgElectronAntiCrackCutBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('electronsBgEstPhotonPlusJetsEnrichedAntiCrackCutCumulative')

cfgElectronEtaCutBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgElectronEtaCutBgEstZtautauEnriched)
cfgElectronEtaCutBgEstPhotonPlusJetsEnriched.pluginName = cms.string('electronEtaCutBgEstPhotonPlusJetsEnriched')
cfgElectronEtaCutBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('electronsBgEstPhotonPlusJetsEnrichedEtaCumulative')

cfgElectronPtCutBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgElectronPtCutBgEstZtautauEnriched)
cfgElectronPtCutBgEstPhotonPlusJetsEnriched.pluginName = cms.string('electronPtCutBgEstPhotonPlusJetsEnriched')
cfgElectronPtCutBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('electronsBgEstPhotonPlusJetsEnrichedPtCumulative')

cfgElectronIsoCutBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgElectronIsoCutBgEstZtautauEnriched)
cfgElectronIsoCutBgEstPhotonPlusJetsEnriched.pluginName = cms.string('electronIsoCutBgEstPhotonPlusJetsEnriched')
cfgElectronIsoCutBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('electronsBgEstPhotonPlusJetsEnrichedIsoCumulative')

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut
cfgElectronConversionVetoBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgElectronConversionVetoBgEstZtautauEnriched)
cfgElectronConversionVetoBgEstPhotonPlusJetsEnriched.pluginName = cms.string('electronConversionVetoBgEstPhotonPlusJetsEnriched')
cfgElectronConversionVetoBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('electronsBgEstPhotonPlusJetsEnrichedConversionVeto')
cfgElectronConversionVetoBgEstPhotonPlusJetsEnriched.pluginType = cms.string('PATCandViewMaxEventSelector')
cfgElectronConversionVetoBgEstPhotonPlusJetsEnriched.maxNumber = cms.uint32(0)

cfgElectronTrkIPcutBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgElectronTrkIPcutBgEstZtautauEnriched)
cfgElectronTrkIPcutBgEstPhotonPlusJetsEnriched.pluginName = cms.string('electronTrkIPcutBgEstPhotonPlusJetsEnriched')
cfgElectronTrkIPcutBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('electronsBgEstPhotonPlusJetsEnrichedTrkIPcumulative')


# tau cuts

cfgTauAntiOverlapWithElectronsVetoBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgTauAntiOverlapWithElectronsVetoBgEstZtautauEnriched)
cfgTauAntiOverlapWithElectronsVetoBgEstPhotonPlusJetsEnriched.pluginName = cms.string('tauAntiOverlapWithElectronsVetoBgEstPhotonPlusJetsEnriched')
cfgTauAntiOverlapWithElectronsVetoBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstPhotonPlusJetsEnrichedAntiOverlapWithElectronsVetoCumulative')

cfgTauEtaCutBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgTauEtaCutBgEstZtautauEnriched)
cfgTauEtaCutBgEstPhotonPlusJetsEnriched.pluginName = cms.string('tauEtaCutBgEstPhotonPlusJetsEnriched')
cfgTauEtaCutBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstPhotonPlusJetsEnrichedEtaCumulative')

cfgTauPtCutBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgTauPtCutBgEstZtautauEnriched)
cfgTauPtCutBgEstPhotonPlusJetsEnriched.pluginName = cms.string('tauPtCutBgEstPhotonPlusJetsEnriched')
cfgTauPtCutBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstPhotonPlusJetsEnrichedPtCumulative')

cfgTauLeadTrkPtCutBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgTauLeadTrkPtCutBgEstZtautauEnriched)
cfgTauLeadTrkPtCutBgEstPhotonPlusJetsEnriched.pluginName = cms.string('tauLeadTrkPtCutBgEstPhotonPlusJetsEnriched')
cfgTauLeadTrkPtCutBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstPhotonPlusJetsEnrichedLeadTrkPtCumulative')

cfgTauTaNCdiscrCutBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgTauTaNCdiscrCutBgEstZtautauEnriched)
cfgTauTaNCdiscrCutBgEstPhotonPlusJetsEnriched.pluginName = cms.string('tauTaNCdiscrCutBgEstPhotonPlusJetsEnriched')
cfgTauTaNCdiscrCutBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstPhotonPlusJetsEnrichedTaNCdiscrCumulative')

cfgTauProngCutBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgTauProngCutBgEstZtautauEnriched)
cfgTauProngCutBgEstPhotonPlusJetsEnriched.pluginName = cms.string('tauProngCutBgEstPhotonPlusJetsEnriched')
cfgTauProngCutBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstPhotonPlusJetsEnrichedProngCumulative')

cfgTauChargeCutBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgTauChargeCutBgEstZtautauEnriched)
cfgTauChargeCutBgEstPhotonPlusJetsEnriched.pluginName = cms.string('tauChargeCutBgEstPhotonPlusJetsEnriched')
cfgTauChargeCutBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstPhotonPlusJetsEnrichedChargeCumulative')

cfgTauElectronVetoBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgTauElectronVetoBgEstZtautauEnriched)
cfgTauElectronVetoBgEstPhotonPlusJetsEnriched.pluginName = cms.string('tauElectronVetoBgEstPhotonPlusJetsEnriched')
cfgTauElectronVetoBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstPhotonPlusJetsEnrichedElectronVetoCumulative')

cfgTauEcalCrackVetoBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgTauEcalCrackVetoBgEstZtautauEnriched)
cfgTauEcalCrackVetoBgEstPhotonPlusJetsEnriched.pluginName = cms.string('tauEcalCrackVetoBgEstPhotonPlusJetsEnriched')
cfgTauEcalCrackVetoBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstPhotonPlusJetsEnrichedEcalCrackVetoCumulative')

cfgTauMuonVetoBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgTauMuonVetoBgEstZtautauEnriched)
cfgTauMuonVetoBgEstPhotonPlusJetsEnriched.pluginName = cms.string('tauMuonVetoBgEstPhotonPlusJetsEnriched')
cfgTauMuonVetoBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstPhotonPlusJetsEnrichedMuonVetoCumulative')

# di-tau candidate selection

cfgDiTauCandidateAntiOverlapVetoBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgDiTauCandidateAntiOverlapVetoBgEstZtautauEnriched)
cfgDiTauCandidateAntiOverlapVetoBgEstPhotonPlusJetsEnriched.pluginName = cms.string('diTauCandidateAntiOverlapVetoBgEstPhotonPlusJetsEnriched')
cfgDiTauCandidateAntiOverlapVetoBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('elecTauPairsBgEstPhotonPlusJetsEnrichedAntiOverlapVetoCumulative')

cfgDiTauCandidateForElecTauMt1METCutBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgDiTauCandidateForElecTauMt1METCutBgEstZtautauEnriched)
cfgDiTauCandidateForElecTauMt1METCutBgEstPhotonPlusJetsEnriched.pluginName = cms.string('diTauCandidateMt1METCutBgEstPhotonPlusJetsEnriched')
cfgDiTauCandidateForElecTauMt1METCutBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('elecTauPairsBgEstPhotonPlusJetsEnrichedMt1METcumulative')

cfgDiTauCandidateForElecTauPzetaDiffCutBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgDiTauCandidateForElecTauPzetaDiffCutBgEstZtautauEnriched)
cfgDiTauCandidateForElecTauPzetaDiffCutBgEstPhotonPlusJetsEnriched.pluginName = cms.string('diTauCandidatePzetaDiffCutBgEstPhotonPlusJetsEnriched')
cfgDiTauCandidateForElecTauPzetaDiffCutBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('elecTauPairsBgEstPhotonPlusJetsEnrichedPzetaDiffCumulative')

cfgDiTauCandidateForElecTauZeroChargeCutBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgDiTauCandidateForElecTauZeroChargeCutBgEstZtautauEnriched)
cfgDiTauCandidateForElecTauZeroChargeCutBgEstPhotonPlusJetsEnriched.pluginName = cms.string('diTauCandidateZeroChargeCutBgEstPhotonPlusJetsEnriched')
cfgDiTauCandidateForElecTauZeroChargeCutBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('elecTauPairsBgEstPhotonPlusJetsEnrichedZeroChargeCumulative')

cfgElecTauPairZeeHypothesisVetoBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgElecTauPairZeeHypothesisVetoBgEstZtautauEnriched)
cfgElecTauPairZeeHypothesisVetoBgEstPhotonPlusJetsEnriched.pluginName = cms.string('diTauCandidateZeeHypothesisVetoBgEstPhotonPlusJetsEnriched')
cfgElecTauPairZeeHypothesisVetoBgEstPhotonPlusJetsEnriched.src = cms.InputTag('selectedDiElecPairBgEstPhotonPlusJetsEnrichedZeeHypothesesByLooseIsolation')


evtSelConfiguratorBgEstPhotonPlusJetsEnriched = eventSelFlagProdConfigurator(
    [ cfgElectronIdCutBgEstPhotonPlusJetsEnriched,
      cfgElectronAntiCrackCutBgEstPhotonPlusJetsEnriched,
      cfgElectronEtaCutBgEstPhotonPlusJetsEnriched,
      cfgElectronPtCutBgEstPhotonPlusJetsEnriched,
      cfgElectronIsoCutBgEstPhotonPlusJetsEnriched,
      cfgElectronConversionVetoBgEstPhotonPlusJetsEnriched,
      cfgElectronTrkIPcutBgEstPhotonPlusJetsEnriched,
      cfgTauAntiOverlapWithElectronsVetoBgEstPhotonPlusJetsEnriched,
      cfgTauEtaCutBgEstPhotonPlusJetsEnriched,
      cfgTauPtCutBgEstPhotonPlusJetsEnriched,
      cfgTauLeadTrkPtCutBgEstPhotonPlusJetsEnriched,
      cfgTauTaNCdiscrCutBgEstPhotonPlusJetsEnriched,
      cfgTauProngCutBgEstPhotonPlusJetsEnriched,
      cfgTauChargeCutBgEstPhotonPlusJetsEnriched,
      cfgTauElectronVetoBgEstPhotonPlusJetsEnriched,
      cfgTauEcalCrackVetoBgEstPhotonPlusJetsEnriched,
      cfgTauMuonVetoBgEstPhotonPlusJetsEnriched,
      cfgDiTauCandidateAntiOverlapVetoBgEstPhotonPlusJetsEnriched,
      cfgDiTauCandidateForElecTauMt1METCutBgEstPhotonPlusJetsEnriched,
      cfgDiTauCandidateForElecTauPzetaDiffCutBgEstPhotonPlusJetsEnriched,
      cfgDiTauCandidateForElecTauZeroChargeCutBgEstPhotonPlusJetsEnriched,
      cfgElecTauPairZeeHypothesisVetoBgEstPhotonPlusJetsEnriched
      ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

selectEventsBgEstPhotonPlusJetsEnriched = evtSelConfiguratorBgEstPhotonPlusJetsEnriched.configure()


#--------------------------------------------------------------------------------  
# apply event selection criteria; fill histograms
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.analyzeZtoElecTau_cff import *


diTauCandidateHistManagerForElecTauBgEstPhotonPlusJetsEnriched = copy.deepcopy(diTauCandidateHistManagerForElecTauBgEstZtautauEnriched)
diTauCandidateHistManagerForElecTauBgEstPhotonPlusJetsEnriched.pluginName = cms.string('diTauCandidateHistManagerForElecTauBgEstPhotonPlusJetsEnriched')
diTauCandidateHistManagerForElecTauBgEstPhotonPlusJetsEnriched.diTauCandidateSource = cms.InputTag('elecTauPairsBgEstPhotonPlusJetsEnriched')

electronHistManagerForElecTauBgEstPhotonPlusJetsEnriched = copy.deepcopy(electronHistManagerForElecTauBgEstZtautauEnriched)
electronHistManagerForElecTauBgEstPhotonPlusJetsEnriched.pluginName = cms.string('electronHistManagerForElecTauBgEstPhotonPlusJetsEnriched')

tauHistManagerForElecTauBgEstPhotonPlusJetsEnriched = copy.deepcopy(tauHistManagerForElecTauBgEstZtautauEnriched)
tauHistManagerForElecTauBgEstPhotonPlusJetsEnriched.pluginName = cms.string('tauHistManagerForElecTauBgEstPhotonPlusJetsEnriched')
tauHistManagerForElecTauBgEstPhotonPlusJetsEnriched.jetSource = cms.InputTag('selectedPatJets')


analyzeEventsBgEstPhotonPlusJetsEnriched = cms.EDAnalyzer("GenericAnalyzer",

    name = cms.string('BgEstTemplateAnalyzer_PhotonPlusJetsEnriched'), 

    filters = cms.VPSet(
#        evtSelGenPhaseSpace,
        evtSelTrigger,
        evtSelPrimaryEventVertex,
        evtSelPrimaryEventVertexQuality,
        evtSelPrimaryEventVertexPosition,

        #start electron cuts
        
        cms.PSet(
            pluginName = cms.string('electronIdCutBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronIdCutBgEstPhotonPlusJetsEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('electronAntiCrackCutBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronAntiCrackCutBgEstPhotonPlusJetsEnriched','cumulative')
        ),        
        cms.PSet(
            pluginName = cms.string('electronEtaCutBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronEtaCutBgEstPhotonPlusJetsEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('electronPtCutBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronPtCutBgEstPhotonPlusJetsEnriched','cumulative')
        ),        
        cms.PSet(
            pluginName = cms.string('electronIsoCutBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronIsoCutBgEstPhotonPlusJetsEnriched','cumulative')
        ),
        #!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut
        cms.PSet(
            pluginName = cms.string('electronConversionVetoBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronConversionVetoBgEstPhotonPlusJetsEnriched','cumulative')
        ),  
        cms.PSet(
            pluginName = cms.string('electronTrkIPcutBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronTrkIPcutBgEstPhotonPlusJetsEnriched','cumulative')
        ),

        #start tau cuts
        
        cms.PSet(
            pluginName = cms.string('tauAntiOverlapWithElectronsVetoBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauAntiOverlapWithElectronsVetoBgEstPhotonPlusJetsEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauEtaCutBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauEtaCutBgEstPhotonPlusJetsEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauPtCutBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauPtCutBgEstPhotonPlusJetsEnriched','cumulative')
        ),    
        cms.PSet(
            pluginName = cms.string('tauLeadTrkPtCutBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauLeadTrkPtCutBgEstPhotonPlusJetsEnriched','cumulative')
        ),  
        cms.PSet(
            pluginName = cms.string('tauTaNCdiscrCutBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauTaNCdiscrCutBgEstPhotonPlusJetsEnriched','cumulative')
        ), 
        cms.PSet(
            pluginName = cms.string('tauProngCutBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauProngCutBgEstPhotonPlusJetsEnriched','cumulative')
        ),  
        cms.PSet(
            pluginName = cms.string('tauChargeCutBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauChargeCutBgEstPhotonPlusJetsEnriched','cumulative')
        ), 
        cms.PSet(
            pluginName = cms.string('tauElectronVetoBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauElectronVetoBgEstPhotonPlusJetsEnriched','cumulative')
        ),  
        cms.PSet(
            pluginName = cms.string('tauEcalCrackVetoBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauEcalCrackVetoBgEstPhotonPlusJetsEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauMuonVetoBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauMuonVetoBgEstPhotonPlusJetsEnriched','cumulative')
        ),

        #start ditau cuts
        
        cms.PSet(
            pluginName = cms.string('diTauCandidateAntiOverlapVetoBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateAntiOverlapVetoBgEstPhotonPlusJetsEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('diTauCandidateMt1METCutBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateMt1METCutBgEstPhotonPlusJetsEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('diTauCandidatePzetaDiffCutBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidatePzetaDiffCutBgEstPhotonPlusJetsEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('diTauCandidateZeroChargeCutBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateZeroChargeCutBgEstPhotonPlusJetsEnriched','cumulative')
        ),        
        cms.PSet(
            pluginName = cms.string('diTauCandidateZeeHypothesisVetoBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateZeeHypothesisVetoBgEstPhotonPlusJetsEnriched')
        )
        
        
    ),
  
    analyzers = cms.VPSet(
         diTauCandidateHistManagerForElecTauBgEstPhotonPlusJetsEnriched,
         electronHistManagerForElecTauBgEstPhotonPlusJetsEnriched,
         tauHistManagerForElecTauBgEstPhotonPlusJetsEnriched,
    ),

    eventDumps = cms.VPSet(),
   
    analysisSequence = cms.VPSet(
    
        # generator level phase-space selection
        # (NOTE: (1) to be used in case of Monte Carlo samples
        #            overlapping in simulated phase-space only !!
        #        (2) genPhaseSpaceCut needs to be **always** the first entry in the list of cuts
        #           - otherwise the script submitToBatch.csh for submission of cmsRun jobs
        #            to the CERN batch system will not work !!)
##         cms.PSet(
##             filter = cms.string('evtSelGenPhaseSpace'),
##             title = cms.string('gen. Phase-Space')
##         ),
        cms.PSet(
            filter = cms.string('evtSelTrigger'),
            title = cms.string('Trigger')
        ),
        cms.PSet(
            filter = cms.string('evtSelPrimaryEventVertex'),
            title = cms.string('Vertex exists')
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
            filter = cms.string('electronIdCutBgEstPhotonPlusJetsEnriched'),
            title = cms.string('Electron ID'),
        ),
        cms.PSet(
            filter = cms.string('electronAntiCrackCutBgEstPhotonPlusJetsEnriched'),
            title = cms.string('Electron crack-Veto'),
        ),
        cms.PSet(
            filter = cms.string('electronEtaCutBgEstPhotonPlusJetsEnriched'),
            title = cms.string('-2.1 < eta(Electron) < +2.1'),
        ),
        cms.PSet(
            filter = cms.string('electronPtCutBgEstPhotonPlusJetsEnriched'),
            title = cms.string('Pt(Electron) > 15 GeV'),
        ),
        cms.PSet(
            filter = cms.string('tauAntiOverlapWithElectronsVetoBgEstPhotonPlusJetsEnriched'),
            title = cms.string('Tau not overlapping with Elec.'),
        ),
        cms.PSet(
            filter = cms.string('tauEtaCutBgEstPhotonPlusJetsEnriched'),
            title = cms.string('-2.3 < eta(Tau) < +2.3'),
        ),
        cms.PSet(
            filter = cms.string('tauPtCutBgEstPhotonPlusJetsEnriched'),
            title = cms.string('Pt(Tau) > 20 GeV'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    'tauHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    ),
            replace = cms.vstring('electronHistManagerForElecTauBgEstPhotonPlusJetsEnriched.electronSource = electronsBgEstPhotonPlusJetsEnrichedPtCumulative',
                                  'tauHistManagerForElecTauBgEstPhotonPlusJetsEnriched.tauSource = tausBgEstPhotonPlusJetsEnrichedPtCumulative'
                                  )
        ),        
        cms.PSet(
            filter = cms.string('electronIsoCutBgEstPhotonPlusJetsEnriched'),
            title = cms.string('Electron Isolation'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    'tauHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    ),
            replace = cms.vstring('electronHistManagerForElecTauBgEstPhotonPlusJetsEnriched.electronSource = electronsBgEstPhotonPlusJetsEnrichedIsoCumulative')
        ),        
        #!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut
        cms.PSet(
            filter = cms.string('electronConversionVetoBgEstPhotonPlusJetsEnriched'),
            title = cms.string('Electron Track conversion veto (inverted)'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    'tauHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    ),
            replace = cms.vstring('electronHistManagerForElecTauBgEstPhotonPlusJetsEnriched.electronSource = electronsBgEstPhotonPlusJetsEnrichedConversionVeto')
        ),         
        cms.PSet(
            filter = cms.string('electronTrkIPcutBgEstPhotonPlusJetsEnriched'),
            title = cms.string('Electron Track IP'),
        ),  
        cms.PSet(
            filter = cms.string('tauLeadTrkPtCutBgEstPhotonPlusJetsEnriched'),
            title = cms.string('Tau lead. Track Pt'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    'tauHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    ),
            replace = cms.vstring('electronHistManagerForElecTauBgEstPhotonPlusJetsEnriched.electronSource = electronsBgEstPhotonPlusJetsEnrichedTrkIPcumulative',
                                  'tauHistManagerForElecTauBgEstPhotonPlusJetsEnriched.tauSource = tausBgEstPhotonPlusJetsEnrichedLeadTrkPtCumulative')
        ),         
        cms.PSet(
            filter = cms.string('tauTaNCdiscrCutBgEstPhotonPlusJetsEnriched'),
            title = cms.string('Tau TaNC by Loose Isolation'),
            #title = cms.string('Tau TaNC by 1%'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    'tauHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    ),
            replace = cms.vstring('tauHistManagerForElecTauBgEstPhotonPlusJetsEnriched.tauSource = tausBgEstPhotonPlusJetsEnrichedTaNCdiscrCumulative')
        ),          

        cms.PSet(
            filter = cms.string('tauProngCutBgEstPhotonPlusJetsEnriched'),
            title = cms.string('Tau 1||3-Prong (off)'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    'tauHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    ),
            replace = cms.vstring('tauHistManagerForElecTauBgEstPhotonPlusJetsEnriched.tauSource = tausBgEstPhotonPlusJetsEnrichedProngCumulative')
        ),          
        cms.PSet(
            filter = cms.string('tauChargeCutBgEstPhotonPlusJetsEnriched'),
            title = cms.string('Charge(Tau) = +/-1 (off)'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    'tauHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    ),
            replace = cms.vstring('tauHistManagerForElecTauBgEstPhotonPlusJetsEnriched.tauSource = tausBgEstPhotonPlusJetsEnrichedChargeCumulative')
        ),           
        cms.PSet(
            filter = cms.string('tauElectronVetoBgEstPhotonPlusJetsEnriched'),
            title = cms.string('Tau e-Veto'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    'tauHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    ),
            replace = cms.vstring('tauHistManagerForElecTauBgEstPhotonPlusJetsEnriched.tauSource = tausBgEstPhotonPlusJetsEnrichedElectronVetoCumulative')
        ),         
        cms.PSet(
            filter = cms.string('tauEcalCrackVetoBgEstPhotonPlusJetsEnriched'),
            title = cms.string('Tau ECAL crack-Veto'),
        ),
        cms.PSet(
            filter = cms.string('tauMuonVetoBgEstPhotonPlusJetsEnriched'),
            title = cms.string('Tau mu-Veto'),
        ),
        cms.PSet(
            filter = cms.string('diTauCandidateAntiOverlapVetoBgEstPhotonPlusJetsEnriched'),
            title = cms.string('dR(Electron-Tau) > 0.5'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    'tauHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    ),
            replace = cms.vstring('tauHistManagerForElecTauBgEstPhotonPlusJetsEnriched.tauSource = tausBgEstPhotonPlusJetsEnrichedMuonVetoCumulative',
                                  'diTauCandidateHistManagerForElecTauBgEstPhotonPlusJetsEnriched.diTauCandidate = elecTauPairsBgEstPhotonPlusJetsEnrichedAntiOverlapVetoCumulative'
                                  )
        ),        
        cms.PSet(
            filter = cms.string('diTauCandidateMt1METCutBgEstPhotonPlusJetsEnriched'),
            title = cms.string('M_{T}(Electron-MET) < 40 GeV (off)'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    'tauHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    ),
        ),          
        cms.PSet(
            filter = cms.string('diTauCandidatePzetaDiffCutBgEstPhotonPlusJetsEnriched'),
            title = cms.string('Pzeta-1.5*Pzeta(vis) > -20 GeV (off)'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    'tauHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    ),
        ),         
        cms.PSet(
            filter = cms.string('diTauCandidateZeroChargeCutBgEstPhotonPlusJetsEnriched'),
            title = cms.string('Charge(Electron+Tau) = 0 (off)'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    'tauHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    ),
        ),          
        cms.PSet(
            filter = cms.string('diTauCandidateZeeHypothesisVetoBgEstPhotonPlusJetsEnriched'),
            title = cms.string('no 2nd OS, loosely-isolated electron')
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    'tauHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstPhotonPlusJetsEnriched',
                                    ),
        ),          

        
    )

)

saveEvents = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('test_output.root')
)




#--------------------------------------------------------------------------------  
# define (final) analysis sequence
#--------------------------------------------------------------------------------

bgEstPhotonPlusJetsEnrichedAnalysisSequence = cms.Sequence(

    selectElectronsBgEstPhotonPlusJetsEnriched
    + selectTausBgEstPhotonPlusJetsEnriched
    + produceNonConversionElectronsBgEstZeeEnriched
    + produceElecTauPairsBgEstPhotonPlusJetsEnriched
    + selectElecTauPairsBgEstPhotonPlusJetsEnriched
    + produceElecTauPairZeeHypothesesBgEstPhotonPlusJetsEnriched
    + selectEventsBgEstPhotonPlusJetsEnriched 
    + analyzeEventsBgEstPhotonPlusJetsEnriched

)
