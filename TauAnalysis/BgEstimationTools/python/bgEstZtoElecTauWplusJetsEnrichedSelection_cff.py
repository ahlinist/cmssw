
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
electronsBgEstWplusJetsEnrichedId = copy.deepcopy(electronsBgEstZtautauEnrichedId)

# require electron candidate to not be within eta-crack
# between Barrel and Encap ECAL calorimeter
electronsBgEstWplusJetsEnrichedAntiCrackCut = copy.deepcopy(electronsBgEstZtautauEnrichedAntiCrackCut)

# require electron candidate to be within geometric acceptance of electron trigger
electronsBgEstWplusJetsEnrichedEta = copy.deepcopy(electronsBgEstZtautauEnrichedEta)

# require electron candidate to have transverse momentum above threshold
electronsBgEstWplusJetsEnrichedPt = copy.deepcopy(electronsBgEstZtautauEnrichedPt)

# require electron candidate to be isolated
# with respect to tracks (of Pt >~ 0.3 GeV)
electronsBgEstWplusJetsEnrichedTrkIso = copy.deepcopy(electronsBgEstZtautauEnrichedTrkIso)

# require electron candidate to be isolated
# with respect to energy deposits in ECAL
# (not associated to electron candidate)
electronsBgEstWplusJetsEnrichedEcalIso = copy.deepcopy(electronsBgEstZtautauEnrichedEcalIso)


# require track of electron candidate to have small transverse impact parameter
# (in order to veto electrons resulting from b-quark decays)
electronsBgEstWplusJetsEnrichedTrkIP = copy.deepcopy(electronsBgEstZtautauEnrichedTrkIP)

# require electron to not be from a photon conversion
electronsBgEstWplusJetsEnrichedConversionVeto = copy.deepcopy(electronsBgEstZtautauEnrichedConversionVeto)


electronSelConfiguratorBgEstWplusJetsEnriched = objSelConfigurator(
    [ electronsBgEstWplusJetsEnrichedId,
      electronsBgEstWplusJetsEnrichedAntiCrackCut,
      electronsBgEstWplusJetsEnrichedEta,
      electronsBgEstWplusJetsEnrichedPt,
      electronsBgEstWplusJetsEnrichedTrkIso,
      electronsBgEstWplusJetsEnrichedEcalIso,
      electronsBgEstWplusJetsEnrichedConversionVeto,
      electronsBgEstWplusJetsEnrichedTrkIP ],
    src = "cleanPatElectrons",
    pyModuleName = __name__,
    doSelIndividual = False
)


selectElectronsBgEstWplusJetsEnriched = electronSelConfiguratorBgEstWplusJetsEnriched.configure(pyNameSpace = locals())

   
#--------------------------------------------------------------------------------  
# produce collection of pat::Taus
#--------------------------------------------------------------------------------

#selectTausBgEstWplusJetsEnriched = copy.deepcopy(selectPatTausForElecTau)

# require tau candidate not to overlap with selected electrons
# (in order to avoid double-counting one and the same physical particle
#  as electron and as tau candidate)
tausBgEstWplusJetsEnrichedAntiOverlapWithElectronsVeto = copy.deepcopy(tausBgEstZtautauEnrichedAntiOverlapWithElectronsVeto)

# require tau candidate to be within geometric acceptance of Pixel + SiTracker detectors
tausBgEstWplusJetsEnrichedEta = copy.deepcopy(tausBgEstZtautauEnrichedEta)

# require tau candidate to have transverse energy above threshold
tausBgEstWplusJetsEnrichedPt = copy.deepcopy(tausBgEstZtautauEnrichedPt)

# require tau candidate to have a leading track
# (track of Pt > 1. GeV within matching cone of size dR = 0.2 around jet-axis)
tausBgEstWplusJetsEnrichedLeadTrk = copy.deepcopy(tausBgEstZtautauEnrichedLeadTrk)

# require leading track of tau candidate to have Pt > 5. GeV
tausBgEstWplusJetsEnrichedLeadTrkPt = copy.deepcopy(tausBgEstZtautauEnrichedLeadTrkPt)

# require tau candidate to pass TaNC discriminator
tausBgEstWplusJetsEnrichedTaNCdiscr = copy.deepcopy(tausBgEstZtautauEnrichedTaNCdiscr)

# require tau candidate to have no tracks of Pt > 1. GeV
# in isolation cone of size dR = 0.8, surrounding signal cone of size dR = 5./Et
tausBgEstWplusJetsEnrichedTrkIso = copy.deepcopy(tausBgEstZtautauEnrichedTrkIso)

# require tau candidate to be isolated
# with respect to energy deposits in ECAL
tausBgEstWplusJetsEnrichedEcalIso = copy.deepcopy(tausBgEstZtautauEnrichedEcalIso)

# require tau candidate to have either one or three tracks within signal cone
tausBgEstWplusJetsEnrichedProng = copy.deepcopy(tausBgEstZtautauEnrichedProng)

# require tau candidate to have charge either +1 or -1
# (computed as sum of charges of tracks within signal cone)
tausBgEstWplusJetsEnrichedCharge = copy.deepcopy(tausBgEstZtautauEnrichedCharge)

# require tau candidate to pass electron veto
tausBgEstWplusJetsEnrichedElectronVeto = copy.deepcopy(tausBgEstZtautauEnrichedElectronVeto)

# require tau candidate not to be in ECAL barrel/endcap crack
tausBgEstWplusJetsEnrichedEcalCrackVeto = copy.deepcopy(tausBgEstZtautauEnrichedEcalCrackVeto)

# require tau candidate to pass muon veto
tausBgEstWplusJetsEnrichedMuonVeto = copy.deepcopy(tausBgEstZtautauEnrichedMuonVeto)



tauSelConfiguratorBgEstWplusJetsEnriched = objSelConfigurator(
    [ tausBgEstWplusJetsEnrichedAntiOverlapWithElectronsVeto,
      tausBgEstWplusJetsEnrichedEta,
      tausBgEstWplusJetsEnrichedPt,
      tausBgEstWplusJetsEnrichedLeadTrk,
      tausBgEstWplusJetsEnrichedLeadTrkPt,
      tausBgEstWplusJetsEnrichedTaNCdiscr,
      tausBgEstWplusJetsEnrichedTrkIso,
      tausBgEstWplusJetsEnrichedEcalIso,
      tausBgEstWplusJetsEnrichedProng,
      tausBgEstWplusJetsEnrichedCharge,
      tausBgEstWplusJetsEnrichedElectronVeto,
      tausBgEstWplusJetsEnrichedEcalCrackVeto,
      tausBgEstWplusJetsEnrichedMuonVeto ],
    src = "cleanPatTaus",
    pyModuleName = __name__,
    doSelIndividual = False
)

selectTausBgEstWplusJetsEnriched = tauSelConfiguratorBgEstWplusJetsEnriched.configure(pyNameSpace = locals())


#--------------------------------------------------------------------------------  
# produce collection of electron + tau-jet combinations
#--------------------------------------------------------------------------------

### production


elecTauPairsBgEstWplusJetsEnriched = copy.deepcopy(elecTauPairsBgEstZtautauEnriched)
elecTauPairsBgEstWplusJetsEnriched.srcLeg1 = cms.InputTag('electronsBgEstWplusJetsEnrichedTrkIPcumulative')
elecTauPairsBgEstWplusJetsEnriched.srcLeg2 = cms.InputTag('tausBgEstWplusJetsEnrichedMuonVetoCumulative')

produceElecTauPairsBgEstWplusJetsEnriched = cms.Sequence(elecTauPairsBgEstWplusJetsEnriched)

### selection

elecTauPairsBgEstWplusJetsEnrichedAntiOverlapVeto = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedAntiOverlapVeto)
elecTauPairsBgEstWplusJetsEnrichedZeroCharge = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedZeroCharge)
elecTauPairsBgEstWplusJetsEnrichedAcoplanarity12 = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedAcoplanarity12)
elecTauPairsBgEstWplusJetsEnrichedMt1MET = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedMt1MET)
elecTauPairsBgEstWplusJetsEnrichedPzetaDiff = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedPzetaDiff)

elecTauPairSelConfiguratorBgEstWplusJetsEnriched = objSelConfigurator(
    [ elecTauPairsBgEstWplusJetsEnrichedAntiOverlapVeto,
      elecTauPairsBgEstWplusJetsEnrichedZeroCharge,
      elecTauPairsBgEstWplusJetsEnrichedAcoplanarity12,
      elecTauPairsBgEstWplusJetsEnrichedMt1MET,
      elecTauPairsBgEstWplusJetsEnrichedPzetaDiff,
      ],
    src = "elecTauPairsBgEstWplusJetsEnriched",
    pyModuleName = __name__,
    doSelIndividual = False
)

selectElecTauPairsBgEstWplusJetsEnriched = elecTauPairSelConfiguratorBgEstWplusJetsEnriched.configure(pyNameSpace = locals())

####### anti Zee Cut #######

### production


elecTauPairBgEstWplusJetsEnrichedZeeHypotheses = copy.deepcopy(elecTauPairBgEstZtautauEnrichedZeeHypotheses)
elecTauPairBgEstWplusJetsEnrichedZeeHypotheses.diCandidatePairSource = cms.InputTag('elecTauPairsBgEstWplusJetsEnrichedPzetaDiffCumulative')

### selection

selectedElecTauPairBgEstWplusJetsEnrichedZeeHypotheses = copy.deepcopy(selectedElecTauPairBgEstZtautauEnrichedZeeHypotheses)
selectedElecTauPairBgEstWplusJetsEnrichedZeeHypotheses.src = cms.InputTag('elecTauPairBgEstWplusJetsEnrichedZeeHypotheses')


produceElecTauPairZeeHypothesesBgEstWplusJetsEnriched = cms.Sequence(elecTauPairBgEstWplusJetsEnrichedZeeHypotheses*selectedElecTauPairBgEstWplusJetsEnrichedZeeHypotheses)

#--------------------------------------------------------------------------------  
# produce boolean event selection flags
#--------------------------------------------------------------------------------


# electron cuts

cfgElectronIdCutBgEstWplusJetsEnriched = copy.deepcopy(cfgElectronIdCutBgEstZtautauEnriched)
cfgElectronIdCutBgEstWplusJetsEnriched.pluginName = cms.string('electronIdCutBgEstWplusJetsEnriched')
cfgElectronIdCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('electronsBgEstWplusJetsEnrichedIdCumulative')

cfgElectronAntiCrackCutBgEstWplusJetsEnriched = copy.deepcopy(cfgElectronAntiCrackCutBgEstZtautauEnriched)
cfgElectronAntiCrackCutBgEstWplusJetsEnriched.pluginName = cms.string('electronAntiCrackCutBgEstWplusJetsEnriched')
cfgElectronAntiCrackCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('electronsBgEstWplusJetsEnrichedAntiCrackCutCumulative')

cfgElectronEtaCutBgEstWplusJetsEnriched = copy.deepcopy(cfgElectronEtaCutBgEstZtautauEnriched)
cfgElectronEtaCutBgEstWplusJetsEnriched.pluginName = cms.string('electronEtaCutBgEstWplusJetsEnriched')
cfgElectronEtaCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('electronsBgEstWplusJetsEnrichedEtaCumulative')

cfgElectronPtCutBgEstWplusJetsEnriched = copy.deepcopy(cfgElectronPtCutBgEstZtautauEnriched)
cfgElectronPtCutBgEstWplusJetsEnriched.pluginName = cms.string('electronPtCutBgEstWplusJetsEnriched')
cfgElectronPtCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('electronsBgEstWplusJetsEnrichedPtCumulative')

cfgElectronTrkIsoCutBgEstWplusJetsEnriched = copy.deepcopy(cfgElectronTrkIsoCutBgEstZtautauEnriched)
cfgElectronTrkIsoCutBgEstWplusJetsEnriched.pluginName = cms.string('electronTrkIsoCutBgEstWplusJetsEnriched')
cfgElectronTrkIsoCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('electronsBgEstWplusJetsEnrichedTrkIsoCumulative')

cfgElectronEcalIsoCutBgEstWplusJetsEnriched = copy.deepcopy(cfgElectronEcalIsoCutBgEstZtautauEnriched)
cfgElectronEcalIsoCutBgEstWplusJetsEnriched.pluginName = cms.string('electronEcalIsoCutBgEstWplusJetsEnriched')
cfgElectronEcalIsoCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('electronsBgEstWplusJetsEnrichedEcalIsoCumulative')

cfgElectronTrkIPcutBgEstWplusJetsEnriched = copy.deepcopy(cfgElectronTrkIPcutBgEstZtautauEnriched)
cfgElectronTrkIPcutBgEstWplusJetsEnriched.pluginName = cms.string('electronTrkIPcutBgEstWplusJetsEnriched')
cfgElectronTrkIPcutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('electronsBgEstWplusJetsEnrichedTrkIPcumulative')

cfgElectronConversionVetoBgEstWplusJetsEnriched = copy.deepcopy(cfgElectronConversionVetoBgEstZtautauEnriched)
cfgElectronConversionVetoBgEstWplusJetsEnriched.pluginName = cms.string('electronConversionVetoBgEstWplusJetsEnriched')
cfgElectronConversionVetoBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('electronsBgEstWplusJetsEnrichedConversionVetoCumulative')

# tau cuts

cfgTauAntiOverlapWithElectronsVetoBgEstWplusJetsEnriched = copy.deepcopy(cfgTauAntiOverlapWithElectronsVetoBgEstZtautauEnriched)
cfgTauAntiOverlapWithElectronsVetoBgEstWplusJetsEnriched.pluginName = cms.string('tauAntiOverlapWithElectronsVetoBgEstWplusJetsEnriched')
cfgTauAntiOverlapWithElectronsVetoBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstWplusJetsEnrichedAntiOverlapWithElectronsVetoCumulative')

cfgTauEtaCutBgEstWplusJetsEnriched = copy.deepcopy(cfgTauEtaCutBgEstZtautauEnriched)
cfgTauEtaCutBgEstWplusJetsEnriched.pluginName = cms.string('tauEtaCutBgEstWplusJetsEnriched')
cfgTauEtaCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstWplusJetsEnrichedEtaCumulative')

cfgTauPtCutBgEstWplusJetsEnriched = copy.deepcopy(cfgTauPtCutBgEstZtautauEnriched)
cfgTauPtCutBgEstWplusJetsEnriched.pluginName = cms.string('tauPtCutBgEstWplusJetsEnriched')
cfgTauPtCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstWplusJetsEnrichedPtCumulative')

cfgTauLeadTrkCutBgEstWplusJetsEnriched = copy.deepcopy(cfgTauLeadTrkCutBgEstZtautauEnriched)
cfgTauLeadTrkCutBgEstWplusJetsEnriched.pluginName = cms.string('tauLeadTrkCutBgEstWplusJetsEnriched')
cfgTauLeadTrkCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstWplusJetsEnrichedLeadTrkCumulative')

cfgTauLeadTrkPtCutBgEstWplusJetsEnriched = copy.deepcopy(cfgTauLeadTrkPtCutBgEstZtautauEnriched)
cfgTauLeadTrkPtCutBgEstWplusJetsEnriched.pluginName = cms.string('tauLeadTrkPtCutBgEstWplusJetsEnriched')
cfgTauLeadTrkPtCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstWplusJetsEnrichedLeadTrkPtCumulative')

cfgTauTaNCdiscrCutBgEstWplusJetsEnriched = copy.deepcopy(cfgTauTaNCdiscrCutBgEstZtautauEnriched)
cfgTauTaNCdiscrCutBgEstWplusJetsEnriched.pluginName = cms.string('tauTaNCdiscrCutBgEstWplusJetsEnriched')
cfgTauTaNCdiscrCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstWplusJetsEnrichedTaNCdiscrCumulative')

cfgTauTrkIsoCutBgEstWplusJetsEnriched = copy.deepcopy(cfgTauTrkIsoCutBgEstZtautauEnriched)
cfgTauTrkIsoCutBgEstWplusJetsEnriched.pluginName = cms.string('tauTrkIsoCutBgEstWplusJetsEnriched')
cfgTauTrkIsoCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstWplusJetsEnrichedTrkIsoCumulative')

cfgTauEcalIsoCutBgEstWplusJetsEnriched = copy.deepcopy(cfgTauEcalIsoCutBgEstZtautauEnriched)
cfgTauEcalIsoCutBgEstWplusJetsEnriched.pluginName = cms.string('tauEcalIsoCutBgEstWplusJetsEnriched')
cfgTauEcalIsoCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstWplusJetsEnrichedEcalIsoCumulative')

cfgTauProngCutBgEstWplusJetsEnriched = copy.deepcopy(cfgTauProngCutBgEstZtautauEnriched)
cfgTauProngCutBgEstWplusJetsEnriched.pluginName = cms.string('tauProngCutBgEstWplusJetsEnriched')
cfgTauProngCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstWplusJetsEnrichedProngCumulative')

cfgTauChargeCutBgEstWplusJetsEnriched = copy.deepcopy(cfgTauChargeCutBgEstZtautauEnriched)
cfgTauChargeCutBgEstWplusJetsEnriched.pluginName = cms.string('tauChargeCutBgEstWplusJetsEnriched')
cfgTauChargeCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstWplusJetsEnrichedChargeCumulative')

cfgTauElectronVetoBgEstWplusJetsEnriched = copy.deepcopy(cfgTauElectronVetoBgEstZtautauEnriched)
cfgTauElectronVetoBgEstWplusJetsEnriched.pluginName = cms.string('tauElectronVetoBgEstWplusJetsEnriched')
cfgTauElectronVetoBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstWplusJetsEnrichedElectronVetoCumulative')

cfgTauEcalCrackVetoBgEstWplusJetsEnriched = copy.deepcopy(cfgTauEcalCrackVetoBgEstZtautauEnriched)
cfgTauEcalCrackVetoBgEstWplusJetsEnriched.pluginName = cms.string('tauEcalCrackVetoBgEstWplusJetsEnriched')
cfgTauEcalCrackVetoBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstWplusJetsEnrichedEcalCrackVetoCumulative')

cfgTauMuonVetoBgEstWplusJetsEnriched = copy.deepcopy(cfgTauMuonVetoBgEstZtautauEnriched)
cfgTauMuonVetoBgEstWplusJetsEnriched.pluginName = cms.string('tauMuonVetoBgEstWplusJetsEnriched')
cfgTauMuonVetoBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstWplusJetsEnrichedMuonVetoCumulative')

# di-tau candidate selection

cfgDiTauCandidateAntiOverlapVetoBgEstWplusJetsEnriched = copy.deepcopy(cfgDiTauCandidateAntiOverlapVetoBgEstZtautauEnriched)
cfgDiTauCandidateAntiOverlapVetoBgEstWplusJetsEnriched.pluginName = cms.string('diTauCandidateAntiOverlapVetoBgEstWplusJetsEnriched')
cfgDiTauCandidateAntiOverlapVetoBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('elecTauPairsBgEstWplusJetsEnrichedAntiOverlapVetoCumulative')

cfgDiTauCandidateForElecTauZeroChargeCutBgEstWplusJetsEnriched = copy.deepcopy(cfgDiTauCandidateForElecTauZeroChargeCutBgEstZtautauEnriched)
cfgDiTauCandidateForElecTauZeroChargeCutBgEstWplusJetsEnriched.pluginName = cms.string('diTauCandidateZeroChargeCutBgEstWplusJetsEnriched')
cfgDiTauCandidateForElecTauZeroChargeCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('elecTauPairsBgEstWplusJetsEnrichedZeroChargeCumulative')

cfgDiTauCandidateForElecTauAcoplanarity12CutBgEstWplusJetsEnriched = copy.deepcopy(cfgDiTauCandidateForElecTauAcoplanarity12CutBgEstZtautauEnriched)
cfgDiTauCandidateForElecTauAcoplanarity12CutBgEstWplusJetsEnriched.pluginName = cms.string('diTauCandidateAcoplanarity12CutBgEstWplusJetsEnriched')
cfgDiTauCandidateForElecTauAcoplanarity12CutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('elecTauPairsBgEstWplusJetsEnrichedAcoplanarity12Cumulative')

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut
cfgDiTauCandidateForElecTauMt1METCutBgEstWplusJetsEnriched = copy.deepcopy(cfgDiTauCandidateForElecTauMt1METCutBgEstZtautauEnriched)
cfgDiTauCandidateForElecTauMt1METCutBgEstWplusJetsEnriched.pluginName = cms.string('diTauCandidateMt1METCutBgEstWplusJetsEnriched')
cfgDiTauCandidateForElecTauMt1METCutBgEstWplusJetsEnriched.pluginType = cms.string('PATCandViewMaxEventSelector')
cfgDiTauCandidateForElecTauMt1METCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('elecTauPairsBgEstWplusJetsEnrichedMt1METcumulative')
cfgDiTauCandidateForElecTauMt1METCutBgEstWplusJetsEnriched.maxNumber = cms.uint32(0)

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut
cfgDiTauCandidateForElecTauPzetaDiffCutBgEstWplusJetsEnriched = copy.deepcopy(cfgDiTauCandidateForElecTauPzetaDiffCutBgEstZtautauEnriched)
cfgDiTauCandidateForElecTauPzetaDiffCutBgEstWplusJetsEnriched.pluginName = cms.string('diTauCandidatePzetaDiffCutBgEstWplusJetsEnriched')
cfgDiTauCandidateForElecTauPzetaDiffCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('elecTauPairsBgEstWplusJetsEnrichedPzetaDiffCumulative')
cfgDiTauCandidateForElecTauPzetaDiffCutBgEstWplusJetsEnriched.pluginType = cms.string('PATCandViewMaxEventSelector')
cfgDiTauCandidateForElecTauPzetaDiffCutBgEstWplusJetsEnriched.maxNumber = cms.uint32(0)


cfgElecTauPairZeeHypothesisVetoBgEstWplusJetsEnriched = copy.deepcopy(cfgElecTauPairZeeHypothesisVetoBgEstZtautauEnriched)
cfgElecTauPairZeeHypothesisVetoBgEstWplusJetsEnriched.pluginName = cms.string('diTauCandidateZeeHypothesisVetoBgEstWplusJetsEnriched')
cfgElecTauPairZeeHypothesisVetoBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('selectedElecTauPairBgEstWplusJetsEnrichedZeeHypotheses')


evtSelConfiguratorBgEstWplusJetsEnriched = eventSelFlagProdConfigurator(
    [ cfgElectronIdCutBgEstWplusJetsEnriched,
      cfgElectronAntiCrackCutBgEstWplusJetsEnriched,
      cfgElectronEtaCutBgEstWplusJetsEnriched,
      cfgElectronPtCutBgEstWplusJetsEnriched,
      cfgElectronTrkIsoCutBgEstWplusJetsEnriched,
      cfgElectronEcalIsoCutBgEstWplusJetsEnriched,
      cfgElectronConversionVetoBgEstWplusJetsEnriched,
      cfgElectronTrkIPcutBgEstWplusJetsEnriched,
      cfgTauAntiOverlapWithElectronsVetoBgEstWplusJetsEnriched,
      cfgTauEtaCutBgEstWplusJetsEnriched,
      cfgTauPtCutBgEstWplusJetsEnriched,
      cfgTauLeadTrkCutBgEstWplusJetsEnriched,
      cfgTauLeadTrkPtCutBgEstWplusJetsEnriched,
      cfgTauTaNCdiscrCutBgEstWplusJetsEnriched,
      cfgTauTrkIsoCutBgEstWplusJetsEnriched,
      cfgTauEcalIsoCutBgEstWplusJetsEnriched,
      cfgTauProngCutBgEstWplusJetsEnriched,
      cfgTauChargeCutBgEstWplusJetsEnriched,
      cfgTauElectronVetoBgEstWplusJetsEnriched,
      cfgTauEcalCrackVetoBgEstWplusJetsEnriched,
      cfgTauMuonVetoBgEstWplusJetsEnriched,
      cfgDiTauCandidateAntiOverlapVetoBgEstWplusJetsEnriched,
      cfgDiTauCandidateForElecTauZeroChargeCutBgEstWplusJetsEnriched,
      cfgDiTauCandidateForElecTauAcoplanarity12CutBgEstWplusJetsEnriched,
      cfgDiTauCandidateForElecTauMt1METCutBgEstWplusJetsEnriched,
      cfgDiTauCandidateForElecTauPzetaDiffCutBgEstWplusJetsEnriched,
      cfgElecTauPairZeeHypothesisVetoBgEstWplusJetsEnriched
      ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

selectEventsBgEstWplusJetsEnriched = evtSelConfiguratorBgEstWplusJetsEnriched.configure()


#--------------------------------------------------------------------------------  
# apply event selection criteria; fill histograms
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.analyzeZtoElecTau_cff import *


diTauCandidateHistManagerForElecTauBgEstWplusJetsEnriched = copy.deepcopy(diTauCandidateHistManagerForElecTauBgEstZtautauEnriched)
diTauCandidateHistManagerForElecTauBgEstWplusJetsEnriched.pluginName = cms.string('diTauCandidateHistManagerForElecTauBgEstWplusJetsEnriched')
diTauCandidateHistManagerForElecTauBgEstWplusJetsEnriched.ZllHypothesisSource = cms.InputTag('selectedElecTauPairBgEstWplusJetsEnrichedZeeHypotheses')



analyzeEventsBgEstWplusJetsEnriched = cms.EDAnalyzer("GenericAnalyzer",

    name = cms.string('BgEstTemplateAnalyzer_WplusJetsEnriched'), 

    filters = cms.VPSet(
        evtSelGenPhaseSpace,
        evtSelTrigger,
        evtSelPrimaryEventVertex,
        evtSelPrimaryEventVertexQuality,
        evtSelPrimaryEventVertexPosition,

        #start electron cuts
        
        cms.PSet(
            pluginName = cms.string('electronIdCutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronIdCutBgEstWplusJetsEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('electronAntiCrackCutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronAntiCrackCutBgEstWplusJetsEnriched','cumulative')
        ),        
        cms.PSet(
            pluginName = cms.string('electronEtaCutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronEtaCutBgEstWplusJetsEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('electronPtCutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronPtCutBgEstWplusJetsEnriched','cumulative')
        ),        
        cms.PSet(
            pluginName = cms.string('electronTrkIsoCutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronTrkIsoCutBgEstWplusJetsEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('electronEcalIsoCutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronEcalIsoCutBgEstWplusJetsEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('electronConversionVetoBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronConversionVetoBgEstWplusJetsEnriched','cumulative')
        ),        
        cms.PSet(
            pluginName = cms.string('electronTrkIPcutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronTrkIPcutBgEstWplusJetsEnriched','cumulative')
        ),


        #start tau cuts
        
        cms.PSet(
            pluginName = cms.string('tauAntiOverlapWithElectronsVetoBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauAntiOverlapWithElectronsVetoBgEstWplusJetsEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauEtaCutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauEtaCutBgEstWplusJetsEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauPtCutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauPtCutBgEstWplusJetsEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauLeadTrkCutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauLeadTrkCutBgEstWplusJetsEnriched','cumulative')
        ),        
        cms.PSet(
            pluginName = cms.string('tauLeadTrkPtCutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauLeadTrkPtCutBgEstWplusJetsEnriched','cumulative')
        ),  
        cms.PSet(
            pluginName = cms.string('tauTaNCdiscrCutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauTaNCdiscrCutBgEstWplusJetsEnriched','cumulative')
        ),  
        cms.PSet(
            pluginName = cms.string('tauTrkIsoCutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauTrkIsoCutBgEstWplusJetsEnriched','cumulative')
        ),  
        cms.PSet(
            pluginName = cms.string('tauEcalIsoCutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauEcalIsoCutBgEstWplusJetsEnriched','cumulative')
        ),  
        cms.PSet(
            pluginName = cms.string('tauProngCutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauProngCutBgEstWplusJetsEnriched','cumulative')
        ),  
        cms.PSet(
            pluginName = cms.string('tauChargeCutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauChargeCutBgEstWplusJetsEnriched','cumulative')
        ),  
        cms.PSet(
            pluginName = cms.string('tauElectronVetoBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauElectronVetoBgEstWplusJetsEnriched','cumulative')
        ),  
        cms.PSet(
            pluginName = cms.string('tauEcalCrackVetoBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauEcalCrackVetoBgEstWplusJetsEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauMuonVetoBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauMuonVetoBgEstWplusJetsEnriched','cumulative')
        ),

        #start ditau cuts
        
        cms.PSet(
            pluginName = cms.string('diTauCandidateAntiOverlapVetoBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateAntiOverlapVetoBgEstWplusJetsEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('diTauCandidateZeroChargeCutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateZeroChargeCutBgEstWplusJetsEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('diTauCandidateAcoplanarity12CutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateAcoplanarity12CutBgEstWplusJetsEnriched','cumulative')
        ),
        #!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut
        cms.PSet(
            pluginName = cms.string('diTauCandidateMt1METCutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateMt1METCutBgEstWplusJetsEnriched','cumulative')
        ),
        #!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut        
        cms.PSet(
            pluginName = cms.string('diTauCandidatePzetaDiffCutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidatePzetaDiffCutBgEstWplusJetsEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('diTauCandidateZeeHypothesisVetoBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateZeeHypothesisVetoBgEstWplusJetsEnriched','cumulative')
        )
        
        
    ),
  
    analyzers = cms.VPSet(
         diTauCandidateHistManagerForElecTauBgEstWplusJetsEnriched
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
            filter = cms.string('electronIdCutBgEstWplusJetsEnriched'),
            title = cms.string('Electron ID'),
        ),
        cms.PSet(
            filter = cms.string('electronAntiCrackCutBgEstWplusJetsEnriched'),
            title = cms.string('Electron crack-Veto'),
        ),
        cms.PSet(
            filter = cms.string('electronEtaCutBgEstWplusJetsEnriched'),
            title = cms.string('-2.1 < eta(Electron) < +2.1'),
        ),
        cms.PSet(
            filter = cms.string('electronPtCutBgEstWplusJetsEnriched'),
            title = cms.string('Pt(Electron) > 15 GeV'),
        ),
        cms.PSet(
            filter = cms.string('tauAntiOverlapWithElectronsVetoBgEstWplusJetsEnriched'),
            title = cms.string('Tau not overlapping with Elec.'),
        ),
        cms.PSet(
            filter = cms.string('tauEtaCutBgEstWplusJetsEnriched'),
            title = cms.string('-2.1 < eta(Tau) < +2.1'),
        ),
        cms.PSet(
            filter = cms.string('tauPtCutBgEstWplusJetsEnriched'),
            title = cms.string('Pt(Tau) > 20 GeV'),
        ),
        cms.PSet(
            filter = cms.string('electronTrkIsoCutBgEstWplusJetsEnriched'),
            title = cms.string('Electron Track iso.'),
        ),
        cms.PSet(
            filter = cms.string('electronEcalIsoCutBgEstWplusJetsEnriched'),
            title = cms.string('Electron ECAL iso.'),
        ),
        cms.PSet(
            filter = cms.string('electronConversionVetoBgEstWplusJetsEnriched'),
            title = cms.string('Electron Track conv. veto'),
        ),
        cms.PSet(
            filter = cms.string('electronTrkIPcutBgEstWplusJetsEnriched'),
            title = cms.string('Electron Track IP'),
        ),
        cms.PSet(
            filter = cms.string('tauLeadTrkPtCutBgEstWplusJetsEnriched'),
            title = cms.string('Tau lead. Track Pt'),
        ),
        cms.PSet(
            filter = cms.string('tauTaNCdiscrCutBgEstWplusJetsEnriched'),
            title = cms.string('Tau TaNC at 0.5%'),
        ),
        cms.PSet(
            filter = cms.string('tauTrkIsoCutBgEstWplusJetsEnriched'),
            title = cms.string('Tau Track iso.'),
        ),
        cms.PSet(
            filter = cms.string('tauEcalIsoCutBgEstWplusJetsEnriched'),
            title = cms.string('Tau ECAL iso.'),
        ),
        cms.PSet(
            filter = cms.string('tauProngCutBgEstWplusJetsEnriched'),
            title = cms.string('Tau 1||3-Prong'),
        ),
        cms.PSet(
            filter = cms.string('tauChargeCutBgEstWplusJetsEnriched'),
            title = cms.string('Charge(Tau) = +/-1'),
        ),
        cms.PSet(
            filter = cms.string('tauElectronVetoBgEstWplusJetsEnriched'),
            title = cms.string('Tau e-Veto'),
        ),
        cms.PSet(
            filter = cms.string('tauEcalCrackVetoBgEstWplusJetsEnriched'),
            title = cms.string('Tau ECAL crack-Veto'),
        ),
        cms.PSet(
            filter = cms.string('tauMuonVetoBgEstWplusJetsEnriched'),
            title = cms.string('Tau mu-Veto'),
        ),
        cms.PSet(
            filter = cms.string('diTauCandidateAntiOverlapVetoBgEstWplusJetsEnriched'),
            title = cms.string('dR(Electron-Tau) > 0.7'),
        ),
        cms.PSet(
            filter = cms.string('diTauCandidateZeroChargeCutBgEstWplusJetsEnriched'),
            title = cms.string('Charge(Electron+Tau) = 0'),
        ),
        cms.PSet(
            filter = cms.string('diTauCandidateAcoplanarity12CutBgEstWplusJetsEnriched'),
            title = cms.string('Acoplanarity(Electron+Tau)'),
        ),
        #!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut
        cms.PSet(
            filter = cms.string('diTauCandidateMt1METCutBgEstWplusJetsEnriched'),
            title = cms.string('! M_{T}(Electron-MET) < 50 GeV'),
        ),
        #!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut        
        cms.PSet(
            filter = cms.string('diTauCandidatePzetaDiffCutBgEstWplusJetsEnriched'),
            title = cms.string('! P_{#zeta} - 1.5*P_{#zeta}^{vis} > -20 GeV'),
        ),
        cms.PSet(
            filter = cms.string('diTauCandidateZeeHypothesisVetoBgEstWplusJetsEnriched'),
            title = cms.string('not 85 < M_{vis} (Electron-Tau) < 100 GeV'),
        ),




        
        cms.PSet(
            analyzers = cms.vstring(
                'diTauCandidateHistManagerForElecTauBgEstWplusJetsEnriched'
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

bgEstWplusJetsEnrichedAnalysisSequence = cms.Sequence(

    selectElectronsBgEstWplusJetsEnriched
    + selectTausBgEstWplusJetsEnriched
    + produceElecTauPairsBgEstWplusJetsEnriched
    + selectElecTauPairsBgEstWplusJetsEnriched
    + produceElecTauPairZeeHypothesesBgEstWplusJetsEnriched
    + selectEventsBgEstWplusJetsEnriched 
    + analyzeEventsBgEstWplusJetsEnriched

)
