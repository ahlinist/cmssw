import FWCore.ParameterSet.Config as cms

process = cms.Process("TESTBTAG")

process.options = cms.untracked.PSet(
   wantSummary = cms.untracked.bool( True )
)
process.load("FWCore.MessageService.MessageLogger_cfi")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'rfio:/castor/cern.ch/user/c/cerati/elecMuSkim.root'
        'file:/data/cerati/elecMuSkim-TTbar-ideal.root'
    )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'IDEAL_V12::All'

process.load("TauAnalysis.Configuration.producePatLayer1_cff")

#produce calo jets for tau analysis
process.load("TauAnalysis.RecoTools.jetPatProducer_cff")

#add also PF jets
from TauAnalysis.RecoTools.jetToolsForTauAnalysis import *
process.load("PhysicsTools.PatAlgos.cleaningLayer0.pfJetCleaner_cfi")
process.allLayer0PFJets.removeOverlaps.electrons.collection = cms.InputTag("allLayer0ElectronsForTauAnalyses")
addJetCollection(process,'iterativeCone5PFJets', 'PF', [0,1], runCleaner="PFJet",doJTA=False,doBTagging=True,jetCorrLabel=None,doType1MET=True,doL1Counters=False)
#switchJetCollection(process,'iterativeCone5PFJets', [0,1], runCleaner="PFJet",doJTA=False,doBTagging=True,jetCorrLabel=None,doType1MET=True)

process.load("TauAnalysis.CandidateTools.elecMuPairProduction_cff")

process.testBTag = cms.EDAnalyzer('TestBTag',
  jetTag = cms.InputTag('allLayer1Jets'),
  genTag = cms.InputTag('genParticles'),
  #algo  = cms.string('simpleSecondaryVertexBJetTags'),
  algo  = cms.string('trackCountingHighEffBJetTags'),
  corrStep  = cms.string(''),
  #discriminator = cms.double(0.5),
  discriminator = cms.double(2.),
  outputFile = cms.string('testbtag_TTbar_test.root'),
  minPt = cms.double(30),
)

process.testBTagPF = cms.EDAnalyzer('TestBTag',
  jetTag = cms.InputTag('allLayer1JetsPF'),
  genTag = cms.InputTag('genParticles'),
  #algo  = cms.string('simpleSecondaryVertexBJetTags'),
  algo  = cms.string('trackCountingHighEffBJetTags'),
  corrStep  = cms.string(''),
  #discriminator = cms.double(0.5),
  discriminator = cms.double(2.),
  outputFile = cms.string('testbtag_TTbar_PF.root'),
  minPt = cms.double(30),
)

process.p = cms.Path( process.producePatLayer1ForTauAnalyses
                     *(process.patLayer0JetsForTauAnalyses + process.patLayer1JetsForTauAnalyses)
                     *process.allElecMuPairs*process.selectElecMuPairs
                     *process.testBTag
                     *process.testBTagPF
                     )
