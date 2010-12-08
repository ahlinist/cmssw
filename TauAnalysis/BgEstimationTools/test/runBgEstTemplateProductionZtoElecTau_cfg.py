import FWCore.ParameterSet.Config as cms
import copy
import os

process = cms.Process('prodBgEstTemplateProductionZtoElecTau')

# import of standard configurations for RECOnstruction
# of electrons, muons and tau-jets with non-standard isolation cones
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.cerr.FwkReport.reportEvery = 1
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
#process.MessageLogger.suppressInfo = cms.untracked.vstring()
process.MessageLogger.suppressWarning = cms.untracked.vstring("PATTriggerProducer",)
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START38_V12::All')

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
    input = cms.untracked.int32(-1) 
    #input = cms.untracked.int32(1000)    
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(

    )
    #skipBadFiles = cms.untracked.bool(True) 
)

#dir = "/pscratch/ndcms/bestman/storage/cms/store/user/lantonel/DYToEE_M-20_TuneZ2_7TeV-pythia6/Zee_Skim_3/fc4017496fe1f426291929e387b48eec/"
#dir = "/pscratch/ndcms/bestman/storage/cms/store/user/lantonel/DYToTauTau_M-20_TuneZ2_7TeV-pythia6-tauola/Ztautau_Skim/fc4017496fe1f426291929e387b48eec/"
#dir = "/pscratch/ndcms/bestman/storage/cms/store/user/lantonel/WToTauNu_TuneZ2_7TeV-pythia6-tauola/WToTauNu_Skim/f1545d90fd2c345a2b7b16d6bd838f7e/"
#dir = "/pscratch/ndcms/bestman/storage/cms/store/user/lantonel/WToENu_TuneZ2_7TeV-pythia6/WToENu_Skim/f1545d90fd2c345a2b7b16d6bd838f7e/"
#dir = "/pscratch/ndcms/bestman/storage/cms/store/user/lantonel/QCD_Pt-20to30_BCtoE_TuneZ2_7TeV-pythia6/QCD_Pt-20to30_BCtoE_Skim/f1545d90fd2c345a2b7b16d6bd838f7e/"
#dir = "/pscratch/ndcms/bestman/storage/cms/store/user/lantonel/QCD_Pt-30to80_BCtoE_TuneZ2_7TeV-pythia6/QCD_Pt-30to80_BCtoE_Skim/f1545d90fd2c345a2b7b16d6bd838f7e/"
#dir="/pscratch/ndcms/bestman/storage/cms/store/user/lantonel/QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia6/QCD_Pt-80to170_BCtoE_Skim/f1545d90fd2c345a2b7b16d6bd838f7e/"
#dir="/pscratch/ndcms/bestman/storage/cms/store/user/lantonel/QCD_Pt-20to30_EMEnriched_TuneZ2_7TeV-pythia6/QCD_Pt-20to30_EMEnriched_Skim/f1545d90fd2c345a2b7b16d6bd838f7e/"
#dir="/pscratch/ndcms/bestman/storage/cms/store/user/lantonel/QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia6/QCD_Pt-30to80_EMEnriched_Skim/f1545d90fd2c345a2b7b16d6bd838f7e/"
#dir="/pscratch/ndcms/bestman/storage/cms/store/user/lantonel/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/QCD_Pt-80to170_EMEnriched_Skim/f1545d90fd2c345a2b7b16d6bd838f7e/"
#dir="/pscratch/ndcms/bestman/storage/cms/store/user/lantonel/G_Pt_15to30_TuneZ2_7TeV_pythia6/G_Pt_15to30_Skim/f1545d90fd2c345a2b7b16d6bd838f7e/"
#dir="/pscratch/ndcms/bestman/storage/cms/store/user/lantonel/G_Pt_30to50_TuneZ2_7TeV_pythia6/G_Pt_30to50_Skim/f1545d90fd2c345a2b7b16d6bd838f7e/"


#dir="/pscratch/ndcms/bestman/storage/cms/store/user/jkolb/DYToTauTau_M-20_TuneZ2_7TeV-pythia6-tauola/skimElecTau_385_v1/0ddb8cbebd1c6c83ded328371cc1c32b/"
dir="/pscratch/ndcms/bestman/storage/cms/store/user/jkolb/DYToEE_M-20_TuneZ2_7TeV-pythia6/skimElecTau_385_v1/0ddb8cbebd1c6c83ded328371cc1c32b/"

for file in os.listdir(dir):
    process.source.fileNames.extend(cms.untracked.vstring('file:' + dir + file))
 

# import utility function for managing pat::Jets
from PhysicsTools.PatAlgos.tools.jetTools import *

# uncomment to replace caloJets by pfJets
##switchJetCollection(process, jetCollection = cms.InputTag("ak5PFJets"))
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



process.load('TauAnalysis.BgEstimationTools.bgEstZtoElecTauWplusJetsEnrichedSelection_cff')
process.load('TauAnalysis.BgEstimationTools.bgEstZtoElecTauPhotonPlusJetsEnrichedSelection_cff')
process.load('TauAnalysis.BgEstimationTools.bgEstZtoElecTauZeeEnrichedSelection_cff')
process.load('TauAnalysis.BgEstimationTools.bgEstZtoElecTauZtautauEnrichedSelection_cff')
process.load('TauAnalysis.BgEstimationTools.bgEstZtoElecTauQCDenrichedSelection_cff')

process.producePatTupleAll = cms.Sequence(process.producePatTuple + process.producePatTupleZtoElecTauSpecific)

saveEvents = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('test_output.root')
)


process.p = cms.Path(
    process.producePatTupleAll
    + process.bgEstZtautauEnrichedAnalysisSequence
    + process.bgEstWplusJetsEnrichedAnalysisSequence
    + process.bgEstPhotonPlusJetsEnrichedAnalysisSequence
    + process.bgEstZeeEnrichedAnalysisSequence
    + process.bgEstQCDenrichedAnalysisSequence
    #+ process.saveEvents
    + process.saveZtoElecTauPlots
)

process.q = cms.Path(process.dataQualityFilters)

process.schedule = cms.Schedule(process.q, process.p)



# print-out all python configuration parameter information
#print process.dumpPython()

