import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# skim event sample for tau id. efficiency measurement
#--------------------------------------------------------------------------------

process = cms.Process("skimTauIdEffSample")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')

# import definition of FEVT (RAW + RECO) event content
from Configuration.EventContent.EventContent_cff import *

process.DQMStore = cms.Service("DQMStore")

#--------------------------------------------------------------------------------
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'file:/data2/friis/CMSSW_4_2_X/skims/06-27-MatthewsZTTEvents/crab_0_110627_082505/ZTTCands_merged_v1.root'
        'file:/data1/veelken/tmp/tauIdEffSample_data_SingleMu_Run2011A_PromptReco_v4_2011Jul04v2_RECO_79_1_qIw.root'
    )
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

##isMC = True # use for MC
isMC = False # use for Data
#HLTprocessName = "HLT" # use for 2011 Data
HLTprocessName = "HLT" # use for Summer'11 MC
pfCandidateCollection = "particleFlow" # pile-up removal disabled
##pfCandidateCollection = "pfNoPileUp" # pile-up removal enabled
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system/grid
#
#__isMC = #isMC#
#__HLTprocessName = #HLTprocessName#
#__pfCandidateCollection = #pfCandidateCollection#
#__applyZrecoilCorrection = #applyZrecoilCorrection#
#
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# define GlobalTag to be used for event reconstruction
if isMC:
    process.GlobalTag.globaltag = cms.string('START42_V12::All')
else:
    process.GlobalTag.globaltag = cms.string('GR_R_42_V14::All')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# define skimming criteria
# (in order to be able to produce Tau Ntuple directly from unskimmed Monte Carlo/datasets;
#  HLT single jet trigger passed && either two CaloJets or two PFJets of Pt > 10 GeV within |eta| < 2.5)
process.load('TauAnalysis.TauIdEfficiency.filterDataQuality_cfi')

process.hltMu = cms.EDFilter("EventSelPluginFilter",
    selector = cms.PSet(
        pluginName = cms.string('hltMu'),             
        pluginType = cms.string('TriggerResultEventSelector'),
        src = cms.InputTag('TriggerResults::%s' % HLTprocessName),
        triggerPaths = cms.vstring(
            'HLT_IsoMu17_v5',
            'HLT_IsoMu17_v6',
            'HLT_IsoMu17_v8',
            'HLT_IsoMu17_v9',
            'HLT_IsoMu17_v11'
        )
    )
)

if isMC:
    process.dataQualityFilters.remove(process.hltPhysicsDeclared)
    process.dataQualityFilters.remove(process.dcsstatus)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# require at least one "good quality" (global || tracker || stand-alone) muon
process.goodMuons = cms.EDFilter("MuonSelector",
    src = cms.InputTag('muons'),
    cut = cms.string("(isGlobalMuon | isStandAloneMuon | isTrackerMuon) & abs(eta) < 2.5 & pt > 15.0"),
    filter = cms.bool(True)
)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# produce collections of objects needed as input for PAT-tuple production
# (e.g. rerun reco::Tau identification algorithms with latest tags)
from TauAnalysis.TauIdEfficiency.tools.configurePrePatProduction import configurePrePatProduction

configurePrePatProduction(process, pfCandidateCollection = pfCandidateCollection, addGenInfo = isMC)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# produce PAT-tuple
from TauAnalysis.TauIdEfficiency.tools.configurePatTupleProductionTauIdEffMeasSpecific import *

patTupleConfig = configurePatTupleProductionTauIdEffMeasSpecific(
    process, hltProcess = HLTprocessName, isMC = isMC, applyZrecoilCorrection = False, runSVfit = False)
#--------------------------------------------------------------------------------

#------------------------------------------------------------------------------------------------------------------------
# configure jet energy corrections
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.jec = cms.ESSource("PoolDBESSource",
    DBParameters = cms.PSet(
        messageLevel = cms.untracked.int32(0)
    ),
    timetype = cms.string('runnumber'),
    toGet = cms.VPSet(
        cms.PSet(
            record = cms.string('JetCorrectionsRecord'),
            tag    = cms.string('JetCorrectorParametersCollection_Jec11V2_AK5PF'),
            label  = cms.untracked.string('AK5PF')
        ),
        cms.PSet(
            record = cms.string('JetCorrectionsRecord'),
            tag    = cms.string('JetCorrectorParametersCollection_Jec11V2_AK5Calo'),
            label  = cms.untracked.string('AK5Calo')
        )
    ),
    connect = cms.string('sqlite_fip:TauAnalysis/Configuration/data/Jec11V2.db')
)
process.es_prefer_jec = cms.ESPrefer('PoolDBESSource', 'jec')
#-------------------------------------------------------------------------------------------------------------------------

#------------------------------------------------------------------------------------------------------------------------
# reconfigure PAT-tuple production sequence to run only the modules absolutely needed for skimming
# (in order to reduce processing time)
#process.load('RecoTauTag.Configuration.RecoPFTauTag_cff')
process.load('CommonTools.ParticleFlow.pfNoPileUp_cff')
process.commonSkimSequence = cms.Sequence(
    process.hltMu
   + process.dataQualityFilters
   + process.goodMuons
   + process.pfNoPileUpSequence
   #+ process.PFTau
   + process.patMuons   
)
#
# CV: disable pat::Muon embedding
#
process.patMuons.embedTpfmsMuon = cms.bool(False)
process.patMuons.embedHighLevelSelection = cms.bool(False)
process.patMuons.embedCaloMETMuonCorrs = cms.bool(False)
process.patMuons.embedPFCandidate = cms.bool(False)
process.patMuons.embedStandAloneMuon = cms.bool(False)
process.patMuons.embedTrack = cms.bool(False)
process.patMuons.embedTcMETMuonCorrs = cms.bool(False)
process.patMuons.addTeVRefits = cms.bool(False)
process.patMuons.embedCombinedMuon = cms.bool(False)
process.patMuons.addGenMatch = cms.bool(False)
process.patMuons.addResolutions = cms.bool(False)
process.patMuons.embedGenMatch = cms.bool(False)
process.patMuons.embedPickyMuon = cms.bool(False)
#
# CV: disable all tau id. discriminators for pat::Taus
#     so that reco::PFTau sequence does not need to be rerun
#
patPFTauProducers = [
    "patPFTausHPS",
    "patPFTausHPSpTaNC"
]
for patPFTauProducer in patPFTauProducers:
    patPFTauProducerModule = getattr(process, patPFTauProducer)
    patPFTauProducerModule.tauIDSources = cms.PSet()
    patPFTauProducerModule.addTauID = cms.bool(False)
    patPFTauProducerModule.addGenJetMatch = cms.bool(False)
    patPFTauProducerModule.embedGenJetMatch = cms.bool(False)
    patPFTauProducerModule.embedLeadTrack = cms.bool(False)
    patPFTauProducerModule.embedLeadPFCand = cms.bool(False)
    patPFTauProducerModule.embedSignalPFChargedHadrCands = cms.bool(False)
    patPFTauProducerModule.embedIsolationPFGammaCands = cms.bool(False)
    patPFTauProducerModule.embedSignalPFGammaCands = cms.bool(False)
    patPFTauProducerModule.embedIsolationPFCands = cms.bool(False)
    patPFTauProducerModule.embedSignalPFCands = cms.bool(False)
    patPFTauProducerModule.addEfficiencies = cms.bool(False)
    patPFTauProducerModule.embedSignalTracks = cms.bool(False)
    patPFTauProducerModule.embedIsolationPFNeutralHadrCands = cms.bool(False)
    patPFTauProducerModule.addTauID = cms.bool(False)
    patPFTauProducerModule.addGenMatch = cms.bool(False)
    patPFTauProducerModule.addResolutions = cms.bool(False)
    patPFTauProducerModule.embedIsolationPFChargedHadrCands = cms.bool(False)
    patPFTauProducerModule.embedIsolationTracks = cms.bool(False)
    patPFTauProducerModule.embedSignalPFNeutralHadrCands = cms.bool(False)
    patPFTauProducerModule.embedLeadPFChargedHadrCand = cms.bool(False)
    patPFTauProducerModule.embedGenMatch = cms.bool(False)
    patPFTauProducerModule.embedLeadPFNeutralCand = cms.bool(False)
#
# CV: loosen Pt and eta cuts applied on tau-jet candidates;
#     enable EDFilter functionality
#
patPFTauSelectorsForTauIdEff = [
    "selectedPatPFTausHPSforTauIdEff",
    "selectedPatPFTausHPSpTaNCforTauIdEff"
]
for patPFTauSelectorForTauIdEff in patPFTauSelectorsForTauIdEff:
    patPFTauSelectorModule = getattr(process, patPFTauSelectorForTauIdEff)
    patPFTauSelectorModule.minJetPt = cms.double(15.0)
    patPFTauSelectorModule.maxJetEta = cms.double(2.5)
    patPFTauSelectorModule.applyECALcrackVeto = cms.bool(False)
    patPFTauSelectorModule.pfIsolation.chargedHadronIso.dRisoCone = cms.double(0.4)
    patPFTauSelectorModule.pfIsolation.neutralHadronIso.dRisoCone = cms.double(0.)
    patPFTauSelectorModule.pfIsolation.photonIso.dRisoCone = cms.double(0.4)
    patPFTauSelectorModule.filter = cms.bool(True)

process.selectedPatPFTausHPSforTauIdEff.src = cms.InputTag('patPFTausHPS')
process.selectedPatPFTausHPSpTaNCforTauIdEff.src = cms.InputTag('patPFTausHPSpTaNC')

process.countEventsProcessed = cms.EDAnalyzer("DQMEventCounter",
    meName = cms.string('numEventsProcessed')                                         
)

process.countEventsPassed = cms.EDAnalyzer("DQMEventCounter",
    meName = cms.string('numEventsPassed')                                         
)

process.skimPathHPS = cms.Path(
    process.countEventsProcessed
   + process.commonSkimSequence
   + process.patPFTausHPS + process.selectedPatPFTausHPSforTauIdEff
   + process.countEventsPassed
)

process.skimPathHPSpTaNC = cms.Path(
    process.countEventsProcessed
   + process.commonSkimSequence
   + process.patPFTausHPSpTaNC + process.selectedPatPFTausHPSpTaNCforTauIdEff
   + process.countEventsPassed
)

# add event counter for Mauro's "self baby-sitting" technology
process.totalEventsProcessed = cms.EDProducer("EventCountProducer")
process.eventCounterPath = cms.Path(process.totalEventsProcessed)
#------------------------------------------------------------------------------------------------------------------------

process.load("Configuration.EventContent.EventContent_cff")
process.origFEVTSIMEventContent = copy.deepcopy(process.FEVTSIMEventContent)
process.origFEVTSIMEventContent.outputCommands.extend(
    cms.untracked.vstring(
        'drop *_*_*_%s' % process.name_(),
        'keep edmMergeableCounter_*_*_*'
    )
)    

process.skimOutputModule = cms.OutputModule("PoolOutputModule",                                 
    process.origFEVTSIMEventContent,
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring(
            'skimPathHPS',
            'skimPathHPSpTaNC'                                        
        )
    ),
    fileName = cms.untracked.string("tauIdEffSample_RECO.root")
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.saveZtoMuTau_tauIdEffPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsZtoMuTau_tauIdEff.root')
)

process.p = cms.Path(process.countEventsProcessed)

process.o = cms.EndPath(process.skimOutputModule + process.saveZtoMuTau_tauIdEffPlots)

#--------------------------------------------------------------------------------
#
# CV: do **not** apply HLT trigger conditions to CMSSW_4_1_x/CMSSW_4_2_x MC,
#     to allow simulated events to be weighted by trigger efficiencies measured in Data instead
#
if isMC:
    process.commonSkimSequence.remove(process.hltMu)
#--------------------------------------------------------------------------------  

#--------------------------------------------------------------------------------  
#
# CV: keep Z --> tau+ tau- --> muon + tau-jet events
#     passing Pt and eta cuts on generator level
#    (for studying preselection efficiencies)
#
if isMC:
    process.load('PhysicsTools.JetMCAlgos.TauGenJets_cfi')
    process.load('TauAnalysis.GenSimTools.gen_decaysFromZs_cfi')

    process.genMuonWithinAccFilter = cms.EDFilter("PATCandViewCountFilter",
        src = cms.InputTag('genMuonsFromZtautauDecaysWithinAcceptance'),
        minNumber = cms.uint32(1),
        maxNumber = cms.uint32(1000)
    )

    process.genHadTauWithinAccFilter = cms.EDFilter("PATCandViewCountFilter",
        src = cms.InputTag('genHadronsFromZtautauDecaysWithinAcceptance'),
        minNumber = cms.uint32(1),
        maxNumber = cms.uint32(1000)
    )

    process.genZtoMuTauWithinAccSkimPath = cms.Path(
        process.tauGenJets
       + process.produceGenDecayProductsFromZs
       + process.genMuonWithinAccFilter + process.genHadTauWithinAccFilter
    )

    extSkimPaths = process.skimOutputModule.SelectEvents.SelectEvents.value()
    extSkimPaths.append('genZtoMuTauWithinAccSkimPath')
    process.skimOutputModule.SelectEvents.SelectEvents = cms.vstring(extSkimPaths)
#-------------------------------------------------------------------------------- 

if isMC:
    process.schedule = cms.Schedule(
        process.eventCounterPath,
        process.skimPathHPS,
        process.skimPathHPSpTaNC,
        process.genZtoMuTauWithinAccSkimPath,
        process.o
    )
else:
    process.schedule = cms.Schedule(
        process.eventCounterPath,
        process.skimPathHPS,
        process.skimPathHPSpTaNC,
        process.o
    )

processDumpFile = open('skimTauIdEffSample.dump' , 'w')
print >> processDumpFile, process.dumpPython()
