import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")
# Example looking at MC truth flavour of JetTag objects from b tag.
#keep the logging output to a nice level
process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/mc/2007/10/20/RelVal-RelValTTbar-1192895175/0000/00C41641-2A81-DC11-B6EA-0019DB29C620.root')
)

process.btagMCTest = cms.EDFilter("BtagMCTest",
    moduleLabel = cms.string('trackCountingHighEffJetTags'),
    jetIdParameters = cms.PSet(
        vetoFlavour = cms.vstring(),
        rejectBCSplitting = cms.bool(False),
        physicsDefinition = cms.bool(False),
        coneSizeToAssociate = cms.double(0.3),
        fillLeptons = cms.bool(False),
        fillHeavyHadrons = cms.bool(False),
        fillPartons = cms.bool(True),
        mcSource = cms.string('source')
    )
)

process.p = cms.Path(process.btagMCTest)


