import FWCore.ParameterSet.Config as cms

process = cms.Process("PAT")
process.load("DQMServices.Core.DQM_cfg")

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")

## Options and Output Report
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

## Source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
	'rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_8_0/RelValTTbar/GEN-SIM-RECO/START38_V7-v1/0004/2CEECFE9-1995-DF11-A731-001A92811714.root',
	'rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_8_0/RelValTTbar/GEN-SIM-RECO/START38_V7-v1/0004/38D102EC-1895-DF11-8C87-0018F3D096DA.root',
	'rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_8_0/RelValTTbar/GEN-SIM-RECO/START38_V7-v1/0004/50410AEC-1A95-DF11-B786-0018F3D0960A.root',
	'rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_8_0/RelValTTbar/GEN-SIM-RECO/START38_V7-v1/0004/5AE276E9-1495-DF11-86F1-0018F3D096C2.root'
    )
)
## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

## Geometry and Detector Conditions (needed for a few patTuple production steps)
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = cms.string('MC_3XY_V12::All')
process.GlobalTag.globaltag = cms.string('START38_V7::All')
#process.GlobalTag.globaltag = cms.string('STARTUP3XY_V9::All')
process.load("Configuration.StandardSequences.MagneticField_cff")

## Standard PAT Configuration File
process.load("PhysicsTools.PatAlgos.patSequences_cff")

from PhysicsTools.PatAlgos.tools.jetTools import *
#switchJetCollection(process, 
#                    cms.InputTag('ak5CaloJets'),   
#                    doJTA            = True,            
#                    doBTagging       = True,            
#                    jetCorrLabel     = ('AK5','Calo'),  
#                    doType1MET       = True,            
#                    genJetCollection = cms.InputTag("ak5GenJets")
#                    ) 
process.makePatJets.remove(process.secondaryVertexNegativeTagInfos)
process.makePatJets.remove(process.simpleSecondaryVertexNegativeBJetTags)
process.patJets.addBTagInfo = False

process.load("Validation.PhysicsTools.patObjectBenchmarkGeneric_cfi")

from JetMETCorrections.Configuration.JetCorrectionServices_cff import *

process.L2CorJetAK5Calo = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("ak5CaloJets"),
    correctors = cms.vstring('ak5CaloL2Relative')
)
process.L2L3CorJetAK5Calo = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("ak5CaloJets"),
    correctors = cms.vstring('ak5CaloL2L3')
)

process.p =cms.Path(
    process.patDefaultSequence*
    process.L2CorJetAK5Calo*
    process.L2L3CorJetAK5Calo*
    process.patJetAK5Validation*
    process.patMuonValidation*
    process.patElectronValidation*
    process.patMETValidation*
    process.patCaloMETValidation*
    process.patTauValidation*
    process.patPhotonValidation
)

# Output Module Configuration (expects a path 'p')
#from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent
#process.out = cms.OutputModule("PoolOutputModule",
#                               fileName = cms.untracked.string('PATLayer1_Output.fromAOD_full.root'),
#                               # save only events passing the full path
#                               SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
#                               # save PAT Layer 1 output; you need a '*' to
#                               # unpack the list of commands 'patEventContent'
#                               outputCommands = cms.untracked.vstring('drop *', *patEventContent ) 
#                               )
#process.outpath = cms.EndPath(process.out)

