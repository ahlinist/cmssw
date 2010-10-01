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
    "file:EdmZmmTreeMC.root",
    "file:EdmZmmTreeData.root"
    )
                           
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.TFileService = cms.Service("TFileService", 
                                   fileName = cms.string("treeMC_histoData.root"),
                                   closeFileFast = cms.untracked.bool(True)
                                   )

process.edmtotreeconverter = cms.EDAnalyzer("EdmToTreeConverterZ",
#                                            UseSTA = cms.untracked.bool(True),
                                            Doups = cms.untracked.bool(False),
                                            Nbins_histomassZ = cms.untracked.int32(40),
                                            Inibin_histomassZ = cms.untracked.int32(70),
                                            Endbin_histomassZ = cms.untracked.int32(110),
                                            Nbins_histomassU = cms.untracked.int32(10),
                                            Inibin_histomassU = cms.untracked.int32(6),
                                            Endbin_histomassU = cms.untracked.int32(8)
                                            )


process.path = cms.Path(
  process.edmtotreeconverter
)
