import copy

process = cms.Process("TTEffAnalysis")

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(1000)
)

process.load("FWCore/MessageService/MessageLogger_cfi")
process.MessageLogger.destinations = cms.untracked.vstring("cout")
process.MessageLogger.cout = cms.untracked.PSet(
#    threshold = cms.untracked.string("DEBUG")    # pring LogDebugs and above
    threshold = cms.untracked.string("INFO")     # print LogInfos and above
#    threshold = cms.untracked.string("WARNING")  # print LogWarnings and above
    )
process.MessageLogger.debugModules = cms.untracked.vstring("TTEffAnalyzer")

#from ElectroWeakAnalysis.TauTriggerEfficiency.QCDpt80_Summer08_IDEAL_V9_v2_GEN_SIM_RECO_RAW_HLTExtra_PFTauFiltered_HighEfficiency import *
#process.source = source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
# 	"rfio:/castor/cern.ch/user/s/slehti/test.root"
#	"file:test.root"
 	"rfio:/castor/cern.ch/user/s/slehti/TauTriggerEfficiencyMeasurementData/QCDpt30_Summer08_IDEAL_V11_redigi_v1_HLT_RECO_PFTauFiltered/HLTFromDigiRaw_960_RECO_sampleProducer.root"
# 	"rfio:/castor/cern.ch/user/s/slehti/TauTriggerEfficiencyMeasurementData/Ztautau_Summer08_IDEAL_V11_redigi_v2_HLT_RECO_PFTauFiltered_run2/HLTFromDigiRaw_91_RECO_sampleProducer.root"
    )
)

#process.PFTausSelected = cms.EDFilter("PFTauSelector",
#   src = cms.InputTag("pfRecoTauProducerHighEfficiency"),
#   discriminator = cms.InputTag("pfRecoTauDiscriminationHighEfficiency")
#)

process.load("L1Trigger/Configuration/L1Config_cff")
process.load("Configuration/StandardSequences/L1TriggerDefaultMenu_cff")
#process.load("L1TriggerConfig/L1GtConfigProducers/Luminosity/lumi1030/L1Menu_2008MC_2E30_Unprescaled_cff")
process.load("HLTrigger/HLTfilters/hltLevel1GTSeed_cfi")
process.tteffL1GTSeed = copy.deepcopy(process.hltLevel1GTSeed)
process.tteffL1GTSeed.L1TechTriggerSeeding = cms.bool(False)
process.tteffL1GTSeed.L1SeedsLogicalExpression = cms.string("L1_SingleTauJet30")
#process.tteffL1GTSeed.L1SeedsLogicalExpression = cms.string("L1_SingleTauJet80")
#process.tteffL1GTSeed.L1SeedsLogicalExpression = cms.string("L1_SingleTauJet60 OR L1_SingleJet100")
process.tteffL1GTSeed.L1GtReadoutRecordTag = cms.InputTag("hltGtDigis")
process.tteffL1GTSeed.L1GtObjectMapTag = cms.InputTag("hltL1GtObjectMap")
process.tteffL1GTSeed.L1CollectionsTag = cms.InputTag("hltL1extraParticles")
process.tteffL1GTSeed.L1MuonCollectionTag = cms.InputTag("hltL1extraParticles")


process.TTEffAnalysis = cms.EDAnalyzer("TTEffAnalyzer",
        #PFTauCollection         = cms.InputTag("IdentifiedTaus"),
        PFTauCollection         = cms.InputTag("PFTausSelected"),
        #PFTauCollection         = cms.InputTag("pfRecoTauProducerHighEfficiency"),
# Check that Isolation collection below actually matched up with Tau Collection above
        PFTauIsoCollection         = cms.InputTag("pfRecoTauDiscriminationByIsolationHighEfficiency"),

	L1extraTauJetSource	= cms.InputTag("hltL1extraParticles", "Tau", "HLT2"),
	L1extraCentralJetSource	= cms.InputTag("hltL1extraParticles", "Central", "HLT2"),
        L1bitInfoSource         = cms.InputTag("l1CaloSim", "L1BitInfos"),
        L1GtReadoutRecord       = cms.InputTag("hltGtDigis"),
        L1GtObjectMapRecord     = cms.InputTag("hltL1GtObjectMap"),
        HltResults              = cms.InputTag("TriggerResults::HLT"),
        L1TauTriggerSource      = cms.InputTag("tteffL1GTSeed"),
	L1JetMatchingCone	= cms.double(0.5),
        L2AssociationCollection = cms.InputTag("hltL2TauNarrowConeIsolationProducer"),
        L2matchingDeltaR        = cms.double(0.3),
        l25JetSource            = cms.InputTag("hltL25TauConeIsolation"),
        l25PtCutSource          = cms.InputTag("hltL25TauLeadingTrackPtCutSelector"),
        l3IsoSource             = cms.InputTag("hltL3TauIsolationSelector"),
        l25MatchingCone         = cms.double(0.3),
        HLTPFTau                = cms.bool(False),
        MCTauCollection         = cms.InputTag("TauMCProducer:HadronicTauOneAndThreeProng"),

        outputFileName          = cms.string("/tmp/chinhan/qcd_tteffAnalysis.root")
#        outputFileName          = cms.string("/tmp/chinhan/ztt_tteffAnalysis.root")
)

process.runEDAna = cms.Path(
#    process.PFTausSelected*
    process.tteffL1GTSeed
    *process.TTEffAnalysis
) 

#process.o1 = cms.OutputModule("PoolOutputModule",
#    outputCommands = cms.untracked.vstring("keep *"),
#    fileName = cms.untracked.string('tree_test.root')
#)
#process.outpath = cms.EndPath(process.o1)
