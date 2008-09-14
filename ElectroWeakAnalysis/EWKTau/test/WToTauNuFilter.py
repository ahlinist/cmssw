import FWCore.ParameterSet.Config as cms

process = cms.Process("WToTauNuFilter")

process.load("FWCore/MessageService/MessageLogger_cfi")
# process.MessageLogger.destinations = cms.untracked.vstring("pyDetailedInfo.txt")
#process.options = cms.untracked.PSet(
#    wantSummary = cms.untracked.bool(True)
#)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/relval/CMSSW_2_1_7/RelValZTT/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/0A4AD6FC-AC7E-DD11-9772-001617C3B6C6.root'
     )
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

process.load("Configuration/StandardSequences/FakeConditions_cff")
#process.load("Configuration/StandardSequences/FrontierConditions_GlobalTag_cff")
#process.load("Configuration/StandardSequences/FrontierConditions_GlobalTag_noesprefer_cff")

process.load("Configuration.StandardSequences.GeometryPilot2_cff")
#process.load("Configuration.StandardSequences.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
process.load("RecoTauTag/RecoTau/PFTauProducer_cff")
process.load("RecoTauTag/RecoTau/PFRecoTauDiscriminationByIsolation_cfi")


process.TFileService = cms.Service("TFileService",
    fileName = cms.string('WToTauNuFilterHistograms.root')
)

##########################################################################################
# The choice of MET and Jet cuts are based on the slides presented at                    #
# http://pages.physics.cornell.edu/~souvik/CMSTau/Presentations/WTauNu8September2008.pdf #
##########################################################################################
process.wToTauNuFilter1 = cms.EDFilter("WToTauNuFilter",
    Display = cms.bool(True),
    METSource = cms.InputTag("met"),
    CaloJetSource = cms.InputTag("iterativeCone5GenJets"),
    TauSource = cms.InputTag("caloRecoTauProducer"),
    METCut = cms.double(40),
    JetCut1 = cms.double(40),
    JetCut2 = cms.double(12)
)


process.p = cms.Path( process.ic5PFJetTracksAssociatorAtVertex*
                      process.pfRecoTauTagInfoProducer*
                      process.pfRecoTauProducer*
                      process.pfRecoTauDiscriminationByIsolation
                      )
process.p1 = cms.Path(process.wToTauNuFilter1)

process.o1 = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring('keep *', 
        'drop FEDRaw*_*_*_*', 
        'drop PSimHits*_*_*_*', 
        'drop PCaloHits*_*_*_*', 
        'drop recoHLT*_mix_*_*', 
        'keep Sim*_*_*_*', 
        'keep *_*_*_WToTauNuFilter'),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('p1')
    ),
    fileName = cms.untracked.string('WToTauNuSkim.root')
)

process.outpath = cms.EndPath(process.o1)

