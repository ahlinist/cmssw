import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------  
# produce collections of pat::Electrons passing selection criteria
#--------------------------------------------------------------------------------

# see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePhysicsCutParser
# on how to use the cut-string parser

# require muon candidate to be a global muon
# (track in muon system linked to track in Pixel + SiTracker detectors)
selectedLayer1MuonsGlobal = cms.EDFilter("PATMuonSelector",
     src = cms.InputTag("cleanLayer1Muons"),
     cut = cms.string('isGlobalMuon()'),
     filter = cms.bool(False)
)

# require muon candidate to be within geometric acceptance of muon trigger
selectedLayer1MuonsEta21Cumulative = cms.EDFilter("PATMuonSelector",
     src = cms.InputTag("selectedLayer1MuonsGlobal"),
     cut = cms.string('abs(eta) < 2.1'),
     filter = cms.bool(False)
)

selectedLayer1MuonsEta21Individual = copy.deepcopy(selectedLayer1MuonsEta21Cumulative)
selectedLayer1MuonsEta21Individual.src = selectedLayer1MuonsGlobal.src

# require muon candidate to have transverse momentum above threshold
selectedLayer1MuonsPt15Cumulative = cms.EDFilter("PATMuonSelector",
     src = cms.InputTag("selectedLayer1MuonsEta21Cumulative"),
     cut = cms.string('pt > 15.'),
     filter = cms.bool(False)
)

selectedLayer1MuonsPt15Individual = copy.deepcopy(selectedLayer1MuonsPt15Cumulative)
selectedLayer1MuonsPt15Individual.src = selectedLayer1MuonsGlobal.src

# require muon candidate to be isolated
# with respect to tracks (of Pt >~ 0.3 GeV)
#selectedLayer1MuonsTrkIsoCumulative = cms.EDFilter("PATMuonSelector",
#     src = cms.InputTag("selectedLayer1MuonsPt15Cumulative"),
#     cut = cms.string('trackIso < 1.'),
#     filter = cms.bool(False)
#)

selectedLayer1MuonsTrkIsoCumulative = cms.EDFilter("PATMuonIsoDepositSelector",
     src = cms.InputTag("selectedLayer1MuonsPt15Cumulative"),
     type = cms.string('tracker'),
     #vetos = cms.vstring("0.01", "Threshold(0.9)"),
     vetos = cms.vstring("0.01"),                          
     dRisoCone = cms.double(0.6),
     #numMax = cms.int32(0),
     sumPtMax = cms.double(1.),
     filter = cms.bool(False)
)

selectedLayer1MuonsTrkIsoIndividual = copy.deepcopy(selectedLayer1MuonsTrkIsoCumulative)
selectedLayer1MuonsTrkIsoIndividual.src = selectedLayer1MuonsGlobal.src

# require muon candidate to be isolated
# with respect to energy deposits in ECAL
# (not associated to muon candidate)
selectedLayer1MuonsEcalIsoCumulative = cms.EDFilter("PATMuonSelector",
     src = cms.InputTag("selectedLayer1MuonsTrkIsoCumulative"),
     cut = cms.string('ecalIso < 1.'),
     filter = cms.bool(False)
)

selectedLayer1MuonsEcalIsoIndividual = copy.deepcopy(selectedLayer1MuonsEcalIsoCumulative)
selectedLayer1MuonsEcalIsoIndividual.src = selectedLayer1MuonsGlobal.src

# require muon candidate to pass pion veto
selectedLayer1MuonsPionVetoCumulative = cms.EDProducer("PATMuonAntiPionSelector",
     src = cms.InputTag("selectedLayer1MuonsEcalIsoCumulative"),
     CaloCompCoefficient = cms.double(0.8),
     SegmCompCoefficient = cms.double(1.2),
     AntiPionCut = cms.double(1.0),
     filter = cms.bool(False)
)

selectedLayer1MuonsPionVetoIndividual = copy.deepcopy(selectedLayer1MuonsPionVetoCumulative)
selectedLayer1MuonsPionVetoIndividual.src = selectedLayer1MuonsGlobal.src

# require muon candidate to be linked to track in silicon strip + pixel detectors
# (all global muons should be linked to tracks in the "inner" tracking detectors;
#  in case the muon is not linked to an "inner" track,
#  the track impact parameter selection will cause processing of the entire event to be skipped !!)
selectedLayer1MuonsTrkCumulative = cms.EDFilter("PATMuonSelector",
     src = cms.InputTag("selectedLayer1MuonsPionVetoCumulative"),
     cut = cms.string('innerTrack.isNonnull'),
     filter = cms.bool(False)
)

selectedLayer1MuonsTrkIndividual = copy.deepcopy(selectedLayer1MuonsTrkCumulative)
selectedLayer1MuonsTrkIndividual.src = selectedLayer1MuonsGlobal.src

# require track of muon candidate to have small transverse impact parameter
# (in order to veto muons resulting from b-quark decays)
selectedLayer1MuonsTrkIPcumulative = cms.EDProducer("PATMuonIpSelector",
     src = cms.InputTag("selectedLayer1MuonsTrkCumulative"),
     vertexSource = cms.InputTag("selectedPrimaryVertexPosition"),
     IpMax = cms.double(0.05),
     filter = cms.bool(False)                                               
)

selectedLayer1MuonsTrkIPindividual = copy.deepcopy(selectedLayer1MuonsTrkIPcumulative)
selectedLayer1MuonsTrkIPindividual.src = selectedLayer1MuonsGlobal.src

selectLayer1Muons = cms.Sequence( selectedLayer1MuonsGlobal
                                 *selectedLayer1MuonsEta21Cumulative * selectedLayer1MuonsEta21Individual
                                 *selectedLayer1MuonsPt15Cumulative * selectedLayer1MuonsPt15Individual 
                                 *selectedLayer1MuonsTrkIsoCumulative * selectedLayer1MuonsTrkIsoIndividual
                                 *selectedLayer1MuonsEcalIsoCumulative * selectedLayer1MuonsEcalIsoIndividual
                                 *selectedLayer1MuonsPionVetoCumulative * selectedLayer1MuonsPionVetoIndividual
                                 *selectedLayer1MuonsTrkCumulative * selectedLayer1MuonsTrkIndividual
                                 *selectedLayer1MuonsTrkIPcumulative * selectedLayer1MuonsTrkIPindividual )

#--------------------------------------------------------------------------------
# produce "summary" collection of pat::Muons with flags added,
# indicating whether or not muons passed individual selection criteria
#--------------------------------------------------------------------------------

cleanLayer1MuonsSel = cms.EDProducer("PATMuonSelProducer",

  leptonSource = cms.InputTag("cleanLayer1Muons"),

  selFlags = cms.PSet(
    tauAnalysisSelMuonGlobal = cms.PSet(
      src = cms.InputTag('selectedLayer1MuonsGlobal')
    ),
    tauAnalysisSelMuonEta21 = cms.PSet(
      src = cms.InputTag('selectedLayer1MuonsEta21Individual')
    ),
    tauAnalysisSelMuonPt15 = cms.PSet(
      src = cms.InputTag('selectedLayer1MuonsPt15Individual')
    ),
    tauAnalysisSelMuonTrkIso = cms.PSet(
      src = cms.InputTag('selectedLayer1MuonsTrkIsoIndividual')
    ),
    tauAnalysisSelMuonEcalIso = cms.PSet(
      src = cms.InputTag('selectedLayer1MuonsEcalIsoIndividual')
    ),
    tauAnalysisSelMuonPionVeto = cms.PSet(
      src = cms.InputTag('selectedLayer1MuonsPionVetoIndividual')
    ),
    tauAnalysisSelMuonTrkIP = cms.PSet(
      src = cms.InputTag('selectedLayer1MuonsTrkIPindividual')
    )
  )
)

produceLayer1SelMuons = cms.Sequence( selectLayer1Muons * cleanLayer1MuonsSel )

#--------------------------------------------------------------------------------
# define additional collections of muon candidates
# with loose track and ECAL isolation applied
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of muon isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)
#--------------------------------------------------------------------------------

selectedLayer1MuonsTrkIsoCumulativeLooseIsolation = copy.deepcopy(selectedLayer1MuonsTrkIsoCumulative)
selectedLayer1MuonsTrkIsoCumulativeLooseIsolation.vetos = cms.vstring("0.01")
selectedLayer1MuonsTrkIsoCumulativeLooseIsolation.numMax = cms.int32(-1)
selectedLayer1MuonsTrkIsoCumulativeLooseIsolation.sumPtMax = cms.double(8.)

selectedLayer1MuonsTrkIsoIndividualLooseIsolation = copy.deepcopy(selectedLayer1MuonsTrkIsoCumulativeLooseIsolation)
selectedLayer1MuonsTrkIsoIndividualLooseIsolation.src = selectedLayer1MuonsGlobal.src

selectedLayer1MuonsEcalIsoCumulativeLooseIsolation = copy.deepcopy(selectedLayer1MuonsEcalIsoCumulative)
selectedLayer1MuonsEcalIsoCumulativeLooseIsolation.src = cms.InputTag("selectedLayer1MuonsTrkIsoCumulativeLooseIsolation")
selectedLayer1MuonsEcalIsoCumulativeLooseIsolation.cut = cms.string('ecalIso < 8.')

selectedLayer1MuonsEcalIsoIndividualLooseIsolation = copy.deepcopy(selectedLayer1MuonsEcalIsoCumulativeLooseIsolation)
selectedLayer1MuonsEcalIsoIndividualLooseIsolation.src = selectedLayer1MuonsGlobal.src

selectedLayer1MuonsPionVetoCumulativeLooseIsolation = copy.deepcopy(selectedLayer1MuonsPionVetoCumulative)
selectedLayer1MuonsPionVetoCumulativeLooseIsolation.src = cms.InputTag("selectedLayer1MuonsEcalIsoCumulativeLooseIsolation")

selectedLayer1MuonsPionVetoIndividualLooseIsolation = copy.deepcopy(selectedLayer1MuonsPionVetoCumulativeLooseIsolation)
selectedLayer1MuonsPionVetoIndividualLooseIsolation.src = selectedLayer1MuonsGlobal.src

selectedLayer1MuonsTrkCumulativeLooseIsolation = copy.deepcopy(selectedLayer1MuonsTrkCumulative)
selectedLayer1MuonsTrkCumulativeLooseIsolation.src = cms.InputTag("selectedLayer1MuonsPionVetoCumulativeLooseIsolation")

selectedLayer1MuonsTrkIndividualLooseIsolation = copy.deepcopy(selectedLayer1MuonsTrkCumulativeLooseIsolation)
selectedLayer1MuonsTrkIndividualLooseIsolation.src = selectedLayer1MuonsGlobal.src

selectedLayer1MuonsTrkIPcumulativeLooseIsolation = copy.deepcopy(selectedLayer1MuonsTrkIPcumulative)
selectedLayer1MuonsTrkIPcumulativeLooseIsolation.src = src = cms.InputTag("selectedLayer1MuonsTrkCumulativeLooseIsolation")

selectedLayer1MuonsTrkIPindividualLooseIsolation = copy.deepcopy(selectedLayer1MuonsTrkIPcumulativeLooseIsolation)
selectedLayer1MuonsTrkIPindividualLooseIsolation.src = selectedLayer1MuonsGlobal.src

selectLayer1MuonsLooseIsolation = cms.Sequence( selectedLayer1MuonsTrkIsoCumulativeLooseIsolation
                                               *selectedLayer1MuonsTrkIsoIndividualLooseIsolation
                                               *selectedLayer1MuonsEcalIsoCumulativeLooseIsolation
                                               *selectedLayer1MuonsEcalIsoIndividualLooseIsolation
                                               *selectedLayer1MuonsPionVetoCumulativeLooseIsolation
                                               *selectedLayer1MuonsPionVetoIndividualLooseIsolation
                                               *selectedLayer1MuonsTrkCumulativeLooseIsolation
                                               *selectedLayer1MuonsTrkIndividualLooseIsolation
                                               *selectedLayer1MuonsTrkIPcumulativeLooseIsolation
                                               *selectedLayer1MuonsTrkIPindividualLooseIsolation )
