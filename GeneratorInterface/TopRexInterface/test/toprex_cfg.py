import FWCore.ParameterSet.Config as cms

process = cms.Process('GEN')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/MixingNoPileUp_cff')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/Generator_cff')
process.load('Configuration/StandardSequences/VtxSmearedEarly10TeVCollision_cff')
process.load('Configuration/StandardSequences/EndOfProcess_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.110 $'),
    annotation = cms.untracked.string('TTbar_cfi nevts:100'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)

# Input source
process.source = cms.Source("EmptySource")

#generator definition
process.load("GeneratorInterface.Pythia6Interface.pythiaDefault_cff")
process.generator = cms.EDProducer("ToprexProducer",
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    pythiaPylistVerbosity = cms.untracked.int32(0),
    pythiaFrame = cms.string('CMS'),
    comEnergy = cms.double(14000.0),
    PythiaParameters = cms.PSet(
        # Default (mostly empty - to keep PYTHIA default) card file
        # Name of the set is "pythiaDefault"
        process.pythiaDefaultBlock,
        # User cards - name is "myParameters"
        myParameters = cms.vstring(
            'PMAS(5,1)=4.8   ! b quark mass',
            'PMAS(6,1)=172.3 ! t quark mass'
            # jets
            #"MSTU(53) = 0  ! no smearing energy",
            #"MSTU(54) = 3  ! massless jets"
        ),
        # This is a vector of ParameterSet names to be read, in this order
        # The first two are in the include files below
        # The last one are simply my additional parameters
        parameterSets = cms.vstring('pythiaDefault', 
            'myParameters')
    ),

    GeneratorParameters = cms.PSet(
        parameterSets =  cms.vstring( "generator"),
        generator = cms.vstring(
            "txpro(20) = 1           ! t bbar ",
            #"txsel = 1               !(D=1) ",  
            "dectop(1)  = 0          ! OFF t W down (D=1)" ,
            "dectop(2)  = 0          ! OFF t W down (D=1)" 
            #"dectop(11) = 1          ! OFF t G up   (D=-1)",
            #"IXpar(5) = 1            ! call PYCELL from PYTHIA"
        )
    )
)

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = process.RECOSIMEventContent.outputCommands,
    fileName = cms.untracked.string('TTbar_cfi_GEN.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string(''),
        filterName = cms.untracked.string('')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    )
)


# Other statements
process.GlobalTag.globaltag = 'IDEAL_30X::All'

# Path and EndPath definitions
process.generation_step = cms.Path(process.generator * process.pgen)
process.out_step = cms.EndPath(process.output)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step)
