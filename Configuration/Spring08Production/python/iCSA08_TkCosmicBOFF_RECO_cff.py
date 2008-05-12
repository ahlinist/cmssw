import FWCore.ParameterSet.Config as cms

def customise(process):

    # switch off magnetic field
    process.UniformMagneticFieldESProducer = cms.ESProducer("UniformMagneticFieldESProducer", ZFieldInTesla = cms.double(0.0) )
    process.es_prefer_UniformMagneticFieldESProducer = cms.ESPrefer("UniformMagneticFieldESProducer")

    process.SteppingHelixPropagatorAny.useInTeslaFromMagField = True
    process.SteppingHelixPropagatorAlong.useInTeslaFromMagField = True
    process.SteppingHelixPropagatorOpposite.useInTeslaFromMagField = True

    process.SteppingHelixPropagatorAny.SetVBFPointer = True
    process.SteppingHelixPropagatorAlong.SetVBFPointer = True
    process.SteppingHelixPropagatorOpposite.SetVBFPointer = True

    process.VolumeBasedMagneticFieldESProducer.label = "VolumeBasedMagneticField"
                     
            
    return process
