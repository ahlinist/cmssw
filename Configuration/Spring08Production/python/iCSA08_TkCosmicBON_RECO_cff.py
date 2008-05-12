import FWCore.ParameterSet.Config as cms

def customise(process):


    process.es_prefer_VolumeBasedMagneticFieldESProducer = cms.ESPrefer("VolumeBasedMagneticFieldESProducer")
            
    return process
