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
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/EEC0BEF0-60F7-DD11-8276-001D096460D5.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/F26DD43F-73F7-DD11-85A8-001D096460D5.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/8C5620F9-77F7-DD11-A8F8-00188B7431B9.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/0AADF08A-5AF7-DD11-8D95-003048C26CB8.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/C8E45D5D-7FF7-DD11-8F1B-00E081334A48.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/0613F564-74F7-DD11-A327-001AA02EAB72.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/5C32C6C9-64F7-DD11-AFE9-001D0964474D.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/BA27309C-61F7-DD11-A213-002219826BD1.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/78102346-5FF7-DD11-A28E-001EC9E14626.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/16A8369B-61F7-DD11-9E74-002219826BCD.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/34D2AB3F-5FF7-DD11-B8C1-001EC9E14A7D.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/FCE4834B-5FF7-DD11-B102-001EC9E14626.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/A2368173-60F7-DD11-A434-0022198273C8.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/1CD7C0ED-5EF7-DD11-B312-000AE488BD0E.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/02D6E33F-64F7-DD11-AB1D-00188B89E7A4.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/A457D241-73F7-DD11-A9B6-001D09646131.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/7222083B-65F7-DD11-9ECA-0015C5E673B3.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/B0E7B863-74F7-DD11-AE4F-00188B743681.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/3AA9DD44-69F7-DD11-A93F-0015C5E5B961.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/6A6E3E08-5FF7-DD11-9145-000AE4887F77.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/385E51F7-76F7-DD11-B8C6-00188B89E8E3.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/E889AEA8-5FF7-DD11-96F2-001D096460D5.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/E873C065-74F7-DD11-845C-00188B89E7A4.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/3EE5EE79-60F7-DD11-A309-002219826BCD.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/A0F07BD6-5EF7-DD11-9BEE-003048C180D8.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/E8FBFA9C-61F7-DD11-AE14-0022198266CB.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/2A0AC3FA-5EF7-DD11-8BED-000AE4887F77.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/B2907C41-73F7-DD11-926C-002219501D1D.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/22E9CD63-5FF7-DD11-BCA6-003048C17FBC.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/445DF76F-61F7-DD11-AFDD-00145ED6E852.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/16963529-5FF7-DD11-BB39-00E081450490.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/B470E162-74F7-DD11-BB9A-00188B89E8E3.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/68714F80-61F7-DD11-8DA3-003048C26CB8.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/08B55043-73F7-DD11-9C2A-0019B9F3FF70.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/020B2FF2-5EF7-DD11-90F0-003048C17FBC.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/1E1FEA64-74F7-DD11-9822-00188B897C9A.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/344F2DAF-64F7-DD11-8B7D-00221981B434.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/184C4164-74F7-DD11-85F3-00188B89EEFA.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/905E02F7-77F7-DD11-9C6A-00188B7431B9.root',
    '/store/mc/Summer08/Zmumu_M20/GEN-SIM-RECO/IDEAL_V9_reco-v2/0000/E811FE9C-61F7-DD11-9FC2-0022198266CB.root'
    )
                            )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(20000) )
##############################

########## Output and its Event Content ##########
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('/tmp/PATLayer1_Output.fromAOD_full_Zmumu.root'),
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
