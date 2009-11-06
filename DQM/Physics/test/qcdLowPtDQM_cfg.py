# $Id:$

import FWCore.ParameterSet.Config as cms

process = cms.Process("QcdLowPtDQM")

process.load("DQMServices.Core.DQM_cfg")
process.load("DQMServices.Components.DQMEnvironment_cfi")
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')

process.GlobalTag.globaltag = 'MC_31X_V9::All'

process.dqmSaver.workflow = cms.untracked.string('/Physics/QCDPhysics/LowPt')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(2500)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/opt/data/cms/CMSSW_3_3_2/RelValMinBias-GEN-SIM-DIGI-RAW-HLTDEBUG-MC_31X_V9-v1.root'
    )
)

process.load('EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi')
process.load('RecoLocalTracker.Configuration.RecoLocalTracker_cff')
process.load('RecoLocalTracker.SiPixelRecHits.PixelCPEESProducers_cff')

#import RecoPixelVertexing.PixelVertexFinding.PixelVertexes_cfi
#process.load('RecoPixelVertexing.PixelVertexFinding.PixelVertexes')
#pixel3Vertices = RecoPixelVertexing.PixelVertexFinding.PixelVertexes_cfi.pixelVertices.clone();
#pixel3Vertices.TrackCollection = 'pixel3ProtoTracks'
#pixel3Vertices.UseError = True
#pixel3Vertices.WtAverage = True
#pixel3Vertices.ZOffset = 5.
#pixel3Vertices.ZSeparation = 0.3
#pixel3Vertices.NTrkMin = 3
#pixel3Vertices.PtMin = 0.15

#import RecoPixelVertexing.PixelLowPtUtilities.AllPixelTracks_cfi
#pixel3ProtoTracks = RecoPixelVertexing.PixelLowPtUtilities.AllPixelTracks_cfi.allPixelTracks.clone()

#pixelVertexFromClusters = cms.EDProducer('PixelVertexProducerClusters')

#dump = cms.EDAnalyzer('EventContentAnalyzer')

#from RecoLocalTracker.SiPixelRecHits.PixelCPEESProducers_cff import *

process.mySiPixelRecHits = cms.EDFilter("SiPixelRecHitConverter",
    src = cms.InputTag("siPixelClusters"),
    CPE = cms.string('PixelCPETemplateReco'),
    VerboseLevel = cms.untracked.int32(0),
)

myRecoSequence = cms.Sequence(process.siPixelDigis*
                              process.siPixelClusters*
                              process.siPixelRecHits)
#                            pixel3ProtoTracks *
#                            pixel3Vertices *
#                            pixelVertexFromClusters)

process.siPixelDigis.InputLabel = cms.InputTag("rawDataCollector")

process.QcdLowPtDQM = cms.EDAnalyzer("QcdLowPtDQM",
    hltTrgNames  = cms.untracked.vstring('HLT_MinBiasHcal',
                                         'HLT_MinBiasEcal',
                                         'HLT_MinBiasPixel'),
    verbose      = cms.untracked.int32(0),                                     
)

process.p = cms.Path(myRecoSequence*process.QcdLowPtDQM+process.dqmSaver)
