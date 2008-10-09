##############################################################
####### Configuration file for the EWK Tau ID analysis #######
#######                                                #######
#######                             September 30, 2008 #######
#######                                Alfredo Gurrola #######
##############################################################

import FWCore.ParameterSet.Config as cms

process = cms.Process("EWKTauIDAnalysis")
process.load("Geometry.CaloEventSetup.CaloTopology_cfi")
process.load("DQMOffline.Trigger.Tau.HLTTauDQMOffline_cff")
process.load("ElectroWeakAnalysis.EWKTau.muForEWKTau.muForEWKTauPatProducer_cff")

####### Define the input data sample and the number of events to process #######

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
        'file:testFEVT.root'
  )
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10))

####### Load the necessary files (startup conditions) #######

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('STARTUP_V4::All')
process.load("Configuration.StandardSequences.MagneticField_cff")

####### Load the files that are required to run the muon isolation producers #######

process.load("PhysicsTools.PatAlgos.recoLayer0.muonIsolation_cff")

####### Load the files that are required to run the PAT Layer 0 cleaning for muons #######

process.load("PhysicsTools.PatAlgos.cleaningLayer0.muonCleaner_cfi")
#process.allLayer0Muons.selection.type = cms.string("GlobalMuons")

####### Define the sequence that runs the PAT Layer 0 modules #######

process.patLayer0Cleaners = cms.Sequence( process.allLayer0Muons )

####### Define the sequence that runs the PAT Layer 0 modules (exclude trig matching) #######

process.patLayer0_withoutTrigMatch = cms.Sequence(
      process.patAODMuonIsolation *
      process.patLayer0Cleaners *
      process.patLayer0MuonIsolation
)

####### Define the module that creates the PAT Layer 1 muons #######

process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
process.load("PhysicsTools.PatAlgos.producersLayer1.muonProducer_cfi")
process.allLayer1Muons.isolation.tracker.deltaR = cms.double(0.8)
process.allLayer1Muons.addTrigMatch = cms.bool(False)
process.allLayer1Muons.addGenMatch = cms.bool(False)

####### Define the selector module that chooses muons based on pt #######

process.selectedLayer1MuonsPt20 = cms.EDProducer("PATMuonSelector",
  src = cms.InputTag("allLayer1MuForEWKTau"),
  cut = cms.string('pt > 20')
)

####### Define the EDFilter that filters events based on a certain number of pat Muons #######

process.minLayer1MuonsPt20 = cms.EDFilter("PATMuonMinFilter",
  src = cms.InputTag("selectedLayer1MuonsPt20"),
  minNumber = cms.uint32(1)
)

####### Define the selector module that chooses muons based on pseudorapidity #######

process.selectedLayer1MuonsEta21 = cms.EDProducer("PATMuonSelector",
  src = cms.InputTag("selectedLayer1MuonsPt20"),
  cut = cms.string('abs(eta) < 2.1')
)

####### Define the EDFilter that filters events based on a certain number of pat Muons #######

process.minLayer1MuonsEta21 = cms.EDFilter("PATMuonMinFilter",
  src = cms.InputTag("selectedLayer1MuonsEta21"),
  minNumber = cms.uint32(1)
)

####### Define the selector module that chooses muons based on impact parameter #######

process.selectedLayer1MuonsIP = cms.EDProducer("PATMuonSelector",
  src = cms.InputTag("selectedLayer1MuonsEta21"),
  cut = cms.string('track.d0 < 0.01')
)

####### Define the EDFilter that filters events based on a certain number of pat Muons #######

process.minLayer1MuonsIP = cms.EDFilter("PATMuonMinFilter",
  src = cms.InputTag("selectedLayer1MuonsIP"),
  minNumber = cms.uint32(1)
)

####### Define the selector module that chooses muons based on PFGamma isolation #######

process.selectedLayer1MuonsPFGammaIso10 = cms.EDProducer("PATMuonPFParticleIsolationSelection",
     src = cms.InputTag("selectedLayer1MuonsIP"),
     PFParticleCode = cms.double(4.0),
     PFCandidateProd = cms.string('particleFlow'),
     OuterIsolationConeSize = cms.double(0.8),
     InnerIsolationConeSize = cms.double(0.0),
     IsolationMinCut = cms.double(0.0),
     IsolationMaxCut = cms.double(10.0),
)

process.selectTrkIsoMuForEWKTau = cms.EDProducer("PATMuonSelector",       
     src = cms.InputTag("selectedLayer1MuonsPFGammaIso10"),   
     cut = cms.string('trackIso < 10.0')   
)

####### Define the b jet rejection module #######

process.selectedLayer1MuonsBJetRejection = cms.EDProducer("PATMuonSVAssociatorSelection",
     src = cms.InputTag("selectTrkIsoMuForEWKTau"),
     TrackProducer = cms.string('generalTracks'),
     VertexProducer = cms.string('offlinePrimaryVertices'),
     VFParameters = cms.PSet(
       finder = cms.string('avr'),
       primcut = cms.double(1.8),
       seccut = cms.double(6.0),
       minweight = cms.double(0.5),
       weightthreshold = cms.double(0.001),
       smoothing = cms.bool(False)
     ),
     VertexMathingDistance = cms.double(0.005),
     VertexMuonMaxDistance = cms.double(1.01),
     MinAssociatedSVCut = cms.double(0.0),
     MaxAssociatedSVCut = cms.double(0.0),
)

process.selectedLayer1MuonsSeperatedFromPATTau = cms.EDProducer("PATMuonPATTauSeperationSelection",
     src = cms.InputTag("allLayer1MuForEWKTau"),
     TauSource = cms.InputTag("allLayer1Taus"),
     DeltaRMinCut = cms.double(0.7),
     DeltaRMaxCut = cms.double(999.0),
)

process.selectedLayer1MuonsSatisfyingMassCut = cms.EDProducer("PATMuonPATTauMassSelection",
     src = cms.InputTag("allLayer1MuForEWKTau"),
     TauSource = cms.InputTag("allLayer1Taus"),
     MassMinCut = cms.double(40.0),
     MassMaxCut = cms.double(100.0),
)

process.selectedLayer1MuonsSatisfyingMetDelPhi = cms.EDProducer("PATMuonPATMetDelPhiSelection",
     src = cms.InputTag("allLayer1MuForEWKTau"),
     MetSource = cms.InputTag("allLayer1Mets"),
     DelPhiMinCut = cms.double(-999.0),
     DelPhiMaxCut = cms.double(999.0),
)

process.patLayer1MuonsEWKTauID = cms.Sequence(process.selectedLayer1MuonsPt20 * process.selectedLayer1MuonsEta21 * process.selectedLayer1MuonsIP * process.selectedLayer1MuonsPFGammaIso10 * process.selectTrkIsoMuForEWKTau * process.selectedLayer1MuonsBJetRejection)
#process.patLayer1MuonsEWKTauID = cms.Sequence(process.selectedLayer1MuonsPt20)
#process.patLayer1MuonsEWKTauID = cms.Sequence(process.selectedLayer1MuonsBJetRejection)
#process.patLayer1MuonsEWKTauID = cms.Sequence(process.selectedLayer1MuonsSeperatedFromPATTau)
#process.patLayer1MuonsEWKTauID = cms.Sequence(process.selectedLayer1MuonsPFGammaIso10)

#process.TFileService = cms.Service("TFileService", fileName = cms.string('histo.root') )

####### Define the path #######

process.p = cms.Path(process.patLayer1MuonsEWKTauID)

####### Output module configuration #######

process.out = cms.OutputModule("PoolOutputModule",
  fileName = cms.untracked.string('PATLayer1_Output.fromAOD_fast.root'),

  ####### save only events passing the full path #######

#  SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
  outputCommands = cms.untracked.vstring('keep *_*_*_*')
)
process.outpath = cms.EndPath(process.out)
