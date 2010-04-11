import FWCore.ParameterSet.Config as cms

rctValParticlesFromZ = cms.EDProducer("GenParticlePruner",
                 src = cms.InputTag("genParticles"),
                 select = cms.vstring(
                "drop * ", # this is the default
                "keep+ pdgId = {Z0}",
                "drop pdgId = {Z0}",
                 "drop pdgId = {Z0} & status = 2"
                )
)


rctValElectronsFromZ = cms.EDProducer("GenParticlePruner",
        src = cms.InputTag("rctValParticlesFromZ"),
        select = cms.vstring(
              "drop * ", # this is the default
              "keep pdgId = {e+} & pt>10 & abs(eta)<2.5",
              "keep pdgId = {e-} & pt>10 & abs(eta)<2.5"
          )
)

rctValEGammaAll = cms.EDProducer("GenParticlePruner",
        src = cms.InputTag("genParticles"),
        select = cms.vstring(
              "drop * ", # this is the default
              "keep pdgId = {e+}& pt>10 & abs(eta)<2.5 & status=1",
              "keep pdgId = {e-}& pt>10 & abs(eta)<2.5 & status=1",
              "keep pdgId = 22 & pt>10 & abs(eta)<2.5  & status=1"
          )
)



rctZeeVal = cms.EDAnalyzer('RctValidation',
                egamma = cms.InputTag('simRctDigis'),
                genEGamma = cms.InputTag("rctValElectronsFromZ"),
                directory = cms.string("L1TEMU/RCTRelVal/electronsFromZ")
)                               

rctInclusiveVal = cms.EDAnalyzer('RctValidation',
                egamma = cms.InputTag('simGctDigis'),
                genEGamma = cms.InputTag("rctValEGammaAll"),
                directory = cms.string("L1TEMU/RCTRelVal/allElectrons")
)                               


rctValHarvesting = cms.EDAnalyzer('RctHarvesting',
                                  directories = cms.vstring(
                                      'L1TEMU/RCTRelVal/electronsFromZ',
                                      'L1TEMU/RCTRelVal/allElectrons',
                                      'L1T/RCTPhotons/photons'
                                      )
)                                  


rctValQuality = cms.EDFilter("QualityTester",
                  qtList = cms.untracked.FileInPath('Validation/L1Trigger/data/rctQualityTests.xml'),
                  reportThreshold         = cms.untracked.string('black'),
                  prescaleFactor          = cms.untracked.int32(1),
                  getQualityTestsFromFile = cms.untracked.bool(True),
                  qtestOnEndJob           = cms.untracked.bool(True),
                  qtestOnEndLumi          = cms.untracked.bool(False),
                  testInEventloop         = cms.untracked.bool(False),
                  verboseQT               = cms.untracked.bool(False)
)

