import FWCore.ParameterSet.Config as cms
import copy

# For MC only

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
####process.MessageLogger.cerr.FwkReport.reportEvery = 100 # print the event number for every 100th event
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


process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring(
       '/store/mc/Spring11/WZtoAnything_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S1_START311_V1G1-v1/0011/106AFBB1-2D50-E011-A529-00E0817918AD.root',
       '/store/mc/Spring11/WZtoAnything_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S1_START311_V1G1-v1/0010/88EA5113-B04F-E011-9704-003048D45F48.root',
       '/store/mc/Spring11/WZtoAnything_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S1_START311_V1G1-v1/0010/76C441E2-AB4F-E011-A71B-003048D45FE2.root',
       '/store/mc/Spring11/WZtoAnything_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S1_START311_V1G1-v1/0010/44C4D221-A94F-E011-AFA2-0025B31E3C00.root',
       '/store/mc/Spring11/WZtoAnything_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S1_START311_V1G1-v1/0010/3E615713-B04F-E011-9643-00E08178C133.root',
       '/store/mc/Spring11/WZtoAnything_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S1_START311_V1G1-v1/0009/E8238961-734F-E011-833D-003048635E34.root',
       '/store/mc/Spring11/WZtoAnything_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S1_START311_V1G1-v1/0009/E61CD016-944F-E011-B97E-003048D47A54.root',
       '/store/mc/Spring11/WZtoAnything_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S1_START311_V1G1-v1/0009/C8BEAF0C-724F-E011-BC10-002481E14E00.root',
       '/store/mc/Spring11/WZtoAnything_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S1_START311_V1G1-v1/0009/C4BA1860-9A4F-E011-BFDA-002481E14FB0.root',
       '/store/mc/Spring11/WZtoAnything_TuneZ2_7TeV-pythia6-tauola/AODSIM/PU_S1_START311_V1G1-v1/0009/B828B0D6-704F-E011-A7FA-00E08178C121.root'
	)
)

process.load("ElectroWeakAnalysis.TauTriggerEfficiency.TTEffPFTau_cff")

process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'START41_V0::All'
process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_COND_31X_GLOBALTAG'
process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
print process.GlobalTag.globaltag


#MET cleaning flag
process.load('CommonTools/RecoAlgos/HBHENoiseFilterResultProducer_cfi')
process.runMETCleaning = cms.Path(process.HBHENoiseFilterResultProducer)

process.TTEffAnalysis = cms.EDAnalyzer("TTEffAnalyzer",
	DoOfflineVariablesOnly  = cms.bool(True), #if true: no trigger info is saved 
        DoMCTauEfficiency       = cms.bool(False), #if true: per MCTau cand; default is false: per offline tau cand
        LoopingOver	        = cms.InputTag("TTEffPFTausSelected"),
        PFTauIsoCollection      = cms.InputTag("TTEffPFTauDiscriminationByIsolation"),
        PFTauMuonRejectionCollection     = cms.InputTag("TTEffPFTauDiscriminationAgainstMuon"),
	PFTauElectronRejectionCollection = cms.InputTag("TTEffPFTauDiscriminationAgainstElectron"),
	PFTauDiscriminators     = cms.VInputTag(),
	Counters		= cms.VInputTag(cms.InputTag("TTEffSkimCounterAllEvents"),
				                cms.InputTag("TTEffSkimCounterSavedEvents")
                                                ),

	MuonSource		= cms.InputTag("muons"),
	MuonPtMin		= cms.double(0.),
	MuonEtaMax		= cms.double(5.),

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


process.TTEffAnalysisHLTCaloTauHPS = process.TTEffAnalysis.clone()                                                                         
process.TTEffAnalysisHLTCaloTauHPS.LoopingOver = cms.InputTag("selectedhpsPFTauProducer")                                                          
process.TTEffAnalysisHLTCaloTauHPS.PFTauIsoCollection = cms.InputTag("selectedhpsPFTauDiscriminationByVLooseIsolation")                            
process.TTEffAnalysisHLTCaloTauHPS.PFTauMuonRejectionCollection = cms.InputTag("selectedhpsPFTauDiscriminationByTightMuonRejection")               
process.TTEffAnalysisHLTCaloTauHPS.PFTauElectronRejectionCollection = cms.InputTag("selectedhpsPFTauDiscriminationByMediumElectronRejection")      
process.TTEffAnalysisHLTCaloTauHPS.PFTauDiscriminators = cms.VInputTag(
    cms.InputTag("selectedhpsPFTauDiscriminationByLooseMuonRejection"),                                                                    
    cms.InputTag("selectedhpsPFTauDiscriminationByTightMuonRejection"),
    cms.InputTag("selectedhpsPFTauDiscriminationByLooseElectronRejection"),
    cms.InputTag("selectedhpsPFTauDiscriminationByMediumElectronRejection"),
    cms.InputTag("selectedhpsPFTauDiscriminationByTightElectronRejection"),
    cms.InputTag("selectedhpsPFTauDiscriminationByTightIsolation"),                                                                                   
    cms.InputTag("selectedhpsPFTauDiscriminationByMediumIsolation"),                                                                                  
    cms.InputTag("selectedhpsPFTauDiscriminationByLooseIsolation"),                                                                                   
    cms.InputTag("selectedhpsPFTauDiscriminationByVLooseIsolation")                                                                                   
)                                                                                                                                             
process.TTEffAnalysisHLTCaloTauHPS.outputFileName = cms.string("tteffAnalysis-hltcalotau-hpspftau.root");                               
process.TTEffAnalysisHLTCaloTauHPS.HLTPFTau = cms.bool(False)

process.TTEffAnalysisHLTPFTauTightHPS = process.TTEffAnalysisHLTCaloTauHPS.clone()
process.TTEffAnalysisHLTPFTauTightHPS.outputFileName = cms.string("tteffAnalysis-hltpftautight-hpspftau.root");                               
process.TTEffAnalysisHLTPFTauTightHPS.l25JetSource = cms.InputTag("hltPFTauTagInfo")                                                          
process.TTEffAnalysisHLTPFTauTightHPS.l25PtCutSource = cms.InputTag("hltPFTausTightCone")                                                     
process.TTEffAnalysisHLTPFTauTightHPS.HLTPFTau = cms.bool(True)

process.TTEffAnalysisHLTPFTauHPS = process.TTEffAnalysisHLTCaloTauHPS.clone()
process.TTEffAnalysisHLTPFTauHPS.outputFileName = cms.string("tteffAnalysis-hltpftau-hpspftau.root");
process.TTEffAnalysisHLTPFTauTightHPS.l25JetSource = cms.InputTag("hltPFTauTagInfo")
process.TTEffAnalysisHLTPFTauTightHPS.l25PtCutSource = cms.InputTag("hltPFTaus")
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

process.runTTEffAna = cms.Path(
        process.hltPhysicsDeclared+
	process.TauMCProducer
) 

process.load("ElectroWeakAnalysis.TauTriggerEfficiency.ZtoMuTauFilter_cfi")

process.TTEffSkimCounterAllEvents   = cms.EDProducer("EventCountProducer")
process.TTEffSkimCounterSavedEvents = cms.EDProducer("EventCountProducer")

process.runTTEffAna += process.TTEffSkimCounterAllEvents
#process.runTTEffAna += process.TTEffPFTau
process.runTTEffAna += process.TTEffHPSPFTau
process.runTTEffAna += process.muTauFilterSequence
process.runTTEffAna += process.TTEffSkimCounterSavedEvents
#process.runTTEffAna += process.TTEffAnalysis
#process.runTTEffAna += process.TTEffAnalysisL1Tau
#process.runTTEffAna += process.TTEffAnalysisL1Cen
#process.runTTEffAna += process.TTEffAnalysisHLTPFTau
#process.runTTEffAna += process.TTEffAnalysisHLTPFTauTight
process.runTTEffAna += process.TTEffAnalysisHLTPFTauTightHPS
#process.runTTEffAna += process.TTEffAnalysisHLTCaloTauHPS
#process.runTTEffAna += process.TTEffAnalysisHLTPFTauHPS

#process.o1 = cms.OutputModule("PoolOutputModule",
#    outputCommands = cms.untracked.vstring("keep *"),
#    fileName = cms.untracked.string('cmssw.root')
#)
#process.outpath = cms.EndPath(process.o1)

process.PFTau_step = cms.Path(process.PFTau)
process.schedule = cms.Schedule(
	process.PFTau_step,
	process.runMETCleaning,
	process.runTTEffAna
#	,process.outpath
)
