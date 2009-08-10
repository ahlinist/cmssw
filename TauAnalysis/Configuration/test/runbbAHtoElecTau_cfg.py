import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process('runbbAHtoElecTau')

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryIdeal_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_noesprefer_cff')
process.GlobalTag.globaltag = 'IDEAL_V9::All'

process.load("TauAnalysis.Configuration.producePatTuple_cff")

from TauAnalysis.Configuration.recoSampleDefinitionsbbAHtoElecTau_cfi import *

#--------------------------------------------------------------------------------

process.DQMStore = cms.Service("DQMStore")

process.savebbAHtoElecTauPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsbbAHtoElecTau.root')
)

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(-1)    
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/localscratch/ah115bb_tautau.root'
    ),
    skipBadFiles = cms.untracked.bool(True)    
)

from PhysicsTools.PatAlgos.tools.tauTools import * 
switchToPFTauFixedCone(process)

#--------------------------------------------------------------------------------
process.load("PhysicsTools.PFCandProducer.pfMET_cfi")
process.patAODExtraReco.replace( process.patJetMETCorrections, process.patJetMETCorrections + process.pfMET )
process.layer1PFMETs = process.layer1METs.clone(
  metSource = cms.InputTag("pfMET"),
  addMuonCorrections = cms.bool(False),
  addTrigMatch = cms.bool(False)
)
process.allLayer1Objects.replace( process.layer1METs, process.layer1METs + process.layer1PFMETs)

from TauAnalysis.Configuration.tools.tauAnalysisMaker import *

from TauAnalysis.Configuration.cutsbbAHtoElecTau_cff import cuts,options

maker = TauAnalysisMaker(cuts,options,process)

process.p = cms.Path( 
  process.producePrePat+
  process.patDefaultSequence+
  process.producePostPat+
  maker.createObjects()+
  process.savebbAHtoElecTauPlots
)                                          

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system
#
#__process.source.fileNames = #inputFileNames#
#__process.maxEvents.input = cms.untracked.int32(#maxEvents#)
#__process.analyzebbAHtoElecTau.filters[0] = copy.deepcopy(#genPhaseSpaceCut#)
#__process.savebbAHtoElecTauPlots.outputFileName = #plotsOutputFileName#
#
#--------------------------------------------------------------------------------

print maker
