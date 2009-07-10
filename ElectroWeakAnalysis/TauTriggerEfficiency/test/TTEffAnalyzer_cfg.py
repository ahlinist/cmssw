import copy

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


#Mike needs Calo Geometry
process.load('Configuration/StandardSequences/GeometryPilot2_cff')


#process.source = cms.Source("PoolSource",
#    fileNames = cms.untracked.vstring(
## 	"rfio:/castor/cern.ch/user/s/slehti/test.root"
#      "file:/tmp/chinhan/skim_996_HLTextra.root"
#    )
#)

from ElectroWeakAnalysis.TauTriggerEfficiency.Ztautau_Summer08_IDEAL_V11_redigi_v2_GEN_SIM_RAW_RECO_Skim_HLT_run5_cfg import *
process.source = source

#process.PFTausSelected = cms.EDFilter("PFTauSelector",
#   src = cms.InputTag("pfRecoTauProducerHighEfficiency"),
#   discriminator = cms.InputTag("pfRecoTauDiscriminationHighEfficiency")
#)

process.load("L1Trigger/Configuration/L1Config_cff")
process.load("Configuration/StandardSequences/L1TriggerDefaultMenu_cff")
#process.load("L1TriggerConfig/L1GtConfigProducers/Luminosity/lumi1031/L1Menu_MC2009_v0_L1T_Scales_20080922_Imp0_Unprescaled_cff")

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



#copying the Discriminator by Isolation
from RecoTauTag.RecoTau.PFRecoTauDiscriminationByIsolationUsingLeadingPion_cfi import *
process.thisPFTauDiscriminationByIsolation = copy.deepcopy(pfRecoTauDiscriminationByIsolationUsingLeadingPion)
process.thisPFTauDiscriminationByIsolation.PFTauProducer = 'PFTausSelected' 
process.thisPFTauDiscriminationByIsolation.MinPtLeadingPion = cms.double(3.0)




process.TTEffAnalysis = cms.EDAnalyzer("TTEffAnalyzer",
        DoMCTauEfficiency       = cms.bool(False), #if true: per MCTau cand; default is false: per offline tau cand
        #PFTauCollection        = cms.InputTag("IdentifiedTaus"),
        PFTauCollection         = cms.InputTag("PFTausSelected"),
        PFTauIsoCollection      = cms.InputTag("thisPFTauDiscriminationByIsolation"),
        # Check that Isolation collection below actually matched up with Tau Collection above
        #PFTauCollection         = cms.InputTag("pfRecoTauProducerHighEfficiency"),
        #PFTauIsoCollection      = cms.InputTag("pfRecoTauDiscriminationByIsolationHighEfficiency"),
	L1extraTauJetSource	= cms.InputTag("hltL1extraParticles", "Tau", "HLT"),
	L1extraCentralJetSource	= cms.InputTag("hltL1extraParticles", "Central", "HLT"),
        L1bitInfoSource         = cms.InputTag("l1CaloSim", "L1BitInfos"),
        L1GtReadoutRecord       = cms.InputTag("hltGtDigis"),
        L1GtObjectMapRecord     = cms.InputTag("hltL1GtObjectMap"),
        HltResults              = cms.InputTag("TriggerResults::HLT"),
        L1TauTriggerSource      = cms.InputTag("tteffL1GTSeed"),
	L1JetMatchingCone	= cms.double(0.5),
        L2AssociationCollection = cms.InputTag("hltL2TauNarrowConeIsolationProducer"),
        EERecHits               = cms.untracked.InputTag("ecalRecHit","EcalRecHitsEE"),
        EBRecHits               = cms.untracked.InputTag("ecalRecHit","EcalRecHitsEB"),
        outerCone               = cms.untracked.double(0.5),
        innerCone               = cms.untracked.double(0.15),
        crystalThresholdEB      = cms.untracked.double(0.15),
        crystalThresholdEE      = cms.untracked.double(0.45),
        L2matchingDeltaR        = cms.double(0.2),
        l25JetSource            = cms.InputTag("hltL25TauConeIsolation"),
        l25PtCutSource          = cms.InputTag("hltL25TauLeadingTrackPtCutSelector"),
        l3IsoSource             = cms.InputTag("hltL3TauIsolationSelector"), #obsolet: L25/L3 merged?
        l25MatchingCone         = cms.double(0.3),
        MCMatchingCone         = cms.double(0.2),
        HLTPFTau                = cms.bool(False),
        MCTauCollection         = cms.InputTag("TauMCProducer:HadronicTauOneAndThreeProng"),
        outputFileName          = cms.string("tteffAnalysis.root")
)

process.runEDAna = cms.Path(
#    process.PFTausSelected*
    process.thisPFTauDiscriminationByIsolation*
    process.tteffL1GTSeed*
    process.TTEffAnalysis
) 

#process.o1 = cms.OutputModule("PoolOutputModule",
#    outputCommands = cms.untracked.vstring("keep *"),
#    fileName = cms.untracked.string('tree_test.root')
#)
#process.outpath = cms.EndPath(process.o1)
