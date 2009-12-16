import FWCore.ParameterSet.Config as cms

process = cms.Process("TTL1Eff")

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(100)
)

process.load("FWCore/MessageService/MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.destinations = cms.untracked.vstring("cout")
#process.MessageLogger.cout = cms.untracked.PSet(
#    threshold = cms.untracked.string("DEBUG")    # pring LogDebugs and above
#    threshold = cms.untracked.string("INFO")     # print LogInfos and above
#    threshold = cms.untracked.string("WARNING")  # print LogWarnings and above
#    )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#    "rfio:/castor/cern.ch/user/s/slehti/skim_238.root"
        "file:/tmp/mkortela/D2E94D4B-507F-DE11-AB8F-00093D1263F3.root"
    )
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("ntuple.root")
)


process.load("ElectroWeakAnalysis.TauTriggerEfficiency.HLTextra_cff")
process.load("Configuration.StandardSequences.Generator_cff")

process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'MC_31X_V3::All'

process.analysis = cms.EDAnalyzer("L1TauMuonEfficiencyAnalyzer",
#	L1extraTauJetSource	= cms.InputTag("hltL1extraParticles", "Tau", "TTEff"),
#	L1extraCentralJetSource	= cms.InputTag("hltL1extraParticles", "Central", "TTEff"),
	L1extraTauJetSource	= cms.InputTag("hltL1extraParticles", "Tau"),
	L1extraCentralJetSource	= cms.InputTag("hltL1extraParticles", "Central"),
	GenParticleCollection	= cms.InputTag("genParticles"),
        GenJetCollection        = cms.InputTag("iterativeCone5GenJets")
)

process.p = cms.Path(
    process.genParticles *
    process.genJetMET *
    process.HLTBeginSequence *
    process.analysis
)

#process.output = cms.OutputModule("PoolOutputModule",
#    fileName = cms.untracked.string("testoutput.root"),
#)
#process.out_step = cms.EndPath(process.output)

