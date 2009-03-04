# Auto generated configuration file
# using: 
# Revision: 1.84 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: step3 -s ALCA:TkAlZMuMu+TkAlMuonIsolated+TkAlJpsiMuMu+TkAlUpsilonMuMu+TkAlMinBias+SiPixelLorentzAngle+SiStripCalMinBias+EcalCalElectron+HcalCalDijets+HcalCalGammaJet+MuAlOverlaps+DQM -n 1000 --filein file:reco.root --conditions FrontierConditions_GlobalTag,IDEAL_V9::All --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('ALCA')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/MixingNoPileUp_cff')
process.load('Configuration/StandardSequences/GeometryPilot2_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/AlCaRecoStreams_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.5 $'),
    annotation = cms.untracked.string('step3 nevts:1000'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)
# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-RECO/IDEAL_V9_v2/0002/56B92AB9-8B7E-DD11-A821-000423D6AF24.root')
)

# Additional output definition
process.ALCARECOStreamTkAlMinBias = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlMinBias')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOTkAlMinBias_*_*', 
        'keep *_MEtoEDMConverter_*_*'),
    fileName = cms.untracked.string('ALCARECOTkAlMinBias.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('StreamALCARECOTkAlMinBias'),
        dataTier = cms.untracked.string('ALCARECO')
    )
)
process.ALCARECOStreamHcalCalGammaJet = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOHcalCalGammaJet')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_GammaJetProd_*_*'),
    fileName = cms.untracked.string('ALCARECOHcalCalGammaJet.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('StreamALCARECOHcalCalGammaJet'),
        dataTier = cms.untracked.string('ALCARECO')
    )
)
process.ALCARECOStreamTkAlUpsilonMuMu = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlUpsilonMuMu')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOTkAlUpsilonMuMu_*_*', 
        'keep *_MEtoEDMConverter_*_*'),
    fileName = cms.untracked.string('ALCARECOTkAlUpsilonMuMu.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('StreamALCARECOTkAlUpsilonMuMu'),
        dataTier = cms.untracked.string('ALCARECO')
    )
)
process.ALCARECOStreamTkAlZMuMu = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlZMuMu')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOTkAlZMuMu_*_*', 
        'keep *_MEtoEDMConverter_*_*'),
    fileName = cms.untracked.string('ALCARECOTkAlZMuMu.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('StreamALCARECOTkAlZMuMu'),
        dataTier = cms.untracked.string('ALCARECO')
    )
)
process.ALCARECOStreamTkAlMuonIsolated = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlMuonIsolated')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOTkAlMuonIsolated_*_*', 
        'keep *_MEtoEDMConverter_*_*'),
    fileName = cms.untracked.string('ALCARECOTkAlMuonIsolated.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('StreamALCARECOTkAlMuonIsolated'),
        dataTier = cms.untracked.string('ALCARECO')
    )
)
process.ALCARECOStreamSiStripCalMinBias = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOSiStripCalMinBias')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOSiStripCalMinBias_*_*', 
        'keep *_siStripClusters_*_*', 
        'keep *_siPixelClusters_*_*', 
        'keep *_offlineBeamSpot_*_*'),
    fileName = cms.untracked.string('ALCARECOSiStripCalMinBias.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('StreamALCARECOSiStripCalMinBias'),
        dataTier = cms.untracked.string('ALCARECO')
    )
)
process.ALCARECOStreamTkAlJpsiMuMu = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOTkAlJpsiMuMu')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOTkAlJpsiMuMu_*_*', 
        'keep *_MEtoEDMConverter_*_*'),
    fileName = cms.untracked.string('ALCARECOTkAlJpsiMuMu.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('StreamALCARECOTkAlJpsiMuMu'),
        dataTier = cms.untracked.string('ALCARECO')
    )
)
process.ALCARECOStreamHcalCalDijets = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOHcalCalDijets')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_DiJProd_*_*'),
    fileName = cms.untracked.string('ALCARECOHcalCalDijets.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('StreamALCARECOHcalCalDijets'),
        dataTier = cms.untracked.string('ALCARECO')
    )
)
process.ALCARECOStreamEcalCalElectron = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOEcalCalElectron')
    ),
    outputCommands = cms.untracked.vstring('drop  *', 
        'keep  *_electronFilter_*_*', 
        'keep  *_alCaIsolatedElectrons_*_*', 
        'keep edmTriggerResults_TriggerResults__HLT'),
    fileName = cms.untracked.string('ALCARECOEcalCalElectron.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('StreamALCARECOEcalCalElectron'),
        dataTier = cms.untracked.string('ALCARECO')
    )
)
process.ALCARECOStreamMuAlOverlaps = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOMuAlOverlaps')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_ALCARECOMuAlOverlaps_*_*'),
    fileName = cms.untracked.string('ALCARECOMuAlOverlaps.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('StreamALCARECOMuAlOverlaps'),
        dataTier = cms.untracked.string('ALCARECO')
    )
)
process.ALCARECOStreamSiPixelLorentzAngle = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathALCARECOSiPixelLorentzAngle')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_globalMuons_*_*', 
        'keep *_siStripClusters_*_*', 
        'keep *_siPixelClusters_*_*', 
        'drop *_*_*_HLT'),
    fileName = cms.untracked.string('ALCARECOSiPixelLorentzAngle.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('StreamALCARECOSiPixelLorentzAngle'),
        dataTier = cms.untracked.string('ALCARECO')
    )
)

# Other statements
process.GlobalTag.globaltag = 'IDEAL_V9::All'

# Path and EndPath definitions
process.pathALCARECOTkAlCosmicsCTFHLT = cms.Path(process.seqALCARECOTkAlCosmicsCTFHLT)
process.pathALCARECORpcCalHLT = cms.Path(process.seqALCARECORpcCalHLT)
process.pathALCARECOHcalCalGammaJet = cms.Path(process.seqALCARECOHcalCalGammaJet)
process.pathALCARECOMuAlBeamHaloOverlaps = cms.Path(process.seqALCARECOMuAlBeamHaloOverlaps)
process.pathALCARECOTkAlCosmicsCosmicTF0THLT = cms.Path(process.seqALCARECOTkAlCosmicsCosmicTF0THLT*process.ALCARECOTkAlCosmicsCosmicTF0TDQM)
process.pathALCARECOTkAlCosmicsCosmicTFHLT = cms.Path(process.seqALCARECOTkAlCosmicsCosmicTFHLT)
process.pathALCARECOSiStripCalMinBias = cms.Path(process.seqALCARECOSiStripCalMinBias)
process.pathALCARECOTkAlCosmicsRS0T = cms.Path(process.seqALCARECOTkAlCosmicsRS0T*process.ALCARECOTkAlCosmicsRS0TDQM)
process.pathALCARECOTkAlMinBias = cms.Path(process.seqALCARECOTkAlMinBias*process.ALCARECOTkAlMinBiasDQM)
process.pathALCARECOTkAlMuonIsolated = cms.Path(process.seqALCARECOTkAlMuonIsolated*process.ALCARECOTkAlMuonIsolatedDQM)
process.pathALCARECOMuAlStandAloneCosmics = cms.Path(process.seqALCARECOMuAlStandAloneCosmics)
process.pathALCARECODQM = cms.Path(process.MEtoEDMConverter)
process.pathALCARECOTkAlZMuMu = cms.Path(process.seqALCARECOTkAlZMuMu*process.ALCARECOTkAlZMuMuDQM)
process.pathALCARECOTkAlUpsilonMuMu = cms.Path(process.seqALCARECOTkAlUpsilonMuMu*process.ALCARECOTkAlUpsilonMuMuDQM)
process.pathALCARECOHcalCalDijets = cms.Path(process.seqALCARECOHcalCalDijets)
process.pathALCARECOTkAlCosmicsCTF0T = cms.Path(process.seqALCARECOTkAlCosmicsCTF0T*process.ALCARECOTkAlCosmicsCTF0TDQM)
process.pathALCARECOMuAlOverlaps = cms.Path(process.seqALCARECOMuAlOverlaps)
process.pathALCARECOTkAlCosmicsRS0THLT = cms.Path(process.seqALCARECOTkAlCosmicsRS0THLT*process.ALCARECOTkAlCosmicsRS0TDQM)
process.pathALCARECOTkAlCosmicsCosmicTF = cms.Path(process.seqALCARECOTkAlCosmicsCosmicTF)
process.pathALCARECOMuAlGlobalCosmics = cms.Path(process.seqALCARECOMuAlGlobalCosmics)
process.pathALCARECOTkAlBeamHalo = cms.Path(process.seqALCARECOTkAlBeamHalo)
process.pathALCARECOTkAlLAS = cms.Path(process.seqALCARECOTkAlLAS)
process.pathALCARECOTkAlCosmicsRS = cms.Path(process.seqALCARECOTkAlCosmicsRS)
process.pathALCARECOSiPixelLorentzAngle = cms.Path(process.seqALCARECOSiPixelLorentzAngle)
process.pathALCARECOMuAlBeamHalo = cms.Path(process.seqALCARECOMuAlBeamHalo)
process.pathALCARECOTkAlCosmicsCTF = cms.Path(process.seqALCARECOTkAlCosmicsCTF)
process.pathALCARECOTkAlCosmicsCosmicTF0T = cms.Path(process.seqALCARECOTkAlCosmicsCosmicTF0T*process.ALCARECOTkAlCosmicsCosmicTF0TDQM)
process.pathALCARECOTkAlJpsiMuMu = cms.Path(process.seqALCARECOTkAlJpsiMuMu*process.ALCARECOTkAlJpsiMuMuDQM)
process.pathALCARECOEcalCalElectron = cms.Path(process.seqALCARECOEcalCalElectron)
process.pathALCARECOTkAlCosmicsCTF0THLT = cms.Path(process.seqALCARECOTkAlCosmicsCTF0THLT*process.ALCARECOTkAlCosmicsCTF0TDQM)
process.pathALCARECOMuAlCalIsolatedMu = cms.Path(process.seqALCARECOMuAlCalIsolatedMu)
process.pathALCARECOHcalCalHO = cms.Path(process.seqALCARECOHcalCalHO)
process.pathALCARECOTkAlCosmicsRSHLT = cms.Path(process.seqALCARECOTkAlCosmicsRSHLT)
process.ALCARECOStreamTkAlMinBiasOutPath = cms.EndPath(process.ALCARECOStreamTkAlMinBias)
process.ALCARECOStreamHcalCalGammaJetOutPath = cms.EndPath(process.ALCARECOStreamHcalCalGammaJet)
process.ALCARECOStreamTkAlUpsilonMuMuOutPath = cms.EndPath(process.ALCARECOStreamTkAlUpsilonMuMu)
process.ALCARECOStreamTkAlZMuMuOutPath = cms.EndPath(process.ALCARECOStreamTkAlZMuMu)
process.ALCARECOStreamTkAlMuonIsolatedOutPath = cms.EndPath(process.ALCARECOStreamTkAlMuonIsolated)
process.ALCARECOStreamSiStripCalMinBiasOutPath = cms.EndPath(process.ALCARECOStreamSiStripCalMinBias)
process.ALCARECOStreamTkAlJpsiMuMuOutPath = cms.EndPath(process.ALCARECOStreamTkAlJpsiMuMu)
process.ALCARECOStreamHcalCalDijetsOutPath = cms.EndPath(process.ALCARECOStreamHcalCalDijets)
process.ALCARECOStreamEcalCalElectronOutPath = cms.EndPath(process.ALCARECOStreamEcalCalElectron)
process.ALCARECOStreamMuAlOverlapsOutPath = cms.EndPath(process.ALCARECOStreamMuAlOverlaps)
process.ALCARECOStreamSiPixelLorentzAngleOutPath = cms.EndPath(process.ALCARECOStreamSiPixelLorentzAngle)

# Schedule definition
process.schedule = cms.Schedule(process.pathALCARECOHcalCalGammaJet,process.pathALCARECOTkAlZMuMu,process.pathALCARECOTkAlUpsilonMuMu,process.pathALCARECOMuAlOverlaps,process.pathALCARECOTkAlMuonIsolated,process.pathALCARECOTkAlMinBias,process.pathALCARECODQM,process.pathALCARECOSiStripCalMinBias,process.pathALCARECOHcalCalDijets,process.pathALCARECOSiPixelLorentzAngle,process.pathALCARECOEcalCalElectron,process.pathALCARECOTkAlJpsiMuMu,process.ALCARECOStreamTkAlMinBiasOutPath,process.ALCARECOStreamHcalCalGammaJetOutPath,process.ALCARECOStreamTkAlUpsilonMuMuOutPath,process.ALCARECOStreamTkAlZMuMuOutPath,process.ALCARECOStreamTkAlMuonIsolatedOutPath,process.ALCARECOStreamSiStripCalMinBiasOutPath,process.ALCARECOStreamTkAlJpsiMuMuOutPath,process.ALCARECOStreamHcalCalDijetsOutPath,process.ALCARECOStreamEcalCalElectronOutPath,process.ALCARECOStreamMuAlOverlapsOutPath,process.ALCARECOStreamSiPixelLorentzAngleOutPath)
