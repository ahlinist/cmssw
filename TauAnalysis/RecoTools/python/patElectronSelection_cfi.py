import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------  
# produce collections of pat::Electrons passing selection criteria
#--------------------------------------------------------------------------------

# see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePhysicsCutParser
# on how to use the cut-string parser

# require electron candidate to pass the tight electron id. criteria
selectedLayer1ElectronsTightId = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("cleanLayer1Electrons"),
     cut = cms.string('(abs(superCluster.eta) < 1.479 & electronID("robust") > 0 & eSuperClusterOverP < 1.05 & eSuperClusterOverP > 0.95) | (abs(superCluster.eta) > 1.479 & electronID("robust") > 0 & eSuperClusterOverP < 1.12 & eSuperClusterOverP > 0.95)'),
     filter = cms.bool(False)
)

# require electron candidate to not be within eta-crack
# between Barrel and Encap ECAL calorimeter
selectedLayer1ElectronsAntiCrackCutCumulative = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("selectedLayer1ElectronsTightId"),
     cut = cms.string('abs(superCluster.eta) < 1.442 | abs(superCluster.eta) > 1.560'),
     filter = cms.bool(False)
)

selectedLayer1ElectronsAntiCrackCutIndividual = copy.deepcopy(selectedLayer1ElectronsAntiCrackCutCumulative)
selectedLayer1ElectronsAntiCrackCutIndividual.src = selectedLayer1ElectronsTightId.src

# require electron candidate to be within geometric acceptance of electron trigger
selectedLayer1ElectronsEta21Cumulative = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("selectedLayer1ElectronsAntiCrackCutCumulative"),
     cut = cms.string('abs(eta) < 2.1'),
     filter = cms.bool(False)
)

selectedLayer1ElectronsEta21Individual = copy.deepcopy(selectedLayer1ElectronsEta21Cumulative)
selectedLayer1ElectronsEta21Individual.src = selectedLayer1ElectronsTightId.src

# require electron candidate to have transverse momentum above threshold
selectedLayer1ElectronsPt15Cumulative = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("selectedLayer1ElectronsEta21Cumulative"),
     cut = cms.string('pt > 15.'),
     filter = cms.bool(False)
)

selectedLayer1ElectronsPt15Individual = copy.deepcopy(selectedLayer1ElectronsPt15Cumulative)
selectedLayer1ElectronsPt15Individual.src = selectedLayer1ElectronsTightId.src

# require electron candidate to be isolated
# with respect to tracks (of Pt >~ 0.3 GeV)
selectedLayer1ElectronsTrkIsoCumulative = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("selectedLayer1ElectronsPt15Cumulative"),
     cut = cms.string('trackIso < 1.'),
     filter = cms.bool(False)
)

#selectedLayer1ElectronsTrkIsoCumulative = cms.EDFilter("PATElectronIsoDepositSelector",
#     src = cms.InputTag("selectedLayer1ElectronsPt15Cumulative"),
#     type = cms.string('tracker'),
#     #vetos = cms.vstring("0.015", "Threshold(0.9)"),
#     vetos = cms.vstring("0.015"),                       
#     dRisoCone = cms.double(0.6),
#     #numMax = cms.int32(0),
#     sumPtMax = cms.double(0.9),
#     filter = cms.bool(False)
#)

selectedLayer1ElectronsTrkIsoIndividual = copy.deepcopy(selectedLayer1ElectronsTrkIsoCumulative)
selectedLayer1ElectronsTrkIsoIndividual.src = selectedLayer1ElectronsTightId.src

# require electron candidate to be isolated
# with respect to energy deposits in ECAL
# (not associated to electron candidate)
selectedLayer1ElectronsEcalIsoCumulative = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("selectedLayer1ElectronsTrkIsoCumulative"),
     cut = cms.string('(abs(superCluster.eta) < 1.479 & ecalIso < 1.0) | (abs(superCluster.eta) > 1.479 & ecalIso < 2.5)'),
     filter = cms.bool(False)
)

selectedLayer1ElectronsEcalIsoIndividual = copy.deepcopy(selectedLayer1ElectronsEcalIsoCumulative)
selectedLayer1ElectronsEcalIsoIndividual.src = selectedLayer1ElectronsTightId.src

# require electron candidate to be linked to (GSF) track
selectedLayer1ElectronsTrkCumulative = cms.EDFilter("PATElectronSelector",
     src = cms.InputTag("selectedLayer1ElectronsEcalIsoCumulative"),
     cut = cms.string('gsfTrack.isNonnull'),
     filter = cms.bool(False)
)

selectedLayer1ElectronsTrkIndividual = copy.deepcopy(selectedLayer1ElectronsTrkCumulative)
selectedLayer1ElectronsTrkIndividual.src = selectedLayer1ElectronsTightId.src

# require track of electron candidate to have small transverse impact parameter
# (in order to veto electrons resulting from b-quark decays)
selectedLayer1ElectronsTrkIPcumulative = cms.EDProducer("PATElectronIpSelector",
     src = cms.InputTag("selectedLayer1ElectronsTrkCumulative"),
     vertexSource = cms.InputTag("selectedPrimaryVertexPosition"),
     IpMax = cms.double(0.05),
     filter = cms.bool(False)                                               
)

selectedLayer1ElectronsTrkIPindividual = copy.deepcopy(selectedLayer1ElectronsTrkIPcumulative)
selectedLayer1ElectronsTrkIPindividual.src = selectedLayer1ElectronsTightId.src

selectLayer1Electrons = cms.Sequence( selectedLayer1ElectronsTightId
                                     *selectedLayer1ElectronsAntiCrackCutCumulative * selectedLayer1ElectronsAntiCrackCutIndividual
                                     *selectedLayer1ElectronsEta21Cumulative * selectedLayer1ElectronsEta21Individual
                                     *selectedLayer1ElectronsPt15Cumulative * selectedLayer1ElectronsPt15Individual
                                     *selectedLayer1ElectronsTrkIsoCumulative * selectedLayer1ElectronsTrkIsoIndividual
                                     *selectedLayer1ElectronsEcalIsoCumulative * selectedLayer1ElectronsEcalIsoIndividual
                                     *selectedLayer1ElectronsTrkCumulative * selectedLayer1ElectronsTrkIndividual
                                     *selectedLayer1ElectronsTrkIPcumulative * selectedLayer1ElectronsTrkIPindividual )

#--------------------------------------------------------------------------------
# produce "summary" collection of pat::Electrons with flags added,
# indicating whether or not electrons passed individual selection criteria
#--------------------------------------------------------------------------------

cleanLayer1ElectronsSel = cms.EDProducer("PATElectronSelProducer",

  leptonSource = cms.InputTag("cleanLayer1Electrons"),

  selFlags = cms.PSet(
    tauAnalysisSelElectronTightIdGlobal = cms.PSet(
      src = cms.InputTag('selectedLayer1ElectronsTightId')
    ),
    tauAnalysisSelElectronAntiCrackCut = cms.PSet(
      src = cms.InputTag('selectedLayer1ElectronsAntiCrackCutIndividual')
    ),
    tauAnalysisSelElectronEta21 = cms.PSet(
      src = cms.InputTag('selectedLayer1ElectronsEta21Individual')
    ),
    tauAnalysisSelElectronPt15 = cms.PSet(
      src = cms.InputTag('selectedLayer1ElectronsPt15Individual')
    ),
    tauAnalysisSelElectronTrkIso = cms.PSet(
      src = cms.InputTag('selectedLayer1ElectronsTrkIsoIndividual')
    ),
    tauAnalysisSelElectronEcalIso = cms.PSet(
      src = cms.InputTag('selectedLayer1ElectronsEcalIsoIndividual')
    ),
    tauAnalysisSelElectronTrk = cms.PSet(
      src = cms.InputTag('selectedLayer1ElectronsTrkIndividual')
    ),
    tauAnalysisSelElectronTrkIP = cms.PSet(
      src = cms.InputTag('selectedLayer1ElectronsTrkIPindividual')
    )
  )
)

produceLayer1SelElectrons = cms.Sequence( selectLayer1Electrons * cleanLayer1ElectronsSel )
