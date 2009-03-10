import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")
# keep the logging output to a nice level ###
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.debugModules = cms.untracked.vstring('')
#process.MessageLogger.cerr.threshold = 'DEBUG'
#process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.threshold = 'WARNING'
process.MessageLogger.categories.append('Analysis')
process.MessageLogger.cerr.DEBUG = cms.untracked.PSet(
    default = cms.untracked.PSet( limit = cms.untracked.int32(0)),
    Analysis = cms.untracked.PSet( limit = cms.untracked.int32(-1))
)
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
    default = cms.untracked.PSet( limit = cms.untracked.int32(0)),
    Analysis = cms.untracked.PSet( limit = cms.untracked.int32(-1))
)

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# Castor FastSimulation 
process.load('FastSimulation.ForwardDetectors.CastorTowerProducer_cfi') 
# Castor Reconstruction 
process.load('RecoLocalCalo.Castor.Castor_cfi')

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/tmp/antoniov/POMWIG_SingleDiffractiveWmunuPlus_10TeV_InitialLumPU_cff_py_RAW2DIGI_RECO.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.load('DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.wmunuSelFilter_cfi')
process.load('DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.wmunuAnalyzer_cfi')
process.wmunuAnalyzerAfterFilter = process.wmunuAnalyzer.clone()

from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.pileUpNumberFilter_cfi import *
process.filter0PU = pileUpNumberFilter.clone(NumberOfPileUpEvents = 0)
process.filter1PU = pileUpNumberFilter.clone(NumberOfPileUpEvents = 1)
process.filter2PU = pileUpNumberFilter.clone(NumberOfPileUpEvents = 2)
process.filter3PU = pileUpNumberFilter.clone(NumberOfPileUpEvents = 3)
process.filter4PU = pileUpNumberFilter.clone(NumberOfPileUpEvents = 4)

process.wmunuAnalyzer0PU = process.wmunuAnalyzer.clone()
process.wmunuAnalyzer1PU = process.wmunuAnalyzer.clone()
process.wmunuAnalyzer2PU = process.wmunuAnalyzer.clone()
process.wmunuAnalyzer3PU = process.wmunuAnalyzer.clone()
process.wmunuAnalyzer4PU = process.wmunuAnalyzer.clone()

process.load('DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.genParticlesCalo_cfi')
process.load('DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.edmDump_cff')

process.load('DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.outputModule_cfi')
#process.output.fileName = 'POMWIG_SDPlusWmunu_EdmDump_InitialLumPU.root'
process.output.fileName = 'POMWIG_SDPlusWmunu_EdmDump_StageA43Bx.root'
process.output.SelectEvents = cms.vstring('p2')

process.add_(cms.Service("TFileService",
		#fileName = cms.string("analysisWMuNu_histos_SDPlusWmunu_InitialLumPU.root")
                fileName = cms.string("analysisWMuNu_histos_SDPlusWmunu_StageA43Bx.root")
	)
)

process.p0 = cms.Path(process.pileUpInfo)
process.wmunu0PU = cms.Sequence(process.filter0PU*process.wmunuAnalyzer0PU)
process.wmunu1PU = cms.Sequence(process.filter1PU*process.wmunuAnalyzer1PU)
process.wmunu2PU = cms.Sequence(process.filter2PU*process.wmunuAnalyzer2PU)
process.wmunu3PU = cms.Sequence(process.filter3PU*process.wmunuAnalyzer3PU)
process.wmunu4PU = cms.Sequence(process.filter4PU*process.wmunuAnalyzer4PU)
process.p1 = cms.Path(process.wmunuAnalyzer + 
                      process.wmunu0PU + 
                      process.wmunu1PU +
                      process.wmunu2PU +
                      process.wmunu3PU)
process.p2 = cms.Path(process.wmunuSelFilter*process.wmunuAnalyzerAfterFilter)
process.p3 = cms.Path(process.wmunuSelFilter*process.CastorTowerReco*process.genParticlesCalo*process.edmDumpAllNoPileUp)

process.out_step = cms.EndPath(process.output)

process.schedule = cms.Schedule(process.p0,process.p1,process.p2,process.p3,process.out_step)
