import FWCore.ParameterSet.Config as cms

process = cms.Process("ZTauTauSkim")

process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 5000
process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START3X_V26::All')


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/home/llr/cms/akalinow/Test/ACF6135C-DE46-DF11-8733-0026189438BF.root'
    )
)


makeDiTauSkimmFlag = True

makeMuTauSkimmFlag = True

makeElecTauSkimmFlag = True

makeElecMuSkimmFlag = True

makeWtaunuSkimmFlag = False

process.dummyModule = cms.EDProducer("DummyModule")

process.validation = cms.Path(process.dummyModule)

if makeDiTauSkimmFlag:
    from TauAnalysis.Skimming.skimDiTau_cff import *
    addDiTauSkimm(process)

if makeMuTauSkimmFlag:
    from TauAnalysis.Skimming.skimMuTau_cff import *
    addMuTauSkimm(process)
    process.muTauValidation = cms.Sequence(
        process.fillMuTauValPlots
        + process.saveMuTauValPlots
        )
    process.validation+=process.muTauValidation    

if makeElecTauSkimmFlag:
    from TauAnalysis.Skimming.skimElecTau_cff import *
    addElecTauSkimm(process)
    process.elecTauValidation = cms.Sequence(
        process.fillElecTauValPlots
        + process.saveElecTauValPlots
        )
    process.validation+=process.elecTauValidation    

if makeElecMuSkimmFlag:
    from TauAnalysis.Skimming.skimElecMu_cff import *
    addElecMuSkimm(process)
    process.elecMuValidation = cms.Sequence(
        process.fillElecMuValPlots
        + process.saveElecMuValPlots
        )
    process.validation+=process.elecMuValidation    


if makeWtaunuSkimmFlag:
    from TauAnalysis.Skimming.skimWTauNu_cff import *
    addWTauNuSkimm(process)
