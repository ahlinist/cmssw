import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.tools.eventSelFlagProdConfigurator import *

#--------------------------------------------------------------------------------
# define event selection criteria for Z --> tau-jet + tau-jet channel
#--------------------------------------------------------------------------------

# generator level phase-space selection
# (NOTE: to be used in case of Monte Carlo samples
#        overlapping in simulated phase-space only !!)
cfgGenPhaseSpaceCut = cms.PSet(
    pluginName = cms.string('genPhaseSpaceCut'),
    pluginType = cms.string('GenPhaseSpaceEventInfoSelector'),
    src = cms.InputTag('genPhaseSpaceEventInfo'),
    cut = cms.string('')
)

# trigger selection
cfgTrigger = cms.PSet(
    pluginName = cms.string('Trigger'),
    pluginType = cms.string('PATTriggerEventSelector'),
    src = cms.InputTag('patTriggerEvent'),
    hltAcceptPaths = cms.vstring('*') # do not reject any events
)

# primary event vertex selection
cfgPrimaryEventVertex = cms.PSet(
    pluginName = cms.string('primaryEventVertex'),
    pluginType = cms.string('VertexMinEventSelector'),
    src = cms.InputTag('offlinePrimaryVerticesWithBS'),
    minNumber = cms.uint32(1)
)
cfgPrimaryEventVertexQuality = cms.PSet(
    pluginName = cms.string('primaryEventVertexQuality'),
    pluginType = cms.string('VertexMinEventSelector'),
    src = cms.InputTag('selectedPrimaryVertexQuality'),
    minNumber = cms.uint32(1)
)
cfgPrimaryEventVertexPosition = cms.PSet(
    pluginName = cms.string('primaryEventVertexPosition'),
    pluginType = cms.string('VertexMinEventSelector'),
    src = cms.InputTag('selectedPrimaryVertexPosition'),
    minNumber = cms.uint32(1)
)

# selection of first tau-jet candidate
cfgFirstTauEtaCut = cms.PSet(
    pluginName = cms.string('firstTauEtaCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedDiTauPairs1stTauEta23Cumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs1stTauEta23Individual'),
    minNumber = cms.uint32(1)
)
cfgFirstTauPtCut = cms.PSet(
    pluginName = cms.string('firstTauPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedDiTauPairs1stTauPt20Cumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs1stTauPt20Individual'),
    minNumber = cms.uint32(1)
)
cfgFirstTauLeadTrkCut = cms.PSet(
    pluginName = cms.string('firstTauLeadTrkCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedDiTauPairs1stTauLeadTrkCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs1stTauLeadTrkIndividual'),
    minNumber = cms.uint32(1)
)
cfgFirstTauLeadTrkPtCut = cms.PSet(
    pluginName = cms.string('firstTauLeadTrkPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedDiTauPairs1stTauLeadTrkPtCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs1stTauLeadTrkPtIndividual'),
    minNumber = cms.uint32(1)
)
cfgFirstTauTaNCdiscrCut = cms.PSet(
    pluginName = cms.string('firstTauTaNCdiscrCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedDiTauPairs1stTauTaNCdiscrCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs1stTauTaNCdiscrIndividual'),
    minNumber = cms.uint32(1)
)
cfgFirstTauTrkIsoCut = cms.PSet(
    pluginName = cms.string('firstTauTrkIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedDiTauPairs1stTauTrkIsoCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs1stTauTrkIsoIndividual'),
    minNumber = cms.uint32(1)
)
cfgFirstTauEcalIsoCut = cms.PSet(
    pluginName = cms.string('firstTauEcalIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedDiTauPairs1stTauEcalIsoCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs1stTauEcalIsoIndividual'),
    minNumber = cms.uint32(1)
)
cfgFirstTauProngCut = cms.PSet(
    pluginName = cms.string('firstTauProngCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedDiTauPairs1stTauProngCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs1stTauProngIndividual'),
    minNumber = cms.uint32(1)
)
cfgFirstTauChargeCut = cms.PSet(
    pluginName = cms.string('firstTauChargeCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedDiTauPairs1stTauChargeCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs1stTauChargeIndividual'),
    minNumber = cms.uint32(1)
)
cfgFirstTauMuonVeto = cms.PSet(
    pluginName = cms.string('firstTauMuonVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedDiTauPairs1stTauMuonVetoCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs1stTauMuonVetoIndividual'),
    minNumber = cms.uint32(1)
)
cfgFirstTauElectronVeto = cms.PSet(
    pluginName = cms.string('firstTauElectronVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedDiTauPairs1stTauElectronVetoCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs1stTauElectronVetoIndividual'),
    minNumber = cms.uint32(1)
)

# selection of second tau-jet candidate
cfgSecondTauEtaCut = cms.PSet(
    pluginName = cms.string('secondTauEtaCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedDiTauPairs2ndTauEta23Cumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs2ndTauEta23Individual'),
    minNumber = cms.uint32(1)
)
cfgSecondTauPtCut = cms.PSet(
    pluginName = cms.string('secondTauPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedDiTauPairs2ndTauPt20Cumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs2ndTauPt20Individual'),
    minNumber = cms.uint32(1)
)
cfgSecondTauLeadTrkCut = cms.PSet(
    pluginName = cms.string('secondTauLeadTrkCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedDiTauPairs2ndTauLeadTrkCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs2ndTauLeadTrkIndividual'),
    minNumber = cms.uint32(1)
)
cfgSecondTauLeadTrkPtCut = cms.PSet(
    pluginName = cms.string('secondTauLeadTrkPtCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedDiTauPairs2ndTauLeadTrkPtCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs2ndTauLeadTrkPtIndividual'),
    minNumber = cms.uint32(1)
)
cfgSecondTauTaNCdiscrCut = cms.PSet(
    pluginName = cms.string('secondTauTaNCdiscrCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedDiTauPairs2ndTauTaNCdiscrCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs2ndTauTaNCdiscrIndividual'),
    minNumber = cms.uint32(1)
)
cfgSecondTauTrkIsoCut = cms.PSet(
    pluginName = cms.string('secondTauTrkIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedDiTauPairs2ndTauTrkIsoCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs2ndTauTrkIsoIndividual'),
    minNumber = cms.uint32(1)
)
cfgSecondTauEcalIsoCut = cms.PSet(
    pluginName = cms.string('secondTauEcalIsoCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedDiTauPairs2ndTauEcalIsoCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs2ndTauEcalIsoIndividual'),
    minNumber = cms.uint32(1)
)
cfgSecondTauProngCut = cms.PSet(
    pluginName = cms.string('secondTauProngCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedDiTauPairs2ndTauProngCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs2ndTauProngIndividual'),
    minNumber = cms.uint32(1)
)
cfgSecondTauChargeCut = cms.PSet(
    pluginName = cms.string('secondTauChargeCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedDiTauPairs2ndTauChargeCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs2ndTauChargeIndividual'),
    minNumber = cms.uint32(1)
)
cfgSecondTauMuonVeto = cms.PSet(
    pluginName = cms.string('secondTauMuonVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedDiTauPairs2ndTauMuonVetoCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs2ndTauMuonVetoIndividual'),
    minNumber = cms.uint32(1)
)
cfgSecondTauElectronVeto = cms.PSet(
    pluginName = cms.string('secondTauElectronVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedDiTauPairs2ndTauElectronVetoCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairs2ndTauElectronVetoIndividual'),
    minNumber = cms.uint32(1)
)

# di-tau candidate selection
cfgDiTauCandidateForDiTauAntiOverlapVeto = cms.PSet(
    pluginName = cms.string('diTauCandidateForDiTauAntiOverlapVeto'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedDiTauPairsAntiOverlapVetoCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairsAntiOverlapVetoIndividual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForDiTauZeroChargeCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForDiTauZeroChargeCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedDiTauPairsZeroChargeCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairsZeroChargeIndividual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForDiTauAcoplanarityCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForDiTauAcoplanarityCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedDiTauPairsAcoplanarityCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairsAcoplanarityIndividual'),
    minNumber = cms.uint32(1)
)
cfgDiTauCandidateForDiTauPzetaDiffCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForDiTauPzetaDiffCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedDiTauPairsPzetaDiffCumulative'),
    src_individual = cms.InputTag('selectedDiTauPairsPzetaDiffIndividual'),
    minNumber = cms.uint32(1)
)

# veto events containing additional central jets with Et > 20 GeV
#cfgCentralJetVeto = cms.PSet(
#    pluginName = cms.string('centralJetVeto'),
#    pluginType = cms.string('PATCandViewMaxEventSelector'),
#    src = cms.InputTag('selectedPatJetsEt20Cumulative'), # CV: need to carefully check InputTag before using central jet veto !!
#    maxNumber = cms.uint32(0)
#)

zToDiTauEventSelConfigurator = eventSelFlagProdConfigurator(
    [ cfgGenPhaseSpaceCut,
      cfgTrigger,
      cfgPrimaryEventVertex,
      cfgPrimaryEventVertexQuality,
      cfgPrimaryEventVertexPosition,
      cfgFirstTauEtaCut,
      cfgFirstTauPtCut,
      cfgFirstTauLeadTrkCut,
      cfgFirstTauLeadTrkPtCut,
      cfgFirstTauTaNCdiscrCut,
      cfgFirstTauTrkIsoCut,
      cfgFirstTauEcalIsoCut,
      cfgFirstTauProngCut,
      cfgFirstTauChargeCut,
      cfgFirstTauMuonVeto,
      cfgFirstTauElectronVeto,
      cfgSecondTauEtaCut,
      cfgSecondTauPtCut,
      cfgSecondTauLeadTrkCut,
      cfgSecondTauLeadTrkPtCut,
      cfgSecondTauTaNCdiscrCut,
      cfgSecondTauTrkIsoCut,
      cfgSecondTauEcalIsoCut,
      cfgSecondTauProngCut,
      cfgSecondTauChargeCut,
      cfgSecondTauMuonVeto,
      cfgSecondTauElectronVeto,
      cfgDiTauCandidateForDiTauAntiOverlapVeto,
      cfgDiTauCandidateForDiTauZeroChargeCut,
      cfgDiTauCandidateForDiTauAcoplanarityCut,
      cfgDiTauCandidateForDiTauPzetaDiffCut ],
      #cfgCentralJetVeto ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

selectZtoDiTauEvents = zToDiTauEventSelConfigurator.configure()
