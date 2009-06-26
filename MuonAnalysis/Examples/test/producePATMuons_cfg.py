import FWCore.ParameterSet.Config as cms

process = cms.Process("PATMuon")

########## Messages ##########
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('PATSummaryTables')
process.MessageLogger.cerr.INFO = cms.untracked.PSet(default = cms.untracked.PSet( limit = cms.untracked.int32(0)),
                                                     PATSummaryTables = cms.untracked.PSet( limit = cms.untracked.int32(-1))
                                                     )
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
##############################

########## Services ##########
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'IDEAL_V9::All'
##############################

########## Input ##########
process.source = cms.Source("PoolSource", 
                            fileNames = cms.untracked.vstring(
    '/store/relval/CMSSW_2_2_1/RelValH200ZZ4L/GEN-SIM-RECO/IDEAL_V9_v1/0004/B492FC1B-06C5-DD11-93B9-000423D33970.root',
    '/store/relval/CMSSW_2_2_1/RelValH200ZZ4L/GEN-SIM-RECO/IDEAL_V9_v1/0003/FE23467D-DFC4-DD11-8E4E-000423D94C68.root',
    '/store/relval/CMSSW_2_2_1/RelValH200ZZ4L/GEN-SIM-RECO/IDEAL_V9_v1/0003/F068280F-DFC4-DD11-9D9D-000423D95030.root',
    '/store/relval/CMSSW_2_2_1/RelValH200ZZ4L/GEN-SIM-RECO/IDEAL_V9_v1/0003/E4D72243-DFC4-DD11-AEE8-000423D944F0.root',
    '/store/relval/CMSSW_2_2_1/RelValH200ZZ4L/GEN-SIM-RECO/IDEAL_V9_v1/0003/E0191DE9-E7C4-DD11-81B6-000423D99996.root',
    '/store/relval/CMSSW_2_2_1/RelValH200ZZ4L/GEN-SIM-RECO/IDEAL_V9_v1/0003/DC6C0164-DFC4-DD11-8213-000423D9880C.root',
    '/store/relval/CMSSW_2_2_1/RelValH200ZZ4L/GEN-SIM-RECO/IDEAL_V9_v1/0003/CC3A8509-DFC4-DD11-A744-001617DF785A.root',
    '/store/relval/CMSSW_2_2_1/RelValH200ZZ4L/GEN-SIM-RECO/IDEAL_V9_v1/0003/BAE83179-EFC4-DD11-8193-001D09F28755.root',
    '/store/relval/CMSSW_2_2_1/RelValH200ZZ4L/GEN-SIM-RECO/IDEAL_V9_v1/0003/AE913083-EFC4-DD11-925A-001D09F29524.root',
    '/store/relval/CMSSW_2_2_1/RelValH200ZZ4L/GEN-SIM-RECO/IDEAL_V9_v1/0003/9C3BBE27-DFC4-DD11-850C-000423D9989E.root',
    '/store/relval/CMSSW_2_2_1/RelValH200ZZ4L/GEN-SIM-RECO/IDEAL_V9_v1/0003/8AF5D00D-DFC4-DD11-B48D-0019DB2F3F9A.root',
    '/store/relval/CMSSW_2_2_1/RelValH200ZZ4L/GEN-SIM-RECO/IDEAL_V9_v1/0003/7E1ECA82-EFC4-DD11-8F97-001D09F251E0.root',
    '/store/relval/CMSSW_2_2_1/RelValH200ZZ4L/GEN-SIM-RECO/IDEAL_V9_v1/0003/606E4E82-DFC4-DD11-97C8-000423D9A2AE.root',
    '/store/relval/CMSSW_2_2_1/RelValH200ZZ4L/GEN-SIM-RECO/IDEAL_V9_v1/0003/4AC53B3B-DFC4-DD11-8374-000423D174FE.root',
    '/store/relval/CMSSW_2_2_1/RelValH200ZZ4L/GEN-SIM-RECO/IDEAL_V9_v1/0003/48369A9E-EFC4-DD11-AB71-001D09F231B0.root',
    '/store/relval/CMSSW_2_2_1/RelValH200ZZ4L/GEN-SIM-RECO/IDEAL_V9_v1/0003/3826DF39-DFC4-DD11-9377-001617C3B6E8.root',
    '/store/relval/CMSSW_2_2_1/RelValH200ZZ4L/GEN-SIM-RECO/IDEAL_V9_v1/0003/34EB3B35-DFC4-DD11-AE23-001617DBD556.root',
    '/store/relval/CMSSW_2_2_1/RelValH200ZZ4L/GEN-SIM-RECO/IDEAL_V9_v1/0003/2C87655E-DFC4-DD11-AEAB-000423D98AF0.root',
    '/store/relval/CMSSW_2_2_1/RelValH200ZZ4L/GEN-SIM-RECO/IDEAL_V9_v1/0003/2223FC17-DFC4-DD11-B2EB-001617E30D40.root',
    '/store/relval/CMSSW_2_2_1/RelValH200ZZ4L/GEN-SIM-RECO/IDEAL_V9_v1/0003/14C5C482-EFC4-DD11-A9D8-001D09F24EC0.root',
    '/store/relval/CMSSW_2_2_1/RelValH200ZZ4L/GEN-SIM-RECO/IDEAL_V9_v1/0003/0EA66B5C-DFC4-DD11-A316-000423D94C68.root',
    '/store/relval/CMSSW_2_2_1/RelValH200ZZ4L/GEN-SIM-RECO/IDEAL_V9_v1/0003/0E34790C-DFC4-DD11-BB22-001617C3B6E2.root',
    '/store/relval/CMSSW_2_2_1/RelValH200ZZ4L/GEN-SIM-RECO/IDEAL_V9_v1/0003/0C9A326E-F6C4-DD11-AFDD-001617E30D38.root'
    )
                            )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(15000) )
##############################

########## Output and its Event Content ##########
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('PATLayer1_Output.fromAOD_full_HIGGS200GeV.root'),
                               # save only events passing the full path
                               SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
                               # save PAT Layer 1 output. You need a '*' to unpack the list of commands 'patEventContent'
                               outputCommands = cms.untracked.vstring('drop *', *patEventContent)
                               )

# # make the links within the pat::Muon working
# RecoMuonPAT = cms.PSet(outputCommands = cms.untracked.vstring('keep recoTracks_standAloneMuons_*_*', 
#                                                               'keep recoTracks_globalMuons_*_*', 
#                                                               'keep recoTracks_tevMuons_*_*', 
#                                                               'keep recoTracks_generalTracks_*_*', 
#                                                               'keep recoMuons_muons_*_*', 
#                                                               'keep recoCaloMuons_calomuons_*_*',
#                                                               # GEN
#                                                               'keep *_genParticles_*_*'
#                                                               )
#                        )
# process.out.outputCommands.extend(RecoMuonPAT.outputCommands)
##############################


########## Fill PAT Information ##########
process.load("PhysicsTools.PatAlgos.patSequences_cff")
# Switch off old trigger matching
from PhysicsTools.PatAlgos.tools.trigTools import switchOffTriggerMatchingOld
switchOffTriggerMatchingOld( process )


process.allLayer1Muons.embedTrack = cms.bool(True)
process.allLayer1Muons.embedGenMatch = cms.bool(True)


# PAT Layer 0+1
process.p = cms.Path(
    process.patDefaultSequence  
)

process.outpath = cms.EndPath(process.out)
