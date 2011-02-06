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

# load definition of event selection 
process.load("TauAnalysis.TauIdEfficiency.filterTauIdEffSample_cfi")

# import definition of FEVT (RAW + RECO) event content
from Configuration.EventContent.EventContent_cff import *

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'file:/data1/veelken/CMSSW_3_6_x/skims/Ztautau_1_1_sXK.root'
        #'file:/data1/veelken/CMSSW_3_8_x/skims/AHtoMuTau/selEvents_AHtoMuTau_woBtag_runs145762to148058_RECO.root'
        #'file:/data1/friis/PickMikesEvents/mikes_events_2010b_incomplete.root',
        #'file:/data1/friis/PickMikesEvents/mikes_events_2010b_incomplete001.root'
        'file:/data1/veelken/CMSSW_3_8_x/skims/test/mcDYttPU156bx_GEN_SIM_RECO_1_1_1VV.root'
    )
)

isMC = True # use for MC
##isMC = False # use for Data
##HLTprocessName = "HLT" # use for non-reprocessed MC samples and Data
##HLTprocessName = "REDIGI36X" # use for Spring'10 reprocessed MC
HLTprocessName = "REDIGI38XPU" # use for Fall'10 reprocessed MC with pile-up
##HLTprocessName = "REDIGI38X" # use for Fall'10 reprocessed MC without pile-up
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
# (only relevant for HPS tau reconstruction algorithm)
if isMC:
    process.GlobalTag.globaltag = cms.string('START38_V14::All')
else:   
    process.GlobalTag.globaltag = cms.string('GR_R_38X_V15::All')
#--------------------------------------------------------------------------------    

#--------------------------------------------------------------------------------
# define skimming criteria
# (in order to be able to produce Tau Ntuple directly from unskimmed Monte Carlo/datasets;
#  HLT single jet trigger passed && either two CaloJets or two PFJets of Pt > 10 GeV within |eta| < 2.5)
process.load('TauAnalysis.TauIdEfficiency.filterTauIdEffSample_cfi')

process.hltMu.selector.src = cms.InputTag('TriggerResults::%s' % HLTprocessName)

if isMC:
    process.dataQualityFilters.remove(process.hltPhysicsDeclared)
    process.dataQualityFilters.remove(process.dcsstatus)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#
# produce collections of objects needed as input for PAT-tuple production
# (e.g. rerun reco::Tau identification algorithms with latest tags)
#
from TauAnalysis.TauIdEfficiency.tools.configurePrePatProduction import configurePrePatProduction

configurePrePatProduction(process, pfCandidateCollection = pfCandidateCollection, addGenInfo = isMC)

#process.prePatProductionSequence.remove(process.tautagging)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for configurating PAT-tuple production
from TauAnalysis.TauIdEfficiency.tools.configurePatTupleProductionTauIdEffMeasSpecific import *

patTupleConfig = configurePatTupleProductionTauIdEffMeasSpecific(
    process, hltProcess = HLTprocessName, addGenInfo = isMC, applyZrecoilCorrection = False)
#--------------------------------------------------------------------------------

process.p = cms.Path(
     process.prePatProductionSequence
   + process.patDefaultSequence
   + process.producePatTupleTauIdEffMeasSpecific
   #+ process.printEventContent
   #+ process.printGenParticleList
)    

process.load("Configuration.EventContent.EventContent_cff")
process.origFEVTSIMEventContent = copy.deepcopy(process.FEVTSIMEventContent)
process.origFEVTSIMEventContent.outputCommands.extend(
    cms.untracked.vstring('drop *_*_*_%s' % process.name_())
)    

process.skimOutputModule = cms.OutputModule("PoolOutputModule",                                 
    process.origFEVTSIMEventContent,
    process.tauIdEffSampleEventSelection,
    verbose = cms.untracked.bool(False),
    fileName = cms.untracked.string("tauIdEffSample_RECO.root")
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.o = cms.EndPath(process.skimOutputModule)

# define order in which different paths are run
process.schedule = cms.Schedule(
    process.p,
    process.muonCaloTauSkimPath,
    process.muonPFTauFixedConeSkimPath,
    process.muonPFTauShrinkingConeSkimPath,
    process.muonPFTauHPSskimPath,
    process.muonPFTauHPSpTaNCskimPath,
    process.o
)

# print-out all python configuration parameter information
#print process.dumpPython()
