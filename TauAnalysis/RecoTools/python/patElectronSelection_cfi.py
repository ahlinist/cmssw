import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------  
# produce collections of pat::Electrons passing selection criteria
#
# NOTE: the final cut values are (re)defined in
#
#         TauAnalysis/RecoTools/python/patLeptonSelection_cff.py
#
#--------------------------------------------------------------------------------

# see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePhysicsCutParser
# on how to use the cut-string parser

# require electron candidate to pass the eidRobustTight electron id. criteria
selectedPatElectronsTightId = cms.EDFilter("PATElectronSelector",
    cut = cms.string('(abs(superCluster.eta) < 1.479 & electronID("eidRobustTight") > 0 & eSuperClusterOverP < 1.05 & eSuperClusterOverP > 0.95) | (abs(superCluster.eta) > 1.479 & electronID("eidRobustTight") > 0 & eSuperClusterOverP < 1.12 & eSuperClusterOverP > 0.95)'),
    filter = cms.bool(False)
)

# require electron candidate to pass the eidRobustLoose electron id. criteria
selectedPatElectronsLooseId = cms.EDFilter("PATElectronSelector",
    cut = cms.string('(abs(superCluster.eta) < 1.479 & electronID("eidRobustLoose") > 0 & eSuperClusterOverP < 1.4 & eSuperClusterOverP > 0.8) | (abs(superCluster.eta) > 1.479 & electronID("eidRobustLoose") > 0 & eSuperClusterOverP < 1.6 & eSuperClusterOverP > 0.8)'),
    filter = cms.bool(False)
)

# require electron candidate to not be within eta-crack
# between Barrel and Encap ECAL calorimeter
selectedPatElectronsAntiCrackCut = cms.EDFilter("PATElectronSelector",
    cut = cms.string('abs(superCluster.eta) < 1.442 | abs(superCluster.eta) > 1.560'),
    filter = cms.bool(False)
)

# require electron candidate to be within geometric acceptance of electron trigger
selectedPatElectronsEta21 = cms.EDFilter("PATElectronSelector",
    cut = cms.string('abs(eta) < 2.1'),
    filter = cms.bool(False)
)

# require electron candidate to have transverse momentum above threshold
selectedPatElectronsPt15 = cms.EDFilter("PATElectronSelector",
    cut = cms.string('pt > 15.'),
    filter = cms.bool(False)
)

# require electron candidate to be isolated
# with respect to tracks (of Pt >~ 0.3 GeV)
selectedPatElectronsTrkIso = cms.EDFilter("PATElectronSelector",
    cut = cms.string('userIsolation("pat::TrackIso") < 1.'),
    filter = cms.bool(False)
)                                    

# require electron candidate to be isolated
# with respect to energy deposits in ECAL
# (not associated to electron candidate)
selectedPatElectronsEcalIso = cms.EDFilter("PATElectronSelector",
    cut = cms.string('(abs(superCluster.eta) < 1.479 & userIsolation("pat::EcalIso") < 2.5) | (abs(superCluster.eta) > 1.479 & userIsolation("pat::EcalIso") < 3.5)'),
    filter = cms.bool(False)
)

# require electron candidate to be linked to (GSF) track
selectedPatElectronsTrk = cms.EDFilter("PATElectronSelector",
    cut = cms.string('gsfTrack.isNonnull'),
    filter = cms.bool(False)
)

# require track of electron candidate to have small transverse impact parameter
# (in order to veto electrons resulting from b-quark decays)
selectedPatElectronsTrkIP = cms.EDFilter("PATElectronIpSelector",
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

selectedPatElectronsTrkIsoLooseIsolation = copy.deepcopy(selectedPatElectronsTrkIso)
selectedPatElectronsTrkIsoLooseIsolation.cut = cms.string('userIsolation("pat::TrackIso") < 8.')

selectedPatElectronsEcalIsoLooseIsolation = copy.deepcopy(selectedPatElectronsEcalIso)
selectedPatElectronsEcalIsoLooseIsolation.cut = cms.string('userIsolation("pat::EcalIso") < 8.')

selectedPatElectronsTrkLooseIsolation = copy.deepcopy(selectedPatElectronsTrk)

selectedPatElectronsTrkIPlooseIsolation = copy.deepcopy(selectedPatElectronsTrkIP)
