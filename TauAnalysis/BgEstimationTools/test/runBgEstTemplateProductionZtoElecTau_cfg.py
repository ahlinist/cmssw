import FWCore.ParameterSet.Config as cms
import copy
import os

process = cms.Process('bgEstTemplateProductionZtoElecTau')

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
process.GlobalTag.globaltag = cms.string('START311_V2::All')

#--------------------------------------------------------------------------------
# import sequences for PAT-tuple production
process.load("TauAnalysis.Configuration.producePatTuple_cff")
process.load("TauAnalysis.Configuration.producePatTupleZtoElecTauSpecific_cff")

# import sequence for event selection
process.load("TauAnalysis.Configuration.selectZtoElecTau_cff")
process.load("TauAnalysis.RecoTools.filterDataQuality_cfi")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.options = cms.untracked.PSet(
 wantSummary = cms.untracked.bool( True )
)
#process.Timing = cms.Service("Timing")


process.DQMStore = cms.Service("DQMStore")

process.saveZtoElecTauPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('bgEstTemplatesZtoElecTau.root')
)

process.maxEvents = cms.untracked.PSet(
    #input = cms.untracked.int32(-1) 
    input = cms.untracked.int32(1000)    
)

process.source = cms.Source("PoolSource",
                          fileNames = cms.untracked.vstring()
    #skipEvents=cms.untracked.uint32(1421)
    #skipBadFiles = cms.untracked.bool(True) 
)

process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound') )

#--------------------------------------------------------------------------------
#  directories pointing to different input samples stored at the ND Tier 3
#--------------------------------------------------------------------------------


#dir = "/pscratch/ndcms/bestman/storage/cms/store/user/lantonel/DYToTauTau_M-20_CT10_TuneZ2_7TeV-powheg-pythia-tauola/patSkim_413_v3/c9dbc6c7165a3d4f1d089077c1253e67/"
#dir = "/pscratch/ndcms/bestman/storage/cms/store/user/lantonel/DYToTauTau_M-20_CT10_TuneZ2_7TeV-powheg-pythia-tauola/patSkim_416_v1/bb744712881680cf83750ebaff93c394/"
#dir = "/pscratch/ndcms/bestman/storage/cms/store/user/jkolb/TauPlusX/skimElecTau_413_v1/eae887ba91c6c27e2f0c00f8aee7bf0a/"
dir = "/pscratch/ndcms/bestman/storage/cms/store/user/jkolb/DYToTauTau_M-20_CT10_TuneZ2_7TeV-powheg-pythia-tauola/skimElecTau_413_v2/eae887ba91c6c27e2f0c00f8aee7bf0a/"

#--------------------------------------------------------------------------------


for file in os.listdir(dir):
    process.source.fileNames.extend(cms.untracked.vstring('file:' + dir + file))
#process.source.fileNames = cms.untracked.vstring(
#    'file:/data2/veelken/CMSSW_4_1_x/skims/ZtoMuTau/DYtautau_spring11_powhegZ2_1_1_XvY.root'
#)

process.load("TauAnalysis.RecoTools.recoVertexSelectionForElecTau_cff")

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
#switchToPFTauHPS(process)
switchToPFTauHPSpTaNC(process)

process.cleanPatTaus.preselection = cms.string('')


#--------------------------------------------------------------------------------
process.patJetCorrections.remove(process.patJetCorrFactors)
process.patJets.jetCorrFactorsSource = cms.VInputTag()
process.patJets.addJetCorrFactors = cms.bool(False)
#process.producePatTupleZtoElecTauSpecific.remove(process.pfMEtType1and2corrected)
#process.producePatTupleZtoElecTauSpecific.remove(process.patPFtype1METs)

#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#  do not produce momentum-corrected muons
## from TauAnalysis.RecoTools.patLeptonSelection_cff import patMuonSelConfigurator
## setattr(patMuonSelConfigurator, "src", "patMuons" )
## process.selectPatMuons = patMuonSelConfigurator.configure(process = process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
from RecoEgamma.EgammaIsolationAlgos.eleIsoDepositEcalFromHits_cff import *
process.eleIsoDepositEcalFromHits.ExtractorPSet.endcapEcalHits = cms.InputTag("reducedEcalRecHitsEE","")
process.eleIsoDepositEcalFromHits.ExtractorPSet.barrelEcalHits = cms.InputTag("reducedEcalRecHitsEB","")

#--------------------------------------------------------------------------------

from PhysicsTools.PatAlgos.tools.trigTools import switchOnTrigger
switchOnTrigger(process, hltProcess = 'REDIGI311X', outputModule = '')

#--------------------------------------------------------------------------------

#from TauAnalysis.Configuration.tools.switchToData import switchToData
#switchToData(process)


from TauAnalysis.Configuration.tools.changeCut import changeCut

process.load('TauAnalysis.BgEstimationTools.bgEstZtoElecTauZtautauEnrichedSelection_cff')


#process.load('TauAnalysis.BgEstimationTools.bgEstZtoElecTauPhotonPlusJetsEnrichedSelection_cff')
process.load('TauAnalysis.BgEstimationTools.bgEstZtoElecTauWplusJetsEnrichedSelection_cff')
process.load('TauAnalysis.BgEstimationTools.bgEstZtoElecTauQCDenrichedSelection_cff')
process.load('TauAnalysis.BgEstimationTools.bgEstZtoElecTauZeeElectronMisIdEnrichedSelection_cff')


## process.saveEvents = cms.OutputModule("PoolOutputModule",
##     fileName = cms.untracked.string('test_output.root')
## )

from TauAnalysis.RecoTools.patLeptonSelection_cff import patMuonSelConfigurator
setattr(patMuonSelConfigurator, "src", "cleanPatMuons" )
process.selectPatMuons = patMuonSelConfigurator.configure(process = process)


process.p = cms.Path(
    process.producePatTuple
    + process.producePatTupleZtoElecTauSpecific
    + process.selectPrimaryVertexForElecTau
    + process.bgEstZtautauEnrichedAnalysisSequence
    + process.bgEstWplusJetsEnrichedAnalysisSequence
    + process.bgEstZeeElectronMisIdEnrichedAnalysisSequence
    + process.bgEstQCDenrichedAnalysisSequence
    + process.saveZtoElecTauPlots
)

#process.q = cms.Path(process.dataQualityFilters)
#process.end = cms.EndPath(process.saveEvents)

#process.schedule = cms.Schedule(process.q, process.p, process.end)
#process.schedule = cms.Schedule(process.q, process.p)


# print-out all python configuration parameter information
#print process.dumpPython()

