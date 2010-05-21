import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------  
# produce collections of pat::Electrons passing selection criteria
#--------------------------------------------------------------------------------

# see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePhysicsCutParser
# on how to use the cut-string parser

# require muon candidate to be a global muon
# (track in muon system linked to track in Pixel + SiTracker detectors)
selectedPatMuonsGlobal = cms.EDFilter("PATMuonSelector",
    cut = cms.string('isGlobalMuon()'),
    filter = cms.bool(False)
)

# require muon candidate to be within geometric acceptance of muon trigger
selectedPatMuonsEta21 = cms.EDFilter("PATMuonSelector",
    cut = cms.string('abs(eta) < 2.1'),
    filter = cms.bool(False)
)

# require muon candidate to have transverse momentum above threshold
selectedPatMuonsPt15 = cms.EDFilter("PATMuonSelector",
    cut = cms.string('pt > 15.'),
    filter = cms.bool(False)
)

# require muon candidate to be isolated
# with respect to tracks (of Pt >~ 0.3 GeV)
selectedPatMuonsTrkIso = cms.EDFilter("PATMuonIsoDepositSelector",
    type = cms.string('tracker'),
    vetos = cms.vstring("0.01"),                          
    dRisoCone = cms.double(0.6),
    sumPtMax = cms.double(1.),
    sumPtMethod = cms.string("absolute"),                                 
    filter = cms.bool(False)
)

# require muon candidate to be isolated
# with respect to energy deposits in ECAL
# (not associated to muon candidate)
selectedPatMuonsEcalIso = cms.EDFilter("PATMuonSelector",
    cut = cms.string('userIsolation("pat::EcalIso") < 1.'),
    filter = cms.bool(False)
)

# require muon candidate to pass pion veto
selectedPatMuonsPionVeto = cms.EDFilter("PATMuonAntiPionSelector",
    CaloCompCoefficient = cms.double(0.8),
    SegmCompCoefficient = cms.double(1.2),
    AntiPionCut = cms.double(1.0),
    filter = cms.bool(False)
)

# require muon candidate to be linked to track in silicon strip + pixel detectors
# (all global muons should be linked to tracks in the "inner" tracking detectors;
#  in case the muon is not linked to an "inner" track,
#  the track impact parameter selection will cause processing of the entire event to be skipped !!)
selectedPatMuonsTrk = cms.EDFilter("PATMuonSelector",
    cut = cms.string('innerTrack.isNonnull'),
    filter = cms.bool(False)
)

# require track of muon candidate to have small transverse impact parameter
# (in order to veto muons resulting from b-quark decays)
selectedPatMuonsTrkIP = cms.EDFilter("PATMuonIpSelector",
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

selectedPatMuonsTrkIsoLooseIsolation = copy.deepcopy(selectedPatMuonsTrkIso)
selectedPatMuonsTrkIsoLooseIsolation.vetos = cms.vstring("0.01")
selectedPatMuonsTrkIsoLooseIsolation.numMax = cms.int32(-1)
selectedPatMuonsTrkIsoLooseIsolation.sumPtMax = cms.double(8.)

selectedPatMuonsEcalIsoLooseIsolation = copy.deepcopy(selectedPatMuonsEcalIso)
selectedPatMuonsEcalIsoLooseIsolation.cut = cms.string('userIsolation("pat::EcalIso") < 8.')

selectedPatMuonsPionVetoLooseIsolation = copy.deepcopy(selectedPatMuonsPionVeto)

selectedPatMuonsTrkLooseIsolation = copy.deepcopy(selectedPatMuonsTrk)

selectedPatMuonsTrkIPlooseIsolation = copy.deepcopy(selectedPatMuonsTrkIP)
