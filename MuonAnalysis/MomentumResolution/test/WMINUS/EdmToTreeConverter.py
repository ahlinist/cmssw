import FWCore.ParameterSet.Config as cms

process = cms.Process("proc")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout']
process.MessageLogger.cout = cms.untracked.PSet(
      threshold = cms.untracked.string('INFO'),
      FwkReport = cms.untracked.PSet(reportEvery=cms.untracked.int32(10))
)
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )


process.source = cms.Source("PoolSource", 
     fileNames = cms.untracked.vstring(
    "file:EdmWmnTreeMCMINUSShort.root",
    "file:EdmWmnTreeDataShort.root"
    )
                           
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.TFileService = cms.Service("TFileService", 
                                   fileName = cms.string("treeMC_histoData_WMINUS.root"),
                                   closeFileFast = cms.untracked.bool(True)
                                   )

process.edmtotreeconverter = cms.EDAnalyzer("EdmToTreeConverterW",
  
                              Nbins_histo = cms.untracked.int32(40),
                             Inibin_histo = cms.untracked.int32(20),
                             Endbin_histo = cms.untracked.int32(90),
                             # charge
                             Charge = cms.untracked.int32(-1),
                             # eta bin 
                             EtaBin = cms.untracked.int32(0)
                                            )


process.path = cms.Path(
  process.edmtotreeconverter
)
