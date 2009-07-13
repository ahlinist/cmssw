import FWCore.ParameterSet.Config as cms

process = cms.Process("PAT")
process.load("DQMServices.Core.DQM_cfg")

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('PATSummaryTables')
#process.MessageLogger.cerr.INFO = cms.untracked.PSet(
#    default          = cms.untracked.PSet( limit = cms.untracked.int32(0)  ),
#    PATSummaryTables = cms.untracked.PSet( limit = cms.untracked.int32(-1) )
#)
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# source
process.source = cms.Source("PoolSource", 
    fileNames = cms.untracked.vstring(  
        '/store/relval/CMSSW_3_1_1/RelValTTbar/GEN-SIM-RECO/STARTUP31X_V1-v2/0002/1221BA1C-EB6B-DE11-9C2C-000423D94700.root'
#      '/store/relval/CMSSW_3_1_0_pre6/RelValTTbar/GEN-SIM-RECO/IDEAL_31X_v1/0002/50D4BADB-FA32-DE11-BA01-000423D98DC4.root'
#         'file:step2_RAW2DIGI_RECO.root'
#          'file:/uscms/home/petar/sw/CMSSW_3_1_0_pre9/src/step2_RAW2DIGI_RECO.root'
#          'file:/uscms/home/petar/sw2/CMSSW_3_1_0_pre9/src/step2_RAW2DIGI_RECO.root'
#           'file:/uscms_data/d2/malik/FORCHARLES/345_TTbar_Tauola_step2_RAW2DIGI_RECO.root'
#           'file:/uscms_data/d2/malik/FORCHARLES/432_TTbar_Tauola_step2_RAW2DIGI_RECO.root'
#           'file:/uscms_data/d2/malik/FORCHARLES/64_432_TTbar_Tauola_step2_RAW2DIGI_RECO.root'
#            'file:/uscms_data/d2/malik/FORCHARLES/345_on_SLC4_recominbias_RAW2DIGI_RECO.root'
#            'file:/uscms_data/d2/malik/FORCHARLES/345_on_SLC5_recominbias_RAW2DIGI_RECO.root'
#            'file:/uscms_data/d2/malik/FORCHARLES/345_on_SLC4_recottbar_RAW2DIGI_RECO.root'
#            'file:/uscms_data/d2/malik/FORCHARLES/345_on_SLC5_recottbar_RAW2DIGI_RECO.root
#            'file:/uscms_data/d2/malik/FORCHARLES/345_on_SLC4_recoqcd_RAW2DIGI_RECO.root'
#            'file:/uscms_data/d2/malik/FORCHARLES/345_on_SLC5_recoqcd_RAW2DIGI_RECO.root' 
    )
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = cms.string('IDEAL_31X::All')
process.GlobalTag.globaltag = cms.string('STARTUP31X_V1::All')
process.load("Configuration.StandardSequences.MagneticField_cff")

# PAT Layer 0+1
process.load("PhysicsTools.PatAlgos.patSequences_cff")
process.content = cms.EDAnalyzer("EventContentAnalyzer")
process.dump = cms.EDAnalyzer("EventContentAnalyzer")
# replacements currently needed to make the jets work
process.allLayer1Jets.addDiscriminators    = False
process.allLayer1Jets.discriminatorSources = []

from PhysicsTools.PatAlgos.tools.jetTools import *
##Added for corrected jets
#process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer08_cff")   
process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer08Redigi_cff")
process.prefer("L2L3JetCorrectorIC5Calo")
process.prefer("L2L3JetCorrectorSC5Calo")
process.prefer("L2L3JetCorrectorSC7Calo")

# PAT Layer 0+1
process.load("Validation.PhysicsTools.patObjectBenchmarkGeneric_cfi")
process.load("Validation.RecoParticleFlow.tauBenchmarkGeneric_cff")
#process.load("Validation.PhysicsTools.tauBenchmarkGeneric_cff")

process.p =cms.Path(
    process.patDefaultSequence*  
#    process.content*
    process.L2L3CorJetIC5Calo*
    process.L2L3CorJetSC5Calo*
    process.L2L3CorJetSC7Calo*
    process.patJetValidation*
    process.patJetIC5Validation*

#    process.patJetSC5CorrValidation*
#    process.patJetSC7CorrValidation*

    process.patMuonValidation*
    process.patElectronValidation*
    process.patMETValidation*
    process.patCaloMETValidation*
    process.patTauValidation*
#    process.pfTauBenchmarkGeneric* 
#    process.tauBenchmarkGeneric*
    process.patPhotonValidation

)

# Output module configuration
#from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent
#process.out = cms.OutputModule("PoolOutputModule",
#    fileName = cms.untracked.string('PATLayer1_Output.fromAOD_full.root'),
## save only events passing the full path
#    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
## save PAT Layer 1 output
#    outputCommands = cms.untracked.vstring('drop *', *patEventContent ) # you need a '*' to unpack the list of commands 'patEventContent'
#)
#process.outpath = cms.EndPath(process.out)

