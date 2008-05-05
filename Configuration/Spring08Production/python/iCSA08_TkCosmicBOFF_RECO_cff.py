import FWCore.ParameterSet.Config as cms

def customise(process):

    process.include("Configuration/GlobalRuns/data/ReconstructionGR.cff")

    process.reconstruction_step._seq = process.reconstructionGR

    # switch off magnetic field
    process.UniformMagneticFieldESProducer = cms.ESProducer("UniformMagneticFieldESProducer", ZFieldInTesla = cms.double(0.0) )
    process.es_prefer_UniformMagneticFieldESProducer = cms.ESPrefer("UniformMagneticFieldESProducer")
            
    return process
