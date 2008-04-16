import FWCore.ParameterSet.Config as cms

def customise(process):

  #Geometry
  # add the description of the Alignment Tubes
  process.XMLIdealGeometryESSource.geomXMLFiles.append("Alignment/LaserAlignmentSimulation/data/AlignmentTubes.xml")

  process.g4SimHits.Physics.type = "SimG4Core/Physics/LaserOpticalPhysics"

  process.g4SimHits.Watchers = cms.VPSet( 
    cms.PSet( 
       type = cms.string("LaserAlignmentSimulation"),
       DebugLevel = cms.untracked.int32(0),
       MaterialPropertiesDebugLevel = cms.untracked.int32(1),
       SiAbsorptionLengthScalingFactor = cms.untracked.double(1.0),
       EnergyLossScalingFactor = cms.untracked.double(1739.130435),
       PhotonEnergy = cms.untracked.double(1.15),
       NumberOfPhotonsInParticleGun = cms.untracked.int32(50),
       NumberOfPhotonsInEachBeam = cms.untracked.int32(50),
    )
  )
  

  return process
