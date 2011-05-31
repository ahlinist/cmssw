import FWCore.ParameterSet.Config as cms
import copy

isData = True
#pftau = 0
hltType = "HLT"
#hltType = "REDIGI38X"

process = cms.Process("TTEff")

### Add HLT stuff (it may contain maxEvents and MessageLogger, so it
### should be loaded first before or maxEvents nad MessageLogger would
### be reset)
process.load("ElectroWeakAnalysis.TauTriggerEfficiency.TTEffAnalysisHLT_cfi")
process.prefer("magfield")
process.hltGctDigis.hltMode = cms.bool(False) # Making L1CaloRegions

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
)

process.load("FWCore/MessageService/MessageLogger_cfi")
process.MessageLogger.categories.append("TTEffAnalyzer")
process.MessageLogger.cerr.FwkReport.reportEvery = 100 # print the event number for every 100th event
process.MessageLogger.cerr.TTEffAnalyzer = cms.untracked.PSet(limit = cms.untracked.int32(100)) # print max 100 warnings from TTEffAnalyzer
# process.MessageLogger.debugModules = cms.untracked.vstring("TTEffAnalyzer")
# process.MessageLogger.cerr.threshold = cms.untracked.string("DEBUG")   # pring LogDebugs and above
# process.MessageLogger.cerr.threshold = cms.untracked.string("INFO")    # print LogInfos and above
# process.MessageLogger.cerr.threshold = cms.untracked.string("WARNING") # print LogWarnings and above

process.options = cms.untracked.PSet(
#    wantSummary = cms.untracked.bool(True)
    wantSummary = cms.untracked.bool(False)
)

#Mike needs Calo Geometry
process.load('Configuration/StandardSequences/GeometryPilot2_cff')


if(isData):
    process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring(
	"/store/user/luiggi/MinimumBias/TTEffSkimRun2011A_GoldenPlusESIgnoredJSON/a6b050dc4acb87f74e46528e006dff64/TTEffSkim_1_1_Zd8.root",
	"/store/user/luiggi/MinimumBias/TTEffSkimRun2011A_GoldenPlusESIgnoredJSON/a6b050dc4acb87f74e46528e006dff64/TTEffSkim_2_1_IA6.root",
	"/store/user/luiggi/MinimumBias/TTEffSkimRun2011A_GoldenPlusESIgnoredJSON/a6b050dc4acb87f74e46528e006dff64/TTEffSkim_3_1_I9j.root"
	)
    )
else:
    process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring(
	"file:TTEffSkim.root"
	)
    )

process.load("ElectroWeakAnalysis.TauTriggerEfficiency.TTEffPFTau_cff")

process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
if (isData):
    process.GlobalTag.globaltag = 'GR_H_V16::All'
#    process.GlobalTag.globaltag = 'TESTL1_GR_P::All'
else:
    process.GlobalTag.globaltag = 'START311_V2A::All'
    #process.GlobalTag.globaltag = 'MC_38Y_V14::All'
process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_COND_31X_GLOBALTAG'
process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
print process.GlobalTag.globaltag


#MET cleaning flag
process.load('CommonTools/RecoAlgos/HBHENoiseFilterResultProducer_cfi')
process.runMETCleaning = cms.Path(process.HBHENoiseFilterResultProducer)

process.TTEffAnalysis = cms.EDAnalyzer("TTEffAnalyzer",
        DoMCTauEfficiency       = cms.bool(False), #if true: per MCTau cand; default is false: per offline tau cand
        LoopingOver	        = cms.InputTag("TTEffPFTausSelected"),
        PFTauIsoCollection      = cms.InputTag("TTEffPFTauDiscriminationByIsolation"),
        PFTauMuonRejectionCollection     = cms.InputTag("TTEffPFTauDiscriminationAgainstMuon"),
	PFTauElectronRejectionCollection = cms.InputTag("TTEffPFTauDiscriminationAgainstElectron"),

	HLTMETSource		= cms.InputTag("hltMet"),
	METSource		= cms.InputTag("pfMet"),
	METCleaningSource	= cms.InputTag("HBHENoiseFilterResultProducer", "HBHENoiseFilterResult"),

	PFJetSource		= cms.InputTag("ak5PFJets"),
	MHTJetThreshold		= cms.double(20.),

#	HLTJetSource            = cms.InputTag("hltAntiKT5CaloJets"), #uncorrected
	HLTJetSource            = cms.InputTag("hltAntiKT5L2L3CorrCaloJets"), #corrected
	HLTNJets		= cms.int32(4),

	L1extraTauJetSource			= cms.InputTag("l1extraParticles", "Tau"),
	L1extraCentralJetSource			= cms.InputTag("l1extraParticles", "Central"),

	L1extraMETSource			= cms.InputTag("l1extraParticles", "MET"),
	L1extraMHTSource			= cms.InputTag("l1extraParticles", "MHT"),

		# "Good" vertex finding parameters
        OfflinePVSource      			= cms.InputTag("offlinePrimaryVertices"),                               
	goodPVminNdof 		 		= cms.int32(4),
	goodPVmaxAbsZ 		 		= cms.double(24.0),
	goodPVmaxRho  		 		= cms.double(2.0),
		# To be implemented: cut = cms.string("!isFake && ndof > 4 && abs(z) < 24.0 && position.rho < 2.0"),

        L1CaloRegionSource      		= cms.InputTag("hltGctDigis"), # "", "TTEff"),                               
        L1GtReadoutRecord       		= cms.InputTag("gtDigis",""),
        L1GtObjectMapRecord     		= cms.InputTag("hltL1GtObjectMap","",hltType),
        HltResults              		= cms.InputTag("TriggerResults","",hltType),
        L1TauTriggerSource      		= cms.InputTag("tteffL1GTSeed"),
	L1JetMatchingCone			= cms.double(0.5),
	L1JetMatchingMode			= cms.string("nearestDR"), # "nearestDR", "highestEt"
        L1IsolationThresholds   		= cms.vuint32(1,2,3,4), # count regions with "et() < threshold", these are in GeV
	L2AssociationCollection 		= cms.InputTag("openhltL2TauIsolationProducer"),
        EERecHits               		= cms.untracked.InputTag("ecalRecHit","EcalRecHitsEE"),
        EBRecHits               		= cms.untracked.InputTag("ecalRecHit","EcalRecHitsEB"),
        CaloTowers              		= cms.untracked.InputTag("towerMaker"),
        outerCone               		= cms.untracked.double(0.5),
        innerCone               		= cms.untracked.double(0.15),
        crystalThresholdEB      		= cms.untracked.double(0.15),
        crystalThresholdEE      		= cms.untracked.double(0.45),
        L2matchingDeltaR        		= cms.double(0.2),
        l25JetSource        			= cms.InputTag("openhltL25TauConeIsolation"),
        l25PtCutSource      			= cms.InputTag("hltL25TauLeadingTrackPtCutSelector"),
        l3IsoSource             		= cms.InputTag("hltL3TauIsolationSelector"), #obsolet: L25/L3 merged?
        l25MatchingCone         		= cms.double(0.3),
        MCMatchingCone         			= cms.double(0.2),
        HLTPFTau                		= cms.bool(False),
        MCTauCollection         		= cms.InputTag("TauMCProducer:HadronicTauOneAndThreeProng"),
	GenParticleCollection			= cms.InputTag("genParticles"),
        outputFileName          		= cms.string("tteffAnalysis-hltcalotau-pftau.root")
)

# One way for running multiple TTEffAnalyzers in one job such that
# each analyzer loops over different collection and produces a
# different output file
process.TTEffAnalysisL1Tau = process.TTEffAnalysis.clone()
process.TTEffAnalysisL1Tau.LoopingOver = cms.InputTag("l1extraParticles", "Tau")
process.TTEffAnalysisL1Tau.outputFileName = cms.string("tteffAnalysis-l1tau.root");
process.TTEffAnalysisL1Cen = process.TTEffAnalysis.clone()
process.TTEffAnalysisL1Cen.LoopingOver = cms.InputTag("l1extraParticles", "Central")
process.TTEffAnalysisL1Cen.outputFileName = cms.string("tteffAnalysis-l1cen.root");

process.TTEffAnalysisHLTPFTau = process.TTEffAnalysis.clone()
process.TTEffAnalysisHLTPFTau.outputFileName = cms.string("tteffAnalysis-hltpftau-pftau.root");
process.TTEffAnalysisHLTPFTau.l25JetSource = cms.InputTag("hltPFTauTagInfo")
process.TTEffAnalysisHLTPFTau.l25PtCutSource = cms.InputTag("hltPFTaus")
process.TTEffAnalysisHLTPFTau.HLTPFTau = cms.bool(True)

process.TTEffAnalysisHLTPFTauTight = process.TTEffAnalysis.clone()
process.TTEffAnalysisHLTPFTauTight.outputFileName = cms.string("tteffAnalysis-hltpftautight-pftau.root");
process.TTEffAnalysisHLTPFTauTight.l25JetSource = cms.InputTag("hltPFTauTagInfo")
process.TTEffAnalysisHLTPFTauTight.l25PtCutSource = cms.InputTag("hltPFTausTightCone")
process.TTEffAnalysisHLTPFTauTight.HLTPFTau = cms.bool(True)

process.TTEffAnalysisHLTPFTauTightHPS = process.TTEffAnalysis.clone()
process.TTEffAnalysisHLTPFTauTightHPS.LoopingOver = cms.InputTag("hpsPFTauProducer")
process.TTEffAnalysisHLTPFTauTightHPS.PFTauIsoCollection = cms.InputTag("hpsPFTauDiscriminationByVLooseIsolation")
process.TTEffAnalysisHLTPFTauTightHPS.PFTauMuonRejectionCollection = cms.InputTag("hpsPFTauDiscriminationByTightMuonRejection")
process.TTEffAnalysisHLTPFTauTightHPS.PFTauElectronRejectionCollection = cms.InputTag("hpsPFTauDiscriminationByMediumElectronRejection")
process.TTEffAnalysisHLTPFTauTightHPS.outputFileName = cms.string("tteffAnalysis-hltpftautight-hpspftau.root");
process.TTEffAnalysisHLTPFTauTightHPS.l25JetSource = cms.InputTag("hltPFTauTagInfo")
process.TTEffAnalysisHLTPFTauTightHPS.l25PtCutSource = cms.InputTag("hltPFTausTightCone")
process.TTEffAnalysisHLTPFTauTightHPS.HLTPFTau = cms.bool(True)

process.TauMCProducer = cms.EDProducer("HLTTauMCProducer",
GenParticles  = cms.untracked.InputTag("genParticles"),
       ptMinTau      = cms.untracked.double(3),
       ptMinMuon     = cms.untracked.double(3),
       ptMinElectron = cms.untracked.double(3),
       BosonID       = cms.untracked.vint32(23),
       EtaMax         = cms.untracked.double(2.5)
)

#Physics bit ON
process.load('HLTrigger.special.hltPhysicsDeclared_cfi')
process.hltPhysicsDeclared.L1GtReadoutRecordTag = 'gtDigis'

process.load("ElectroWeakAnalysis.TauTriggerEfficiency.TTEffPFTau_cff")

if(isData):
    process.runTTEffAna = cms.Path(
    )
else:
    process.runTTEffAna = cms.Path(
        process.hltPhysicsDeclared+
	process.TauMCProducer
    ) 
process.runTTEffAna += process.TTEffPFTau
process.runTTEffAna += process.TTEffAnalysis
process.runTTEffAna += process.TTEffAnalysisL1Tau
process.runTTEffAna += process.TTEffAnalysisL1Cen
process.runTTEffAna += process.TTEffAnalysisHLTPFTau
process.runTTEffAna += process.TTEffAnalysisHLTPFTauTight
process.runTTEffAna += process.TTEffAnalysisHLTPFTauTightHPS

#process.o1 = cms.OutputModule("PoolOutputModule",
#    outputCommands = cms.untracked.vstring("keep *"),
#    fileName = cms.untracked.string('cmssw.root')
#)
#process.outpath = cms.EndPath(process.o1)

#process.HLTPFTauSequence+= process.hltPFTausTightCone
process.schedule = cms.Schedule(process.DoHLTJets,
				process.DoHltMuon,
				process.DoHLTPhoton,
				process.DoHLTElectron,
				process.DoHLTTau,
				process.DoHLTMinBiasPixelTracks,
				process.runMETCleaning,
				process.runTTEffAna#,
#				process.outpath
)

if (isData):  # replace all instances of "rawDataCollector" with "source" in In$
    from FWCore.ParameterSet import Mixins
    for module in process.__dict__.itervalues():
        if isinstance(module, Mixins._Parameterizable):
            for parameter in module.__dict__.itervalues():
                if isinstance(parameter, cms.InputTag):
                    if parameter.moduleLabel == 'rawDataCollector':
                        parameter.moduleLabel = 'source'

