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
electronsBgEstQCDenrichedId = copy.deepcopy(electronsBgEstZtautauEnrichedId)

# require electron candidate to not be within eta-crack
# between Barrel and Encap ECAL calorimeter
electronsBgEstQCDenrichedAntiCrackCut = copy.deepcopy(electronsBgEstZtautauEnrichedAntiCrackCut)

# require electron candidate to be within geometric acceptance of electron trigger
electronsBgEstQCDenrichedEta = copy.deepcopy(electronsBgEstZtautauEnrichedEta)

# require electron candidate to have transverse momentum above threshold
electronsBgEstQCDenrichedPt = copy.deepcopy(electronsBgEstZtautauEnrichedPt)

# require electron candidate to be isolated
# with respect to tracks (of Pt >~ 0.3 GeV)
electronsBgEstQCDenrichedTrkIso = copy.deepcopy(electronsBgEstZtautauEnrichedTrkIso)

# require electron candidate to be isolated
# with respect to energy deposits in ECAL
# (not associated to electron candidate)
electronsBgEstQCDenrichedEcalIso = copy.deepcopy(electronsBgEstZtautauEnrichedEcalIso)


# require track of electron candidate to have small transverse impact parameter
# (in order to veto electrons resulting from b-quark decays)
electronsBgEstQCDenrichedTrkIP = copy.deepcopy(electronsBgEstZtautauEnrichedTrkIP)

# require electron to not be from a photon conversion
electronsBgEstQCDenrichedConversionVeto = copy.deepcopy(electronsBgEstZtautauEnrichedConversionVeto)


electronSelConfiguratorBgEstQCDenriched = objSelConfigurator(
    [ electronsBgEstQCDenrichedId,
      electronsBgEstQCDenrichedAntiCrackCut,
      electronsBgEstQCDenrichedEta,
      electronsBgEstQCDenrichedPt,
      electronsBgEstQCDenrichedTrkIso,
      electronsBgEstQCDenrichedEcalIso,
      electronsBgEstQCDenrichedConversionVeto,
      electronsBgEstQCDenrichedTrkIP ],
    src = "cleanPatElectrons",
    pyModuleName = __name__,
    doSelIndividual = False
)


selectElectronsBgEstQCDenriched = electronSelConfiguratorBgEstQCDenriched.configure(pyNameSpace = locals())

   
#--------------------------------------------------------------------------------  
# produce collection of pat::Taus
#--------------------------------------------------------------------------------

#selectTausBgEstQCDenriched = copy.deepcopy(selectPatTausForElecTau)

# require tau candidate not to overlap with selected electrons
# (in order to avoid double-counting one and the same physical particle
#  as electron and as tau candidate)
tausBgEstQCDenrichedAntiOverlapWithElectronsVeto = copy.deepcopy(tausBgEstZtautauEnrichedAntiOverlapWithElectronsVeto)

# require tau candidate to be within geometric acceptance of Pixel + SiTracker detectors
tausBgEstQCDenrichedEta = copy.deepcopy(tausBgEstZtautauEnrichedEta)

# require tau candidate to have transverse energy above threshold
tausBgEstQCDenrichedPt = copy.deepcopy(tausBgEstZtautauEnrichedPt)

# require tau candidate to have a leading track
# (track of Pt > 1. GeV within matching cone of size dR = 0.2 around jet-axis)
tausBgEstQCDenrichedLeadTrk = copy.deepcopy(tausBgEstZtautauEnrichedLeadTrk)

# require leading track of tau candidate to have Pt > 5. GeV
tausBgEstQCDenrichedLeadTrkPt = copy.deepcopy(tausBgEstZtautauEnrichedLeadTrkPt)

# require tau candidate to pass TaNC discriminator
tausBgEstQCDenrichedTaNCdiscr = copy.deepcopy(tausBgEstZtautauEnrichedTaNCdiscr)

# require tau candidate to have no tracks of Pt > 1. GeV
# in isolation cone of size dR = 0.8, surrounding signal cone of size dR = 5./Et
tausBgEstQCDenrichedTrkIso = copy.deepcopy(tausBgEstZtautauEnrichedTrkIso)

# require tau candidate to be isolated
# with respect to energy deposits in ECAL
tausBgEstQCDenrichedEcalIso = copy.deepcopy(tausBgEstZtautauEnrichedEcalIso)

# require tau candidate to have either one or three tracks within signal cone
tausBgEstQCDenrichedProng = copy.deepcopy(tausBgEstZtautauEnrichedProng)

# require tau candidate to have charge either +1 or -1
# (computed as sum of charges of tracks within signal cone)
tausBgEstQCDenrichedCharge = copy.deepcopy(tausBgEstZtautauEnrichedCharge)

# require tau candidate to pass electron veto
tausBgEstQCDenrichedElectronVeto = copy.deepcopy(tausBgEstZtautauEnrichedElectronVeto)

# require tau candidate not to be in ECAL barrel/endcap crack
tausBgEstQCDenrichedEcalCrackVeto = copy.deepcopy(tausBgEstZtautauEnrichedEcalCrackVeto)

# require tau candidate to pass muon veto
tausBgEstQCDenrichedMuonVeto = copy.deepcopy(tausBgEstZtautauEnrichedMuonVeto)



tauSelConfiguratorBgEstQCDenriched = objSelConfigurator(
    [ tausBgEstQCDenrichedAntiOverlapWithElectronsVeto,
      tausBgEstQCDenrichedEta,
      tausBgEstQCDenrichedPt,
      tausBgEstQCDenrichedLeadTrk,
      tausBgEstQCDenrichedLeadTrkPt,
      tausBgEstQCDenrichedTaNCdiscr,
      tausBgEstQCDenrichedTrkIso,
      tausBgEstQCDenrichedEcalIso,
      tausBgEstQCDenrichedProng,
      tausBgEstQCDenrichedCharge,
      tausBgEstQCDenrichedElectronVeto,
      tausBgEstQCDenrichedEcalCrackVeto,
      tausBgEstQCDenrichedMuonVeto ],
    src = "cleanPatTaus",
    pyModuleName = __name__,
    doSelIndividual = False
)

selectTausBgEstQCDenriched = tauSelConfiguratorBgEstQCDenriched.configure(pyNameSpace = locals())


#--------------------------------------------------------------------------------  
# produce collection of electron + tau-jet combinations
#--------------------------------------------------------------------------------

### production


elecTauPairsBgEstQCDenriched = copy.deepcopy(elecTauPairsBgEstZtautauEnriched)
elecTauPairsBgEstQCDenriched.srcLeg1 = cms.InputTag('electronsBgEstQCDenrichedTrkIPcumulative')
elecTauPairsBgEstQCDenriched.srcLeg2 = cms.InputTag('tausBgEstQCDenrichedMuonVetoCumulative')

produceElecTauPairsBgEstQCDenriched = cms.Sequence(elecTauPairsBgEstQCDenriched)

### selection

elecTauPairsBgEstQCDenrichedAntiOverlapVeto = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedAntiOverlapVeto)
elecTauPairsBgEstQCDenrichedZeroCharge = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedZeroCharge)
elecTauPairsBgEstQCDenrichedAcoplanarity12 = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedAcoplanarity12)
elecTauPairsBgEstQCDenrichedMt1MET = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedMt1MET)
elecTauPairsBgEstQCDenrichedPzetaDiff = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedPzetaDiff)

elecTauPairSelConfiguratorBgEstQCDenriched = objSelConfigurator(
    [ elecTauPairsBgEstQCDenrichedAntiOverlapVeto,
      elecTauPairsBgEstQCDenrichedZeroCharge,
      elecTauPairsBgEstQCDenrichedAcoplanarity12,
      elecTauPairsBgEstQCDenrichedMt1MET,
      elecTauPairsBgEstQCDenrichedPzetaDiff,
      ],
    src = "elecTauPairsBgEstQCDenriched",
    pyModuleName = __name__,
    doSelIndividual = False
)

selectElecTauPairsBgEstQCDenriched = elecTauPairSelConfiguratorBgEstQCDenriched.configure(pyNameSpace = locals())

####### anti Zee Cut #######

### production


elecTauPairBgEstQCDenrichedZeeHypotheses = copy.deepcopy(elecTauPairBgEstZtautauEnrichedZeeHypotheses)
elecTauPairBgEstQCDenrichedZeeHypotheses.diCandidatePairSource = cms.InputTag('elecTauPairsBgEstQCDenrichedPzetaDiffCumulative')

### selection

selectedElecTauPairBgEstQCDenrichedZeeHypotheses = copy.deepcopy(selectedElecTauPairBgEstZtautauEnrichedZeeHypotheses)
selectedElecTauPairBgEstQCDenrichedZeeHypotheses.src = cms.InputTag('elecTauPairBgEstQCDenrichedZeeHypotheses')


produceElecTauPairZeeHypothesesBgEstQCDenriched = cms.Sequence(elecTauPairBgEstQCDenrichedZeeHypotheses*selectedElecTauPairBgEstQCDenrichedZeeHypotheses)

#--------------------------------------------------------------------------------  
# produce boolean event selection flags
#--------------------------------------------------------------------------------


# electron cuts


cfgElectronIdCutBgEstQCDenriched = copy.deepcopy(cfgElectronIdCutBgEstZtautauEnriched)
cfgElectronIdCutBgEstQCDenriched.pluginName = cms.string('electronIdCutBgEstQCDenriched')
cfgElectronIdCutBgEstQCDenriched.src_cumulative = cms.InputTag('electronsBgEstQCDenrichedIdCumulative')

cfgElectronAntiCrackCutBgEstQCDenriched = copy.deepcopy(cfgElectronAntiCrackCutBgEstZtautauEnriched)
cfgElectronAntiCrackCutBgEstQCDenriched.pluginName = cms.string('electronAntiCrackCutBgEstQCDenriched')
cfgElectronAntiCrackCutBgEstQCDenriched.src_cumulative = cms.InputTag('electronsBgEstQCDenrichedAntiCrackCutCumulative')

cfgElectronEtaCutBgEstQCDenriched = copy.deepcopy(cfgElectronEtaCutBgEstZtautauEnriched)
cfgElectronEtaCutBgEstQCDenriched.pluginName = cms.string('electronEtaCutBgEstQCDenriched')
cfgElectronEtaCutBgEstQCDenriched.src_cumulative = cms.InputTag('electronsBgEstQCDenrichedEtaCumulative')

cfgElectronPtCutBgEstQCDenriched = copy.deepcopy(cfgElectronPtCutBgEstZtautauEnriched)
cfgElectronPtCutBgEstQCDenriched.pluginName = cms.string('electronPtCutBgEstQCDenriched')
cfgElectronPtCutBgEstQCDenriched.src_cumulative = cms.InputTag('electronsBgEstQCDenrichedPtCumulative')

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut
cfgElectronTrkIsoCutBgEstQCDenriched = copy.deepcopy(cfgElectronTrkIsoCutBgEstZtautauEnriched)
cfgElectronTrkIsoCutBgEstQCDenriched.pluginName = cms.string('electronTrkIsoCutBgEstQCDenriched')
cfgElectronTrkIsoCutBgEstQCDenriched.src_cumulative = cms.InputTag('electronsBgEstQCDenrichedTrkIsoCumulative')
cfgElectronTrkIsoCutBgEstQCDenriched.pluginType = cms.string('PATCandViewMaxEventSelector')
cfgElectronTrkIsoCutBgEstQCDenriched.maxNumber = cms.uint32(0)

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut
cfgElectronEcalIsoCutBgEstQCDenriched = copy.deepcopy(cfgElectronEcalIsoCutBgEstZtautauEnriched)
cfgElectronEcalIsoCutBgEstQCDenriched.pluginName = cms.string('electronEcalIsoCutBgEstQCDenriched')
cfgElectronEcalIsoCutBgEstQCDenriched.src_cumulative = cms.InputTag('electronsBgEstQCDenrichedEcalIsoCumulative')
cfgElectronEcalIsoCutBgEstQCDenriched.pluginType = cms.string('PATCandViewMaxEventSelector')
cfgElectronEcalIsoCutBgEstQCDenriched.maxNumber = cms.uint32(0)

cfgElectronTrkIPcutBgEstQCDenriched = copy.deepcopy(cfgElectronTrkIPcutBgEstZtautauEnriched)
cfgElectronTrkIPcutBgEstQCDenriched.pluginName = cms.string('electronTrkIPcutBgEstQCDenriched')
cfgElectronTrkIPcutBgEstQCDenriched.src_cumulative = cms.InputTag('electronsBgEstQCDenrichedTrkIPcumulative')

cfgElectronConversionVetoBgEstQCDenriched = copy.deepcopy(cfgElectronConversionVetoBgEstZtautauEnriched)
cfgElectronConversionVetoBgEstQCDenriched.pluginName = cms.string('electronConversionVetoBgEstQCDenriched')
cfgElectronConversionVetoBgEstQCDenriched.src_cumulative = cms.InputTag('electronsBgEstQCDenrichedConversionVetoCumulative')

# tau cuts

cfgTauAntiOverlapWithElectronsVetoBgEstQCDenriched = copy.deepcopy(cfgTauAntiOverlapWithElectronsVetoBgEstZtautauEnriched)
cfgTauAntiOverlapWithElectronsVetoBgEstQCDenriched.pluginName = cms.string('tauAntiOverlapWithElectronsVetoBgEstQCDenriched')
cfgTauAntiOverlapWithElectronsVetoBgEstQCDenriched.src_cumulative = cms.InputTag('tausBgEstQCDenrichedAntiOverlapWithElectronsVetoCumulative')

cfgTauEtaCutBgEstQCDenriched = copy.deepcopy(cfgTauEtaCutBgEstZtautauEnriched)
cfgTauEtaCutBgEstQCDenriched.pluginName = cms.string('tauEtaCutBgEstQCDenriched')
cfgTauEtaCutBgEstQCDenriched.src_cumulative = cms.InputTag('tausBgEstQCDenrichedEtaCumulative')

cfgTauPtCutBgEstQCDenriched = copy.deepcopy(cfgTauPtCutBgEstZtautauEnriched)
cfgTauPtCutBgEstQCDenriched.pluginName = cms.string('tauPtCutBgEstQCDenriched')
cfgTauPtCutBgEstQCDenriched.src_cumulative = cms.InputTag('tausBgEstQCDenrichedPtCumulative')

cfgTauLeadTrkCutBgEstQCDenriched = copy.deepcopy(cfgTauLeadTrkCutBgEstZtautauEnriched)
cfgTauLeadTrkCutBgEstQCDenriched.pluginName = cms.string('tauLeadTrkCutBgEstQCDenriched')
cfgTauLeadTrkCutBgEstQCDenriched.src_cumulative = cms.InputTag('tausBgEstQCDenrichedLeadTrkCumulative')

cfgTauLeadTrkPtCutBgEstQCDenriched = copy.deepcopy(cfgTauLeadTrkPtCutBgEstZtautauEnriched)
cfgTauLeadTrkPtCutBgEstQCDenriched.pluginName = cms.string('tauLeadTrkPtCutBgEstQCDenriched')
cfgTauLeadTrkPtCutBgEstQCDenriched.src_cumulative = cms.InputTag('tausBgEstQCDenrichedLeadTrkPtCumulative')

cfgTauTaNCdiscrCutBgEstQCDenriched = copy.deepcopy(cfgTauTaNCdiscrCutBgEstZtautauEnriched)
cfgTauTaNCdiscrCutBgEstQCDenriched.pluginName = cms.string('tauTaNCdiscrCutBgEstQCDenriched')
cfgTauTaNCdiscrCutBgEstQCDenriched.src_cumulative = cms.InputTag('tausBgEstQCDenrichedTaNCdiscrCumulative')

cfgTauTrkIsoCutBgEstQCDenriched = copy.deepcopy(cfgTauTrkIsoCutBgEstZtautauEnriched)
cfgTauTrkIsoCutBgEstQCDenriched.pluginName = cms.string('tauTrkIsoCutBgEstQCDenriched')
cfgTauTrkIsoCutBgEstQCDenriched.src_cumulative = cms.InputTag('tausBgEstQCDenrichedTrkIsoCumulative')

cfgTauEcalIsoCutBgEstQCDenriched = copy.deepcopy(cfgTauEcalIsoCutBgEstZtautauEnriched)
cfgTauEcalIsoCutBgEstQCDenriched.pluginName = cms.string('tauEcalIsoCutBgEstQCDenriched')
cfgTauEcalIsoCutBgEstQCDenriched.src_cumulative = cms.InputTag('tausBgEstQCDenrichedEcalIsoCumulative')

cfgTauProngCutBgEstQCDenriched = copy.deepcopy(cfgTauProngCutBgEstZtautauEnriched)
cfgTauProngCutBgEstQCDenriched.pluginName = cms.string('tauProngCutBgEstQCDenriched')
cfgTauProngCutBgEstQCDenriched.src_cumulative = cms.InputTag('tausBgEstQCDenrichedProngCumulative')

cfgTauChargeCutBgEstQCDenriched = copy.deepcopy(cfgTauChargeCutBgEstZtautauEnriched)
cfgTauChargeCutBgEstQCDenriched.pluginName = cms.string('tauChargeCutBgEstQCDenriched')
cfgTauChargeCutBgEstQCDenriched.src_cumulative = cms.InputTag('tausBgEstQCDenrichedChargeCumulative')

cfgTauElectronVetoBgEstQCDenriched = copy.deepcopy(cfgTauElectronVetoBgEstZtautauEnriched)
cfgTauElectronVetoBgEstQCDenriched.pluginName = cms.string('tauElectronVetoBgEstQCDenriched')
cfgTauElectronVetoBgEstQCDenriched.src_cumulative = cms.InputTag('tausBgEstQCDenrichedElectronVetoCumulative')

cfgTauEcalCrackVetoBgEstQCDenriched = copy.deepcopy(cfgTauEcalCrackVetoBgEstZtautauEnriched)
cfgTauEcalCrackVetoBgEstQCDenriched.pluginName = cms.string('tauEcalCrackVetoBgEstQCDenriched')
cfgTauEcalCrackVetoBgEstQCDenriched.src_cumulative = cms.InputTag('tausBgEstQCDenrichedEcalCrackVetoCumulative')

cfgTauMuonVetoBgEstQCDenriched = copy.deepcopy(cfgTauMuonVetoBgEstZtautauEnriched)
cfgTauMuonVetoBgEstQCDenriched.pluginName = cms.string('tauMuonVetoBgEstQCDenriched')
cfgTauMuonVetoBgEstQCDenriched.src_cumulative = cms.InputTag('tausBgEstQCDenrichedMuonVetoCumulative')

# di-tau candidate selection

cfgDiTauCandidateAntiOverlapVetoBgEstQCDenriched = copy.deepcopy(cfgDiTauCandidateAntiOverlapVetoBgEstZtautauEnriched)
cfgDiTauCandidateAntiOverlapVetoBgEstQCDenriched.pluginName = cms.string('diTauCandidateAntiOverlapVetoBgEstQCDenriched')
cfgDiTauCandidateAntiOverlapVetoBgEstQCDenriched.src_cumulative = cms.InputTag('elecTauPairsBgEstQCDenrichedAntiOverlapVetoCumulative')

cfgDiTauCandidateForElecTauZeroChargeCutBgEstQCDenriched = copy.deepcopy(cfgDiTauCandidateForElecTauZeroChargeCutBgEstZtautauEnriched)
cfgDiTauCandidateForElecTauZeroChargeCutBgEstQCDenriched.pluginName = cms.string('diTauCandidateZeroChargeCutBgEstQCDenriched')
cfgDiTauCandidateForElecTauZeroChargeCutBgEstQCDenriched.src_cumulative = cms.InputTag('elecTauPairsBgEstQCDenrichedZeroChargeCumulative')

cfgDiTauCandidateForElecTauAcoplanarity12CutBgEstQCDenriched = copy.deepcopy(cfgDiTauCandidateForElecTauAcoplanarity12CutBgEstZtautauEnriched)
cfgDiTauCandidateForElecTauAcoplanarity12CutBgEstQCDenriched.pluginName = cms.string('diTauCandidateAcoplanarity12CutBgEstQCDenriched')
cfgDiTauCandidateForElecTauAcoplanarity12CutBgEstQCDenriched.src_cumulative = cms.InputTag('elecTauPairsBgEstQCDenrichedAcoplanarity12Cumulative')

cfgDiTauCandidateForElecTauMt1METCutBgEstQCDenriched = copy.deepcopy(cfgDiTauCandidateForElecTauMt1METCutBgEstZtautauEnriched)
cfgDiTauCandidateForElecTauMt1METCutBgEstQCDenriched.pluginName = cms.string('diTauCandidateMt1METCutBgEstQCDenriched')
cfgDiTauCandidateForElecTauMt1METCutBgEstQCDenriched.src_cumulative = cms.InputTag('elecTauPairsBgEstQCDenrichedMt1METcumulative')

cfgDiTauCandidateForElecTauPzetaDiffCutBgEstQCDenriched = copy.deepcopy(cfgDiTauCandidateForElecTauPzetaDiffCutBgEstZtautauEnriched)
cfgDiTauCandidateForElecTauPzetaDiffCutBgEstQCDenriched.pluginName = cms.string('diTauCandidatePzetaDiffCutBgEstQCDenriched')
cfgDiTauCandidateForElecTauPzetaDiffCutBgEstQCDenriched.src_cumulative = cms.InputTag('elecTauPairsBgEstQCDenrichedPzetaDiffCumulative')

cfgElecTauPairZeeHypothesisVetoBgEstQCDenriched = copy.deepcopy(cfgElecTauPairZeeHypothesisVetoBgEstZtautauEnriched)
cfgElecTauPairZeeHypothesisVetoBgEstQCDenriched.pluginName = cms.string('diTauCandidateZeeHypothesisVetoBgEstQCDenriched')
cfgElecTauPairZeeHypothesisVetoBgEstQCDenriched.src_cumulative = cms.InputTag('selectedElecTauPairBgEstQCDenrichedZeeHypotheses')


evtSelConfiguratorBgEstQCDenriched = eventSelFlagProdConfigurator(
    [ cfgElectronIdCutBgEstQCDenriched,
      cfgElectronAntiCrackCutBgEstQCDenriched,
      cfgElectronEtaCutBgEstQCDenriched,
      cfgElectronPtCutBgEstQCDenriched,
      cfgElectronTrkIsoCutBgEstQCDenriched,
      cfgElectronEcalIsoCutBgEstQCDenriched,
      cfgElectronConversionVetoBgEstQCDenriched,
      cfgElectronTrkIPcutBgEstQCDenriched,
      cfgTauAntiOverlapWithElectronsVetoBgEstQCDenriched,
      cfgTauEtaCutBgEstQCDenriched,
      cfgTauPtCutBgEstQCDenriched,
      cfgTauLeadTrkCutBgEstQCDenriched,
      cfgTauLeadTrkPtCutBgEstQCDenriched,
      cfgTauTaNCdiscrCutBgEstQCDenriched,
      cfgTauTrkIsoCutBgEstQCDenriched,
      cfgTauEcalIsoCutBgEstQCDenriched,
      cfgTauProngCutBgEstQCDenriched,
      cfgTauChargeCutBgEstQCDenriched,
      cfgTauElectronVetoBgEstQCDenriched,
      cfgTauEcalCrackVetoBgEstQCDenriched,
      cfgTauMuonVetoBgEstQCDenriched,
      cfgDiTauCandidateAntiOverlapVetoBgEstQCDenriched,
      cfgDiTauCandidateForElecTauZeroChargeCutBgEstQCDenriched,
      cfgDiTauCandidateForElecTauAcoplanarity12CutBgEstQCDenriched,
      cfgDiTauCandidateForElecTauMt1METCutBgEstQCDenriched,
      cfgDiTauCandidateForElecTauPzetaDiffCutBgEstQCDenriched,
      cfgElecTauPairZeeHypothesisVetoBgEstQCDenriched
      ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

selectEventsBgEstQCDenriched = evtSelConfiguratorBgEstQCDenriched.configure()


#--------------------------------------------------------------------------------  
# apply event selection criteria; fill histograms
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.analyzeZtoElecTau_cff import *


diTauCandidateHistManagerForElecTauBgEstQCDenriched = copy.deepcopy(diTauCandidateHistManagerForElecTauBgEstZtautauEnriched)
diTauCandidateHistManagerForElecTauBgEstQCDenriched.pluginName = cms.string('diTauCandidateHistManagerForElecTauBgEstQCDenriched')
diTauCandidateHistManagerForElecTauBgEstQCDenriched.ZllHypothesisSource = cms.InputTag('selectedElecTauPairBgEstQCDenrichedZeeHypotheses')



analyzeEventsBgEstQCDenriched = cms.EDAnalyzer("GenericAnalyzer",

    name = cms.string('BgEstTemplateAnalyzer_QCDenriched'), 

    filters = cms.VPSet(
        evtSelGenPhaseSpace,
        evtSelTrigger,
        evtSelPrimaryEventVertex,
        evtSelPrimaryEventVertexQuality,
        evtSelPrimaryEventVertexPosition,

        #start electron cuts
        cms.PSet(
            pluginName = cms.string('electronIdCutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronIdCutBgEstQCDenriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('electronAntiCrackCutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronAntiCrackCutBgEstQCDenriched','cumulative')
        ),        
        cms.PSet(
            pluginName = cms.string('electronEtaCutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronEtaCutBgEstQCDenriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('electronPtCutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronPtCutBgEstQCDenriched','cumulative')
        ),
        #!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut
        cms.PSet(
            pluginName = cms.string('electronTrkIsoCutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronTrkIsoCutBgEstQCDenriched','cumulative')
        ),
        #!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut
        cms.PSet(
            pluginName = cms.string('electronEcalIsoCutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronEcalIsoCutBgEstQCDenriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('electronConversionVetoBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronConversionVetoBgEstQCDenriched','cumulative')
        ),        
        cms.PSet(
            pluginName = cms.string('electronTrkIPcutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronTrkIPcutBgEstQCDenriched','cumulative')
        ),


        #start tau cuts
        
        cms.PSet(
            pluginName = cms.string('tauAntiOverlapWithElectronsVetoBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauAntiOverlapWithElectronsVetoBgEstQCDenriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauEtaCutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauEtaCutBgEstQCDenriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauPtCutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauPtCutBgEstQCDenriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauLeadTrkCutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauLeadTrkCutBgEstQCDenriched','cumulative')
        ),        
        cms.PSet(
            pluginName = cms.string('tauLeadTrkPtCutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauLeadTrkPtCutBgEstQCDenriched','cumulative')
        ),  
        cms.PSet(
            pluginName = cms.string('tauTaNCdiscrCutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauTaNCdiscrCutBgEstQCDenriched','cumulative')
        ),  
        cms.PSet(
            pluginName = cms.string('tauTrkIsoCutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauTrkIsoCutBgEstQCDenriched','cumulative')
        ),  
        cms.PSet(
            pluginName = cms.string('tauEcalIsoCutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauEcalIsoCutBgEstQCDenriched','cumulative')
        ),  
        cms.PSet(
            pluginName = cms.string('tauProngCutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauProngCutBgEstQCDenriched','cumulative')
        ),  
        cms.PSet(
            pluginName = cms.string('tauChargeCutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauChargeCutBgEstQCDenriched','cumulative')
        ),  
        cms.PSet(
            pluginName = cms.string('tauElectronVetoBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauElectronVetoBgEstQCDenriched','cumulative')
        ),  
        cms.PSet(
            pluginName = cms.string('tauEcalCrackVetoBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauEcalCrackVetoBgEstQCDenriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauMuonVetoBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauMuonVetoBgEstQCDenriched','cumulative')
        ),

        #start ditau cuts
        
        cms.PSet(
            pluginName = cms.string('diTauCandidateAntiOverlapVetoBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateAntiOverlapVetoBgEstQCDenriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('diTauCandidateZeroChargeCutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateZeroChargeCutBgEstQCDenriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('diTauCandidateAcoplanarity12CutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateAcoplanarity12CutBgEstQCDenriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('diTauCandidateMt1METCutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateMt1METCutBgEstQCDenriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('diTauCandidatePzetaDiffCutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidatePzetaDiffCutBgEstQCDenriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('diTauCandidateZeeHypothesisVetoBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateZeeHypothesisVetoBgEstQCDenriched','cumulative')
        )
        
        
    ),
  
    analyzers = cms.VPSet(
         diTauCandidateHistManagerForElecTauBgEstQCDenriched
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
            filter = cms.string('electronIdCutBgEstQCDenriched'),
            title = cms.string('Electron ID'),
        ),
        cms.PSet(
            filter = cms.string('electronAntiCrackCutBgEstQCDenriched'),
            title = cms.string('Electron crack-Veto'),
        ),
        cms.PSet(
            filter = cms.string('electronEtaCutBgEstQCDenriched'),
            title = cms.string('-2.1 < eta(Electron) < +2.1'),
        ),
        cms.PSet(
            filter = cms.string('electronPtCutBgEstQCDenriched'),
            title = cms.string('Pt(Electron) > 15 GeV'),
        ),
        cms.PSet(
            filter = cms.string('tauAntiOverlapWithElectronsVetoBgEstQCDenriched'),
            title = cms.string('Tau not overlapping with Elec.'),
        ),
        cms.PSet(
            filter = cms.string('tauEtaCutBgEstQCDenriched'),
            title = cms.string('-2.1 < eta(Tau) < +2.1'),
        ),
        cms.PSet(
            filter = cms.string('tauPtCutBgEstQCDenriched'),
            title = cms.string('Pt(Tau) > 20 GeV'),
        ),
        #!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut
        cms.PSet(
            filter = cms.string('electronTrkIsoCutBgEstQCDenriched'),
            title = cms.string('! Electron Track iso.'),
        ),
        #!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut
        cms.PSet(
            filter = cms.string('electronEcalIsoCutBgEstQCDenriched'),
            title = cms.string('! Electron ECAL iso.'),
        ),
        cms.PSet(
            filter = cms.string('electronConversionVetoBgEstQCDenriched'),
            title = cms.string('Electron Track conv. veto'),
        ),
        cms.PSet(
            filter = cms.string('electronTrkIPcutBgEstQCDenriched'),
            title = cms.string('Electron Track IP'),
        ),
        cms.PSet(
            filter = cms.string('tauLeadTrkPtCutBgEstQCDenriched'),
            title = cms.string('Tau lead. Track Pt'),
        ),
        cms.PSet(
            filter = cms.string('tauTaNCdiscrCutBgEstQCDenriched'),
            title = cms.string('Tau TaNC at 0.5%'),
        ),
        cms.PSet(
            filter = cms.string('tauTrkIsoCutBgEstQCDenriched'),
            title = cms.string('Tau Track iso.'),
        ),
        cms.PSet(
            filter = cms.string('tauEcalIsoCutBgEstQCDenriched'),
            title = cms.string('Tau ECAL iso.'),
        ),
        cms.PSet(
            filter = cms.string('tauProngCutBgEstQCDenriched'),
            title = cms.string('Tau 1||3-Prong'),
        ),
        cms.PSet(
            filter = cms.string('tauChargeCutBgEstQCDenriched'),
            title = cms.string('Charge(Tau) = +/-1'),
        ),
        cms.PSet(
            filter = cms.string('tauElectronVetoBgEstQCDenriched'),
            title = cms.string('Tau e-Veto'),
        ),
        cms.PSet(
            filter = cms.string('tauEcalCrackVetoBgEstQCDenriched'),
            title = cms.string('Tau ECAL crack-Veto'),
        ),
        cms.PSet(
            filter = cms.string('tauMuonVetoBgEstQCDenriched'),
            title = cms.string('Tau mu-Veto'),
        ),
        cms.PSet(
            filter = cms.string('diTauCandidateAntiOverlapVetoBgEstQCDenriched'),
            title = cms.string('dR(Electron-Tau) > 0.7'),
        ),
        cms.PSet(
            filter = cms.string('diTauCandidateZeroChargeCutBgEstQCDenriched'),
            title = cms.string('Charge(Electron+Tau) = 0'),
        ),
        cms.PSet(
            filter = cms.string('diTauCandidateAcoplanarity12CutBgEstQCDenriched'),
            title = cms.string('Acoplanarity(Electron+Tau)'),
        ),
        cms.PSet(
            filter = cms.string('diTauCandidateMt1METCutBgEstQCDenriched'),
            title = cms.string('M_{T}(Electron-MET) < 50 GeV'),
        ),
        cms.PSet(
            filter = cms.string('diTauCandidatePzetaDiffCutBgEstQCDenriched'),
            title = cms.string('P_{#zeta} - 1.5*P_{#zeta}^{vis} > -20 GeV'),
        ),
        cms.PSet(
            filter = cms.string('diTauCandidateZeeHypothesisVetoBgEstQCDenriched'),
            title = cms.string('not 85 < M_{vis} (Electron-Tau) < 100 GeV'),
        ),




        
        cms.PSet(
            analyzers = cms.vstring(
                'diTauCandidateHistManagerForElecTauBgEstQCDenriched'
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

bgEstQCDenrichedAnalysisSequence = cms.Sequence(

    selectElectronsBgEstQCDenriched
    + selectTausBgEstQCDenriched
    + produceElecTauPairsBgEstQCDenriched
    + selectElecTauPairsBgEstQCDenriched
    + produceElecTauPairZeeHypothesesBgEstQCDenriched
    + selectEventsBgEstQCDenriched 
    + analyzeEventsBgEstQCDenriched

)
