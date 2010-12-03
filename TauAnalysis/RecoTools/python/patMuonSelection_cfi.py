import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.RecoTools.patLeptonPFIsolationSelector_cfi import patMuonPFIsolationSelector

#--------------------------------------------------------------------------------  
# produce collections of pat::Muons passing selection criteria
#
# NOTE: the final cut values are (re)defined in
#
#         TauAnalysis/RecoTools/python/patLeptonSelection_cff.py
#
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

# require muon candidate to pass VBTF selection
# (selection criteria defined by Vector Boson Task Force
#  and documented in CMS AN-10-264)
selectedPatMuonsVbTfId = cms.EDFilter("PATMuonVbTfSelector",
    beamSpotSource = cms.InputTag("offlineBeamSpot")
)                                      

# require muon candidate to be isolated
# with respect to particle-flow candidates
selectedPatMuonsPFRelIso = cms.EDFilter("PATMuonPFIsolationSelector",
    patMuonPFIsolationSelector,
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
    vertexSource = cms.InputTag("selectedPrimaryVertexHighestPtTrackSum"),
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

selectedPatMuonsPFRelIsoLooseIsolation = selectedPatMuonsPFRelIso.clone(
    sumPtMax = cms.double(0.30)
)    

selectedPatMuonsTrkLooseIsolation = selectedPatMuonsTrk.clone()

selectedPatMuonsTrkIPlooseIsolation = selectedPatMuonsTrkIP.clone()
