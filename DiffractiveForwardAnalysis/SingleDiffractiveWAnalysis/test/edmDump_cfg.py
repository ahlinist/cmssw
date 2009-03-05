import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")
# keep the logging output to a nice level ###
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.debugModules = cms.untracked.vstring('')
#process.MessageLogger.cerr.threshold = 'DEBUG'
process.MessageLogger.cerr.threshold = 'INFO'
#process.MessageLogger.cerr.threshold = 'WARNING'
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

process.load('DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.edmDump_cff')

process.pileUpInfo = cms.EDProducer("PileUpEdmNtupleDumper")

process.MyEventContent = cms.PSet(
        outputCommands = cms.untracked.vstring('drop *')
)
process.MyEventContent.outputCommands.append('keep *_*_*_Analysis')
process.MyEventContent.outputCommands.append('keep recoMuons_muons_*_*')
process.MyEventContent.outputCommands.append('keep recoTracks_generalTracks_*_*')
process.MyEventContent.outputCommands.append('keep *_offlinePrimaryVertices_*_*')
process.MyEventContent.outputCommands.append('keep *_offlinePrimaryVerticesWithBS_*_*')
process.MyEventContent.outputCommands.append('keep *_pixelVertices_*_*')

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = process.MyEventContent.outputCommands,
    #fileName = cms.untracked.string('POMWIG_SDPlusWmunu_EdmDump_noPU.root'),
    fileName = cms.untracked.string('POMWIG_SDPlusWmunu_EdmDump_InitialLumPU.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('USER'),
        filterName = cms.untracked.string('')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('p2')
    )
)

process.add_(cms.Service("TFileService",
		fileName = cms.string("analysisWMuNu_histos_SDPlusWmunu_InitialLumPU.root")
	)
)

process.p1 = cms.Path(process.wmunuAnalyzer)
process.p2 = cms.Path(process.wmunuSelFilter*process.wmunuAnalyzerAfterFilter*process.CastorTowerReco*process.genParticlesCalo*process.castorGen*process.castorGenNoThreshold*process.castorTower*process.hfTower*process.xiTower*process.xiTowerNoMET*process.pileUpInfo) 
process.out_step = cms.EndPath(process.output)

process.schedule = cms.Schedule(process.p1,process.p2,process.out_step)
