import FWCore.ParameterSet.Config as cms


				       
rpcTriggerValidation = cms.EDAnalyzer('RPCTriggerValidation',
      L1CandsCollections = cms.VInputTag(cms.InputTag("gtDigis:RPCb"), cms.InputTag("gtDigis:RPCf") ),
      
      MCCollection  = cms.InputTag("genParticles"),
     
      
      
      outputDirectory = cms.string("L1_RPC"),
      outputFile=cms.string("RPC_validation.root"),
      deltaEtaThreshold=cms.double(0.2),
      deltaphiThreshold=cms.double(1.0),
      L1MuonFromReco=cms.bool(False),
      takeGMT = cms.bool(False),
      GlobalMuon= cms.bool(False),
      StandAloneMuon= cms.bool(False),
      NumberOfQuality=cms.int32(4),
      etaMax=cms.double(1.61),
      etaMin=cms.double(-1.61),
      dev= cms.bool(True),
      
#       etaPtRanges = cms.VPSet (
#         cms.PSet( etaL=cms.double(-1.61), etaH=cms.double(-1.24), ptL=cms.double(9.80), ptH=cms.double(10.2) ),
#         cms.PSet( etaL=cms.double(-1.61), etaH=cms.double(-1.24), ptL=cms.double(98.0), ptH=cms.double(102.) ),
#         cms.PSet( etaL=cms.double(-1.61), etaH=cms.double(-1.24), ptL=cms.double(980.), ptH=cms.double(1020.) ),
# 	
# 	cms.PSet( etaL=cms.double(-1.24), etaH=cms.double(-0.93), ptL=cms.double(9.80), ptH=cms.double(10.2) ),
#         cms.PSet( etaL=cms.double(-1.24), etaH=cms.double(-0.93), ptL=cms.double(98.0), ptH=cms.double(102.) ),
#         cms.PSet( etaL=cms.double(-1.24), etaH=cms.double(-0.93), ptL=cms.double(980.), ptH=cms.double(1020.) ),
# 	
# 	cms.PSet( etaL=cms.double(-0.93), etaH=cms.double(0.93), ptL=cms.double(9.80), ptH=cms.double(10.2) ),
#         cms.PSet( etaL=cms.double(-0.93), etaH=cms.double(0.93), ptL=cms.double(98.0), ptH=cms.double(102.) ),
#         cms.PSet( etaL=cms.double(-0.93), etaH=cms.double(0.93), ptL=cms.double(980.), ptH=cms.double(1020.) ),
# 	
# 	cms.PSet( etaL=cms.double(0.93), etaH=cms.double(1.24), ptL=cms.double(9.80), ptH=cms.double(10.2) ),
#         cms.PSet( etaL=cms.double(0.93), etaH=cms.double(1.24), ptL=cms.double(98.0), ptH=cms.double(102.) ),
#         cms.PSet( etaL=cms.double(0.93), etaH=cms.double(1.24), ptL=cms.double(980.), ptH=cms.double(1020.) ),
# 	
# 	cms.PSet( etaL=cms.double(1.24), etaH=cms.double(1.61), ptL=cms.double(9.80), ptH=cms.double(10.2) ),
#         cms.PSet( etaL=cms.double(1.24), etaH=cms.double(1.61), ptL=cms.double(98.0), ptH=cms.double(102.) ),
#         cms.PSet( etaL=cms.double(1.24), etaH=cms.double(1.61), ptL=cms.double(980.), ptH=cms.double(1020.) ),
#       ),
#       
#       PtRanges = cms.VPSet (
#         cms.PSet(  ptL=cms.double(9.80), ptH=cms.double(10.2) ),
#         cms.PSet( ptL=cms.double(98.0), ptH=cms.double(102.) ),
#         cms.PSet(  ptL=cms.double(980.), ptH=cms.double(1020.) )
#       ),

etaPtRanges = cms.VPSet (
            # 10
            cms.PSet( etaL=cms.double(-1.6), etaH=cms.double(-1.24), ptL=cms.double(9.90), ptH=cms.double(10.1) ),
            cms.PSet( etaL=cms.double(-1.24), etaH=cms.double(-0.8), ptL=cms.double(9.90), ptH=cms.double(10.1) ),
            cms.PSet( etaL=cms.double(-0.8), etaH=cms.double(-0.05), ptL=cms.double(9.90), ptH=cms.double(10.1) ),
            cms.PSet( etaL=cms.double(-0.05), etaH=cms.double(0.05), ptL=cms.double(9.90), ptH=cms.double(10.1) ),
            cms.PSet( etaL=cms.double(0.05),  etaH=cms.double(0.8), ptL=cms.double(9.9), ptH=cms.double(10.1) ),
            cms.PSet( etaL=cms.double(0.8),  etaH=cms.double(1.24), ptL=cms.double(9.90), ptH=cms.double(10.1) ),
            cms.PSet( etaL=cms.double(1.24),  etaH=cms.double(1.6), ptL=cms.double(9.90), ptH=cms.double(10.1) ),

            #100
            cms.PSet( etaL=cms.double(-1.6), etaH=cms.double(-1.24), ptL=cms.double(98.0), ptH=cms.double(102.) ),
            cms.PSet( etaL=cms.double(-1.24), etaH=cms.double(-0.8), ptL=cms.double(98.0), ptH=cms.double(102.) ),
            cms.PSet( etaL=cms.double(-0.8), etaH=cms.double(-0.05), ptL=cms.double(98.0), ptH=cms.double(102.) ),
            cms.PSet( etaL=cms.double(-0.05), etaH=cms.double(0.05), ptL=cms.double(98.0), ptH=cms.double(102.) ),
            cms.PSet( etaL=cms.double(0.05),  etaH=cms.double(0.8), ptL=cms.double(98.0), ptH=cms.double(102.) ),
            cms.PSet( etaL=cms.double(0.8),  etaH=cms.double(1.24), ptL=cms.double(98.0), ptH=cms.double(102.) ),
            cms.PSet( etaL=cms.double(1.24),  etaH=cms.double(1.6), ptL=cms.double(98.0), ptH=cms.double(102.) ),

	    #1000
            cms.PSet( etaL=cms.double(-1.6), etaH=cms.double(-1.24), ptL=cms.double(980.), ptH=cms.double(1020.) ),
            cms.PSet( etaL=cms.double(-1.24), etaH=cms.double(-0.8), ptL=cms.double(980.), ptH=cms.double(1020.) ),
            cms.PSet( etaL=cms.double(-0.8), etaH=cms.double(-0.05), ptL=cms.double(980.), ptH=cms.double(1020.) ),
            cms.PSet( etaL=cms.double(-0.05), etaH=cms.double(0.05), ptL=cms.double(980.), ptH=cms.double(1020.) ),
            cms.PSet( etaL=cms.double(0.05),  etaH=cms.double(0.8), ptL=cms.double(980.), ptH=cms.double(1020.) ),
            cms.PSet( etaL=cms.double(0.8),  etaH=cms.double(1.24), ptL=cms.double(980.), ptH=cms.double(1020.) ),
            cms.PSet( etaL=cms.double(1.24),  etaH=cms.double(1.6), ptL=cms.double(980.), ptH=cms.double(1020.) )

            
      ),
      
      PtRanges = cms.VPSet (
        cms.PSet(  ptL=cms.double(9.80), ptH=cms.double(10.2) ),
        cms.PSet( ptL=cms.double(98.0), ptH=cms.double(102.) ),
        cms.PSet(  ptL=cms.double(980.), ptH=cms.double(1020.) )
       ),
      
      
EtaRanges = cms.VPSet (
        cms.PSet(  etaL=cms.double(-1.61), etaH=cms.double(1.61) )
      ) 

)

