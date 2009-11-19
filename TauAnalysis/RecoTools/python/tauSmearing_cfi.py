import FWCore.ParameterSet.Config as cms

distortedTaus        = cms.EDProducer('DistortedTauProducer',
                               src = cms.InputTag('finalTaus'),
                               fileName = cms.FileInPath('resolutions.root'),
                               smearMCParticle = cms.bool(False),
                               smearConstituents = cms.bool(False),              
                               smearFromPtHistogram = cms.bool(False),
                               smearFromEtaHistogram = cms.bool(False),
                               ptHistogram = cms.string('res1_1/deltaPtAll'),
                               etaHistogram = cms.string('res1_1/deltaEtaAll'),
                               energyScale = cms.double(1.05),
                               hadronEnergyScale = cms.double(1.00),
                               gammaEnergyScale = cms.double(1.00),
                               deltaEta = cms.double(0.0),
                               deltaPt = cms.double(0.0),
                               gaussianSmearingSigmaPt =cms.double(0.0),
                               gaussianSmearingSigmaEta = cms.double(0.0)
)                               
                            
####
#### WARNING: To run the smearing modules you need the random number generator service
####If you need it in other stufff, remove it from here but add the PSET to the global random Number configuration

RandomNumberGeneratorService = cms.Service(
        "RandomNumberGeneratorService",

            # To save the status of the last event (useful for crashes)
            # Just give a name to the file you want the status to be saved
            # otherwise just put saveFileName = ""
            saveFileName = cms.untracked.string(''),

            # To restore the status of the last event, just un-comment the
            # following line (and comment the saveFileName line!)
            # restoreFileName = cms.string('RandomEngineState.log'),

            # To reproduce events using the RandomEngineStateProducer (source
            # excluded), comment the sourceSeed definition, and un-comment
            # the restoreStateLabel
            # restoreStateLabel = cms.string('randomEngineStateProducer'),

            # This is to initialize the random engine of the source
            distortedTausEnergyScale105 = cms.PSet(
            initialSeed = cms.untracked.uint32(987346),
                    engineName = cms.untracked.string('TRandom3')
                ),
            distortedTausEnergyScale110 = cms.PSet(
            initialSeed = cms.untracked.uint32(234235),
                    engineName = cms.untracked.string('TRandom3')
                ),
            distortedTausEnergyScale120 = cms.PSet(
            initialSeed = cms.untracked.uint32(765856),
                    engineName = cms.untracked.string('TRandom3')
                ),
            distortedTausDispl = cms.PSet(
            initialSeed = cms.untracked.uint32(1234124),
                    engineName = cms.untracked.string('TRandom3')
                ),
            distortedTausDisplIn = cms.PSet(
            initialSeed = cms.untracked.uint32(2345234),
                    engineName = cms.untracked.string('TRandom3')
                )

        )

randomEngineStateProducer = cms.EDProducer("RandomEngineStateProducer")
