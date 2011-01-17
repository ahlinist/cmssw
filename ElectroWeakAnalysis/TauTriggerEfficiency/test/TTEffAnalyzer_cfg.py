import FWCore.ParameterSet.Config as cms
import copy

isData = 0

process = cms.Process("TTEff")

### Add HLT stuff (it may contain maxEvents and MessageLogger, so it
### should be loaded first before or maxEvents nad MessageLogger would
### be reset)
process.load("ElectroWeakAnalysis.TauTriggerEfficiency.TTEffAnalysisHLT_cfi")
process.prefer("magfield")
process.hltGctDigis.hltMode = cms.bool(False) # Making L1CaloRegions

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(1000)
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
    wantSummary = cms.untracked.bool(True)
)

#Mike needs Calo Geometry
process.load('Configuration/StandardSequences/GeometryPilot2_cff')


if(isData):
    process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring(
#	   '/store/user/eluiggi/MinimumBias/MinBiasRun2010A_CSTauSkim371Run2/5b16de9afc6d7bc42a5712a35e6482fe/CSTauSkim_1_1_FFp.root'
	"rfio:/castor/cern.ch/user/s/slehti/TauTriggerEfficiencyMeasurementData/pickevents_Ztautau_MikeOct2010_Mu_Run2010B-v1_RAW_CSTauSkim.root"
#"file:pickevents_Ztautau_MikeOct2010_Mu_Run2010B-v1_RAW_CSTauSkim.root"
	)
    )
else:
    process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring(
	'/store/user/luiggi/TTEffSkim_10_1_MV8.root',
	'/store/user/luiggi/TTEffSkim_15_1_7Zn.root',
	'/store/user/luiggi/TTEffSkim_1_1_zWT.root',
	'/store/user/luiggi/TTEffSkim_20_1_26A.root',
	'/store/user/luiggi/TTEffSkim_23_1_ecx.root',
	'/store/user/luiggi/TTEffSkim_25_1_beC.root',
	'/store/user/luiggi/TTEffSkim_30_1_AB6.root',
	'/store/user/luiggi/TTEffSkim_35_1_10V.root',
	'/store/user/luiggi/TTEffSkim_40_1_cyO.root',
	'/store/user/luiggi/TTEffSkim_45_1_NLG.root',
	'/store/user/luiggi/TTEffSkim_46_1_XpU.root',
	'/store/user/luiggi/TTEffSkim_47_1_fkY.root',
	'/store/user/luiggi/TTEffSkim_5_1_P2H.root'
	)
    )

process.load("ElectroWeakAnalysis.TauTriggerEfficiency.TTEffPFTau_cff")

process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
if (isData):
    process.GlobalTag.globaltag = 'GR_R_38X_V15::All'
else:
    process.GlobalTag.globaltag = 'START38_V12::All'
    #process.GlobalTag.globaltag = 'MC_38Y_V14::All'

print process.GlobalTag.globaltag

#process.prefer("magfield")

<<<<<<< TTEffAnalyzer_cfg.py
#copying the Discriminator by Isolation
#prediscriminator
from RecoTauTag.RecoTau.PFRecoTauDiscriminationByLeadingTrackFinding_cfi import *
process.thisPFTauDiscriminationByLeadingTrackFinding = copy.deepcopy(pfRecoTauDiscriminationByLeadingTrackFinding)
process.thisPFTauDiscriminationByLeadingTrackFinding.PFTauProducer = 'PFTausSelected'

from RecoTauTag.RecoTau.PFRecoTauDiscriminationByIsolationUsingLeadingPion_cfi import *
process.thisPFTauDiscriminationByIsolation = copy.deepcopy(pfRecoTauDiscriminationByIsolationUsingLeadingPion)
process.thisPFTauDiscriminationByIsolation.PFTauProducer = 'PFTausSelected' 
process.thisPFTauDiscriminationByIsolation.MinPtLeadingPion = cms.double(3.0)
process.thisPFTauDiscriminationByIsolation.Prediscriminants.leadPion.Producer = cms.InputTag('thisPFTauDiscriminationByLeadingTrackFinding')

#copying the Discriminator against Muon
from RecoTauTag.RecoTau.PFRecoTauDiscriminationAgainstMuon_cfi import *
process.thisPFTauDiscriminationAgainstMuon = copy.deepcopy(pfRecoTauDiscriminationAgainstMuon)
process.thisPFTauDiscriminationAgainstMuon.PFTauProducer = 'PFTausSelected' 
process.thisPFTauDiscriminationAgainstMuon.Prediscriminants.leadPion.Producer = cms.InputTag('thisPFTauDiscriminationByLeadingTrackFinding')

hltType = "HLT"
if(not isData):
  hltType = "REDIGI38X"
=======
#MET cleaning flag
process.load('CommonTools/RecoAlgos/HBHENoiseFilterResultProducer_cfi')
process.hbheflag = cms.Path(process.HBHENoiseFilterResultProducer)
>>>>>>> 1.96

process.TTEffAnalysis = cms.EDAnalyzer("TTEffAnalyzer",
        DoMCTauEfficiency       = cms.bool(False), #if true: per MCTau cand; default is false: per offline tau cand
        LoopingOver	        = cms.InputTag("PFTausSelected"),
        PFTauIsoCollection      = cms.InputTag("thisPFTauDiscriminationByIsolation"),
        PFTauMuonRejectionCollection      = cms.InputTag("thisPFTauDiscriminationAgainstMuon"),
        # Check that Isolation collection below actually matched up with Tau Collection above
        #PFTauCollection         = cms.InputTag("pfRecoTauProducerHighEfficiency"),
        #PFTauIsoCollection      = cms.InputTag("pfRecoTauDiscriminationByIsolationHighEfficiency"),

	HLTMETSource		= cms.InputTag("hltMet"),
	METSource		= cms.InputTag("pfMet"),
	METCleaningSource	= cms.InputTag("HBHENoiseFilterResultProducer", "HBHENoiseFilterResult"),

	PFJetSource		= cms.InputTag("ak5PFJets"),
	MHTJetThreshold		= cms.double(20.),

	HLTJetSource		= cms.InputTag("iterativeCone5CaloJets"),
	HLTNJets		= cms.int32(4),

	L1extraTauJetSource	= cms.InputTag("l1extraParticles", "Tau"),
	L1extraCentralJetSource	= cms.InputTag("l1extraParticles", "Central"),

	L1extraMETSource	= cms.InputTag("l1extraParticles", "MET"),
	L1extraMHTSource	= cms.InputTag("l1extraParticles", "MHT"),

		# "Good" vertex finding parameters
        OfflinePVSource      = cms.InputTag("offlinePrimaryVertices"),                               
	    goodPVminNdof 		 = cms.int32(4),
		goodPVmaxAbsZ 		 = cms.double(24.0),
		goodPVmaxRho  		 = cms.double(2.0),
		# To be implemented: cut = cms.string("!isFake && ndof > 4 && abs(z) < 24.0 && position.rho < 2.0"),

        L1CaloRegionSource      = cms.InputTag("hltGctDigis"), # "", "TTEff"),                               
        L1GtReadoutRecord       = cms.InputTag("gtDigis",""),
        L1GtObjectMapRecord     = cms.InputTag("hltL1GtObjectMap","",hltType),
        HltResults              = cms.InputTag("TriggerResults","",hltType),
        L1TauTriggerSource      = cms.InputTag("tteffL1GTSeed"),
	L1JetMatchingCone	= cms.double(0.5),
	L1JetMatchingMode	= cms.string("nearestDR"), # "nearestDR", "highestEt"
        L1IsolationThresholds   = cms.vuint32(1,2,3,4), # count regions with "et() < threshold", these are in GeV
	L2AssociationCollection = cms.InputTag("openhltL2TauIsolationProducer"),
        EERecHits               = cms.untracked.InputTag("ecalRecHit","EcalRecHitsEE"),
        EBRecHits               = cms.untracked.InputTag("ecalRecHit","EcalRecHitsEB"),
        CaloTowers              = cms.untracked.InputTag("towerMaker"),
        outerCone               = cms.untracked.double(0.5),
        innerCone               = cms.untracked.double(0.15),
        crystalThresholdEB      = cms.untracked.double(0.15),
        crystalThresholdEE      = cms.untracked.double(0.45),
        L2matchingDeltaR        = cms.double(0.2),
        l25JetSource            = cms.InputTag("hltPFTauTagInfo"),
        l25PtCutSource          = cms.InputTag("hltPFTaus"),
        l3IsoSource             = cms.InputTag("hltL3TauIsolationSelector"), #obsolet: L25/L3 merged?
        l25MatchingCone         = cms.double(0.3),
        MCMatchingCone         = cms.double(0.2),
        HLTPFTau                = cms.bool(True),
        MCTauCollection         = cms.InputTag("TauMCProducer:HadronicTauOneAndThreeProng"),
	GenParticleCollection	= cms.InputTag("genParticles"),
        outputFileName          = cms.string("tteffAnalysis-pftau.root")
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

if(isData):
    process.runEDAna = cms.Path(
#    	process.hltPhysicsDeclared*
#	process.myHLTL25ConeIsolation+
#	process.ic5PFJetTracksAssociatorAtVertex*
	process.ak5PFJetTracksAssociatorAtVertex *
	process.myPFTauTagInfoProducer *
	process.myConeProducer *
    	process.thisPFTauDiscriminationByLeadingPionPtCut *
    	process.PFTausSelected *
    	process.thisPFTauDiscriminationByLeadingTrackFinding *
    	process.thisPFTauDiscriminationByIsolation *
    	process.thisPFTauDiscriminationAgainstMuon *
#    	process.tteffL1GTSeed*
    	process.TTEffAnalysis *
    	process.TTEffAnalysisL1Tau *
    	process.TTEffAnalysisL1Cen
    )
else:
    process.runEDAna = cms.Path(
        process.hltPhysicsDeclared+
#	process.myHLTL25ConeIsolation+
#	process.ic5PFJetTracksAssociatorAtVertex+
	process.ak5PFJetTracksAssociatorAtVertex *
	process.myPFTauTagInfoProducer+
	process.myConeProducer+
####	process.TauMCProducer*
        process.thisPFTauDiscriminationByLeadingPionPtCut *
        process.PFTausSelected *
        process.thisPFTauDiscriminationByLeadingTrackFinding *
        process.thisPFTauDiscriminationByIsolation *
        process.thisPFTauDiscriminationAgainstMuon *
#       process.tteffL1GTSeed*
        process.TTEffAnalysis *
        process.TTEffAnalysisL1Tau *
        process.TTEffAnalysisL1Cen
    ) 

process.o1 = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring("keep *"),
    fileName = cms.untracked.string('cmssw.root')
)
#process.outpath = cms.Path(process.o1)

process.schedule = cms.Schedule(process.DoHLTJetsU,process.DoHLTTau,process.hbheflag,
#                               process.runEDAna,process.outpath)
                                process.runEDAna)

if (isData):  # replace all instances of "rawDataCollector" with "source" in In$
    from FWCore.ParameterSet import Mixins
    for module in process.__dict__.itervalues():
        if isinstance(module, Mixins._Parameterizable):
            for parameter in module.__dict__.itervalues():
                if isinstance(parameter, cms.InputTag):
                    if parameter.moduleLabel == 'rawDataCollector':
                        parameter.moduleLabel = 'source'

