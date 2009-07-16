import FWCore.ParameterSet.Config as cms
process = cms.Process('runbbAHtoElecTau')

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
#process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_noesprefer_cff')
process.GlobalTag.globaltag = 'IDEAL_V9::All'

process.load("TauAnalysis.Configuration.producePatTuple_cff")
#process.load("PhysicsTools.PatAlgos.patSequences_cff")
#process.load("TauAnalysis.RecoTools.prePatProduction_cff")
#process.load("TauAnalysis.RecoTools.postPatProduction_cff")



process.DQMStore = cms.Service("DQMStore")
process.printEventContent = cms.EDAnalyzer("EventContentAnalyzer")

process.savebbAHtoElecTauPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('test.root')
)

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(-1)    
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'file:/localscratch/patskim.root'
        #'file:/localscratch/ah115bb_tautau.root'
        #'file:/localscratch/elecTauSkim.root'
        'file:/localscratch/ah115bb_tautau.root'
    ),
    skipBadFiles = cms.untracked.bool(True)    
)
from PhysicsTools.PatAlgos.tools.tauTools import * 
switchToPFTauFixedCone(process)

process.load("PhysicsTools.PFCandProducer.pfMET_cfi")
process.patAODExtraReco.replace( process.patJetMETCorrections, process.patJetMETCorrections + process.pfMET )
process.layer1PFMETs = process.layer1METs.clone(
  metSource = cms.InputTag("pfMET"),
  addMuonCorrections = cms.bool(False),
  addTrigMatch = cms.bool(False)
)
process.allLayer1Objects.replace( process.layer1METs, process.layer1METs + process.layer1PFMETs)

from tauAnalysisMaker import TauAnalysisMaker, cuts, options
maker = TauAnalysisMaker(cuts, options, process)

process.p = cms.Path(
  process.producePrePat+
  process.patDefaultSequence+
  process.producePostPat+
  maker.createObjects()+
  process.savebbAHtoElecTauPlots
)