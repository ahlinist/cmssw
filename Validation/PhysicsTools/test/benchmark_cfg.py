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
'rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_8_5/RelValTTbar/GEN-SIM-RECO/START38_V12-v1/0040/0840E1E4-B5D2-DF11-8F73-002618943870.root',
'rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_8_5/RelValTTbar/GEN-SIM-RECO/START38_V12-v1/0040/16B7DF72-22D2-DF11-9901-002618943943.root',
'rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_8_5/RelValTTbar/GEN-SIM-RECO/START38_V12-v1/0040/1A487D44-23D2-DF11-9DB4-003048678B0C.root',
'rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_8_5/RelValTTbar/GEN-SIM-RECO/START38_V12-v1/0040/3667674E-EAD1-DF11-8507-002618943933.root'
    )
)
## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

## Geometry and Detector Conditions (needed for a few patTuple production steps)
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = cms.string('MC_38Y_V13::All')
process.GlobalTag.globaltag = cms.string('START38_V14::All')
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

process.patJetCorrFactors.levels = ['L2Relative', 'L3Absolute']

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

