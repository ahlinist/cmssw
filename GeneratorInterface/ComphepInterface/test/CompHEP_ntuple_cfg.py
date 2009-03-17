import FWCore.ParameterSet.Config as cms

process = cms.Process("Print")

process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring('file:comphep_source.root'),
	skipEvents = cms.untracked.uint32(0)
)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("comphep_ntuple.root")
)


process.printList = cms.EDFilter("ParticleListDrawer",
	maxEventsToPrint = cms.untracked.int32(1)
)

## process.printTree = cms.EDFilter("ParticleTreeDrawer",
## 	status = cms.untracked.vint32(3),
## 	src = cms.InputTag("genParticles")##,
## ## 	printP4 = cms.untracked.bool(False),
## ## 	printStatus = cms.untracked.bool(True),
## ## 	printIndex = cms.untracked.bool(True),
## ## 	printVertex = cms.untracked.bool(True),
## ## 	printPtEtaPhi = cms.untracked.bool(True)
## )


##  module trackHistos= CandViewHistoAnalyzer {
##    InputTag src = muons
##    untracked double massMin = 0
##    untracked double massMax = 300
##    untracked double ptMax = 200
##   }


process.p = cms.EDAnalyzer(
    "CandViewHistoAnalyzer",
    src = cms.InputTag("genParticles"),
    massMin = cms.untracked.double(0),
    massMax = cms.untracked.double(300.0),
    ptMax = cms.untracked.double(300.0)

)

process.p = cms.Path(process.p)
