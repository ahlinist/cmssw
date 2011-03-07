import FWCore.ParameterSet.Config as cms
import copy
import os

process = cms.Process('prodBgEstTemplateProductionZtoElecTau')

# import of standard configurations for RECOnstruction
# of electrons, muons and tau-jets with non-standard isolation cones
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
#process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.FwkReport.reportEvery = 1
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
#process.MessageLogger.suppressInfo = cms.untracked.vstring()
process.MessageLogger.suppressWarning = cms.untracked.vstring("PATTriggerProducer",)
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START38_V14::All')

#--------------------------------------------------------------------------------
# import sequences for PAT-tuple production
process.load("TauAnalysis.Configuration.producePatTuple_cff")
process.load("TauAnalysis.Configuration.producePatTupleZtoElecTauSpecific_cff")

# import sequence for event selection
process.load("TauAnalysis.Configuration.selectZtoElecTau_cff")
process.load("TauAnalysis.RecoTools.filterDataQuality_cfi")


process.DQMStore = cms.Service("DQMStore")

process.saveZtoElecTauPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('bgEstTemplatesZtoElecTau.root')
)

process.maxEvents = cms.untracked.PSet(
    #input = cms.untracked.int32(-1) 
    input = cms.untracked.int32(1000)    
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(

    )
    #skipBadFiles = cms.untracked.bool(True) 
)

#--------------------------------------------------------------------------------
#  directories pointing to different input samples stored at the ND Tier 3
#--------------------------------------------------------------------------------

dir="/pscratch/ndcms/bestman/storage/cms/store/user/jkolb/DYToTauTau_M-20_TuneZ2_7TeV-pythia6-tauola/skimElecTau_385_v1/0ddb8cbebd1c6c83ded328371cc1c32b/"
#dir="/pscratch/ndcms/bestman/storage/cms/store/user/jkolb/DYToEE_M-20_TuneZ2_7TeV-pythia6/skimElecTau_385_v1/0ddb8cbebd1c6c83ded328371cc1c32b/"


#dir="/pscratch/ndcms/bestman/storage/cms/store/user/jkolb/EG/skimElecTau_385_v3/f555ceec0d7f274eb20b4abf4dbe76f7/"
#dir="/pscratch/ndcms/bestman/storage/cms/store/user/jkolb/Electron/skimElecTau_387_v1/f555ceec0d7f274eb20b4abf4dbe76f7/"

#dir="/pscratch/ndcms/bestman/storage/cms/store/user/jkolb/QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia6/skimElecTau_385_v3/c7ae91f3f823bf7a101c2a0b38387496/"
#dir="/pscratch/ndcms/bestman/storage/cms/store/user/jkolb/WToENu_TuneZ2_7TeV-pythia6/skimElecTau_385_v1/0ddb8cbebd1c6c83ded328371cc1c32b/"
#dir="/pscratch/ndcms/bestman/storage/cms/store/user/jkolb/G_Pt_30to50_TuneZ2_7TeV_pythia6/skimElecTau_385_v2/c7ae91f3f823bf7a101c2a0b38387496/"


############ PILE UP SAMPLES

#dir = "/pscratch/ndcms/bestman/storage/cms/store/user/jkolb/DYToEE_M-20_TuneZ2_7TeV-pythia6/skimElecTau_387_v1/f555ceec0d7f274eb20b4abf4dbe76f7/"
#dir = "/pscratch/ndcms/bestman/storage/cms/store/user/jkolb/DYToTauTau_M-20_TuneZ2_7TeV-pythia6-tauola/skimElecTau_387_PU_v2/f555ceec0d7f274eb20b4abf4dbe76f7/"

#--------------------------------------------------------------------------------


for file in os.listdir(dir):
    process.source.fileNames.extend(cms.untracked.vstring('file:' + dir + file))
 


# import utility function for managing pat::Jets
from PhysicsTools.PatAlgos.tools.jetTools import *
# uncomment to replace caloJets by pfJets
##switchJetCollection(process, jetCollection = cms.InputTag("ak5PFJets"))
#switchJetCollection(process, jetCollection = cms.InputTag("ak5PFJets"),outputModule = "")

##runBTagging(process, cms.InputTag("ak5CaloJets"), 'AOD')
process.patJets.addDiscriminators = False
process.patJets.addTagInfos = False


#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for managing pat::METs
from TauAnalysis.Configuration.tools.metTools import *

# uncomment to add pfMET
# set Boolean swich to true in order to apply type-1 corrections
addPFMet(process, correct = False)

# uncomment to replace caloMET by pfMET in all di-tau objects
process.load("TauAnalysis.CandidateTools.diTauPairProductionAllKinds_cff")
replaceMETforDiTaus(process, cms.InputTag('patMETs'), cms.InputTag('patPFMETs'))
#--------------------------------------------------------------------------------


#--------------------------------------------------------------------------------
#choose type of Taus
from PhysicsTools.PatAlgos.tools.tauTools import *
switchToPFTauHPS(process)
#switchToPFTauHPSpTaNC(process)


#--------------------------------------------------------------------------------
#   trigger switches for the different sets of samples
#--------------------------------------------------------------------------------

process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cfi")
process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerEventProducer_cfi")
## #for running on the fall10 samples
process.patTriggerEvent.processName = cms.string( 'REDIGI38X' )
process.patTrigger.processName = cms.string( 'REDIGI38X' )
## #for running on the pileup samples
## process.patTriggerEvent.processName = cms.string( 'REDIGI38XPU' )
## process.patTrigger.processName = cms.string( 'REDIGI38XPU' )
## #for running on data
#process.patTriggerEvent.processName = cms.string( 'HLT' )
#process.patTrigger.processName = cms.string( 'HLT' )

#--------------------------------------------------------------------------------



process.load('TauAnalysis.BgEstimationTools.bgEstZtoElecTauWplusJetsEnrichedSelection_cff')
process.load('TauAnalysis.BgEstimationTools.bgEstZtoElecTauPhotonPlusJetsEnrichedSelection_cff')
process.load('TauAnalysis.BgEstimationTools.bgEstZtoElecTauZtautauEnrichedSelection_cff')
process.load('TauAnalysis.BgEstimationTools.bgEstZtoElecTauQCDenrichedSelection_cff')
process.load('TauAnalysis.BgEstimationTools.bgEstZtoElecTauZeeJetMisIdEnrichedSelection_cff')
process.load('TauAnalysis.BgEstimationTools.bgEstZtoElecTauZeeElectronMisIdEnrichedSelection_cff')

process.producePatTupleAll = cms.Sequence(process.producePatTuple + process.producePatTupleZtoElecTauSpecific)

## saveEvents = cms.OutputModule("PoolOutputModule",
##     fileName = cms.untracked.string('test_output.root')
## )


process.p = cms.Path(
    process.producePatTupleAll
    + process.bgEstZtautauEnrichedAnalysisSequence
    + process.bgEstWplusJetsEnrichedAnalysisSequence
    + process.bgEstPhotonPlusJetsEnrichedAnalysisSequence
    + process.bgEstZeeElectronMisIdEnrichedAnalysisSequence
    + process.bgEstZeeJetMisIdEnrichedAnalysisSequence
    + process.bgEstQCDenrichedAnalysisSequence
    #+ process.saveEvents
    + process.saveZtoElecTauPlots
)

process.q = cms.Path(process.dataQualityFilters)
#process.end = cms.EndPath(process.saveEvents)

#process.schedule = cms.Schedule(process.q, process.p, process.end)
process.schedule = cms.Schedule(process.q, process.p)


# print-out all python configuration parameter information
#print process.dumpPython()

