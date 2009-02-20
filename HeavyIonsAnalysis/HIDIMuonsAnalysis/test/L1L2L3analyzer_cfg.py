import FWCore.ParameterSet.Config as cms

process = cms.Process("MAna")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
			'dcache:/pnfs/cmsaf.mit.edu/hibat/cms/users/jkim/dhmoon/cms2111/HLT_pp_Jpsi/HLT_DoubleMu3_pp_Jpsi_DoubleMuOpen_2111_0001.root'
    )
)

process.L1L2L3DimuonTrig = cms.EDAnalyzer('L1L2L3Analyzer',
L1muons = cms.untracked.InputTag("hltL1extraParticles"),
L2muons = cms.untracked.InputTag("hltL2Muons"),
L3muons = cms.untracked.InputTag("hltL3Muons"),
#L3muons = cms.untracked.InputTag("preFilterZeroStepTracks"),
L2muonCands = cms.untracked.InputTag("hltL2MuonCandidates"),
L3muonCands = cms.untracked.InputTag("hltL3MuonCandidates"),
MinPt = cms.double( 0.0 ),
HistOutFile = cms.untracked.string("L1L2L3_jpsi_pp_DoubleMuOpen_v2_JpsiMM_e290k.root")
)

process.p = cms.Path(process.L1L2L3DimuonTrig)
