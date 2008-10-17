process = cms.Process("TTEffAnalysis")

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(100)
)

process.load("FWCore/MessageService/MessageLogger_cfi")
process.MessageLogger.destinations = cms.untracked.vstring("cout")
process.MessageLogger.cout = cms.untracked.PSet(
#    threshold = cms.untracked.string("DEBUG")    # pring LogDebugs and above
    threshold = cms.untracked.string("INFO")     # print LogInfos and above
#    threshold = cms.untracked.string("WARNING")  # print LogWarnings and above
    )
process.MessageLogger.debugModules = cms.untracked.vstring("TTEffAnalyzer")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#	"root://madhatter.csc.fi/pnfs/csc.fi/data/cms/TauTriggerAnalysisData/RelValQCD_Pt_80_120_CMSSW_2_1_8_STARTUP_V7_v1_TauTriggerAnalysis_cmssw219_10kEvents_v2.root"
 	"rfio:/castor/cern.ch/user/s/slehti/TauTriggerAnalysisData/RelValQCD_Pt_80_120_CMSSW_2_1_10_IDEAL_V9_v2_GEN_SIM_DIGI_RAW_HLTDEBUG_RECO.root"
#	"file:test.root"
    )
)

process.TTEffAnalysis = cms.EDAnalyzer("TTEffAnalyzer",
        PFTauCollection         = cms.InputTag("IdentifiedTaus"),
	L1extraTauJetSource	= cms.InputTag("hltL1extraParticles:Tau"),
        L1bitInfoSource         = cms.InputTag("l1CaloSim", "L1BitInfos"),
	JetMatchingCone		= cms.double(0.5),
        L2AssociationCollection = cms.InputTag("hltL2DoubleTauIsolationProducer","L2TauIsolationInfoAssociator"),
        L2matchingDeltaR        = cms.double(0.3),
        outputFileName          = cms.string("test.root")
)

process.runEDAna = cms.Path(
	process.TTEffAnalysis
) 
