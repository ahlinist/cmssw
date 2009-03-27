import FWCore.ParameterSet.Config as cms

process = cms.Process('GEN')

process.load('Configuration/StandardSequences/Services_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
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


process.generation_step = cms.Path(process.generator)


# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('TTbar_cfi_GEN.root'),
                                  )

# Path and EndPath definitions

process.out_step = cms.EndPath(process.output)

