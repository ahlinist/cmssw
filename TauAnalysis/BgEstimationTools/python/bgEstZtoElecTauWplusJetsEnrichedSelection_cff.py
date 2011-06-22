import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *
from TauAnalysis.RecoTools.tools.eventSelFlagProdConfigurator import *

#--------------------------------------------------------------------------------
# select W --> l nu background enriched event sample
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

electronsBgEstWplusJetsEnrichedIso = copy.deepcopy(electronsBgEstZtautauEnrichedIso)

# require electron to not be from a photon conversion
electronsBgEstWplusJetsEnrichedConversionVeto = copy.deepcopy(electronsBgEstZtautauEnrichedConversionVeto)

electronsBgEstWplusJetsEnrichedTrkIP = copy.deepcopy(electronsBgEstZtautauEnrichedTrkIP)


electronSelConfiguratorBgEstWplusJetsEnriched = objSelConfigurator(
    [ electronsBgEstWplusJetsEnrichedId,
      electronsBgEstWplusJetsEnrichedAntiCrackCut,
      electronsBgEstWplusJetsEnrichedEta,
      electronsBgEstWplusJetsEnrichedPt,
      electronsBgEstWplusJetsEnrichedIso,
      electronsBgEstWplusJetsEnrichedConversionVeto,
      electronsBgEstWplusJetsEnrichedTrkIP
      ],
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
tausBgEstWplusJetsEnrichedAntiOverlapWithElectronsVeto.srcNotToBeFiltered = cms.VInputTag("electronsBgEstWplusJetsEnrichedPtCumulative")

# require tau candidate to be within geometric acceptance of Pixel + SiTracker detectors
tausBgEstWplusJetsEnrichedEta = copy.deepcopy(tausBgEstZtautauEnrichedEta)

# require tau candidate to have transverse energy above threshold
tausBgEstWplusJetsEnrichedPt = copy.deepcopy(tausBgEstZtautauEnrichedPt)

# require leading track of tau candidate to have Pt > 5. GeV
tausBgEstWplusJetsEnrichedLeadTrkPt = copy.deepcopy(tausBgEstZtautauEnrichedLeadTrkPt)

# require tau candidate to pass TaNC discriminator
tausBgEstWplusJetsEnrichedIso = copy.deepcopy(tausBgEstZtautauEnrichedIso)
tausBgEstWplusJetsEnrichedIso.cut = cms.string('tauID("byHPSvloose") > 0.5 & tauID("byHPSmedium") < 0.5')


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
      tausBgEstWplusJetsEnrichedLeadTrkPt,
      tausBgEstWplusJetsEnrichedIso,
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

### !!!!! inverted cut
elecTauPairsBgEstWplusJetsEnrichedMt1MET = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedMt1MET)
elecTauPairsBgEstWplusJetsEnrichedMt1MET.cut = cms.string('mt1MET > 60.')

### !!!!! turned off
elecTauPairsBgEstWplusJetsEnrichedPzetaDiff = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedPzetaDiff)
elecTauPairsBgEstWplusJetsEnrichedPzetaDiff.cut = cms.string('(pZeta - 1.5*pZetaVis) > -2000.')

### !!!!! turned off
elecTauPairsBgEstWplusJetsEnrichedZeroCharge = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedZeroCharge)
elecTauPairsBgEstWplusJetsEnrichedZeroCharge.cut = cms.string('charge != 10')


elecTauPairSelConfiguratorBgEstWplusJetsEnriched = objSelConfigurator(
    [ elecTauPairsBgEstWplusJetsEnrichedAntiOverlapVeto,
      elecTauPairsBgEstWplusJetsEnrichedMt1MET,
      elecTauPairsBgEstWplusJetsEnrichedPzetaDiff,
      elecTauPairsBgEstWplusJetsEnrichedZeroCharge
      ],
    src = "elecTauPairsBgEstWplusJetsEnriched",
    pyModuleName = __name__,
    doSelIndividual = False
)

selectElecTauPairsBgEstWplusJetsEnriched = elecTauPairSelConfiguratorBgEstWplusJetsEnriched.configure(pyNameSpace = locals())

####### anti Zee Cut #######



### production

allDiElecPairBgEstWplusJetsEnrichedZeeHypothesesByLooseIsolation = copy.deepcopy(allDiElecPairBgEstZtautauEnrichedZeeHypothesesByLooseIsolation)
allDiElecPairBgEstWplusJetsEnrichedZeeHypothesesByLooseIsolation.srcLeg1 = cms.InputTag("electronsBgEstWplusJetsEnrichedTrkIPcumulative")

### selection

selectedDiElecPairBgEstWplusJetsEnrichedZeeHypothesesByLooseIsolation = copy.deepcopy(selectedDiElecPairBgEstZtautauEnrichedZeeHypothesesByLooseIsolation)
selectedDiElecPairBgEstWplusJetsEnrichedZeeHypothesesByLooseIsolation.src = cms.InputTag("allDiElecPairBgEstWplusJetsEnrichedZeeHypothesesByLooseIsolation")

produceElecTauPairZeeHypothesesBgEstWplusJetsEnriched = cms.Sequence(
	selectedPatElectronsForZeeHypotheses * 
	allDiElecPairBgEstWplusJetsEnrichedZeeHypothesesByLooseIsolation *
	selectedDiElecPairBgEstWplusJetsEnrichedZeeHypothesesByLooseIsolation
)

#--------------------------------------------------------------------------------  
# produce boolean event selection flags
#--------------------------------------------------------------------------------


# electron cuts

cfgElectronIdCutBgEstWplusJetsEnrichedMin = copy.deepcopy(cfgElectronIdCutBgEstZtautauEnriched)
cfgElectronIdCutBgEstWplusJetsEnrichedMin.pluginName = cms.string('electronIdCutBgEstWplusJetsEnrichedMin')
cfgElectronIdCutBgEstWplusJetsEnrichedMin.src_cumulative = cms.InputTag('electronsBgEstWplusJetsEnrichedIdCumulative')

## cfgElectronIdCutBgEstWplusJetsEnrichedMax = copy.deepcopy(cfgElectronIdCutBgEstWplusJetsEnrichedMin)
## cfgElectronIdCutBgEstWplusJetsEnrichedMax.pluginName = cms.string('electronIdCutBgEstWplusJetsEnrichedMax')
## cfgElectronIdCutBgEstWplusJetsEnrichedMax.src_cumulative = cms.InputTag('electronsBgEstWplusJetsEnrichedIdCumulative')
## cfgElectronIdCutBgEstWplusJetsEnrichedMax.pluginType = cms.string('PATCandViewMaxEventSelector')
## cfgElectronIdCutBgEstWplusJetsEnrichedMax.maxNumber = cms.uint32(1)

cfgElectronAntiCrackCutBgEstWplusJetsEnriched = copy.deepcopy(cfgElectronAntiCrackCutBgEstZtautauEnriched)
cfgElectronAntiCrackCutBgEstWplusJetsEnriched.pluginName = cms.string('electronAntiCrackCutBgEstWplusJetsEnriched')
cfgElectronAntiCrackCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('electronsBgEstWplusJetsEnrichedAntiCrackCutCumulative')

cfgElectronEtaCutBgEstWplusJetsEnriched = copy.deepcopy(cfgElectronEtaCutBgEstZtautauEnriched)
cfgElectronEtaCutBgEstWplusJetsEnriched.pluginName = cms.string('electronEtaCutBgEstWplusJetsEnriched')
cfgElectronEtaCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('electronsBgEstWplusJetsEnrichedEtaCumulative')

cfgElectronPtCutBgEstWplusJetsEnriched = copy.deepcopy(cfgElectronPtCutBgEstZtautauEnriched)
cfgElectronPtCutBgEstWplusJetsEnriched.pluginName = cms.string('electronPtCutBgEstWplusJetsEnriched')
cfgElectronPtCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('electronsBgEstWplusJetsEnrichedPtCumulative')

cfgElectronIsoCutBgEstWplusJetsEnriched = copy.deepcopy(cfgElectronIsoCutBgEstZtautauEnriched)
cfgElectronIsoCutBgEstWplusJetsEnriched.pluginName = cms.string('electronIsoCutBgEstWplusJetsEnriched')
cfgElectronIsoCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('electronsBgEstWplusJetsEnrichedIsoCumulative')

cfgElectronConversionVetoBgEstWplusJetsEnriched = copy.deepcopy(cfgElectronConversionVetoBgEstZtautauEnriched)
cfgElectronConversionVetoBgEstWplusJetsEnriched.pluginName = cms.string('electronConversionVetoBgEstWplusJetsEnriched')
cfgElectronConversionVetoBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('electronsBgEstWplusJetsEnrichedConversionVetoCumulative')

cfgElectronTrkIPcutBgEstWplusJetsEnriched = copy.deepcopy(cfgElectronTrkIPcutBgEstZtautauEnriched)
cfgElectronTrkIPcutBgEstWplusJetsEnriched.pluginName = cms.string('electronTrkIPcutBgEstWplusJetsEnriched')
cfgElectronTrkIPcutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('electronsBgEstWplusJetsEnrichedTrkIPcumulative')


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

cfgTauLeadTrkPtCutBgEstWplusJetsEnriched = copy.deepcopy(cfgTauLeadTrkPtCutBgEstZtautauEnriched)
cfgTauLeadTrkPtCutBgEstWplusJetsEnriched.pluginName = cms.string('tauLeadTrkPtCutBgEstWplusJetsEnriched')
cfgTauLeadTrkPtCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstWplusJetsEnrichedLeadTrkPtCumulative')

cfgTauIsoCutBgEstWplusJetsEnriched = copy.deepcopy(cfgTauIsoCutBgEstZtautauEnriched)
cfgTauIsoCutBgEstWplusJetsEnriched.pluginName = cms.string('tauIsoCutBgEstWplusJetsEnriched')
cfgTauIsoCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('tausBgEstWplusJetsEnrichedIsoCumulative')

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

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut
cfgDiTauCandidateForElecTauMt1METCutBgEstWplusJetsEnriched = copy.deepcopy(cfgDiTauCandidateForElecTauMt1METCutBgEstZtautauEnriched)
cfgDiTauCandidateForElecTauMt1METCutBgEstWplusJetsEnriched.pluginName = cms.string('diTauCandidateMt1METCutBgEstWplusJetsEnriched')
cfgDiTauCandidateForElecTauMt1METCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('elecTauPairsBgEstWplusJetsEnrichedMt1METcumulative')

cfgDiTauCandidateForElecTauPzetaDiffCutBgEstWplusJetsEnriched = copy.deepcopy(cfgDiTauCandidateForElecTauPzetaDiffCutBgEstZtautauEnriched)
cfgDiTauCandidateForElecTauPzetaDiffCutBgEstWplusJetsEnriched.pluginName = cms.string('diTauCandidatePzetaDiffCutBgEstWplusJetsEnriched')
cfgDiTauCandidateForElecTauPzetaDiffCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('elecTauPairsBgEstWplusJetsEnrichedPzetaDiffCumulative')

cfgDiTauCandidateForElecTauZeroChargeCutBgEstWplusJetsEnriched = copy.deepcopy(cfgDiTauCandidateForElecTauZeroChargeCutBgEstZtautauEnriched)
cfgDiTauCandidateForElecTauZeroChargeCutBgEstWplusJetsEnriched.pluginName = cms.string('diTauCandidateZeroChargeCutBgEstWplusJetsEnriched')
cfgDiTauCandidateForElecTauZeroChargeCutBgEstWplusJetsEnriched.src_cumulative = cms.InputTag('elecTauPairsBgEstWplusJetsEnrichedZeroChargeCumulative')

cfgElecTauPairZeeHypothesisVetoBgEstWplusJetsEnriched = copy.deepcopy(cfgElecTauPairZeeHypothesisVetoBgEstZtautauEnriched)
cfgElecTauPairZeeHypothesisVetoBgEstWplusJetsEnriched.pluginName = cms.string('diTauCandidateZeeHypothesisVetoBgEstWplusJetsEnriched')
cfgElecTauPairZeeHypothesisVetoBgEstWplusJetsEnriched.src = cms.InputTag('selectedDiElecPairBgEstWplusJetsEnrichedZeeHypothesesByLooseIsolation')


evtSelConfiguratorBgEstWplusJetsEnriched = eventSelFlagProdConfigurator(
    [ cfgElectronIdCutBgEstWplusJetsEnrichedMin,
      #cfgElectronIdCutBgEstWplusJetsEnrichedMax,
      cfgElectronAntiCrackCutBgEstWplusJetsEnriched,
      cfgElectronEtaCutBgEstWplusJetsEnriched,
      cfgElectronPtCutBgEstWplusJetsEnriched,
      cfgElectronIsoCutBgEstWplusJetsEnriched,
      cfgElectronConversionVetoBgEstWplusJetsEnriched,
      cfgElectronTrkIPcutBgEstWplusJetsEnriched,     
      cfgTauAntiOverlapWithElectronsVetoBgEstWplusJetsEnriched,
      cfgTauEtaCutBgEstWplusJetsEnriched,
      cfgTauPtCutBgEstWplusJetsEnriched,
      cfgTauLeadTrkPtCutBgEstWplusJetsEnriched,
      cfgTauIsoCutBgEstWplusJetsEnriched,
      cfgTauElectronVetoBgEstWplusJetsEnriched,
      cfgTauEcalCrackVetoBgEstWplusJetsEnriched,
      cfgTauMuonVetoBgEstWplusJetsEnriched,
      cfgDiTauCandidateAntiOverlapVetoBgEstWplusJetsEnriched,
      cfgDiTauCandidateForElecTauMt1METCutBgEstWplusJetsEnriched,
      cfgDiTauCandidateForElecTauPzetaDiffCutBgEstWplusJetsEnriched,
      cfgDiTauCandidateForElecTauZeroChargeCutBgEstWplusJetsEnriched,
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
diTauCandidateHistManagerForElecTauBgEstWplusJetsEnriched.diTauCandidateSource = cms.InputTag('elecTauPairsBgEstWplusJetsEnriched')

electronHistManagerForElecTauBgEstWplusJetsEnriched = copy.deepcopy(electronHistManagerForElecTauBgEstZtautauEnriched)
electronHistManagerForElecTauBgEstWplusJetsEnriched.pluginName = cms.string('electronHistManagerForElecTauBgEstWplusJetsEnriched')

tauHistManagerForElecTauBgEstWplusJetsEnriched = copy.deepcopy(tauHistManagerForElecTauBgEstZtautauEnriched)
tauHistManagerForElecTauBgEstWplusJetsEnriched.pluginName = cms.string('tauHistManagerForElecTauBgEstWplusJetsEnriched')
tauHistManagerForElecTauBgEstWplusJetsEnriched.jetSource = cms.InputTag('cleanPatJets')

diTauCandidateNSVfitHistManagerForElecTauBgEstWplusJetsEnriched = copy.deepcopy(diTauCandidateNSVfitHistManagerForElecTauBgEstZtautauEnriched)
diTauCandidateNSVfitHistManagerForElecTauBgEstWplusJetsEnriched.pluginName = cms.string('diTauCandidateNSVfitHistManagerForElecTauBgEstWplusJetsEnriched')
diTauCandidateNSVfitHistManagerForElecTauBgEstWplusJetsEnriched.diTauCandidateSource = cms.InputTag('elecTauPairsBgEstWplusJetsEnriched')


analyzeEventsBgEstWplusJetsEnriched = cms.EDAnalyzer("GenericAnalyzer",

    name = cms.string('BgEstTemplateAnalyzer_WplusJetsEnriched'), 

    filters = cms.VPSet(
        evtSelGenPhaseSpace,
        evtSelTrigger,
        evtSelPrimaryEventVertex,


        #start electron cuts
        
        cms.PSet(
            pluginName = cms.string('electronIdCutBgEstWplusJetsEnrichedMin'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronIdCutBgEstWplusJetsEnrichedMin','cumulative')
        ),
##         cms.PSet(
##             pluginName = cms.string('electronIdCutBgEstWplusJetsEnrichedMax'),
##             pluginType = cms.string('BoolEventSelector'),
##             src = cms.InputTag('electronIdCutBgEstWplusJetsEnrichedMax','cumulative')
##         ), 
        
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
            pluginName = cms.string('electronIsoCutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronIsoCutBgEstWplusJetsEnriched','cumulative')
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
            pluginName = cms.string('tauLeadTrkPtCutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauLeadTrkPtCutBgEstWplusJetsEnriched','cumulative')
        ),  
        cms.PSet(
            pluginName = cms.string('tauIsoCutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauIsoCutBgEstWplusJetsEnriched','cumulative')
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
        #!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut
        cms.PSet(
            pluginName = cms.string('diTauCandidateMt1METCutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateMt1METCutBgEstWplusJetsEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('diTauCandidatePzetaDiffCutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidatePzetaDiffCutBgEstWplusJetsEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('diTauCandidateZeroChargeCutBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateZeroChargeCutBgEstWplusJetsEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('diTauCandidateZeeHypothesisVetoBgEstWplusJetsEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateZeeHypothesisVetoBgEstWplusJetsEnriched')
        )
        
        
    ),

    analyzers = cms.VPSet(
         diTauCandidateHistManagerForElecTauBgEstWplusJetsEnriched,
         diTauCandidateNSVfitHistManagerForElecTauBgEstWplusJetsEnriched,         
         electronHistManagerForElecTauBgEstWplusJetsEnriched,
         tauHistManagerForElecTauBgEstWplusJetsEnriched,
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
            filter = cms.string('electronIdCutBgEstWplusJetsEnrichedMin'),
            title = cms.string('Electron ID'),
        ),         
##         cms.PSet(
##             filter = cms.string('electronIdCutBgEstWplusJetsEnrichedMin'),
##             title = cms.string('Electron ID: Min of 1 passing'),
##         ),  
        
##         cms.PSet(
##             filter = cms.string('electronIdCutBgEstWplusJetsEnrichedMax'),
##             title = cms.string('Electron ID: Max of 1 passing'),
##         ),   
        
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
            title = cms.string('-2.3 < eta(Tau) < +2.3'),
        ),
        cms.PSet(
            filter = cms.string('tauPtCutBgEstWplusJetsEnriched'),
            title = cms.string('Pt(Tau) > 20 GeV'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstWplusJetsEnriched',
                                    'tauHistManagerForElecTauBgEstWplusJetsEnriched',
                                    ),
            replace = cms.vstring('electronHistManagerForElecTauBgEstWplusJetsEnriched.electronSource = electronsBgEstWplusJetsEnrichedPtCumulative',
                                  'tauHistManagerForElecTauBgEstWplusJetsEnriched.tauSource = tausBgEstWplusJetsEnrichedPtCumulative'
                                  )
        ),            
        cms.PSet(
            filter = cms.string('electronIsoCutBgEstWplusJetsEnriched'),
            title = cms.string('Electron Isolation'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstWplusJetsEnriched',
                                    'tauHistManagerForElecTauBgEstWplusJetsEnriched',
                                    ),
            replace = cms.vstring('electronHistManagerForElecTauBgEstWplusJetsEnriched.electronSource = electronsBgEstWplusJetsEnrichedIsoCumulative')
        ),           
        cms.PSet(
            filter = cms.string('electronConversionVetoBgEstWplusJetsEnriched'),
            title = cms.string('Electron Track conv. veto'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstWplusJetsEnriched',
                                    'tauHistManagerForElecTauBgEstWplusJetsEnriched',
                                    ),
            replace = cms.vstring('electronHistManagerForElecTauBgEstWplusJetsEnriched.electronSource = electronsBgEstWplusJetsEnrichedConversionVetoCumulative')
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
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstWplusJetsEnriched',
                                    'tauHistManagerForElecTauBgEstWplusJetsEnriched',
                                    ),
            replace = cms.vstring('electronHistManagerForElecTauBgEstWplusJetsEnriched.electronSource = electronsBgEstWplusJetsEnrichedTrkIPcumulative',
                                  'tauHistManagerForElecTauBgEstWplusJetsEnriched.tauSource = tausBgEstWplusJetsEnrichedLeadTrkPtCumulative')
        ),
        cms.PSet(
            filter = cms.string('tauIsoCutBgEstWplusJetsEnriched'),
            title = cms.string('Tau ID by HPS Very Loose'),
            #title = cms.string('Tau TaNC by 1%'),            
        ),        
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstWplusJetsEnriched',
                                    'tauHistManagerForElecTauBgEstWplusJetsEnriched',
                                    ),
            replace = cms.vstring('tauHistManagerForElecTauBgEstWplusJetsEnriched.tauSource = tausBgEstWplusJetsEnrichedIsoCumulative')
        ),             
        cms.PSet(
            filter = cms.string('tauElectronVetoBgEstWplusJetsEnriched'),
            title = cms.string('Tau e-Veto'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstWplusJetsEnriched',
                                    'tauHistManagerForElecTauBgEstWplusJetsEnriched',
                                    ),
            replace = cms.vstring('tauHistManagerForElecTauBgEstWplusJetsEnriched.tauSource = tausBgEstWplusJetsEnrichedElectronVetoCumulative')
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
            title = cms.string('dR(Electron-Tau) > 0.5'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstWplusJetsEnriched',
                                    'tauHistManagerForElecTauBgEstWplusJetsEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstWplusJetsEnriched',
                                    'diTauCandidateNSVfitHistManagerForElecTauBgEstWplusJetsEnriched',
                                    ),
            replace = cms.vstring('tauHistManagerForElecTauBgEstWplusJetsEnriched.tauSource = tausBgEstWplusJetsEnrichedMuonVetoCumulative',
                                  'diTauCandidateHistManagerForElecTauBgEstWplusJetsEnriched.diTauCandidateSource = elecTauPairsBgEstWplusJetsEnrichedAntiOverlapVetoCumulative',
                                  'diTauCandidateNSVfitHistManagerForElecTauBgEstWplusJetsEnriched.diTauCandidateSource = elecTauPairsBgEstWplusJetsEnrichedAntiOverlapVetoCumulative'
                                  
                                  )
        ),          
        #!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut
        cms.PSet(
            filter = cms.string('diTauCandidateMt1METCutBgEstWplusJetsEnriched'),
            title = cms.string('M_{T}(Electron-MET) < 40 GeV (inverted)'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstWplusJetsEnriched',
                                    'tauHistManagerForElecTauBgEstWplusJetsEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstWplusJetsEnriched',
                                    'diTauCandidateNSVfitHistManagerForElecTauBgEstWplusJetsEnriched',
                                    ),
        ),         
        cms.PSet(
            filter = cms.string('diTauCandidatePzetaDiffCutBgEstWplusJetsEnriched'),
            title = cms.string('Pzeta-1.5*Pzeta(vis) > -20 GeV (off)'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstWplusJetsEnriched',
                                    'tauHistManagerForElecTauBgEstWplusJetsEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstWplusJetsEnriched',
                                    'diTauCandidateNSVfitHistManagerForElecTauBgEstWplusJetsEnriched',
                                    ),
        ),          
        cms.PSet(
            filter = cms.string('diTauCandidateZeroChargeCutBgEstWplusJetsEnriched'),
            title = cms.string('Charge(Electron+Tau) = 0 (off)'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstWplusJetsEnriched',
                                    'tauHistManagerForElecTauBgEstWplusJetsEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstWplusJetsEnriched',
                                    'diTauCandidateNSVfitHistManagerForElecTauBgEstWplusJetsEnriched',
                                    ),
        ),         
        cms.PSet(
            filter = cms.string('diTauCandidateZeeHypothesisVetoBgEstWplusJetsEnriched'),
            title = cms.string('no 2nd OS, loosely-isolated electron')
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstWplusJetsEnriched',
                                    'tauHistManagerForElecTauBgEstWplusJetsEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstWplusJetsEnriched',
                                    'diTauCandidateNSVfitHistManagerForElecTauBgEstWplusJetsEnriched',
                                    ),
        ),




    )

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
