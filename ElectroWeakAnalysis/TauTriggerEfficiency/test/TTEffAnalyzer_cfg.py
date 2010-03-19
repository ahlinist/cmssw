import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process("TTEff")

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(1000)
)

process.load("FWCore/MessageService/MessageLogger_cfi")
process.MessageLogger.destinations = cms.untracked.vstring("cout")
process.MessageLogger.cout = cms.untracked.PSet(
#    threshold = cms.untracked.string("DEBUG")    # pring LogDebugs and above
#    threshold = cms.untracked.string("INFO")     # print LogInfos and above
    threshold = cms.untracked.string("WARNING")  # print LogWarnings and above
    )
process.MessageLogger.debugModules = cms.untracked.vstring("TTEffAnalyzer")
process.MessageLogger.cerr.FwkReport.reportEvery = 1
#process.MessageLogger.cout.FwkReport.reportEvery = 1

#Mike needs Calo Geometry
process.load('Configuration/StandardSequences/GeometryPilot2_cff')


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        "rfio:/castor/cern.ch/user/s/slehti/testData/skim_1054.root"
    )
)


### Add HLT stuff
process.load("ElectroWeakAnalysis.TauTriggerEfficiency.TTEffAnalysisHLT_cfg")
process.prefer("magfield")
process.hltGctDigis.hltMode = cms.bool(False) # Making L1CaloRegions


process.load("RecoTauTag.RecoTau.PFRecoTauDiscriminationByLeadingPionPtCut_cfi")
from RecoTauTag.RecoTau.TauDiscriminatorTools import noPrediscriminants
process.thisPFTauDiscriminationByLeadingPionPtCut = cms.EDFilter("PFRecoTauDiscriminationByLeadingObjectPtCut",

    # Tau collection to discriminate
    PFTauProducer = cms.InputTag('shrinkingConePFTauProducer'),

    # no pre-reqs for this cut
    Prediscriminants = noPrediscriminants,

    # Allow either charged or neutral PFCandidates to meet this requirement
    UseOnlyChargedHadrons = cms.bool(False),

    MinPtLeadingObject = cms.double(3.0)
)

process.PFTausSelected = cms.EDFilter("PFTauSelector",
    src = cms.InputTag("shrinkingConePFTauProducer"),
    discriminators = cms.VPSet(
	cms.PSet( discriminator=cms.InputTag("thisPFTauDiscriminationByLeadingPionPtCut"),selectionCut=cms.double(-0.5))
    )
)


process.load("L1Trigger/Configuration/L1Config_cff")
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = 'MC_31X_V3::All'
process.GlobalTag.globaltag = 'GR09_R_34X_V5::All'



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



process.TTEffAnalysis = cms.EDAnalyzer("TTEffAnalyzer",
        DoMCTauEfficiency       = cms.bool(False), #if true: per MCTau cand; default is false: per offline tau cand
        LoopingOver	        = cms.InputTag("PFTausSelected"),
        PFTauIsoCollection      = cms.InputTag("thisPFTauDiscriminationByIsolation"),
        PFTauMuonRejectionCollection      = cms.InputTag("thisPFTauDiscriminationAgainstMuon"),
        # Check that Isolation collection below actually matched up with Tau Collection above
        #PFTauCollection         = cms.InputTag("pfRecoTauProducerHighEfficiency"),
        #PFTauIsoCollection      = cms.InputTag("pfRecoTauDiscriminationByIsolationHighEfficiency"),

	L1extraTauJetSource	= cms.InputTag("hltL1extraParticles", "Tau", "HLT2"),
	L1extraCentralJetSource	= cms.InputTag("hltL1extraParticles", "Central", "HLT2"),

	L1extraMETSource	= cms.InputTag("hltL1extraParticles", "MET", "HLT2"),
	L1extraMHTSource	= cms.InputTag("hltL1extraParticles", "MHT", "HLT2"),


        L1CaloRegionSource      = cms.InputTag("hltGctDigis"), # "", "HLT2"),                               
        L1GtReadoutRecord       = cms.InputTag("hltGtDigis","","HLT2"),
        L1GtObjectMapRecord     = cms.InputTag("hltL1GtObjectMap","","HLT2"),
        HltResults              = cms.InputTag("TriggerResults"),
        L1TauTriggerSource      = cms.InputTag("tteffL1GTSeed"),
	L1JetMatchingCone	= cms.double(0.5),
        L1IsolationThreshold    = cms.uint32(2), # count regions with "et() < threshold"
	L2AssociationCollection = cms.InputTag("openhltL2TauIsolationProducer"),
        EERecHits               = cms.untracked.InputTag("ecalRecHit","EcalRecHitsEE"),
        EBRecHits               = cms.untracked.InputTag("ecalRecHit","EcalRecHitsEB"),
        CaloTowers              = cms.untracked.InputTag("towerMaker"),
        outerCone               = cms.untracked.double(0.5),
        innerCone               = cms.untracked.double(0.15),
        crystalThresholdEB      = cms.untracked.double(0.15),
        crystalThresholdEE      = cms.untracked.double(0.45),
        L2matchingDeltaR        = cms.double(0.2),
        l25JetSource            = cms.InputTag("openhltL25TauConeIsolation"),
        l25PtCutSource          = cms.InputTag("hltL25TauLeadingTrackPtCutSelector"),
        l3IsoSource             = cms.InputTag("hltL3TauIsolationSelector"), #obsolet: L25/L3 merged?
        l25MatchingCone         = cms.double(0.3),
        MCMatchingCone         = cms.double(0.2),
        HLTPFTau                = cms.bool(False),
        MCTauCollection         = cms.InputTag("TauMCProducer:HadronicTauOneAndThreeProng"),
	GenParticleCollection	= cms.InputTag("genParticles"),
        outputFileName          = cms.string("tteffAnalysis.root")
)

# One way for running multiple TTEffAnalyzers in one job such that
# each analyzer loops over different collection and produces a
# different output file
#process.TTEffAnalysisL1Tau = process.TTEffAnalysis.clone()
#process.TTEffAnalysisL1Tau.LoopingOver = cms.InputTag("hltL1extraParticles", "Tau", "HLT2")
#process.TTEffAnalysisL1Tau.outputFileName = cms.string("tteffAnalysis-l1tau.root");
#process.TTEffAnalysisL1Cen = process.TTEffAnalysis.clone()
#process.TTEffAnalysisL1Cen.LoopingOver = cms.InputTag("hltL1extraParticles", "Central", "HLT2")
#process.TTEffAnalysisL1Cen.outputFileName = cms.string("tteffAnalysis-l1cen.root");

process.TauMCProducer = cms.EDProducer("HLTTauMCProducer",
GenParticles  = cms.untracked.InputTag("genParticles"),
       ptMinTau      = cms.untracked.double(3),
       ptMinMuon     = cms.untracked.double(3),
       ptMinElectron = cms.untracked.double(3),
       BosonID       = cms.untracked.vint32(23),
       EtaMax         = cms.untracked.double(2.5)
)

process.runEDAna = cms.Path(
#    process.TauMCProducer*
    process.thisPFTauDiscriminationByLeadingPionPtCut *
    process.PFTausSelected *
    process.thisPFTauDiscriminationByLeadingTrackFinding *
    process.thisPFTauDiscriminationByIsolation *
    process.thisPFTauDiscriminationAgainstMuon *
#    process.tteffL1GTSeed*
    process.TTEffAnalysis
#    process.TTEffAnalysisL1Tau *
#    process.TTEffAnalysisL1Cen
) 

#process.o1 = cms.OutputModule("PoolOutputModule",
#    outputCommands = cms.untracked.vstring("keep *"),
#    fileName = cms.untracked.string('cmssw.root')
#)
#process.outpath = cms.EndPath(process.o1)
#process.outpath = cms.Path(process.o1)

process.schedule = cms.Schedule(process.DoHLTJetsU,process.DoHLTTau,
#                                ,process.PFTausSelected,
#process.runEDAna,process.outpath)
process.runEDAna)
