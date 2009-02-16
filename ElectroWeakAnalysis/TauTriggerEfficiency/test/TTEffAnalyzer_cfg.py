process = cms.Process("TTEffAnalysis")

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(10)
)

process.load("FWCore/MessageService/MessageLogger_cfi")
process.MessageLogger.destinations = cms.untracked.vstring("cout")
process.MessageLogger.cout = cms.untracked.PSet(
#    threshold = cms.untracked.string("DEBUG")    # pring LogDebugs and above
    threshold = cms.untracked.string("INFO")     # print LogInfos and above
#    threshold = cms.untracked.string("WARNING")  # print LogWarnings and above
    )
process.MessageLogger.debugModules = cms.untracked.vstring("TTEffAnalyzer")

from ElectroWeakAnalysis.TauTriggerEfficiency.QCD_Mike_PFTauFiltered_HighEfficiency_run2 import *
process.source = source
#process.source = cms.Source("PoolSource",
#    fileNames = cms.untracked.vstring(
## 	"rfio:/castor/cern.ch/user/s/slehti/TauTriggerAnalysisData/RelValQCD_Pt_80_120_CMSSW_2_1_10_IDEAL_V9_v2_GEN_SIM_DIGI_RAW_HLTDEBUG_RECO.root"
#	"file:test.root"
#    )
#)

#process.PFTausSelected = cms.EDFilter("PFTauSelector",
#   src = cms.InputTag("pfRecoTauProducerHighEfficiency"),
#   discriminator = cms.InputTag("pfRecoTauDiscriminationHighEfficiency")
#)

process.TTEffAnalysis = cms.EDAnalyzer("TTEffAnalyzer",
        #PFTauCollection         = cms.InputTag("IdentifiedTaus"),
        PFTauCollection         = cms.InputTag("PFTausSelected"),
        #PFTauCollection         = cms.InputTag("pfRecoTauProducerHighEfficiency"),
        #PFTauIsoCollection         = cms.InputTag("pfRecoTauDiscriminationByIsolationHighEfficiency"), # choose one which matched to pf tau collection you are using above
        PFTauIsoCollection         = cms.InputTag("pfRecoTauDiscriminationHighEfficiency"),
	L1extraTauJetSource	= cms.InputTag("hltL1extraParticles", "Tau", "HLT"),
	L1extraCentralJetSource	= cms.InputTag("hltL1extraParticles", "Central", "HLT"),
        L1bitInfoSource         = cms.InputTag("l1CaloSim", "L1BitInfos"),
	L1JetMatchingCone	= cms.double(0.5),
        L2AssociationCollection = cms.InputTag("hltL2TauIsolationProducer","L2TauIsolationInfoAssociator"),
        L2matchingDeltaR        = cms.double(0.3),
        l25JetSource            = cms.InputTag("hltL25TauPixelTracksConeIsolation"),
        l25PtCutSource          = cms.InputTag("hltL25TauPixelTracksLeadingTrackPtCutSelector"),
        l25IsoSource            = cms.InputTag("hltL25TauPixelTracksIsolationSelector"),
        l25MatchingCone         = cms.double(0.3),
        outputFileName          = cms.string("tteffAnalysis.root")
)

process.runEDAna = cms.Path(
#    process.PFTausSelected*
    process.TTEffAnalysis
) 

#process.o1 = cms.OutputModule("PoolOutputModule",
#    outputCommands = cms.untracked.vstring("keep *"),
#    fileName = cms.untracked.string('tree_test.root')
#)
#process.outpath = cms.EndPath(process.o1)
