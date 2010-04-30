import FWCore.ParameterSet.Config as cms

process = cms.Process("VgKit")

## Message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('START3X_V26::All')
process.load("Configuration.StandardSequences.MagneticField_cff")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
    )

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring(['file:testZMuMuGammaSubskim.root']),
  noEventSort = cms.untracked.bool(True),
  duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)

process.load("ElectroWeakAnalysis.MultiBosons.VgAnalyzerKit_cfi")
#process.VgAnalyzerKit.saveHLTInfo = cms.untracked.bool(False);
# process.VgAnalyzerKit.doPhoton       = True
# process.VgAnalyzerKit.doElectron     = False
# process.VgAnalyzerKit.doMuon         = True
# process.VgAnalyzerKit.doTau          = False
# process.VgAnalyzerKit.doJet          = False
# process.VgAnalyzerKit.doMET          = False
# process.VgAnalyzerKit.doTrack        = False
# process.VgAnalyzerKit.doGenParticles = False
process.VgAnalyzerKit.doZmumu = True
# process.VgAnalyzerKit.photonSrc = "selectedPatPhotons"
# process.VgAnalyzerKit.electronSrc = "selectedPatElectrons"
# process.VgAnalyzerKit.muonSrc = "selectedPatMuons"
process.VgAnalyzerKit.muonSrc   = "vgMuonsGlobal"
# process.VgAnalyzerKit.tauSrc = "selectedPatTaus"
# process.VgAnalyzerKit.jetSrc = "selectedPatJets"
process.VgAnalyzerKit.zmumuSrc   = "dimuonsGlobal"


process.TFileService = cms.Service("TFileService", fileName = cms.string('vgtree.root'))

process.p = cms.Path(process.VgAnalyzerKit)

## Add tab-completion during the inspection
if __name__ == "__main__":
  import user
