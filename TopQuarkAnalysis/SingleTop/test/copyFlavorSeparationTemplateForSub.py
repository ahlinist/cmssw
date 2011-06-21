import FWCore.ParameterSet.Config as cms

process = cms.Process("SingleTopSkimMerge")

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.source = cms.Source ("PoolSource",
                             fileNames = cms.untracked.vstring (
#'rfio:/castor/cern.ch/user/o/oiorio/SingleTop/SingleTop_tChan/TChanSampleTChanMu_9_1_S3X.root',
    #'file:/tmp/oiorio/00012F91-72E5-DF11-A763-00261834B5F1.root'
    #    'file:/tmp/oiorio/00012F91-72E5-DF11-A763-00261834B5F1.root'
    'file:/tmp/oiorio/TTBarMerged.root',
),
skipBadFiles = cms.untracked.bool(True),
duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)


## Load additional RECO config
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

from Configuration.PyReleaseValidation.autoCond import autoCond
process.GlobalTag.globaltag = autoCond['startup']


process.load("PhysicsTools.HepMCCandAlgos.flavorHistoryPaths_cfi")

process.wlightFilter = process.flavorHistoryFilter.clone(pathToSelect = cms.int32(11))

process.wccFilter = process.flavorHistoryFilter.clone(pathToSelect = cms.int32(6))

process.wccFilter1 = process.flavorHistoryFilter.clone(pathToSelect = cms.int32(3))
process.wccFilter2 = process.flavorHistoryFilter.clone(pathToSelect = cms.int32(4))

process.wcFilter = process.flavorHistoryFilter.clone(pathToSelect = cms.int32(4))

process.wbbFilter = process.flavorHistoryFilter.clone(pathToSelect = cms.int32(5))

process.wbbFilter1 = process.flavorHistoryFilter.clone(pathToSelect = cms.int32(1))
process.wbbFilter2 = process.flavorHistoryFilter.clone(pathToSelect = cms.int32(2))


switch = "switch_instruction" #SWITCH_INSTRUCTION

if switch == "wlight":
    process.Pathwlight = cms.Path(
        process.wlightFilter 
        )

    process.Pathwcc = cms.Path(
        process.wccFilter 
        )

    process.Pathwbb = cms.Path(
        process.wbbFilter 
        )
    process.skimwlight = cms.OutputModule("PoolOutputModule",
                                          fileName = cms.untracked.string('rfio:/castor/cern.ch/user/o/oiorio/SingleTop/2011/MergedNew/TChannel_wlightMerged.root'),
                                          outputCommands = cms.untracked.vstring(    'keep *',   ),
                                          SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('Pathwlight')),
                                          )
    process.skimwcc = cms.OutputModule("PoolOutputModule",
                                       fileName = cms.untracked.string('rfio:/castor/cern.ch/user/o/oiorio/SingleTop/2011/MergedNew/TChannel_wccMerged.root'),
                                       outputCommands = cms.untracked.vstring(    'keep *',   ),
                                       SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('Pathwcc')),
                                       )
    process.skimwbb = cms.OutputModule("PoolOutputModule",
                                       fileName = cms.untracked.string('rfio:/castor/cern.ch/user/o/oiorio/SingleTop/2011/MergedNew/TChannel_wbbMerged.root'),
                                       outputCommands = cms.untracked.vstring(    'keep *',   ),
                                       SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('Pathwbb')),
                                       )
    process.skimwall = cms.OutputModule("PoolOutputModule",
                                        fileName = cms.untracked.string('rfio:/castor/cern.ch/user/o/oiorio/SingleTop/2011/MergedNew/TChannelMerged.root'),
                                        outputCommands = cms.untracked.vstring(    'keep *',   ),
#                                        SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('*')),
                                        )
    process.outpath = cms.EndPath( process.skimwlight +
                                   process.skimwall +
                                   process.skimwcc +
                                   process.skimwbb
                                   )


if switch == "vqq":
#### Part for vqq
    process.Pathwcc1 = cms.Path(
        process.wccFilter1 
        )
    
    process.Pathwbb1 = cms.Path(
        process.wbbFilter1 
        )
    
    process.Pathwcc2 = cms.Path(
        process.wccFilter2 
        )
    
    process.Pathwbb2 = cms.Path(
        process.wbbFilter2 
        )
    
    process.skimwcc = cms.OutputModule("PoolOutputModule",
                                       fileName = cms.untracked.string('rfio:/castor/cern.ch/user/o/oiorio/SingleTop/2011/MergedNew/TChannel_wccMerged.root'),
                                       outputCommands = cms.untracked.vstring(    'keep *',   ),
                                       SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('Pathwcc1','Pathwcc2')),
                                       )
    process.skimwbb = cms.OutputModule("PoolOutputModule",
                                       fileName = cms.untracked.string('rfio:/castor/cern.ch/user/o/oiorio/SingleTop/2011/MergedNew/TChannel_wbbMerged.root'),
                                       outputCommands = cms.untracked.vstring(    'keep *',   ),
                                       SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('Pathwbb1','Pathwbb2')),
                                       )
    
    process.outpath = cms.EndPath(
        process.skimwcc +
        process.skimwbb
        )




if switch == "wc":
#### Part for wc
    process.Pathwc = cms.Path(
        process.wcFilter 
        )
    process.skimwc = cms.OutputModule("PoolOutputModule",
                                       fileName = cms.untracked.string('rfio:/castor/cern.ch/user/o/oiorio/SingleTop/2011/MergedNew/TChannel_wcMerged.root'),
                                       outputCommands = cms.untracked.vstring(    'keep *',   ),
                                       SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('Pathwc')),
                                       )
    process.outpath = cms.EndPath( process.skimwc)
    
if switch == "None":
    process.skimwall = cms.OutputModule("PoolOutputModule",
                                        fileName = cms.untracked.string('rfio:/castor/cern.ch/user/o/oiorio/SingleTop/2011/MergedNew/TChannelMerged.root'),
                                        outputCommands = cms.untracked.vstring(    'keep *',   ),
#                                        SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('*')),
                                        )
    process.outpath = cms.EndPath( process.skimwall)
    #process.skimwall.fileName = "rfio:/castor/cern.ch/user/o/oiorio/SingleTop/2011/Merged/rfio:/castor/cern.ch/user/o/oiorio/SingleTop/2011/Merged/TChannelMerged.root"

    
#process.source.fileNames = TChannel_ntuple
#process.skimwall.fileName = "/castor/cern.ch/user/o/oiorio/SingleTop/2011/MC2011/NewNtuples/Merged/TChannelMerged.root"

#Save the skims
#process.outpath = cms.EndPath( process.skimlight + process.skimwcc + process.skimwbb )
