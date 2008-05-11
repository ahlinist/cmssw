import FWCore.ParameterSet.Config as cms

def customise(process):

    # switch off magnetic field
    process.UniformMagneticFieldESProducer = cms.ESProducer("UniformMagneticFieldESProducer", ZFieldInTesla = cms.double(0.0) )
    process.es_prefer_UniformMagneticFieldESProducer = cms.ESPrefer("UniformMagneticFieldESProducer")
    process.g4SimHits.UseMagneticField = False

    process.hcalDigis.hbhe.level = -10000
    process.hcalDigis.ho.level   = -10000
    process.hcalDigis.hf.level   = -10000

    return process
