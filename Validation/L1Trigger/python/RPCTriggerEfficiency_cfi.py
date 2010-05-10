import FWCore.ParameterSet.Config as cms



RPCTriggerVal = cms.EDAnalyzer("RPCTriggerValidation",
      
      L1CandsCollections = cms.VInputTag(cms.InputTag("gtDigis") ),
      
      MCCollection  = cms.InputTag("muons"),
      
      
      outputDirectory = cms.string("L1_RPC"),
      outputFile=cms.string("RPC_efficiency.root"),
      deltaEtaThreshold=cms.double(0.2),
      deltaPhiThreshold=cms.double(0.25),
      L1MuonFromReco=cms.bool(True),
      takeGMT = cms.bool(False),
      GlobalMuon= cms.bool(True),
      StandAloneMuon= cms.bool(False),
      NumberOfQuality=cms.int32(4),
      etaMax=cms.double(1.61),
      etaMin=cms.double(-1.61),
      dev= cms.bool(True),
# etaPtRanges = cms.VPSet (
#         cms.PSet( etaL=cms.double(-1.61), etaH=cms.double(-1.24), ptL=cms.double(0.0), ptH=cms.double(10.0) ),
#         cms.PSet( etaL=cms.double(-1.61), etaH=cms.double(-1.24), ptL=cms.double(10.0), ptH=cms.double(100.) ),
#         cms.PSet( etaL=cms.double(-1.61), etaH=cms.double(-1.24), ptL=cms.double(100.), ptH=cms.double(1000.) ),
# 	
# 	cms.PSet( etaL=cms.double(-1.24), etaH=cms.double(-0.93), ptL=cms.double(0.0), ptH=cms.double(10.0) ),
#         cms.PSet( etaL=cms.double(-1.24), etaH=cms.double(-0.93), ptL=cms.double(10.0), ptH=cms.double(100.) ),
#         cms.PSet( etaL=cms.double(-1.24), etaH=cms.double(-0.93), ptL=cms.double(100.), ptH=cms.double(1000.) ),
# 	
# 	cms.PSet( etaL=cms.double(-0.93), etaH=cms.double(0.93), ptL=cms.double(0.0), ptH=cms.double(10.0) ),
#         cms.PSet( etaL=cms.double(-0.93), etaH=cms.double(0.93), ptL=cms.double(10.0), ptH=cms.double(100.) ),
#         cms.PSet( etaL=cms.double(-0.93), etaH=cms.double(0.93), ptL=cms.double(100.), ptH=cms.double(1000.) ),
# 	
# 	cms.PSet( etaL=cms.double(0.93), etaH=cms.double(1.24), ptL=cms.double(0.0), ptH=cms.double(10.0) ),
#         cms.PSet( etaL=cms.double(0.93), etaH=cms.double(1.24), ptL=cms.double(10.0), ptH=cms.double(100.) ),
#         cms.PSet( etaL=cms.double(0.93), etaH=cms.double(1.24), ptL=cms.double(100.), ptH=cms.double(1000.) ),
# 	
# 	cms.PSet( etaL=cms.double(1.24), etaH=cms.double(1.61), ptL=cms.double(0.0), ptH=cms.double(10.0) ),
#         cms.PSet( etaL=cms.double(1.24), etaH=cms.double(1.61), ptL=cms.double(10.0), ptH=cms.double(100.) ),
#         cms.PSet( etaL=cms.double(1.24), etaH=cms.double(1.61), ptL=cms.double(100.), ptH=cms.double(1000.) ),
#       ),
      etaPtRanges = cms.VPSet (
            # 10
            cms.PSet( etaL=cms.double(-1.6), etaH=cms.double(-1.24), ptL=cms.double(5.), ptH=cms.double(10.) ),
            cms.PSet( etaL=cms.double(-1.24), etaH=cms.double(-0.8), ptL=cms.double(5.), ptH=cms.double(10.) ),
            cms.PSet( etaL=cms.double(-0.8), etaH=cms.double(-0.05), ptL=cms.double(5.), ptH=cms.double(10.) ),
            cms.PSet( etaL=cms.double(-0.05), etaH=cms.double(0.05), ptL=cms.double(5.), ptH=cms.double(10.) ),
            cms.PSet( etaL=cms.double(0.05),  etaH=cms.double(0.8), ptL=cms.double(5.), ptH=cms.double(10.) ),
            cms.PSet( etaL=cms.double(0.8),  etaH=cms.double(1.24), ptL=cms.double(5.), ptH=cms.double(10.) ),
            cms.PSet( etaL=cms.double(1.24),  etaH=cms.double(1.6), ptL=cms.double(5.), ptH=cms.double(10.) ),
            # 5
            cms.PSet( etaL=cms.double(-1.6), etaH=cms.double(-1.24), ptL=cms.double(0.), ptH=cms.double(5.) ),
            cms.PSet( etaL=cms.double(-1.24), etaH=cms.double(-0.8), ptL=cms.double(0.), ptH=cms.double(5.) ),
            cms.PSet( etaL=cms.double(-0.8), etaH=cms.double(-0.05), ptL=cms.double(0.), ptH=cms.double(5.) ),
            cms.PSet( etaL=cms.double(-0.05), etaH=cms.double(0.05), ptL=cms.double(0.), ptH=cms.double(5.) ),
            cms.PSet( etaL=cms.double(0.05),  etaH=cms.double(0.8), ptL=cms.double(0.), ptH=cms.double(5.) ),
            cms.PSet( etaL=cms.double(0.8),  etaH=cms.double(1.24), ptL=cms.double(0.), ptH=cms.double(5.) ),
            cms.PSet( etaL=cms.double(1.24),  etaH=cms.double(1.6), ptL=cms.double(0.), ptH=cms.double(5.) ),
            #15
            cms.PSet( etaL=cms.double(-1.6), etaH=cms.double(-1.24), ptL=cms.double(10.), ptH=cms.double(15.) ),
            cms.PSet( etaL=cms.double(-1.24), etaH=cms.double(-0.8), ptL=cms.double(10.), ptH=cms.double(15.) ),
            cms.PSet( etaL=cms.double(-0.8), etaH=cms.double(-0.05), ptL=cms.double(10.), ptH=cms.double(15.) ),
            cms.PSet( etaL=cms.double(-0.05), etaH=cms.double(0.05), ptL=cms.double(10.), ptH=cms.double(15.) ),
            cms.PSet( etaL=cms.double(0.05),  etaH=cms.double(0.8), ptL=cms.double(10.), ptH=cms.double(15.) ),
            cms.PSet( etaL=cms.double(0.8),  etaH=cms.double(1.24), ptL=cms.double(10.), ptH=cms.double(15.) ),
            cms.PSet( etaL=cms.double(1.24),  etaH=cms.double(1.6), ptL=cms.double(10.), ptH=cms.double(15.) ),
            #20
            cms.PSet( etaL=cms.double(-1.6), etaH=cms.double(-1.24), ptL=cms.double(15.), ptH=cms.double(20.) ),
            cms.PSet( etaL=cms.double(-1.24), etaH=cms.double(-0.8), ptL=cms.double(15.), ptH=cms.double(20.) ),
            cms.PSet( etaL=cms.double(-0.8), etaH=cms.double(-0.05), ptL=cms.double(15.), ptH=cms.double(20.) ),
            cms.PSet( etaL=cms.double(-0.05), etaH=cms.double(0.05), ptL=cms.double(15.), ptH=cms.double(20.) ),
            cms.PSet( etaL=cms.double(0.05),  etaH=cms.double(0.8), ptL=cms.double(15.), ptH=cms.double(20.) ),
            cms.PSet( etaL=cms.double(0.8),  etaH=cms.double(1.24), ptL=cms.double(15.), ptH=cms.double(20.) ),
            cms.PSet( etaL=cms.double(1.24),  etaH=cms.double(1.6), ptL=cms.double(15.), ptH=cms.double(20.) ),

            cms.PSet( etaL=cms.double(-1.6), etaH=cms.double(-1.24), ptL=cms.double(20.), ptH=cms.double(100.) ),
            cms.PSet( etaL=cms.double(-1.24), etaH=cms.double(-0.8), ptL=cms.double(20.), ptH=cms.double(100.) ),
            cms.PSet( etaL=cms.double(-0.8), etaH=cms.double(-0.05), ptL=cms.double(20.), ptH=cms.double(100.) ),
            cms.PSet( etaL=cms.double(-0.05), etaH=cms.double(0.05), ptL=cms.double(20.), ptH=cms.double(100.) ),
            cms.PSet( etaL=cms.double(0.05),  etaH=cms.double(0.8), ptL=cms.double(20.), ptH=cms.double(100.) ),
            cms.PSet( etaL=cms.double(0.8),  etaH=cms.double(1.24), ptL=cms.double(20.), ptH=cms.double(100.) ),
            cms.PSet( etaL=cms.double(1.24),  etaH=cms.double(1.6),ptL=cms.double(20.), ptH=cms.double(100.) ),
	    

            cms.PSet( etaL=cms.double(-1.6), etaH=cms.double(-1.24), ptL=cms.double(100.), ptH=cms.double(1000.) ),
            cms.PSet( etaL=cms.double(-1.24), etaH=cms.double(-0.8), ptL=cms.double(100.), ptH=cms.double(1000.) ),
            cms.PSet( etaL=cms.double(-0.8), etaH=cms.double(-0.05), ptL=cms.double(100.), ptH=cms.double(1000.) ),
            cms.PSet( etaL=cms.double(-0.05), etaH=cms.double(0.05), ptL=cms.double(100.), ptH=cms.double(1000.) ),
            cms.PSet( etaL=cms.double(0.05),  etaH=cms.double(0.8), ptL=cms.double(100.), ptH=cms.double(1000.) ),
            cms.PSet( etaL=cms.double(0.8),  etaH=cms.double(1.24), ptL=cms.double(100.), ptH=cms.double(1000.) ),
            cms.PSet( etaL=cms.double(1.24),  etaH=cms.double(1.6),ptL=cms.double(100.), ptH=cms.double(1000.) )

            
      ),
# PtRanges = cms.VPSet (
#         cms.PSet(  ptL=cms.double(0.0), ptH=cms.double(10.0) ),
#         cms.PSet( ptL=cms.double(10.0), ptH=cms.double(100.) ),
#         cms.PSet(  ptL=cms.double(100.), ptH=cms.double(1000.) )
#       ), 
      
      PtRanges = cms.VPSet (
        cms.PSet(  ptL=cms.double(0.), ptH=cms.double(5.) ),
        cms.PSet( ptL=cms.double(5.), ptH=cms.double(10.) ),
        cms.PSet(  ptL=cms.double(10.), ptH=cms.double(15.) ),
	cms.PSet(  ptL=cms.double(15.), ptH=cms.double(20.) ),
        cms.PSet( ptL=cms.double(20.), ptH=cms.double(100.) ),
        cms.PSet(  ptL=cms.double(100.), ptH=cms.double(1000.) ),
	cms.PSet(  ptL=cms.double(0.), ptH=cms.double(1000.) )
      ), 
      
EtaRanges = cms.VPSet (
        cms.PSet(  etaL=cms.double(-1.61), etaH=cms.double(1.61) ),
	cms.PSet(  etaL=cms.double(-0.93), etaH=cms.double(0.93) )
      ) 

)