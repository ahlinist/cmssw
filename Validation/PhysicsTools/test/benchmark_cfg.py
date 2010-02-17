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
        'rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_4_1/RelValTTbar/GEN-SIM-RECO/STARTUP3X_V14-v1/0004/2CF90F4D-87ED-DE11-A3AF-003048D375AA.root',
        'rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_4_1/RelValTTbar/GEN-SIM-RECO/STARTUP3X_V14-v1/0004/3CCCE28D-86ED-DE11-A583-000423D986C4.root',
        'rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_4_1/RelValTTbar/GEN-SIM-RECO/STARTUP3X_V14-v1/0004/3EA206BD-B5ED-DE11-B481-000423D6C8E6.root',
        'rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_4_1/RelValTTbar/GEN-SIM-RECO/STARTUP3X_V14-v1/0004/4CFCC894-86ED-DE11-B3F4-001D09F2447F.root'
    )
)
## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

## Geometry and Detector Conditions (needed for a few patTuple production steps)
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = cms.string('MC_3XY_V12::All')
process.GlobalTag.globaltag = cms.string('STARTUP3X_V11::All')
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

process.load("Validation.PhysicsTools.patObjectBenchmarkGeneric_cfi")

process.L2JetCorrectorAK5Calo = cms.ESSource("L2RelativeCorrectionService",
    tagName = cms.string('Summer09_L2Relative_AK5Calo'),
    label = cms.string('L2RelativeJetCorrectorAK5Calo')
)
process.L3JetCorrectorAK5Calo = cms.ESSource("L3AbsoluteCorrectionService",
    tagName = cms.string('Summer09_L3Absolute_AK5Calo'),
    label = cms.string('L3AbsoluteJetCorrectorAK5Calo')
)
process.L2L3JetCorrectorAK5Calo = cms.ESSource("JetCorrectionServiceChain",
    correctors = cms.vstring('L2RelativeJetCorrectorAK5Calo','L3AbsoluteJetCorrectorAK5Calo'),
    label = cms.string('L2L3JetCorrectorAK5Calo')
)
process.L2CorJetAK5Calo = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("ak5CaloJets"),
    correctors = cms.vstring('L2RelativeJetCorrectorAK5Calo')
)
process.L2L3CorJetAK5Calo = cms.EDProducer("CaloJetCorrectionProducer",
    src = cms.InputTag("ak5CaloJets"),
    correctors = cms.vstring('L2L3JetCorrectorAK5Calo')
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

