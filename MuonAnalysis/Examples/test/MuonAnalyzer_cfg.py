import FWCore.ParameterSet.Config as cms

process = cms.Process("RecoMuon")
# Messages
process.load("FWCore.MessageService.MessageLogger_cfi")


process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

process.GlobalTag.globaltag = 'IDEAL_V9::All'

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring('file:/afs/cern.ch/cms/Physics/muon/MPOG/Tutorial/PATLayer1_Output.fromAOD_full_HIGGS200GeV.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.TFileService=cms.Service('TFileService',
                                 fileName=cms.string('MyMuonPlots_HIGGS200GeV.root')
                                 )


process.muonAnalysis = cms.EDAnalyzer("MuonAnalyzer",
                                      MuonCollection = cms.untracked.string('cleanLayer1Muons'),
                                      )


process.p = cms.Path(process.muonAnalysis)




