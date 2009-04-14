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
    cut = cms.string('isGlobalMuon()'),
    filter = cms.bool(False)
)

# require muon candidate to be within geometric acceptance of muon trigger
selectedLayer1MuonsEta21 = cms.EDFilter("PATMuonSelector",
    cut = cms.string('abs(eta) < 2.1'),
    filter = cms.bool(False)
)

# require muon candidate to have transverse momentum above threshold
selectedLayer1MuonsPt15 = cms.EDFilter("PATMuonSelector",
    cut = cms.string('pt > 15.'),
    filter = cms.bool(False)
)

selectedLayer1MuonsTrkIso = cms.EDFilter("PATMuonIsoDepositSelector",
    type = cms.string('tracker'),
    #vetos = cms.vstring("0.01", "Threshold(0.9)"),
    vetos = cms.vstring("0.01"),                          
    dRisoCone = cms.double(0.6),
    #numMax = cms.int32(0),
    sumPtMax = cms.double(1.),
    filter = cms.bool(False)
)

# require muon candidate to be isolated
# with respect to energy deposits in ECAL
# (not associated to muon candidate)
selectedLayer1MuonsEcalIso = cms.EDFilter("PATMuonSelector",
    cut = cms.string('ecalIso < 1.'),
    filter = cms.bool(False)
)

# require muon candidate to pass pion veto
selectedLayer1MuonsPionVeto = cms.EDFilter("PATMuonAntiPionSelector",
    CaloCompCoefficient = cms.double(0.8),
    SegmCompCoefficient = cms.double(1.2),
    AntiPionCut = cms.double(1.0),
    filter = cms.bool(False)
)

# require muon candidate to be linked to track in silicon strip + pixel detectors
# (all global muons should be linked to tracks in the "inner" tracking detectors;
#  in case the muon is not linked to an "inner" track,
#  the track impact parameter selection will cause processing of the entire event to be skipped !!)
selectedLayer1MuonsTrk = cms.EDFilter("PATMuonSelector",
    cut = cms.string('innerTrack.isNonnull'),
    filter = cms.bool(False)
)

# require track of muon candidate to have small transverse impact parameter
# (in order to veto muons resulting from b-quark decays)
selectedLayer1MuonsTrkIP = cms.EDFilter("PATMuonIpSelector",
    vertexSource = cms.InputTag("selectedPrimaryVertexPosition"),
    IpMax = cms.double(0.05),
    filter = cms.bool(False)                                               
)

#--------------------------------------------------------------------------------
# define additional collections of muon candidates
# with loose track and ECAL isolation applied
#
# (NOTE: to be used for the purpose of factorizing efficiencies
#        of muon isolation from other event selection criteria,
#        in order to avoid problems with limited Monte Carlo statistics)
#--------------------------------------------------------------------------------

selectedLayer1MuonsTrkIsoLooseIsolation = copy.deepcopy(selectedLayer1MuonsTrkIso)
selectedLayer1MuonsTrkIsoLooseIsolation.vetos = cms.vstring("0.01")
selectedLayer1MuonsTrkIsoLooseIsolation.numMax = cms.int32(-1)
selectedLayer1MuonsTrkIsoLooseIsolation.sumPtMax = cms.double(8.)

selectedLayer1MuonsEcalIsoLooseIsolation = copy.deepcopy(selectedLayer1MuonsEcalIso)
selectedLayer1MuonsEcalIsoLooseIsolation.cut = cms.string('ecalIso < 8.')

selectedLayer1MuonsPionVetoLooseIsolation = copy.deepcopy(selectedLayer1MuonsPionVeto)

selectedLayer1MuonsTrkLooseIsolation = copy.deepcopy(selectedLayer1MuonsTrk)

selectedLayer1MuonsTrkIPlooseIsolation = copy.deepcopy(selectedLayer1MuonsTrkIP)

#--------------------------------------------------------------------------------
# produce "summary" collection of pat::Muons with flags added,
# indicating whether or not muons passed individual selection criteria
#--------------------------------------------------------------------------------

cleanLayer1MuonsSel = cms.EDProducer("PATMuonSelProducer",

  leptonSource = cms.InputTag("cleanLayer1Muons"),

  selFlags = cms.PSet(
    tauAnalysisSelMuonGlobal = cms.PSet(
      src = cms.InputTag('selectedLayer1MuonsGlobalIndividual')
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

produceLayer1SelMuons = cms.Sequence( cleanLayer1MuonsSel )
