# The following comments couldn't be translated into the new config version:

#  include "FWCore/MessageLogger/data/MessageLogger.cfi"

# standAloneMuons
import FWCore.ParameterSet.Config as cms

process = cms.Process("HOCalibAnalyser")
# include "MagneticField/Engine/data/volumeBasedMagneticField.cfi"
process.load("MagneticField.Engine.uniformMagneticField_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
#    input = cms.untracked.int32(1000)
)
process.source = cms.Source("PoolSource",
    dropMetaData = cms.untracked.bool(True),
    fileNames = cms.untracked.vstring('/store/data/2008/5/20/T0ReReco-GlobalCruzet1-A-v1/0005/BE657CCB-DE26-DD11-BBB8-000423D99614.root', 
        '/store/data/2008/5/20/T0ReReco-GlobalCruzet1-A-v1/0005/BE8FABF6-D726-DD11-AD40-000423D98EA8.root',
        '/store/data/2008/5/20/T0ReReco-GlobalCruzet1-A-v1/0005/BE9A2701-DD26-DD11-9D49-001D09F29619.root') 
)

process.MessageLogger = cms.Service("MessageLogger",
    cout = cms.untracked.PSet(
        default = cms.untracked.PSet( ## kill all messages in the log

            limit = cms.untracked.int32(0)
        ),
        FwkJob = cms.untracked.PSet( ## but FwkJob category - those unlimitted

            limit = cms.untracked.int32(-1)
        )
    ),
    categories = cms.untracked.vstring('FwkJob'),
    destinations = cms.untracked.vstring('cout')
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True) ## default is false

)
process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
    ignoreTotal = cms.untracked.int32(1) ## default is one

)

process.hoprompt = cms.EDFilter("PromptHOAnalyser",
    histFit = cms.untracked.bool(True),
    l1Input = cms.InputTag("gtDigis"), ## l1extraParticleMap

    lastTS = cms.untracked.int32(8),
    get_figure = cms.untracked.bool(True),
    m_scale = cms.untracked.double(4.0),
    inputtxtFileName = cms.untracked.string('test_input.txt'),
    txtFileName = cms.untracked.string('promptho_reco_0.txt'),
    muons = cms.untracked.InputTag("cosmicMuons"),
    pedSuppr = cms.untracked.bool(True),
    hltInput = cms.InputTag("TriggerResults","","FU"),
    hoInput = cms.InputTag("horeco"), ## hodigi 

    digiInput = cms.untracked.bool(False),
    hotime = cms.untracked.bool(False),
    towerInput = cms.InputTag("towerMaker"),
    cosmic = cms.untracked.bool(True),
#    psFileName = cms.untracked.string('promptho_reco_0.ps'),
#    RootFileName = cms.untracked.string('promptho_reco_0.root'),
    get_constant = cms.untracked.bool(True),
    debug = cms.untracked.bool(False),
    firstTS = cms.untracked.int32(5),
    sigma = cms.untracked.double(0.1),
#    mCombined = cms.untracked.bool(True),
    mCombined = cms.untracked.bool(False),
    baseHtmlDir = cms.untracked.string('.'),
    printPromptHTML  = cms.untracked.bool(True)

)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('promptHO_reco.root')
)

process.p1 = cms.Path(process.hoprompt)
process.UniformMagneticFieldESProducer.ZFieldInTesla = 0.001


