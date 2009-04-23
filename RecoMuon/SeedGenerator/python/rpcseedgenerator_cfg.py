import FWCore.ParameterSet.Config as cms

process = cms.Process("RPCSeed")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.Services_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")

process.GlobalTag.globaltag = 'IDEAL_31X::All'

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
fileNames = cms.untracked.vstring(
    #'rfio:/castor/cern.ch/user/h/hyteng/CMSSW_2_1_10/muongun_1pair_10Gev.root'
    '/store/relval/CMSSW_3_1_0_pre5/RelValSingleMuPt10/GEN-SIM-RECO/IDEAL_31X_v1/0000/F85A8D98-CB2B-DE11-93D6-000423D98634.root'
    )
#fileNames = cms.untracked.vstring('file:muongun.root')
)


process.myRPCSeed = cms.EDProducer('RPCSeedGenerator', 
        RangeofLayersinBarrel = cms.vuint32(5),
        RangeofLayersinEndcap = cms.vuint32(3),
        isSpecialLayers = cms.bool(False), 
        isMixBarrelwithEndcap = cms.bool(False),
        LayersinBarrel = cms.vuint32(1,1,1,1,0,0),
        LayersinEndcap = cms.vuint32(1,1,1,1,1,1),
        constrainedLayersinBarrel = cms.vuint32(1,1,1,1,0,0),
        RPCRecHitsLabel = cms.InputTag("rpcRecHits"),
        BxRange = cms.uint32(0),
        ClusterSet = cms.vint32(),
        MaxDeltaPhi = cms.double(3.14159265359/6),
        MaxRSD = cms.double(50.0),
        deltaRThreshold = cms.double(3.0),
        ZError = cms.double(130.0),
        MinDeltaPhi = cms.double(0.05),
        AlgorithmType = cms.uint32(3),
        autoAlgorithmChoose = cms.bool(False),
        stepLength = cms.double(1),
        sampleCount = cms.uint32(10),
        ShareRecHitsNumberThreshold = cms.uint32(1),
        isCheckcandidateOverlap = cms.bool(True),
        isCheckgoodOverlap = cms.bool(True)
)

process.content = cms.PSet( 
        outputCommands = cms.untracked.vstring('keep *_*_*_RPCSeed',
            'keep *_rpcRecHits_*_*',
            'keep *_source_*_*',
            'keep SimTracks_g4SimHits_*_*',
            'keep *_g4SimHits_MuonRPCHits_*',
            'keep *_simMuonRPCDigis_RPCDigiSimLink_*')
        )
process.out = cms.OutputModule("PoolOutputModule",
#        process.content, 
        fileName = cms.untracked.string('muonseed.root')
)



# Stand alone made with RPC seeds as starting states, but it can collect DT/CSC/RPC hits during pattern recognition 
process.standAloneMuonsRPCSeed = process.standAloneMuons.clone()
process.standAloneMuonsRPCSeed.InputObjects = cms.InputTag('myRPCSeed','goodSeeds')

# Stand alone made with RPC hits only
process.standAloneMuonsFullRPC = process.standAloneMuonsRPCSeed.clone()
process.standAloneMuonsFullRPC.STATrajBuilderParameters.FilterParameters.EnableCSCMeasurement=False
process.standAloneMuonsFullRPC.STATrajBuilderParameters.BWFilterParameters.EnableCSCMeasurement=False
process.standAloneMuonsFullRPC.STATrajBuilderParameters.FilterParameters.EnableDTMeasurement=False
process.standAloneMuonsFullRPC.STATrajBuilderParameters.BWFilterParameters.EnableDTMeasurement=False


  
process.p = cms.Path(process.myRPCSeed*process.standAloneMuonsRPCSeed*process.standAloneMuonsFullRPC)

process.e = cms.EndPath(process.out)
