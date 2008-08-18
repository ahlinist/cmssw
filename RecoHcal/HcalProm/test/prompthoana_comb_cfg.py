# The following comments couldn't be translated into the new config version:

#  include "FWCore/MessageLogger/data/MessageLogger.cfi"

# standAloneMuons
import FWCore.ParameterSet.Config as cms

process = cms.Process("HOCalibAnalyser")
# include "MagneticField/Engine/data/volumeBasedMagneticField.cfi"
process.load("MagneticField.Engine.uniformMagneticField_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(2)
)
process.source = cms.Source("PoolSource",
    dropMetaData = cms.untracked.bool(True),
    fileNames = cms.untracked.vstring('/store/data/2008/5/20/T0ReReco-GlobalCruzet1-A-v1/0005/BEE6FCEF-E026-DD11-8B35-001D09F29849.root')
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
#    RootFileName = cms.untracked.string('promptho_comb_0.root'),
    histFit = cms.untracked.bool(False), # False), ## false 

    l1Input = cms.InputTag("gtDigis"), ## l1extraParticleMap

    #        untracked bool   hotime = false
    #        untracked int32 firstTS = 5
    #        untracked int32 lastTS = 8
    #        untracked double m_scale = 4.0
    sigma = cms.untracked.double(0.1),
    towerInput = cms.InputTag("towerMaker"),
    cosmic = cms.untracked.bool(True),
    hoInput = cms.InputTag("horeco"), ## hodigi 

    digiInput = cms.untracked.bool(False),
    inputtxtFileName = cms.untracked.string('promptho_high_input.txt'),
    txtFileName = cms.untracked.string('promptho_comb_0.txt'),
#    psFileName = cms.untracked.string('promptho_comb_0.ps'),
    get_constant = cms.untracked.bool(True),
    debug = cms.untracked.bool(False),
    muons = cms.untracked.InputTag("cosmicMuons"),
    pedSuppr = cms.untracked.bool(False), # True),
    hltInput = cms.InputTag("TriggerResults","","FU"),
    mCombined = cms.untracked.bool(True),
    get_figure = cms.untracked.bool(True),
    baseHtmlDir = cms.untracked.string('.'),
    printPromptHTML  = cms.untracked.bool(True)

)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('test_hist_cobm.root')
)

process.p1 = cms.Path(process.hoprompt)
process.UniformMagneticFieldESProducer.ZFieldInTesla = 0.001


