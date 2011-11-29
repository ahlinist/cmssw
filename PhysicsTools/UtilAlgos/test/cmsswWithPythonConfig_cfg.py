import FWCore.ParameterSet.Config as cms

process = cms.Process("Test")

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(
    "/store/relval/CMSSW_5_0_0_pre6/RelValZTT/GEN-SIM-RECO/START50_V5-v1/0202/A0D83694-4917-E111-85E1-0026189438F2.root"
  )
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

process.MessageLogger = cms.Service("MessageLogger")

## ---
## This is an example of the use of the BasicAnalyzer concept used to exploit C++ classes to do anaysis
## in full framework or FWLite using the same class. You can find the implementation of this module in
## PhysicsTools/UtilAlgos/plugins/WrappedEDMuonAnlyzer. You can find the EDAnalyzerWrapper.h class in
## PhysicsTools/UtilAlgos/interface/EDAnalyzerWrapper.h. You can find the implementation of the
## BasicMuonAnalyzer class in PhysicsTools/UtilAlgos/interface/BasicMuonAnlyzer.h. You will also find
## back the input parameters to the module.
process.muonAnalyzer = cms.EDAnalyzer("WrappedEDMuonAnalyzer",
  muons = cms.InputTag("muons"),                                             
)

process.TFileService = cms.Service("TFileService",
  fileName = cms.string('analyzeCMSSWHistograms.root')
)

process.p = cms.Path(process.muonAnalyzer)

