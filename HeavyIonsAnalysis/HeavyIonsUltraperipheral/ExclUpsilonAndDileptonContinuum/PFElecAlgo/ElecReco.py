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

#process.load("Configuration.StandardSequences.FakeConditions_cff")
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')

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

#process.load("Configuration.StandardSequences.VtxSmearedNoSmear_cff")
#process.load("Configuration.StandardSequences.VtxSmearedBetafuncEarlyCollision_cff")
process.load("Configuration.StandardSequences.VtxSmearedEarly10TeVCollision_cff")


process.load("Configuration.StandardSequences.Reconstruction_cff")

# Event output
process.load("Configuration.EventContent.EventContent_cff")

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(50000)
        )


process.source = cms.Source("MCFileSource",catalog=cms.untracked.string('PoolFileCatalog.xml'),
                            fileNames  = cms.untracked.vstring('file:starlight_pbpb_eecont.hepmc-011.dat')
                            )



#****************************************************************************************************************
process.MessageLogger = cms.Service("MessageLogger",
                                        reco = cms.untracked.PSet(
            threshold = cms.untracked.string('DEBUG')
                ),
                                        destinations = cms.untracked.vstring('reco')
                                    )


process.FEVT = cms.OutputModule('PoolOutputModule',
                                #   process.FEVTSIMEventContent,
                                #  fileName = cms.untracked.string('rfio:/castor/cern.ch/user/k/kumarv/LowPtElec/Up1s_ee_flow_01.root')
                                fileName = cms.untracked.string('file:pbpb_eecont_flow_011.root'),
                                outputCommands = cms.untracked.vstring(
    'drop *',
    'keep recoPFRecHits_*_*_*',
    'keep recoPFClusters_*_*_*',
    'keep FEDRawDataCollection_rawDataCollector_*_RAW',
    'keep recoPFRecTracks_*_*_*',
    'keep recoPFBlocks_particleFlowBlock_*_*',
    'keep recoPFCandidates_particleFlow_*_*',
    'keep recoCandidatesOwned_*_*_*',
    'keep recoPFSimParticles_*_*_*',
    'keep recoTracks_*_*_*',
    'keep recoCaloJets_*_*_*',
    'keep CaloTowersSorted_*_*_*',
    'keep edmHepMCProduct_*_*_*',
    'keep recoBasicClusters_*_*_*',
    'keep recoSuperClusters_*_*_*',
    'keep recoTracks_*_*_*',
    'keep recoElectrons_*_*_*',
    'keep recoGsfElectrons_*_*_*',
    )
)
process.electronChi2.MaxChi2 = 2000
process.electronChi2.nSigma = 3
process.pfTrackElec.ModeMomentum = True
process.particleFlow.usePFElectrons = True
#process.particleFlow.usePFElectrons = ('False')
process.GlobalTag.globaltag = 'IDEAL_V9::All'



process.p0 = cms.Path(process.pgen)
process.p1 = cms.Path(process.psim)
process.p2 = cms.Path(process.pdigi)
process.p3 = cms.Path(process.L1Emulator)
process.p4 = cms.Path(process.DigiToRaw)
process.p5= cms.Path(process.RawToDigi)
process.p6= cms.Path(process.reconstruction)
process.outpath = cms.EndPath(process.FEVT)
process.schedule = cms.Schedule(process.p0,process.p1,process.p2,process.p3,process.p4,process.p5,process.p6,process.outpath)
