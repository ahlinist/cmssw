import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------  
# produce collections of pat::Electrons passing selection criteria
#--------------------------------------------------------------------------------

# see https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePhysicsCutParser
# on how to use the cut-string parser

# require electron candidate to pass the tight electron id. criteria
selectedLayer1ElectronsTightId = cms.PSet(
    pluginName = cms.string("selectedLayer1ElectronsTightId"),
    pluginType = cms.string("PATElectronSelector"),
    cut = cms.string('(abs(superCluster.eta) < 1.479 & electronID("robust") > 0 & eSuperClusterOverP < 1.05 & eSuperClusterOverP > 0.95) | (abs(superCluster.eta) > 1.479 & electronID("robust") > 0 & eSuperClusterOverP < 1.12 & eSuperClusterOverP > 0.95)'),
    filter = cms.bool(False)
)

# require electron candidate to not be within eta-crack
# between Barrel and Encap ECAL calorimeter
selectedLayer1ElectronsAntiCrackCut = cms.PSet(
    pluginName = cms.string("selectedLayer1ElectronsAntiCrackCut"),
    pluginType = cms.string("PATElectronSelector"),
    cut = cms.string('abs(superCluster.eta) < 1.442 | abs(superCluster.eta) > 1.560'),
    filter = cms.bool(False)
)

# require electron candidate to be within geometric acceptance of electron trigger
selectedLayer1ElectronsEta21 = cms.PSet(
    pluginName = cms.string("selectedLayer1ElectronsEta21"),
    pluginType = cms.string("PATElectronSelector"),
    cut = cms.string('abs(eta) < 2.1'),
    filter = cms.bool(False)
)

# require electron candidate to have transverse momentum above threshold
selectedLayer1ElectronsPt15 = cms.PSet(
    pluginName = cms.string("selectedLayer1ElectronsPt15"),
    pluginType = cms.string("PATElectronSelector"),
    cut = cms.string('pt > 15.'),
    filter = cms.bool(False)
)

# require electron candidate to be isolated
# with respect to tracks (of Pt >~ 0.3 GeV)
selectedLayer1ElectronsTrkIso = cms.PSet(
    pluginName = cms.string("selectedLayer1ElectronsTrkIso"),
    pluginType = cms.string("PATElectronSelector"),
    cut = cms.string('trackIso < 1.'),
    filter = cms.bool(False)
)

# require electron candidate to be isolated
# with respect to energy deposits in ECAL
# (not associated to electron candidate)
selectedLayer1ElectronsEcalIso = cms.PSet(
    pluginName = cms.string("selectedLayer1ElectronsEcalIso"),
    pluginType = cms.string("PATElectronSelector"),
    cut = cms.string('(abs(superCluster.eta) < 1.479 & ecalIso < 1.0) | (abs(superCluster.eta) > 1.479 & ecalIso < 2.5)'),
    filter = cms.bool(False)
)

# require electron candidate to be linked to (GSF) track
selectedLayer1ElectronsTrk = cms.PSet(
    pluginName = cms.string("selectedLayer1ElectronsTrk"),
    pluginType = cms.string("PATElectronSelector"),
    cut = cms.string('gsfTrack.isNonnull'),
    filter = cms.bool(False)
)

# require track of electron candidate to have small transverse impact parameter
# (in order to veto electrons resulting from b-quark decays)
selectedLayer1ElectronsTrkIP = cms.PSet(
    pluginName = cms.string("selectedLayer1ElectronsTrkIP"),
    pluginType = cms.string("PATElectronIpSelector"),
    vertexSource = cms.InputTag("selectedPrimaryVertexPosition"),
    IpMax = cms.double(0.05),
    filter = cms.bool(False)                                               
)

#--------------------------------------------------------------------------------
# produce "summary" collection of pat::Electrons with flags added,
# indicating whether or not electrons passed individual selection criteria
#--------------------------------------------------------------------------------

cleanLayer1ElectronsSel = cms.EDProducer("PATElectronSelProducer",

  leptonSource = cms.InputTag("cleanLayer1Electrons"),

  selFlags = cms.PSet(
    tauAnalysisSelElectronTightIdGlobal = cms.PSet(
      src = cms.InputTag('selectedLayer1ElectronsTightIdIndividual')
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

produceLayer1SelElectrons = cms.Sequence( cleanLayer1ElectronsSel )
