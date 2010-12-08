
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
electronsBgEstPhotonPlusJetsEnrichedId = copy.deepcopy(electronsBgEstZtautauEnrichedId)

# require electron candidate to not be within eta-crack
# between Barrel and Encap ECAL calorimeter
electronsBgEstPhotonPlusJetsEnrichedAntiCrackCut = copy.deepcopy(electronsBgEstZtautauEnrichedAntiCrackCut)

# require electron candidate to be within geometric acceptance of electron trigger
electronsBgEstPhotonPlusJetsEnrichedEta = copy.deepcopy(electronsBgEstZtautauEnrichedEta)

# require electron candidate to have transverse momentum above threshold
electronsBgEstPhotonPlusJetsEnrichedPt = copy.deepcopy(electronsBgEstZtautauEnrichedPt)

# require electron candidate to be isolated
# with respect to tracks (of Pt >~ 0.3 GeV)
electronsBgEstPhotonPlusJetsEnrichedTrkIso = copy.deepcopy(electronsBgEstZtautauEnrichedTrkIso)

# require electron candidate to be isolated
# with respect to energy deposits in ECAL
# (not associated to electron candidate)
electronsBgEstPhotonPlusJetsEnrichedEcalIso = copy.deepcopy(electronsBgEstZtautauEnrichedEcalIso)


# require track of electron candidate to have small transverse impact parameter
# (in order to veto electrons resulting from b-quark decays)
electronsBgEstPhotonPlusJetsEnrichedTrkIP = copy.deepcopy(electronsBgEstZtautauEnrichedTrkIP)

# require electron to not be from a photon conversion
electronsBgEstPhotonPlusJetsEnrichedConversionVeto = copy.deepcopy(electronsBgEstZtautauEnrichedConversionVeto)


electronSelConfiguratorBgEstPhotonPlusJetsEnriched = objSelConfigurator(
    [ electronsBgEstPhotonPlusJetsEnrichedId,
      electronsBgEstPhotonPlusJetsEnrichedAntiCrackCut,
      electronsBgEstPhotonPlusJetsEnrichedEta,
      electronsBgEstPhotonPlusJetsEnrichedPt,
      electronsBgEstPhotonPlusJetsEnrichedTrkIso,
      electronsBgEstPhotonPlusJetsEnrichedEcalIso,
      electronsBgEstPhotonPlusJetsEnrichedConversionVeto,
      electronsBgEstPhotonPlusJetsEnrichedTrkIP ],
    src = "cleanPatElectrons",
    pyModuleName = __name__,
    doSelIndividual = False
)


selectElectronsBgEstPhotonPlusJetsEnriched = electronSelConfiguratorBgEstPhotonPlusJetsEnriched.configure(pyNameSpace = locals())

   
#--------------------------------------------------------------------------------  
# produce collection of pat::Taus
#--------------------------------------------------------------------------------

#selectTausBgEstPhotonPlusJetsEnriched = copy.deepcopy(selectPatTausForElecTau)

# require tau candidate not to overlap with selected electrons
# (in order to avoid double-counting one and the same physical particle
#  as electron and as tau candidate)
tausBgEstPhotonPlusJetsEnrichedAntiOverlapWithElectronsVeto = copy.deepcopy(tausBgEstZtautauEnrichedAntiOverlapWithElectronsVeto)

# require tau candidate to be within geometric acceptance of Pixel + SiTracker detectors
tausBgEstPhotonPlusJetsEnrichedEta = copy.deepcopy(tausBgEstZtautauEnrichedEta)

# require tau candidate to have transverse energy above threshold
tausBgEstPhotonPlusJetsEnrichedPt = copy.deepcopy(tausBgEstZtautauEnrichedPt)

# require tau candidate to have a leading track
# (track of Pt > 1. GeV within matching cone of size dR = 0.2 around jet-axis)
tausBgEstPhotonPlusJetsEnrichedLeadTrk = copy.deepcopy(tausBgEstZtautauEnrichedLeadTrk)

# require leading track of tau candidate to have Pt > 5. GeV
tausBgEstPhotonPlusJetsEnrichedLeadTrkPt = copy.deepcopy(tausBgEstZtautauEnrichedLeadTrkPt)

# require tau candidate to pass TaNC discriminator
tausBgEstPhotonPlusJetsEnrichedTaNCdiscr = copy.deepcopy(tausBgEstZtautauEnrichedTaNCdiscr)

# require tau candidate to have no tracks of Pt > 1. GeV
# in isolation cone of size dR = 0.8, surrounding signal cone of size dR = 5./Et
tausBgEstPhotonPlusJetsEnrichedTrkIso = copy.deepcopy(tausBgEstZtautauEnrichedTrkIso)

# require tau candidate to be isolated
# with respect to energy deposits in ECAL
tausBgEstPhotonPlusJetsEnrichedEcalIso = copy.deepcopy(tausBgEstZtautauEnrichedEcalIso)

# require tau candidate to have either one or three tracks within signal cone
tausBgEstPhotonPlusJetsEnrichedProng = copy.deepcopy(tausBgEstZtautauEnrichedProng)

# require tau candidate to have charge either +1 or -1
# (computed as sum of charges of tracks within signal cone)
tausBgEstPhotonPlusJetsEnrichedCharge = copy.deepcopy(tausBgEstZtautauEnrichedCharge)

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
      tausBgEstPhotonPlusJetsEnrichedLeadTrk,
      tausBgEstPhotonPlusJetsEnrichedLeadTrkPt,
      tausBgEstPhotonPlusJetsEnrichedTaNCdiscr,
      tausBgEstPhotonPlusJetsEnrichedTrkIso,
      tausBgEstPhotonPlusJetsEnrichedEcalIso,
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
elecTauPairsBgEstPhotonPlusJetsEnrichedZeroCharge = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedZeroCharge)
elecTauPairsBgEstPhotonPlusJetsEnrichedAcoplanarity12 = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedAcoplanarity12)
elecTauPairsBgEstPhotonPlusJetsEnrichedMt1MET = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedMt1MET)
elecTauPairsBgEstPhotonPlusJetsEnrichedPzetaDiff = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedPzetaDiff)

elecTauPairSelConfiguratorBgEstPhotonPlusJetsEnriched = objSelConfigurator(
    [ elecTauPairsBgEstPhotonPlusJetsEnrichedAntiOverlapVeto,
      elecTauPairsBgEstPhotonPlusJetsEnrichedZeroCharge,
      elecTauPairsBgEstPhotonPlusJetsEnrichedAcoplanarity12,
      elecTauPairsBgEstPhotonPlusJetsEnrichedMt1MET,
      elecTauPairsBgEstPhotonPlusJetsEnrichedPzetaDiff,
      ],
    src = "elecTauPairsBgEstPhotonPlusJetsEnriched",
    pyModuleName = __name__,
    doSelIndividual = False
)

selectElecTauPairsBgEstPhotonPlusJetsEnriched = elecTauPairSelConfiguratorBgEstPhotonPlusJetsEnriched.configure(pyNameSpace = locals())

####### anti Zee Cut #######

### production


elecTauPairBgEstPhotonPlusJetsEnrichedZeeHypotheses = copy.deepcopy(elecTauPairBgEstZtautauEnrichedZeeHypotheses)
elecTauPairBgEstPhotonPlusJetsEnrichedZeeHypotheses.diCandidatePairSource = cms.InputTag('elecTauPairsBgEstPhotonPlusJetsEnrichedPzetaDiffCumulative')

### selection

selectedElecTauPairBgEstPhotonPlusJetsEnrichedZeeHypotheses = copy.deepcopy(selectedElecTauPairBgEstZtautauEnrichedZeeHypotheses)
selectedElecTauPairBgEstPhotonPlusJetsEnrichedZeeHypotheses.src = cms.InputTag('elecTauPairBgEstPhotonPlusJetsEnrichedZeeHypotheses')


produceElecTauPairZeeHypothesesBgEstPhotonPlusJetsEnriched = cms.Sequence(elecTauPairBgEstPhotonPlusJetsEnrichedZeeHypotheses*selectedElecTauPairBgEstPhotonPlusJetsEnrichedZeeHypotheses)

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

cfgElectronTrkIsoCutBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgElectronTrkIsoCutBgEstZtautauEnriched)
cfgElectronTrkIsoCutBgEstPhotonPlusJetsEnriched.pluginName = cms.string('electronTrkIsoCutBgEstPhotonPlusJetsEnriched')
cfgElectronTrkIsoCutBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('electronsBgEstPhotonPlusJetsEnrichedTrkIsoCumulative')

cfgElectronEcalIsoCutBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgElectronEcalIsoCutBgEstZtautauEnriched)
cfgElectronEcalIsoCutBgEstPhotonPlusJetsEnriched.pluginName = cms.string('electronEcalIsoCutBgEstPhotonPlusJetsEnriched')
cfgElectronEcalIsoCutBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('electronsBgEstPhotonPlusJetsEnrichedEcalIsoCumulative')

cfgElectronTrkIPcutBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgElectronTrkIPcutBgEstZtautauEnriched)
cfgElectronTrkIPcutBgEstPhotonPlusJetsEnriched.pluginName = cms.string('electronTrkIPcutBgEstPhotonPlusJetsEnriched')
cfgElectronTrkIPcutBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('electronsBgEstPhotonPlusJetsEnrichedTrkIPcumulative')

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut
cfgElectronConversionVetoBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgElectronConversionVetoBgEstZtautauEnriched)
cfgElectronConversionVetoBgEstPhotonPlusJetsEnriched.pluginName = cms.string('electronConversionVetoBgEstPhotonPlusJetsEnriched')
cfgElectronConversionVetoBgEstPhotonPlusJetsEnriched.pluginType = cms.string('PATCandViewMaxEventSelector')
cfgElectronConversionVetoBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('electronsBgEstPhotonPlusJetsEnrichedConversionVetoCumulative')
cfgElectronConversionVetoBgEstPhotonPlusJetsEnriched.maxNumber = cms.uint32(0)

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

cfgTauLeadTrkCutBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgTauLeadTrkCutBgEstZtautauEnriched)
cfgTauLeadTrkCutBgEstPhotonPlusJetsEnriched.pluginName = cms.string('tauLeadTrkCutBgEstPhotonPlusJetsEnriched')
cfgTauLeadTrkCutBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstPhotonPlusJetsEnrichedLeadTrkCumulative')

cfgTauLeadTrkPtCutBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgTauLeadTrkPtCutBgEstZtautauEnriched)
cfgTauLeadTrkPtCutBgEstPhotonPlusJetsEnriched.pluginName = cms.string('tauLeadTrkPtCutBgEstPhotonPlusJetsEnriched')
cfgTauLeadTrkPtCutBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstPhotonPlusJetsEnrichedLeadTrkPtCumulative')

cfgTauTaNCdiscrCutBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgTauTaNCdiscrCutBgEstZtautauEnriched)
cfgTauTaNCdiscrCutBgEstPhotonPlusJetsEnriched.pluginName = cms.string('tauTaNCdiscrCutBgEstPhotonPlusJetsEnriched')
cfgTauTaNCdiscrCutBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstPhotonPlusJetsEnrichedTaNCdiscrCumulative')

cfgTauTrkIsoCutBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgTauTrkIsoCutBgEstZtautauEnriched)
cfgTauTrkIsoCutBgEstPhotonPlusJetsEnriched.pluginName = cms.string('tauTrkIsoCutBgEstPhotonPlusJetsEnriched')
cfgTauTrkIsoCutBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstPhotonPlusJetsEnrichedTrkIsoCumulative')

cfgTauEcalIsoCutBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgTauEcalIsoCutBgEstZtautauEnriched)
cfgTauEcalIsoCutBgEstPhotonPlusJetsEnriched.pluginName = cms.string('tauEcalIsoCutBgEstPhotonPlusJetsEnriched')
cfgTauEcalIsoCutBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstPhotonPlusJetsEnrichedEcalIsoCumulative')

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

cfgDiTauCandidateForElecTauZeroChargeCutBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgDiTauCandidateForElecTauZeroChargeCutBgEstZtautauEnriched)
cfgDiTauCandidateForElecTauZeroChargeCutBgEstPhotonPlusJetsEnriched.pluginName = cms.string('diTauCandidateZeroChargeCutBgEstPhotonPlusJetsEnriched')
cfgDiTauCandidateForElecTauZeroChargeCutBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('elecTauPairsBgEstPhotonPlusJetsEnrichedZeroChargeCumulative')

cfgDiTauCandidateForElecTauAcoplanarity12CutBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgDiTauCandidateForElecTauAcoplanarity12CutBgEstZtautauEnriched)
cfgDiTauCandidateForElecTauAcoplanarity12CutBgEstPhotonPlusJetsEnriched.pluginName = cms.string('diTauCandidateAcoplanarity12CutBgEstPhotonPlusJetsEnriched')
cfgDiTauCandidateForElecTauAcoplanarity12CutBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('elecTauPairsBgEstPhotonPlusJetsEnrichedAcoplanarity12Cumulative')

cfgDiTauCandidateForElecTauMt1METCutBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgDiTauCandidateForElecTauMt1METCutBgEstZtautauEnriched)
cfgDiTauCandidateForElecTauMt1METCutBgEstPhotonPlusJetsEnriched.pluginName = cms.string('diTauCandidateMt1METCutBgEstPhotonPlusJetsEnriched')
cfgDiTauCandidateForElecTauMt1METCutBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('elecTauPairsBgEstPhotonPlusJetsEnrichedMt1METcumulative')

cfgDiTauCandidateForElecTauPzetaDiffCutBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgDiTauCandidateForElecTauPzetaDiffCutBgEstZtautauEnriched)
cfgDiTauCandidateForElecTauPzetaDiffCutBgEstPhotonPlusJetsEnriched.pluginName = cms.string('diTauCandidatePzetaDiffCutBgEstPhotonPlusJetsEnriched')
cfgDiTauCandidateForElecTauPzetaDiffCutBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('elecTauPairsBgEstPhotonPlusJetsEnrichedPzetaDiffCumulative')

cfgElecTauPairZeeHypothesisVetoBgEstPhotonPlusJetsEnriched = copy.deepcopy(cfgElecTauPairZeeHypothesisVetoBgEstZtautauEnriched)
cfgElecTauPairZeeHypothesisVetoBgEstPhotonPlusJetsEnriched.pluginName = cms.string('diTauCandidateZeeHypothesisVetoBgEstPhotonPlusJetsEnriched')
cfgElecTauPairZeeHypothesisVetoBgEstPhotonPlusJetsEnriched.src_cumulative = cms.InputTag('selectedElecTauPairBgEstPhotonPlusJetsEnrichedZeeHypotheses')


evtSelConfiguratorBgEstPhotonPlusJetsEnriched = eventSelFlagProdConfigurator(
    [ cfgElectronIdCutBgEstPhotonPlusJetsEnriched,
      cfgElectronAntiCrackCutBgEstPhotonPlusJetsEnriched,
      cfgElectronEtaCutBgEstPhotonPlusJetsEnriched,
      cfgElectronPtCutBgEstPhotonPlusJetsEnriched,
      cfgElectronTrkIsoCutBgEstPhotonPlusJetsEnriched,
      cfgElectronEcalIsoCutBgEstPhotonPlusJetsEnriched,
      cfgElectronConversionVetoBgEstPhotonPlusJetsEnriched,
      cfgElectronTrkIPcutBgEstPhotonPlusJetsEnriched,
      cfgTauAntiOverlapWithElectronsVetoBgEstPhotonPlusJetsEnriched,
      cfgTauEtaCutBgEstPhotonPlusJetsEnriched,
      cfgTauPtCutBgEstPhotonPlusJetsEnriched,
      cfgTauLeadTrkCutBgEstPhotonPlusJetsEnriched,
      cfgTauLeadTrkPtCutBgEstPhotonPlusJetsEnriched,
      cfgTauTaNCdiscrCutBgEstPhotonPlusJetsEnriched,
      cfgTauTrkIsoCutBgEstPhotonPlusJetsEnriched,
      cfgTauEcalIsoCutBgEstPhotonPlusJetsEnriched,
      cfgTauProngCutBgEstPhotonPlusJetsEnriched,
      cfgTauChargeCutBgEstPhotonPlusJetsEnriched,
      cfgTauElectronVetoBgEstPhotonPlusJetsEnriched,
      cfgTauEcalCrackVetoBgEstPhotonPlusJetsEnriched,
      cfgTauMuonVetoBgEstPhotonPlusJetsEnriched,
      cfgDiTauCandidateAntiOverlapVetoBgEstPhotonPlusJetsEnriched,
      cfgDiTauCandidateForElecTauZeroChargeCutBgEstPhotonPlusJetsEnriched,
      cfgDiTauCandidateForElecTauAcoplanarity12CutBgEstPhotonPlusJetsEnriched,
      cfgDiTauCandidateForElecTauMt1METCutBgEstPhotonPlusJetsEnriched,
      cfgDiTauCandidateForElecTauPzetaDiffCutBgEstPhotonPlusJetsEnriched,
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
diTauCandidateHistManagerForElecTauBgEstPhotonPlusJetsEnriched.ZllHypothesisSource = cms.InputTag('selectedElecTauPairBgEstPhotonPlusJetsEnrichedZeeHypotheses')



analyzeEventsBgEstPhotonPlusJetsEnriched = cms.EDAnalyzer("GenericAnalyzer",

    name = cms.string('BgEstTemplateAnalyzer_PhotonPlusJetsEnriched'), 

    filters = cms.VPSet(
        evtSelGenPhaseSpace,
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
            pluginName = cms.string('electronTrkIsoCutBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronTrkIsoCutBgEstPhotonPlusJetsEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('electronEcalIsoCutBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronEcalIsoCutBgEstPhotonPlusJetsEnriched','cumulative')
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
            pluginName = cms.string('tauLeadTrkCutBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauLeadTrkCutBgEstPhotonPlusJetsEnriched','cumulative')
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
            pluginName = cms.string('tauTrkIsoCutBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauTrkIsoCutBgEstPhotonPlusJetsEnriched','cumulative')
        ),  
        cms.PSet(
            pluginName = cms.string('tauEcalIsoCutBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauEcalIsoCutBgEstPhotonPlusJetsEnriched','cumulative')
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
            pluginName = cms.string('diTauCandidateZeroChargeCutBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateZeroChargeCutBgEstPhotonPlusJetsEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('diTauCandidateAcoplanarity12CutBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateAcoplanarity12CutBgEstPhotonPlusJetsEnriched','cumulative')
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
            pluginName = cms.string('diTauCandidateZeeHypothesisVetoBgEstPhotonPlusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateZeeHypothesisVetoBgEstPhotonPlusJetsEnriched','cumulative')
        )
        
        
    ),
  
    analyzers = cms.VPSet(
         diTauCandidateHistManagerForElecTauBgEstPhotonPlusJetsEnriched
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
            title = cms.string('-2.1 < eta(Tau) < +2.1'),
        ),
        cms.PSet(
            filter = cms.string('tauPtCutBgEstPhotonPlusJetsEnriched'),
            title = cms.string('Pt(Tau) > 20 GeV'),
        ),
        cms.PSet(
            filter = cms.string('electronTrkIsoCutBgEstPhotonPlusJetsEnriched'),
            title = cms.string('Electron Track iso.'),
        ),
        cms.PSet(
            filter = cms.string('electronEcalIsoCutBgEstPhotonPlusJetsEnriched'),
            title = cms.string('Electron ECAL iso.'),
        ),
        #!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut
        cms.PSet(
            filter = cms.string('electronConversionVetoBgEstPhotonPlusJetsEnriched'),
            title = cms.string('! Electron Track conv. veto (inverted)'),
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
            filter = cms.string('tauTaNCdiscrCutBgEstPhotonPlusJetsEnriched'),
            title = cms.string('Tau TaNC at 0.5%'),
        ),
        cms.PSet(
            filter = cms.string('tauTrkIsoCutBgEstPhotonPlusJetsEnriched'),
            title = cms.string('Tau Track iso.'),
        ),
        cms.PSet(
            filter = cms.string('tauEcalIsoCutBgEstPhotonPlusJetsEnriched'),
            title = cms.string('Tau ECAL iso.'),
        ),
        cms.PSet(
            filter = cms.string('tauProngCutBgEstPhotonPlusJetsEnriched'),
            title = cms.string('Tau 1||3-Prong'),
        ),
        cms.PSet(
            filter = cms.string('tauChargeCutBgEstPhotonPlusJetsEnriched'),
            title = cms.string('Charge(Tau) = +/-1'),
        ),
        cms.PSet(
            filter = cms.string('tauElectronVetoBgEstPhotonPlusJetsEnriched'),
            title = cms.string('Tau e-Veto'),
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
            title = cms.string('dR(Electron-Tau) > 0.7'),
        ),
        cms.PSet(
            filter = cms.string('diTauCandidateZeroChargeCutBgEstPhotonPlusJetsEnriched'),
            title = cms.string('Charge(Electron+Tau) = 0'),
        ),
        cms.PSet(
            filter = cms.string('diTauCandidateAcoplanarity12CutBgEstPhotonPlusJetsEnriched'),
            title = cms.string('Acoplanarity(Electron+Tau)'),
        ),
        cms.PSet(
            filter = cms.string('diTauCandidateMt1METCutBgEstPhotonPlusJetsEnriched'),
            title = cms.string('M_{T}(Electron-MET) < 50 GeV'),
        ),
        cms.PSet(
            filter = cms.string('diTauCandidatePzetaDiffCutBgEstPhotonPlusJetsEnriched'),
            title = cms.string('P_{#zeta} - 1.5*P_{#zeta}^{vis} > -20 GeV'),
        ),
        cms.PSet(
            filter = cms.string('diTauCandidateZeeHypothesisVetoBgEstPhotonPlusJetsEnriched'),
            title = cms.string('not 85 < M_{vis} (Electron-Tau) < 100 GeV'),
        ),




        
        cms.PSet(
            analyzers = cms.vstring(
                'diTauCandidateHistManagerForElecTauBgEstPhotonPlusJetsEnriched'
            )
        )

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
    + produceElecTauPairsBgEstPhotonPlusJetsEnriched
    + selectElecTauPairsBgEstPhotonPlusJetsEnriched
    + produceElecTauPairZeeHypothesesBgEstPhotonPlusJetsEnriched
    + selectEventsBgEstPhotonPlusJetsEnriched 
    + analyzeEventsBgEstPhotonPlusJetsEnriched

)
