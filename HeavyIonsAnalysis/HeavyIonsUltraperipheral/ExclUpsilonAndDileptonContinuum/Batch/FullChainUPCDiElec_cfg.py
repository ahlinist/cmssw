import FWCore.ParameterSet.Config as cms

process = cms.Process("Rec")
# this example configuration offers some minimum 
# annotation, to help users get through; please
# don't hesitate to read through the comments
# use MessageLogger to redirect/suppress multiple
# service messages coming from the system
#
# in this config below, we use the replace option to make
# the logger let out messages of severity ERROR (INFO level
# will be suppressed), and we want to limit the number to 10
#
process.load("Configuration.StandardSequences.Services_cff")

process.load("Configuration.StandardSequences.Geometry_cff")

process.load("Configuration.StandardSequences.MagneticField_cff")

process.load("Configuration.StandardSequences.FakeConditions_cff")

process.load("FWCore.MessageService.MessageLogger_cfi")

# this config frament brings you the generator information
process.load("Configuration.StandardSequences.Generator_cff")

# this config frament brings you 3 steps of the detector simulation:
# -- vertex smearing (IR modeling)
# -- G4-based hit level detector simulation
# -- digitization (electronics readout modeling)
# it returns 2 sequences : 
# -- psim (vtx smearing + G4 sim)
# -- pdigi (digitization in all subsystems, i.e. tracker=pix+sistrips,
#           cal=ecal+ecal-0-suppression+hcal), muon=csc+dt+rpc)
#
process.load("Configuration.StandardSequences.Simulation_cff")

# please note the IMPORTANT: 
# in order to operate Digis, one needs to include Mixing module 
# (pileup modeling), at least in the 0-pileup mode
#
# There're 3 possible configurations of the Mixing module :
# no-pileup, low luminosity pileup, and high luminosity pileup
#
# they come, respectively, through the 3 config fragments below
#
# *each* config returns label "mix"; thus you canNOT have them
# all together in the same configuration, but only one !!!
#
process.load("Configuration.StandardSequences.MixingNoPileUp_cff")

#include "Configuration/StandardSequences/data/MixingLowLumiPileUp.cff" 
#include "Configuration/StandardSequences/data/MixingHighLumiPileUp.cff" 
process.load("Configuration.StandardSequences.L1Emulator_cff")

process.load("Configuration.StandardSequences.DigiToRaw_cff")

process.load("Configuration.StandardSequences.RawToDigi_cff")

process.load("Configuration.StandardSequences.VtxSmearedBetafuncEarlyCollision_cff")

process.load("Configuration.StandardSequences.Reconstruction_cff")
#*************************************************************************************


#process.replace("islandBasicClusters.IslandBarrelSeedThr = 0.18")

IslandBarrelSeedThr=cms.double(0.18)
seedTransverseEnergyThreshold =cms.double(0.50)
barrelPhiSearchRoad =cms.double(1.5)
endcapPhiSearchRoad =cms.double(0.8)
ePhiMin1=cms.double(-0.175)
ePhiMax1=cms.double(0.1)
pPhiMin1=cms.double(-0.1)
pPhiMin1=cms.double(0.175)
SCEtCut =cms.double(1.0)

###CriticalEnergyForVacuum = cms.double(1.0) #changed
###mySequence.replace(oldModule, newModule)
###myPath.replace(oldSequence, newSequence)

#************************************************************************************

# Event output
process.load("Configuration.EventContent.EventContent_cff")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(50000)
)

process.source = cms.Source("MCFileSource",
                            catalog    = cms.untracked.string('PoolFileCatalog.xml'),
                            fileNames  = cms.untracked.vstring('file:starlight_pbpb_eecont.hepmc-019.dat'),
                           #skipEvents = cms.untracked.uint32(25000)

                                                        )


process.FEVT = cms.OutputModule("PoolOutputModule",
   #process.FEVTSIMEventContent,
    fileName = cms.untracked.string('pbpb-eecont-reco-019_root'),
    outputCommands = cms.untracked.vstring(
      'drop *',
      'keep edmHepMCProduct_*_*_*',
      'keep recoBasicClusters_*_*_*',
      'keep recoSuperClusters_*_*_*',
      'keep recoTracks_*_*_*',
      'keep recoElectrons_*_*_*',
      'keep recoGsfElectrons_*_*_*',
       ) 
         )

process.p0 = cms.Path(process.pgen)
process.p1 = cms.Path(process.psim)
process.p2 = cms.Path(process.pdigi)
process.p3 = cms.Path(process.L1Emulator)
process.p4 = cms.Path(process.DigiToRaw)
process.p5= cms.Path(process.RawToDigi)
process.p6= cms.Path(process.reconstruction)
process.outpath = cms.EndPath(process.FEVT)
process.schedule = cms.Schedule(process.p0,process.p1,process.p2,process.p3,process.p4,process.p5,process.p6,process.outpath)


