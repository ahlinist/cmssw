import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *
from TauAnalysis.RecoTools.tools.eventSelFlagProdConfigurator import *

#--------------------------------------------------------------------------------
# select Z --> e+ e- background enriched event sample
#--------------------------------------------------------------------------------

from TauAnalysis.BgEstimationTools.bgEstZtoElecTauZtautauEnrichedSelection_cff import *

#--------------------------------------------------------------------------------  
# produce collection of pat::Electrons
#--------------------------------------------------------------------------------

# require electron candidate to pass the eidRobustTight electron id. criteria
electronsBgEstZeeJetMisIdEnrichedId = copy.deepcopy(electronsBgEstZtautauEnrichedId)

# require electron candidate to not be within eta-crack
# between Barrel and Encap ECAL calorimeter
electronsBgEstZeeJetMisIdEnrichedAntiCrackCut = copy.deepcopy(electronsBgEstZtautauEnrichedAntiCrackCut)

# require electron candidate to be within geometric acceptance of electron trigger
electronsBgEstZeeJetMisIdEnrichedEta = copy.deepcopy(electronsBgEstZtautauEnrichedEta)

# require electron candidate to have transverse momentum above threshold
electronsBgEstZeeJetMisIdEnrichedPt = copy.deepcopy(electronsBgEstZtautauEnrichedPt)

electronsBgEstZeeJetMisIdEnrichedIso = copy.deepcopy(electronsBgEstZtautauEnrichedIso)
electronsBgEstZeeJetMisIdEnrichedIso.sumPtMaxEB = cms.double(0.05)
electronsBgEstZeeJetMisIdEnrichedIso.sumPtMaxEE = cms.double(0.05)


# require electron to not be from a photon conversion
electronsBgEstZeeJetMisIdEnrichedConversionVeto = copy.deepcopy(electronsBgEstZtautauEnrichedConversionVeto)

electronsBgEstZeeJetMisIdEnrichedTrkIP = copy.deepcopy(electronsBgEstZtautauEnrichedTrkIP)


electronSelConfiguratorBgEstZeeJetMisIdEnriched = objSelConfigurator(
    [ electronsBgEstZeeJetMisIdEnrichedId,
      electronsBgEstZeeJetMisIdEnrichedAntiCrackCut,
      electronsBgEstZeeJetMisIdEnrichedEta,
      electronsBgEstZeeJetMisIdEnrichedPt,
      electronsBgEstZeeJetMisIdEnrichedIso,
      electronsBgEstZeeJetMisIdEnrichedConversionVeto,
      electronsBgEstZeeJetMisIdEnrichedTrkIP
      ],
    src = "cleanPatElectrons",
    pyModuleName = __name__,
    doSelIndividual = False
)


selectElectronsBgEstZeeJetMisIdEnriched = electronSelConfiguratorBgEstZeeJetMisIdEnriched.configure(pyNameSpace = locals())

   
#--------------------------------------------------------------------------------  
# produce collection of pat::Taus
#--------------------------------------------------------------------------------

# require tau candidate not to overlap with selected electrons
# (in order to avoid double-counting one and the same physical particle
#  as electron and as tau candidate)
tausBgEstZeeJetMisIdEnrichedAntiOverlapWithElectronsVeto = copy.deepcopy(tausBgEstZtautauEnrichedAntiOverlapWithElectronsVeto)
tausBgEstZeeJetMisIdEnrichedAntiOverlapWithElectronsVeto.srcNotToBeFiltered = cms.VInputTag("electronsBgEstZeeJetMisIdEnrichedPtCumulative")

# require tau candidate to be within geometric acceptance of Pixel + SiTracker detectors
tausBgEstZeeJetMisIdEnrichedEta = copy.deepcopy(tausBgEstZtautauEnrichedEta)

# require tau candidate to have transverse energy above threshold
tausBgEstZeeJetMisIdEnrichedPt = copy.deepcopy(tausBgEstZtautauEnrichedPt)

# require leading track of tau candidate to have Pt > 5. GeV
tausBgEstZeeJetMisIdEnrichedLeadTrkPt = copy.deepcopy(tausBgEstZtautauEnrichedLeadTrkPt)

# require tau candidate to pass TaNC discriminator
tausBgEstZeeJetMisIdEnrichedIso = copy.deepcopy(tausBgEstZtautauEnrichedIso)
#tausBgEstZeeJetMisIdEnrichedIso.cut = cms.string('tauID("byLooseIsolation") > 0.5')
#tausBgEstZeeJetMisIdEnrichedIso.cut = cms.string('tauID("byTaNCfrOnePercent") > 0.5')


# require tau candidate to have either one or three tracks within signal cone
tausBgEstZeeJetMisIdEnrichedProng = copy.deepcopy(tausBgEstZtautauEnrichedProng)
tausBgEstZeeJetMisIdEnrichedProng.cut = cms.string("signalPFChargedHadrCands.size() != -1")

# require tau candidate to have charge either +1 or -1
# (computed as sum of charges of tracks within signal cone)
tausBgEstZeeJetMisIdEnrichedCharge = copy.deepcopy(tausBgEstZtautauEnrichedCharge)
tausBgEstZeeJetMisIdEnrichedCharge.cut = cms.string('abs(charge) > -1')

# require tau candidate to fail electron veto  !!!!! inverted cut
tausBgEstZeeJetMisIdEnrichedElectronVeto = copy.deepcopy(tausBgEstZtautauEnrichedElectronVeto)


# require tau candidate not to be in ECAL barrel/endcap crack
tausBgEstZeeJetMisIdEnrichedEcalCrackVeto = copy.deepcopy(tausBgEstZtautauEnrichedEcalCrackVeto)


# require tau candidate to pass muon veto
tausBgEstZeeJetMisIdEnrichedMuonVeto = copy.deepcopy(tausBgEstZtautauEnrichedMuonVeto)



tauSelConfiguratorBgEstZeeJetMisIdEnriched = objSelConfigurator(
    [ tausBgEstZeeJetMisIdEnrichedAntiOverlapWithElectronsVeto,
      tausBgEstZeeJetMisIdEnrichedEta,
      tausBgEstZeeJetMisIdEnrichedPt,
      tausBgEstZeeJetMisIdEnrichedLeadTrkPt,
      tausBgEstZeeJetMisIdEnrichedIso,
      tausBgEstZeeJetMisIdEnrichedProng,
      tausBgEstZeeJetMisIdEnrichedCharge,
      tausBgEstZeeJetMisIdEnrichedElectronVeto,
      tausBgEstZeeJetMisIdEnrichedEcalCrackVeto,
      tausBgEstZeeJetMisIdEnrichedMuonVeto ],
    src = "cleanPatTaus",
    pyModuleName = __name__,
    doSelIndividual = False
)

selectTausBgEstZeeJetMisIdEnriched = tauSelConfiguratorBgEstZeeJetMisIdEnriched.configure(pyNameSpace = locals())



#--------------------------------------------------------------------------------  
# produce collection of electron + tau-jet combinations
#--------------------------------------------------------------------------------

### production


elecTauPairsBgEstZeeJetMisIdEnriched = copy.deepcopy(elecTauPairsBgEstZtautauEnriched)
elecTauPairsBgEstZeeJetMisIdEnriched.srcLeg1 = cms.InputTag('electronsBgEstZeeJetMisIdEnrichedTrkIPcumulative')
elecTauPairsBgEstZeeJetMisIdEnriched.srcLeg2 = cms.InputTag('tausBgEstZeeJetMisIdEnrichedMuonVetoCumulative')

produceElecTauPairsBgEstZeeJetMisIdEnriched = cms.Sequence(elecTauPairsBgEstZeeJetMisIdEnriched)

### selection

elecTauPairsBgEstZeeJetMisIdEnrichedAntiOverlapVeto = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedAntiOverlapVeto)
elecTauPairsBgEstZeeJetMisIdEnrichedMt1MET = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedMt1MET)
elecTauPairsBgEstZeeJetMisIdEnrichedMt1MET.cut = cms.string('mt1MET < 500.')
elecTauPairsBgEstZeeJetMisIdEnrichedPzetaDiff = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedPzetaDiff)
elecTauPairsBgEstZeeJetMisIdEnrichedPzetaDiff.cut = cms.string('(pZeta - 1.5*pZetaVis) > -2000.')
elecTauPairsBgEstZeeJetMisIdEnrichedZeroCharge = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedZeroCharge)
elecTauPairsBgEstZeeJetMisIdEnrichedZeroCharge.cut = cms.string('charge != 10')


elecTauPairSelConfiguratorBgEstZeeJetMisIdEnriched = objSelConfigurator(
    [ elecTauPairsBgEstZeeJetMisIdEnrichedAntiOverlapVeto,
      elecTauPairsBgEstZeeJetMisIdEnrichedMt1MET,
      elecTauPairsBgEstZeeJetMisIdEnrichedPzetaDiff,
      elecTauPairsBgEstZeeJetMisIdEnrichedZeroCharge
      ],
    src = "elecTauPairsBgEstZeeJetMisIdEnriched",
    pyModuleName = __name__,
    doSelIndividual = False
)

selectElecTauPairsBgEstZeeJetMisIdEnriched = elecTauPairSelConfiguratorBgEstZeeJetMisIdEnriched.configure(pyNameSpace = locals())

####### anti Zee Cut #######



### production

allDiElecPairBgEstZeeJetMisIdEnrichedZeeHypothesesByLooseIsolation = copy.deepcopy(allDiElecPairBgEstZtautauEnrichedZeeHypothesesByLooseIsolation)
allDiElecPairBgEstZeeJetMisIdEnrichedZeeHypothesesByLooseIsolation.srcLeg1 = cms.InputTag("electronsBgEstZeeJetMisIdEnrichedTrkIPcumulative")

### selection

selectedDiElecPairBgEstZeeJetMisIdEnrichedZeeHypothesesByLooseIsolation = copy.deepcopy(selectedDiElecPairBgEstZtautauEnrichedZeeHypothesesByLooseIsolation)
selectedDiElecPairBgEstZeeJetMisIdEnrichedZeeHypothesesByLooseIsolation.src = cms.InputTag("allDiElecPairBgEstZeeJetMisIdEnrichedZeeHypothesesByLooseIsolation")

produceElecTauPairZeeHypothesesBgEstZeeJetMisIdEnriched = cms.Sequence(
	selectedPatElectronsForZeeHypotheses * 
	allDiElecPairBgEstZeeJetMisIdEnrichedZeeHypothesesByLooseIsolation *
	selectedDiElecPairBgEstZeeJetMisIdEnrichedZeeHypothesesByLooseIsolation
)


#--------------------------------------------------------------------------------  
# produce boolean event selection flags
#--------------------------------------------------------------------------------


# electron cuts

cfgElectronIdCutBgEstZeeJetMisIdEnriched = copy.deepcopy(cfgElectronIdCutBgEstZtautauEnriched)
cfgElectronIdCutBgEstZeeJetMisIdEnriched.pluginName = cms.string('electronIdCutBgEstZeeJetMisIdEnriched')
cfgElectronIdCutBgEstZeeJetMisIdEnriched.src_cumulative = cms.InputTag('electronsBgEstZeeJetMisIdEnrichedIdCumulative')

cfgElectronAntiCrackCutBgEstZeeJetMisIdEnriched = copy.deepcopy(cfgElectronAntiCrackCutBgEstZtautauEnriched)
cfgElectronAntiCrackCutBgEstZeeJetMisIdEnriched.pluginName = cms.string('electronAntiCrackCutBgEstZeeJetMisIdEnriched')
cfgElectronAntiCrackCutBgEstZeeJetMisIdEnriched.src_cumulative = cms.InputTag('electronsBgEstZeeJetMisIdEnrichedAntiCrackCutCumulative')

cfgElectronEtaCutBgEstZeeJetMisIdEnriched = copy.deepcopy(cfgElectronEtaCutBgEstZtautauEnriched)
cfgElectronEtaCutBgEstZeeJetMisIdEnriched.pluginName = cms.string('electronEtaCutBgEstZeeJetMisIdEnriched')
cfgElectronEtaCutBgEstZeeJetMisIdEnriched.src_cumulative = cms.InputTag('electronsBgEstZeeJetMisIdEnrichedEtaCumulative')

cfgElectronPtCutBgEstZeeJetMisIdEnriched = copy.deepcopy(cfgElectronPtCutBgEstZtautauEnriched)
cfgElectronPtCutBgEstZeeJetMisIdEnriched.pluginName = cms.string('electronPtCutBgEstZeeJetMisIdEnriched')
cfgElectronPtCutBgEstZeeJetMisIdEnriched.src_cumulative = cms.InputTag('electronsBgEstZeeJetMisIdEnrichedPtCumulative')

cfgElectronIsoCutBgEstZeeJetMisIdEnriched = copy.deepcopy(cfgElectronIsoCutBgEstZtautauEnriched)
cfgElectronIsoCutBgEstZeeJetMisIdEnriched.pluginName = cms.string('electronIsoCutBgEstZeeJetMisIdEnriched')
cfgElectronIsoCutBgEstZeeJetMisIdEnriched.src_cumulative = cms.InputTag('electronsBgEstZeeJetMisIdEnrichedIsoCumulative')

cfgElectronConversionVetoBgEstZeeJetMisIdEnriched = copy.deepcopy(cfgElectronConversionVetoBgEstZtautauEnriched)
cfgElectronConversionVetoBgEstZeeJetMisIdEnriched.pluginName = cms.string('electronConversionVetoBgEstZeeJetMisIdEnriched')
cfgElectronConversionVetoBgEstZeeJetMisIdEnriched.src_cumulative = cms.InputTag('electronsBgEstZeeJetMisIdEnrichedConversionVetoCumulative')

cfgElectronTrkIPcutBgEstZeeJetMisIdEnriched = copy.deepcopy(cfgElectronTrkIPcutBgEstZtautauEnriched)
cfgElectronTrkIPcutBgEstZeeJetMisIdEnriched.pluginName = cms.string('electronTrkIPcutBgEstZeeJetMisIdEnriched')
cfgElectronTrkIPcutBgEstZeeJetMisIdEnriched.src_cumulative = cms.InputTag('electronsBgEstZeeJetMisIdEnrichedTrkIPcumulative')


# tau cuts

cfgTauAntiOverlapWithElectronsVetoBgEstZeeJetMisIdEnriched = copy.deepcopy(cfgTauAntiOverlapWithElectronsVetoBgEstZtautauEnriched)
cfgTauAntiOverlapWithElectronsVetoBgEstZeeJetMisIdEnriched.pluginName = cms.string('tauAntiOverlapWithElectronsVetoBgEstZeeJetMisIdEnriched')
cfgTauAntiOverlapWithElectronsVetoBgEstZeeJetMisIdEnriched.src_cumulative = cms.InputTag('tausBgEstZeeJetMisIdEnrichedAntiOverlapWithElectronsVetoCumulative')

cfgTauEtaCutBgEstZeeJetMisIdEnriched = copy.deepcopy(cfgTauEtaCutBgEstZtautauEnriched)
cfgTauEtaCutBgEstZeeJetMisIdEnriched.pluginName = cms.string('tauEtaCutBgEstZeeJetMisIdEnriched')
cfgTauEtaCutBgEstZeeJetMisIdEnriched.src_cumulative = cms.InputTag('tausBgEstZeeJetMisIdEnrichedEtaCumulative')

cfgTauPtCutBgEstZeeJetMisIdEnriched = copy.deepcopy(cfgTauPtCutBgEstZtautauEnriched)
cfgTauPtCutBgEstZeeJetMisIdEnriched.pluginName = cms.string('tauPtCutBgEstZeeJetMisIdEnriched')
cfgTauPtCutBgEstZeeJetMisIdEnriched.src_cumulative = cms.InputTag('tausBgEstZeeJetMisIdEnrichedPtCumulative')

## cfgTauLeadTrkCutBgEstZeeJetMisIdEnriched = copy.deepcopy(cfgTauLeadTrkCutBgEstZtautauEnriched)
## cfgTauLeadTrkCutBgEstZeeJetMisIdEnriched.pluginName = cms.string('tauLeadTrkCutBgEstZeeJetMisIdEnriched')
## cfgTauLeadTrkCutBgEstZeeJetMisIdEnriched.src_cumulative = cms.InputTag('tausBgEstZeeJetMisIdEnrichedLeadTrkCumulative')

cfgTauLeadTrkPtCutBgEstZeeJetMisIdEnriched = copy.deepcopy(cfgTauLeadTrkPtCutBgEstZtautauEnriched)
cfgTauLeadTrkPtCutBgEstZeeJetMisIdEnriched.pluginName = cms.string('tauLeadTrkPtCutBgEstZeeJetMisIdEnriched')
cfgTauLeadTrkPtCutBgEstZeeJetMisIdEnriched.src_cumulative = cms.InputTag('tausBgEstZeeJetMisIdEnrichedLeadTrkPtCumulative')

cfgTauIsoCutBgEstZeeJetMisIdEnriched = copy.deepcopy(cfgTauIsoCutBgEstZtautauEnriched)
cfgTauIsoCutBgEstZeeJetMisIdEnriched.pluginName = cms.string('tauIsoCutBgEstZeeJetMisIdEnriched')
cfgTauIsoCutBgEstZeeJetMisIdEnriched.src_cumulative = cms.InputTag('tausBgEstZeeJetMisIdEnrichedIsoCumulative')

cfgTauProngCutBgEstZeeJetMisIdEnriched = copy.deepcopy(cfgTauProngCutBgEstZtautauEnriched)
cfgTauProngCutBgEstZeeJetMisIdEnriched.pluginName = cms.string('tauProngCutBgEstZeeJetMisIdEnriched')
cfgTauProngCutBgEstZeeJetMisIdEnriched.src_cumulative = cms.InputTag('tausBgEstZeeJetMisIdEnrichedProngCumulative')

cfgTauChargeCutBgEstZeeJetMisIdEnriched = copy.deepcopy(cfgTauChargeCutBgEstZtautauEnriched)
cfgTauChargeCutBgEstZeeJetMisIdEnriched.pluginName = cms.string('tauChargeCutBgEstZeeJetMisIdEnriched')
cfgTauChargeCutBgEstZeeJetMisIdEnriched.src_cumulative = cms.InputTag('tausBgEstZeeJetMisIdEnrichedChargeCumulative')

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut
cfgTauElectronVetoBgEstZeeJetMisIdEnriched = copy.deepcopy(cfgTauElectronVetoBgEstZtautauEnriched)
cfgTauElectronVetoBgEstZeeJetMisIdEnriched.pluginName = cms.string('tauElectronVetoBgEstZeeJetMisIdEnriched')
cfgTauElectronVetoBgEstZeeJetMisIdEnriched.src_cumulative = cms.InputTag('tausBgEstZeeJetMisIdEnrichedElectronVetoCumulative')

cfgTauEcalCrackVetoBgEstZeeJetMisIdEnriched = copy.deepcopy(cfgTauEcalCrackVetoBgEstZtautauEnriched)
cfgTauEcalCrackVetoBgEstZeeJetMisIdEnriched.pluginName = cms.string('tauEcalCrackVetoBgEstZeeJetMisIdEnriched')
cfgTauEcalCrackVetoBgEstZeeJetMisIdEnriched.src_cumulative = cms.InputTag('tausBgEstZeeJetMisIdEnrichedEcalCrackVetoCumulative')

cfgTauMuonVetoBgEstZeeJetMisIdEnriched = copy.deepcopy(cfgTauMuonVetoBgEstZtautauEnriched)
cfgTauMuonVetoBgEstZeeJetMisIdEnriched.pluginName = cms.string('tauMuonVetoBgEstZeeJetMisIdEnriched')
cfgTauMuonVetoBgEstZeeJetMisIdEnriched.src_cumulative = cms.InputTag('tausBgEstZeeJetMisIdEnrichedMuonVetoCumulative')

# di-tau candidate selection

cfgDiTauCandidateAntiOverlapVetoBgEstZeeJetMisIdEnriched = copy.deepcopy(cfgDiTauCandidateAntiOverlapVetoBgEstZtautauEnriched)
cfgDiTauCandidateAntiOverlapVetoBgEstZeeJetMisIdEnriched.pluginName = cms.string('diTauCandidateAntiOverlapVetoBgEstZeeJetMisIdEnriched')
cfgDiTauCandidateAntiOverlapVetoBgEstZeeJetMisIdEnriched.src_cumulative = cms.InputTag('elecTauPairsBgEstZeeJetMisIdEnrichedAntiOverlapVetoCumulative')

cfgDiTauCandidateForElecTauMt1METCutBgEstZeeJetMisIdEnriched = copy.deepcopy(cfgDiTauCandidateForElecTauMt1METCutBgEstZtautauEnriched)
cfgDiTauCandidateForElecTauMt1METCutBgEstZeeJetMisIdEnriched.pluginName = cms.string('diTauCandidateMt1METCutBgEstZeeJetMisIdEnriched')
cfgDiTauCandidateForElecTauMt1METCutBgEstZeeJetMisIdEnriched.src_cumulative = cms.InputTag('elecTauPairsBgEstZeeJetMisIdEnrichedMt1METcumulative')

cfgDiTauCandidateForElecTauPzetaDiffCutBgEstZeeJetMisIdEnriched = copy.deepcopy(cfgDiTauCandidateForElecTauPzetaDiffCutBgEstZtautauEnriched)
cfgDiTauCandidateForElecTauPzetaDiffCutBgEstZeeJetMisIdEnriched.pluginName = cms.string('diTauCandidatePzetaDiffCutBgEstZeeJetMisIdEnriched')
cfgDiTauCandidateForElecTauPzetaDiffCutBgEstZeeJetMisIdEnriched.src_cumulative = cms.InputTag('elecTauPairsBgEstZeeJetMisIdEnrichedPzetaDiffCumulative')

cfgDiTauCandidateForElecTauZeroChargeCutBgEstZeeJetMisIdEnriched = copy.deepcopy(cfgDiTauCandidateForElecTauZeroChargeCutBgEstZtautauEnriched)
cfgDiTauCandidateForElecTauZeroChargeCutBgEstZeeJetMisIdEnriched.pluginName = cms.string('diTauCandidateZeroChargeCutBgEstZeeJetMisIdEnriched')
cfgDiTauCandidateForElecTauZeroChargeCutBgEstZeeJetMisIdEnriched.src_cumulative = cms.InputTag('elecTauPairsBgEstZeeJetMisIdEnrichedZeroChargeCumulative')

cfgElecTauPairZeeHypothesisVetoBgEstZeeJetMisIdEnriched = copy.deepcopy(cfgDiElecPairZeeHypothesisVetoByLooseIsolation)
cfgElecTauPairZeeHypothesisVetoBgEstZeeJetMisIdEnriched.pluginName = cms.string('diTauCandidateZeeHypothesisVetoBgEstZeeJetMisIdEnriched')
cfgElecTauPairZeeHypothesisVetoBgEstZeeJetMisIdEnriched.src = cms.InputTag('selectedDiElecPairBgEstZeeJetMisIdEnrichedZeeHypothesesByLooseIsolation')
cfgElecTauPairZeeHypothesisVetoBgEstZeeJetMisIdEnriched.minNumber = cms.uint32(1)

evtSelConfiguratorBgEstZeeJetMisIdEnriched = eventSelFlagProdConfigurator(
    [ cfgElectronIdCutBgEstZeeJetMisIdEnriched,
      cfgElectronAntiCrackCutBgEstZeeJetMisIdEnriched,
      cfgElectronEtaCutBgEstZeeJetMisIdEnriched,
      cfgElectronPtCutBgEstZeeJetMisIdEnriched,
      cfgElectronIsoCutBgEstZeeJetMisIdEnriched,
      cfgElectronConversionVetoBgEstZeeJetMisIdEnriched,
      cfgElectronTrkIPcutBgEstZeeJetMisIdEnriched,
      cfgTauAntiOverlapWithElectronsVetoBgEstZeeJetMisIdEnriched,
      cfgTauEtaCutBgEstZeeJetMisIdEnriched,
      cfgTauPtCutBgEstZeeJetMisIdEnriched,
      cfgTauLeadTrkPtCutBgEstZeeJetMisIdEnriched,
      cfgTauIsoCutBgEstZeeJetMisIdEnriched,
      cfgTauProngCutBgEstZeeJetMisIdEnriched,
      cfgTauChargeCutBgEstZeeJetMisIdEnriched,
      cfgTauElectronVetoBgEstZeeJetMisIdEnriched,
      cfgTauEcalCrackVetoBgEstZeeJetMisIdEnriched,
      cfgTauMuonVetoBgEstZeeJetMisIdEnriched,
      cfgDiTauCandidateAntiOverlapVetoBgEstZeeJetMisIdEnriched,
      cfgDiTauCandidateForElecTauMt1METCutBgEstZeeJetMisIdEnriched,
      cfgDiTauCandidateForElecTauPzetaDiffCutBgEstZeeJetMisIdEnriched,
      cfgDiTauCandidateForElecTauZeroChargeCutBgEstZeeJetMisIdEnriched,
      cfgElecTauPairZeeHypothesisVetoBgEstZeeJetMisIdEnriched
      ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

selectEventsBgEstZeeJetMisIdEnriched = evtSelConfiguratorBgEstZeeJetMisIdEnriched.configure()


#--------------------------------------------------------------------------------  
# apply event selection criteria; fill histograms
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.analyzeZtoElecTau_cff import *


diTauCandidateHistManagerForElecTauBgEstZeeJetMisIdEnriched = copy.deepcopy(diTauCandidateHistManagerForElecTauBgEstZtautauEnriched)
diTauCandidateHistManagerForElecTauBgEstZeeJetMisIdEnriched.pluginName = cms.string('diTauCandidateHistManagerForElecTauBgEstZeeJetMisIdEnriched')
diTauCandidateHistManagerForElecTauBgEstZeeJetMisIdEnriched.diTauCandidateSource = cms.InputTag('elecTauPairsBgEstZeeJetMisIdEnriched')

electronHistManagerForElecTauBgEstZeeJetMisIdEnriched = copy.deepcopy(electronHistManagerForElecTauBgEstZtautauEnriched)
electronHistManagerForElecTauBgEstZeeJetMisIdEnriched.pluginName = cms.string('electronHistManagerForElecTauBgEstZeeJetMisIdEnriched')

tauHistManagerForElecTauBgEstZeeJetMisIdEnriched = copy.deepcopy(tauHistManagerForElecTauBgEstZtautauEnriched)
tauHistManagerForElecTauBgEstZeeJetMisIdEnriched.pluginName = cms.string('tauHistManagerForElecTauBgEstZeeJetMisIdEnriched')
tauHistManagerForElecTauBgEstZeeJetMisIdEnriched.jetSource = cms.InputTag('selectedPatJets')



analyzeEventsBgEstZeeJetMisIdEnriched = cms.EDAnalyzer("GenericAnalyzer",

    name = cms.string('BgEstTemplateAnalyzer_ZeeJetMisIdEnriched'), 

    filters = cms.VPSet(
        evtSelGenPhaseSpace,
        evtSelTrigger,
        evtSelPrimaryEventVertex,
        evtSelPrimaryEventVertexQuality,
        evtSelPrimaryEventVertexPosition,

        #start electron cuts
        
        cms.PSet(
            pluginName = cms.string('electronIdCutBgEstZeeJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronIdCutBgEstZeeJetMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('electronAntiCrackCutBgEstZeeJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronAntiCrackCutBgEstZeeJetMisIdEnriched','cumulative')
        ),        
        cms.PSet(
            pluginName = cms.string('electronEtaCutBgEstZeeJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronEtaCutBgEstZeeJetMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('electronPtCutBgEstZeeJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronPtCutBgEstZeeJetMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('electronIsoCutBgEstZeeJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronIsoCutBgEstZeeJetMisIdEnriched','cumulative')
        ),        
        cms.PSet(
            pluginName = cms.string('electronConversionVetoBgEstZeeJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronConversionVetoBgEstZeeJetMisIdEnriched','cumulative')
        ),        
        cms.PSet(
            pluginName = cms.string('electronTrkIPcutBgEstZeeJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronTrkIPcutBgEstZeeJetMisIdEnriched','cumulative')
        ),


        #start tau cuts
        
        cms.PSet(
            pluginName = cms.string('tauAntiOverlapWithElectronsVetoBgEstZeeJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauAntiOverlapWithElectronsVetoBgEstZeeJetMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauEtaCutBgEstZeeJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauEtaCutBgEstZeeJetMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauPtCutBgEstZeeJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauPtCutBgEstZeeJetMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauLeadTrkPtCutBgEstZeeJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauLeadTrkPtCutBgEstZeeJetMisIdEnriched','cumulative')
        ),  
        cms.PSet(
            pluginName = cms.string('tauIsoCutBgEstZeeJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauIsoCutBgEstZeeJetMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauProngCutBgEstZeeJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauProngCutBgEstZeeJetMisIdEnriched','cumulative')
        ),  
        cms.PSet(
            pluginName = cms.string('tauChargeCutBgEstZeeJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauChargeCutBgEstZeeJetMisIdEnriched','cumulative')
        ), 
        cms.PSet(
            pluginName = cms.string('tauElectronVetoBgEstZeeJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauElectronVetoBgEstZeeJetMisIdEnriched','cumulative')
        ), 
        cms.PSet(
            pluginName = cms.string('tauEcalCrackVetoBgEstZeeJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauEcalCrackVetoBgEstZeeJetMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauMuonVetoBgEstZeeJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauMuonVetoBgEstZeeJetMisIdEnriched','cumulative')
        ),

        #start ditau cuts
        
        cms.PSet(
            pluginName = cms.string('diTauCandidateAntiOverlapVetoBgEstZeeJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateAntiOverlapVetoBgEstZeeJetMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('diTauCandidateMt1METCutBgEstZeeJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateMt1METCutBgEstZeeJetMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('diTauCandidatePzetaDiffCutBgEstZeeJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidatePzetaDiffCutBgEstZeeJetMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('diTauCandidateZeroChargeCutBgEstZeeJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateZeroChargeCutBgEstZeeJetMisIdEnriched','cumulative')
        ),        
        cms.PSet(
            pluginName = cms.string('diTauCandidateZeeHypothesisVetoBgEstZeeJetMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateZeeHypothesisVetoBgEstZeeJetMisIdEnriched')
        )
    ),
  
    analyzers = cms.VPSet(
         diTauCandidateHistManagerForElecTauBgEstZeeJetMisIdEnriched,
         electronHistManagerForElecTauBgEstZeeJetMisIdEnriched,
         tauHistManagerForElecTauBgEstZeeJetMisIdEnriched,
    ),

    eventDumps = cms.VPSet(),
   
    analysisSequence = cms.VPSet(
    
        # generator level phase-space selection
        # (NOTE: (1) to be used in case of Monte Carlo samples
        #            overlapping in simulated phase-space only !!
        #        (2) genPhaseSpaceCut needs to be **always** the first entry in the list of cuts
        #           - otherwise the script submitToBatch.csh for submission of cmsRun jobs
        #            to the CERN batch system will not work !!)
        cms.PSet(
            filter = cms.string('genPhaseSpaceCut'),
            title = cms.string('gen. Phase-Space')
        ),
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
            filter = cms.string('electronIdCutBgEstZeeJetMisIdEnriched'),
            title = cms.string('Electron ID'),
        ),
        cms.PSet(
            filter = cms.string('electronAntiCrackCutBgEstZeeJetMisIdEnriched'),
            title = cms.string('Electron crack-Veto'),
        ),
        cms.PSet(
            filter = cms.string('electronEtaCutBgEstZeeJetMisIdEnriched'),
            title = cms.string('-2.1 < eta(Electron) < +2.1'),
        ),
        cms.PSet(
            filter = cms.string('electronPtCutBgEstZeeJetMisIdEnriched'),
            title = cms.string('Pt(Electron) > 15 GeV'),
        ),
        cms.PSet(
            filter = cms.string('tauAntiOverlapWithElectronsVetoBgEstZeeJetMisIdEnriched'),
            title = cms.string('Tau not overlapping with Elec.'),
        ),
        cms.PSet(
            filter = cms.string('tauEtaCutBgEstZeeJetMisIdEnriched'),
            title = cms.string('-2.3 < eta(Tau) < +2.3'),
        ),
        cms.PSet(
            filter = cms.string('tauPtCutBgEstZeeJetMisIdEnriched'),
            title = cms.string('Pt(Tau) > 20 GeV'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    ),
            replace = cms.vstring('electronHistManagerForElecTauBgEstZeeJetMisIdEnriched.electronSource = electronsBgEstZeeJetMisIdEnrichedPtCumulative',
                                  'tauHistManagerForElecTauBgEstZeeJetMisIdEnriched.tauSource = tausBgEstZeeJetMisIdEnrichedPtCumulative'
                                  )
        ),           
        cms.PSet(
            filter = cms.string('electronIsoCutBgEstZeeJetMisIdEnriched'),
            title = cms.string('Electron Isolation'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    ),
            replace = cms.vstring('electronHistManagerForElecTauBgEstZeeJetMisIdEnriched.electronSource = electronsBgEstZeeJetMisIdEnrichedIsoCumulative')
        ),          
        cms.PSet(
            filter = cms.string('electronConversionVetoBgEstZeeJetMisIdEnriched'),
            title = cms.string('Electron Track conversion veto'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    ),
            replace = cms.vstring('electronHistManagerForElecTauBgEstZeeJetMisIdEnriched.electronSource = electronsBgEstZeeJetMisIdEnrichedConversionVetoCumulative')
        ),          
        cms.PSet(
            filter = cms.string('electronTrkIPcutBgEstZeeJetMisIdEnriched'),
            title = cms.string('Electron Track IP'),
        ),        
        cms.PSet(
            filter = cms.string('tauLeadTrkPtCutBgEstZeeJetMisIdEnriched'),
            title = cms.string('Tau lead. Track Pt'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    ),
            replace = cms.vstring('electronHistManagerForElecTauBgEstZeeJetMisIdEnriched.electronSource = electronsBgEstZeeJetMisIdEnrichedTrkIPcumulative',
                                  'tauHistManagerForElecTauBgEstZeeJetMisIdEnriched.tauSource = tausBgEstZeeJetMisIdEnrichedLeadTrkPtCumulative')
        ),           
        cms.PSet(
            filter = cms.string('tauIsoCutBgEstZeeJetMisIdEnriched'),
#            title = cms.string('Tau TaNC by 1%'),
            title = cms.string('Tau ID byHPSloose'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    ),
            replace = cms.vstring('tauHistManagerForElecTauBgEstZeeJetMisIdEnriched.tauSource = tausBgEstZeeJetMisIdEnrichedIsoCumulative')
        ),         
        cms.PSet(
            filter = cms.string('tauProngCutBgEstZeeJetMisIdEnriched'),
            title = cms.string('Tau 1||3-Prong (off)'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    ),
            replace = cms.vstring('tauHistManagerForElecTauBgEstZeeJetMisIdEnriched.tauSource = tausBgEstZeeJetMisIdEnrichedProngCumulative')
        ),            
        cms.PSet(
            filter = cms.string('tauChargeCutBgEstZeeJetMisIdEnriched'),
            title = cms.string('Charge(Tau) = +/-1 (off)'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    ),
            replace = cms.vstring('tauHistManagerForElecTauBgEstZeeJetMisIdEnriched.tauSource = tausBgEstZeeJetMisIdEnrichedChargeCumulative')
        ),          
        #!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut
        cms.PSet(
            filter = cms.string('tauElectronVetoBgEstZeeJetMisIdEnriched'),
            title = cms.string('Tau e-Veto'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    ),
            replace = cms.vstring('tauHistManagerForElecTauBgEstZeeJetMisIdEnriched.tauSource = tausBgEstZeeJetMisIdEnrichedElectronVetoCumulative')
        ),              
        cms.PSet(
            filter = cms.string('tauEcalCrackVetoBgEstZeeJetMisIdEnriched'),
            title = cms.string('Tau ECAL crack-Veto'),
        ),      
        cms.PSet(
            filter = cms.string('tauMuonVetoBgEstZeeJetMisIdEnriched'),
            title = cms.string('Tau mu-Veto'),
        ),        
        cms.PSet(
            filter = cms.string('diTauCandidateAntiOverlapVetoBgEstZeeJetMisIdEnriched'),
            title = cms.string('dR(Electron-Tau) > 0.5'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    ),
            replace = cms.vstring('tauHistManagerForElecTauBgEstZeeJetMisIdEnriched.tauSource = tausBgEstZeeJetMisIdEnrichedMuonVetoCumulative',
                                  'diTauCandidateHistManagerForElecTauBgEstZeeJetMisIdEnriched.diTauCandidate = elecTauPairsBgEstZeeJetMisIdEnrichedAntiOverlapVetoCumulative'
                                  )
        ),         
        cms.PSet(
            filter = cms.string('diTauCandidateMt1METCutBgEstZeeJetMisIdEnriched'),
            title = cms.string('M_{T}(Electron-MET) < 40 GeV (off)'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    ),
        ),        
        cms.PSet(
            filter = cms.string('diTauCandidatePzetaDiffCutBgEstZeeJetMisIdEnriched'),
            title = cms.string('Pzeta-1.5*Pzeta(vis) > -20 GeV (off)'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    ),
        ),            
        cms.PSet(
            filter = cms.string('diTauCandidateZeroChargeCutBgEstZeeJetMisIdEnriched'),
            title = cms.string('Charge(Electron+Tau) = 0 (off)'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    ),
        ),          
        cms.PSet(
            filter = cms.string('diTauCandidateZeeHypothesisVetoBgEstZeeJetMisIdEnriched'),
            title = cms.string('no 2nd OS, loosely-isolated e- (inverted)')
         ),

        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstZeeJetMisIdEnriched',
                                    ),
        ),     

    )

)



#--------------------------------------------------------------------------------  
# define (final) analysis sequence
#--------------------------------------------------------------------------------

bgEstZeeJetMisIdEnrichedAnalysisSequence = cms.Sequence(


      selectElectronsBgEstZeeJetMisIdEnriched
    + selectTausBgEstZeeJetMisIdEnriched
    + produceElecTauPairsBgEstZeeJetMisIdEnriched
    + selectElecTauPairsBgEstZeeJetMisIdEnriched
    + produceElecTauPairZeeHypothesesBgEstZeeJetMisIdEnriched
    + selectEventsBgEstZeeJetMisIdEnriched 
    + analyzeEventsBgEstZeeJetMisIdEnriched

)
