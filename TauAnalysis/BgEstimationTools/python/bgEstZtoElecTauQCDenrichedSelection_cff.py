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

electronsBgEstQCDenrichedIso = copy.deepcopy(electronsBgEstZtautauEnrichedIso)
electronsBgEstQCDenrichedIso.sumPtMinEB = cms.double(0.15)
electronsBgEstQCDenrichedIso.sumPtMaxEB = cms.double(0.30)
electronsBgEstQCDenrichedIso.sumPtMinEE = cms.double(0.15)
electronsBgEstQCDenrichedIso.sumPtMaxEE = cms.double(0.30)


# require electron to not be from a photon conversion 
electronsBgEstQCDenrichedConversionVeto = copy.deepcopy(electronsBgEstZtautauEnrichedConversionVeto)

electronsBgEstQCDenrichedTrkIP = copy.deepcopy(electronsBgEstZtautauEnrichedTrkIP)

electronSelConfiguratorBgEstQCDenriched = objSelConfigurator(
    [ electronsBgEstQCDenrichedId,
      electronsBgEstQCDenrichedAntiCrackCut,
      electronsBgEstQCDenrichedEta,
      electronsBgEstQCDenrichedPt,
      electronsBgEstQCDenrichedIso,
      electronsBgEstQCDenrichedConversionVeto,
      electronsBgEstQCDenrichedTrkIP
      ],
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
tausBgEstQCDenrichedAntiOverlapWithElectronsVeto.srcNotToBeFiltered = cms.VInputTag("electronsBgEstQCDenrichedPtCumulative")

# require tau candidate to be within geometric acceptance of Pixel + SiTracker detectors
tausBgEstQCDenrichedEta = copy.deepcopy(tausBgEstZtautauEnrichedEta)

# require tau candidate to have transverse energy above threshold
tausBgEstQCDenrichedPt = copy.deepcopy(tausBgEstZtautauEnrichedPt)

# require leading track of tau candidate to have Pt > 5. GeV
tausBgEstQCDenrichedLeadTrkPt = copy.deepcopy(tausBgEstZtautauEnrichedLeadTrkPt)

# require tau candidate to pass TaNC discriminator
tausBgEstQCDenrichedIso = copy.deepcopy(tausBgEstZtautauEnrichedIso)
tausBgEstQCDenrichedIso.cut = cms.string('tauID("byHPSvloose") > 0.5 & tauID("byHPSmedium") < 0.5')

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
      tausBgEstQCDenrichedLeadTrkPt,
      tausBgEstQCDenrichedIso,
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

### !!!!! turned off
elecTauPairsBgEstQCDenrichedMt1MET = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedMt1MET)
elecTauPairsBgEstQCDenrichedMt1MET.cut = cms.string('mt1MET < 500.')

### !!!!! turned off
elecTauPairsBgEstQCDenrichedPzetaDiff = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedPzetaDiff)
elecTauPairsBgEstQCDenrichedPzetaDiff.cut = cms.string('(pZeta - 1.5*pZetaVis) > -2000.')

### !!!!! turned off
elecTauPairsBgEstQCDenrichedZeroCharge = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedZeroCharge)
elecTauPairsBgEstQCDenrichedZeroCharge.cut = cms.string('charge != 10')


elecTauPairSelConfiguratorBgEstQCDenriched = objSelConfigurator(
    [ elecTauPairsBgEstQCDenrichedAntiOverlapVeto,
      elecTauPairsBgEstQCDenrichedMt1MET,
      elecTauPairsBgEstQCDenrichedPzetaDiff,
      elecTauPairsBgEstQCDenrichedZeroCharge
      ],
    src = "elecTauPairsBgEstQCDenriched",
    pyModuleName = __name__,
    doSelIndividual = False
)

selectElecTauPairsBgEstQCDenriched = elecTauPairSelConfiguratorBgEstQCDenriched.configure(pyNameSpace = locals())

####### anti Zee Cut #######


### production

allDiElecPairBgEstQCDenrichedZeeHypothesesByLooseIsolation = copy.deepcopy(allDiElecPairBgEstZtautauEnrichedZeeHypothesesByLooseIsolation)
allDiElecPairBgEstQCDenrichedZeeHypothesesByLooseIsolation.srcLeg1 = cms.InputTag("electronsBgEstQCDenrichedTrkIPcumulative")

### selection

selectedDiElecPairBgEstQCDenrichedZeeHypothesesByLooseIsolation = copy.deepcopy(selectedDiElecPairBgEstZtautauEnrichedZeeHypothesesByLooseIsolation)
selectedDiElecPairBgEstQCDenrichedZeeHypothesesByLooseIsolation.src = cms.InputTag("allDiElecPairBgEstQCDenrichedZeeHypothesesByLooseIsolation")

produceElecTauPairZeeHypothesesBgEstQCDenriched = cms.Sequence(
	selectedPatElectronsForZeeHypotheses * 
	allDiElecPairBgEstQCDenrichedZeeHypothesesByLooseIsolation *
	selectedDiElecPairBgEstQCDenrichedZeeHypothesesByLooseIsolation
)



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

cfgElectronIsoCutBgEstQCDenriched = copy.deepcopy(cfgElectronIsoCutBgEstZtautauEnriched)
cfgElectronIsoCutBgEstQCDenriched.pluginName = cms.string('electronIsoCutBgEstQCDenriched')
cfgElectronIsoCutBgEstQCDenriched.src_cumulative = cms.InputTag('electronsBgEstQCDenrichedIsoCumulative')

cfgElectronConversionVetoBgEstQCDenriched = copy.deepcopy(cfgElectronConversionVetoBgEstZtautauEnriched)
cfgElectronConversionVetoBgEstQCDenriched.pluginName = cms.string('electronConversionVetoBgEstQCDenriched')
cfgElectronConversionVetoBgEstQCDenriched.src_cumulative = cms.InputTag('electronsBgEstQCDenrichedConversionVetoCumulative')

cfgElectronTrkIPcutBgEstQCDenriched = copy.deepcopy(cfgElectronTrkIPcutBgEstZtautauEnriched)
cfgElectronTrkIPcutBgEstQCDenriched.pluginName = cms.string('electronTrkIPcutBgEstQCDenriched')
cfgElectronTrkIPcutBgEstQCDenriched.src_cumulative = cms.InputTag('electronsBgEstQCDenrichedTrkIPcumulative')


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

cfgTauLeadTrkPtCutBgEstQCDenriched = copy.deepcopy(cfgTauLeadTrkPtCutBgEstZtautauEnriched)
cfgTauLeadTrkPtCutBgEstQCDenriched.pluginName = cms.string('tauLeadTrkPtCutBgEstQCDenriched')
cfgTauLeadTrkPtCutBgEstQCDenriched.src_cumulative = cms.InputTag('tausBgEstQCDenrichedLeadTrkPtCumulative')

cfgTauIsoCutBgEstQCDenriched = copy.deepcopy(cfgTauIsoCutBgEstZtautauEnriched)
cfgTauIsoCutBgEstQCDenriched.pluginName = cms.string('tauIsoCutBgEstQCDenriched')
cfgTauIsoCutBgEstQCDenriched.src_cumulative = cms.InputTag('tausBgEstQCDenrichedIsoCumulative')

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

cfgDiTauCandidateForElecTauMt1METCutBgEstQCDenriched = copy.deepcopy(cfgDiTauCandidateForElecTauMt1METCutBgEstZtautauEnriched)
cfgDiTauCandidateForElecTauMt1METCutBgEstQCDenriched.pluginName = cms.string('diTauCandidateMt1METCutBgEstQCDenriched')
cfgDiTauCandidateForElecTauMt1METCutBgEstQCDenriched.src_cumulative = cms.InputTag('elecTauPairsBgEstQCDenrichedMt1METcumulative')

cfgDiTauCandidateForElecTauPzetaDiffCutBgEstQCDenriched = copy.deepcopy(cfgDiTauCandidateForElecTauPzetaDiffCutBgEstZtautauEnriched)
cfgDiTauCandidateForElecTauPzetaDiffCutBgEstQCDenriched.pluginName = cms.string('diTauCandidatePzetaDiffCutBgEstQCDenriched')
cfgDiTauCandidateForElecTauPzetaDiffCutBgEstQCDenriched.src_cumulative = cms.InputTag('elecTauPairsBgEstQCDenrichedPzetaDiffCumulative')

cfgDiTauCandidateForElecTauZeroChargeCutBgEstQCDenriched = copy.deepcopy(cfgDiTauCandidateForElecTauZeroChargeCutBgEstZtautauEnriched)
cfgDiTauCandidateForElecTauZeroChargeCutBgEstQCDenriched.pluginName = cms.string('diTauCandidateZeroChargeCutBgEstQCDenriched')
cfgDiTauCandidateForElecTauZeroChargeCutBgEstQCDenriched.src_cumulative = cms.InputTag('elecTauPairsBgEstQCDenrichedZeroChargeCumulative')

cfgElecTauPairZeeHypothesisVetoBgEstQCDenriched = copy.deepcopy(cfgElecTauPairZeeHypothesisVetoBgEstZtautauEnriched)
cfgElecTauPairZeeHypothesisVetoBgEstQCDenriched.pluginName = cms.string('diTauCandidateZeeHypothesisVetoBgEstQCDenriched')
cfgElecTauPairZeeHypothesisVetoBgEstQCDenriched.src = cms.InputTag('selectedDiElecPairBgEstQCDenrichedZeeHypothesesByLooseIsolation')


evtSelConfiguratorBgEstQCDenriched = eventSelFlagProdConfigurator(
    [ cfgElectronIdCutBgEstQCDenriched,
      cfgElectronAntiCrackCutBgEstQCDenriched,
      cfgElectronEtaCutBgEstQCDenriched,
      cfgElectronPtCutBgEstQCDenriched,
      cfgElectronIsoCutBgEstQCDenriched,
      cfgElectronConversionVetoBgEstQCDenriched,
      cfgElectronTrkIPcutBgEstQCDenriched,
      cfgTauAntiOverlapWithElectronsVetoBgEstQCDenriched,
      cfgTauEtaCutBgEstQCDenriched,
      cfgTauPtCutBgEstQCDenriched,
      cfgTauLeadTrkPtCutBgEstQCDenriched,
      cfgTauIsoCutBgEstQCDenriched,
      cfgTauElectronVetoBgEstQCDenriched,
      cfgTauEcalCrackVetoBgEstQCDenriched,
      cfgTauMuonVetoBgEstQCDenriched,
      cfgDiTauCandidateAntiOverlapVetoBgEstQCDenriched,
      cfgDiTauCandidateForElecTauMt1METCutBgEstQCDenriched,
      cfgDiTauCandidateForElecTauPzetaDiffCutBgEstQCDenriched,
      cfgDiTauCandidateForElecTauZeroChargeCutBgEstQCDenriched,      
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
diTauCandidateHistManagerForElecTauBgEstQCDenriched.diTauCandidateSource = cms.InputTag('elecTauPairsBgEstQCDenriched')

diTauCandidateNSVfitHistManagerForElecTauBgEstQCDenriched = copy.deepcopy(diTauCandidateNSVfitHistManagerForElecTauBgEstZtautauEnriched)
diTauCandidateNSVfitHistManagerForElecTauBgEstQCDenriched.pluginName = cms.string('diTauCandidateNSVfitHistManagerForElecTauBgEstQCDenriched')
diTauCandidateNSVfitHistManagerForElecTauBgEstQCDenriched.diTauCandidateSource = cms.InputTag('elecTauPairsBgEstQCDenriched')

electronHistManagerForElecTauBgEstQCDenriched = copy.deepcopy(electronHistManagerForElecTauBgEstZtautauEnriched)
electronHistManagerForElecTauBgEstQCDenriched.pluginName = cms.string('electronHistManagerForElecTauBgEstQCDenriched')

tauHistManagerForElecTauBgEstQCDenriched = copy.deepcopy(tauHistManagerForElecTauBgEstZtautauEnriched)
tauHistManagerForElecTauBgEstQCDenriched.pluginName = cms.string('tauHistManagerForElecTauBgEstQCDenriched')
tauHistManagerForElecTauBgEstQCDenriched.jetSource = cms.InputTag('cleanPatJets')


analyzeEventsBgEstQCDenriched = cms.EDAnalyzer("GenericAnalyzer",

    name = cms.string('BgEstTemplateAnalyzer_QCDenriched'), 

    filters = cms.VPSet(
        evtSelGenPhaseSpace,
        evtSelTrigger,
        evtSelPrimaryEventVertexQuality,
        evtSelPrimaryEventVertexPosition,        
        evtSelPrimaryEventVertexHighestPtTrackSum,

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
        cms.PSet(
            pluginName = cms.string('electronIsoCutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronIsoCutBgEstQCDenriched','cumulative')
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
            pluginName = cms.string('tauLeadTrkPtCutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauLeadTrkPtCutBgEstQCDenriched','cumulative')
        ),  
        cms.PSet(
            pluginName = cms.string('tauIsoCutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauIsoCutBgEstQCDenriched','cumulative')
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
            pluginName = cms.string('diTauCandidateZeroChargeCutBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateZeroChargeCutBgEstQCDenriched','cumulative')
        ),        
        cms.PSet(
            pluginName = cms.string('diTauCandidateZeeHypothesisVetoBgEstQCDenriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateZeeHypothesisVetoBgEstQCDenriched')
        )
        
        
    ),
  
    analyzers = cms.VPSet(
         diTauCandidateHistManagerForElecTauBgEstQCDenriched,
         diTauCandidateNSVfitHistManagerForElecTauBgEstQCDenriched,
         electronHistManagerForElecTauBgEstQCDenriched,
         tauHistManagerForElecTauBgEstQCDenriched,
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
            filter = cms.string('evtSelPrimaryEventVertexQuality'),
            title = cms.string('Valid vertex with #dof >= 4')
        ),
        cms.PSet(
            filter = cms.string('evtSelPrimaryEventVertexPosition'),
            title = cms.string('Vertex abs(z) < 24 & Rho < 2')
        ),
        cms.PSet(
            filter = cms.string('evtSelPrimaryEventVertexHighestPtTrackSum'),
            title = cms.string('Highest Pt Vertex')
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
            title = cms.string('-2.3 < eta(Tau) < +2.3'),
        ),
        cms.PSet(
            filter = cms.string('tauPtCutBgEstQCDenriched'),
            title = cms.string('Pt(Tau) > 20 GeV'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstQCDenriched',
                                    'tauHistManagerForElecTauBgEstQCDenriched',
                                    ),
            replace = cms.vstring('electronHistManagerForElecTauBgEstQCDenriched.electronSource = electronsBgEstQCDenrichedPtCumulative',
                                  'tauHistManagerForElecTauBgEstQCDenriched.tauSource = tausBgEstQCDenrichedPtCumulative'
                                  )
        ),     
        cms.PSet(
            filter = cms.string('electronIsoCutBgEstQCDenriched'),
            title = cms.string('Electron Isolation (inverted)'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstQCDenriched',
                                    'tauHistManagerForElecTauBgEstQCDenriched',
                                    ),
            replace = cms.vstring('electronHistManagerForElecTauBgEstQCDenriched.electronSource = electronsBgEstQCDenrichedIsoCumulative')
        ),           
        cms.PSet(
            filter = cms.string('electronConversionVetoBgEstQCDenriched'),
            title = cms.string('Electron Track conversion veto'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstQCDenriched',
                                    'tauHistManagerForElecTauBgEstQCDenriched',
                                    ),
            replace = cms.vstring('electronHistManagerForElecTauBgEstQCDenriched.electronSource = electronsBgEstQCDenrichedConversionVetoCumulative')
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
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstQCDenriched',
                                    'tauHistManagerForElecTauBgEstQCDenriched',
                                    ),
            replace = cms.vstring('electronHistManagerForElecTauBgEstQCDenriched.electronSource = electronsBgEstQCDenrichedTrkIPcumulative',
                                  'tauHistManagerForElecTauBgEstQCDenriched.tauSource = tausBgEstQCDenrichedLeadTrkPtCumulative')
        ),             
        cms.PSet(
            filter = cms.string('tauIsoCutBgEstQCDenriched'),
#            title = cms.string('Tau TaNC by 1%'),
            title = cms.string('Tau TaNC by Very Loose Isolation'),            
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstQCDenriched',
                                    'tauHistManagerForElecTauBgEstQCDenriched',
                                    ),
            replace = cms.vstring('tauHistManagerForElecTauBgEstQCDenriched.tauSource = tausBgEstQCDenrichedIsoCumulative')
        ),         
        cms.PSet(
            filter = cms.string('tauElectronVetoBgEstQCDenriched'),
            title = cms.string('Tau e-Veto'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstQCDenriched',
                                    'tauHistManagerForElecTauBgEstQCDenriched',
                                    ),
            replace = cms.vstring('tauHistManagerForElecTauBgEstQCDenriched.tauSource = tausBgEstQCDenrichedElectronVetoCumulative')
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
            title = cms.string('dR(Electron-Tau) > 0.5'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstQCDenriched',
                                    'tauHistManagerForElecTauBgEstQCDenriched',
                                    'diTauCandidateHistManagerForElecTauBgEstQCDenriched',
                                    'diTauCandidateNSVfitHistManagerForElecTauBgEstQCDenriched',
                                    ),
            replace = cms.vstring('tauHistManagerForElecTauBgEstQCDenriched.tauSource = tausBgEstQCDenrichedMuonVetoCumulative',
                                  'diTauCandidateHistManagerForElecTauBgEstQCDenriched.diTauCandidateSource = elecTauPairsBgEstQCDenrichedAntiOverlapVetoCumulative',
                                  'diTauCandidateNSVfitHistManagerForElecTauBgEstQCDenriched.diTauCandidateSource = elecTauPairsBgEstQCDenrichedAntiOverlapVetoCumulative'
                                  
                                  )
        ),        
        cms.PSet(
            filter = cms.string('diTauCandidateMt1METCutBgEstQCDenriched'),
            title = cms.string('M_{T}(Electron-MET) < 40 GeV (off)'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstQCDenriched',
                                    'tauHistManagerForElecTauBgEstQCDenriched',
                                    'diTauCandidateHistManagerForElecTauBgEstQCDenriched',
                                    'diTauCandidateNSVfitHistManagerForElecTauBgEstQCDenriched',                                    
                                    ),
        ),          
        cms.PSet(
            filter = cms.string('diTauCandidatePzetaDiffCutBgEstQCDenriched'),
            title = cms.string('Pzeta-1.5*Pzeta(vis) > -20 GeV (off)'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstQCDenriched',
                                    'tauHistManagerForElecTauBgEstQCDenriched',
                                    'diTauCandidateHistManagerForElecTauBgEstQCDenriched',
                                    'diTauCandidateNSVfitHistManagerForElecTauBgEstQCDenriched',                                    
                                    ),
        ),           
        cms.PSet(
            filter = cms.string('diTauCandidateZeroChargeCutBgEstQCDenriched'),
            title = cms.string('Charge(Electron+Tau) = 0 (off)'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstQCDenriched',
                                    'tauHistManagerForElecTauBgEstQCDenriched',
                                    'diTauCandidateHistManagerForElecTauBgEstQCDenriched',
                                    'diTauCandidateNSVfitHistManagerForElecTauBgEstQCDenriched',                                                                        
                                    ),
        ),            
        cms.PSet(
            filter = cms.string('diTauCandidateZeeHypothesisVetoBgEstQCDenriched'),
            title = cms.string('no 2nd OS, loosely-isolated electron')
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstQCDenriched',
                                    'tauHistManagerForElecTauBgEstQCDenriched',
                                    'diTauCandidateHistManagerForElecTauBgEstQCDenriched',
                                    'diTauCandidateNSVfitHistManagerForElecTauBgEstQCDenriched',                                                                        
                                    ),
        ),  



        


    )

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
