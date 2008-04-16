import FWCore.ParameterSet.Config as cms

def customise(process):
    # Pixel digitization: set the TOF window for digitizer to accept hits, 
    # window should stay fixed to 25 ns, shift of TOF for the strip tracker 

    process.siPixelDigis.TofLowerCut = 18.5  
    process.siPixelDigis.TofUpperCut = 43.5
    process.siStripDigis.CosmicDelayShift = 31.
    process.siStripDigis.CouplingCostantPeak = (0.94, 0.03) #FROM TIF DATA

    # switch off magnetic field
    process.UniformMagneticFieldESProducer = cms.ESProducer("UniformMagneticFieldESProducer", ZFieldInTesla = cms.double(0.0) )
    process.es_prefer_UniformMagneticFieldESProducer = cms.ESPrefer("UniformMagneticFieldESProducer")
    process.g4SimHits.UseMagneticField = False

    return process
