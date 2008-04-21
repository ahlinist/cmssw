# The following comments couldn't be translated into the new config version:

# last update: $Date: 2008/04/07 21:15:04 $ by $Author: futyand $

import FWCore.ParameterSet.Config as cms

process = cms.Process("P")
#service = SimpleMemoryCheck {
#        untracked int32 ignoreTotal = 1 # default is one
#      } 
process.load("Configuration.StandardSequences.Reconstruction_cff")

process.load("Configuration.StandardSequences.AlCaReco_cff")

process.load("Configuration.EventContent.AlCaRecoOutput_cff")

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.26 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/Examples/data/AlCaReco.cfg,v $'),
    annotation = cms.untracked.string('CFG for AlcaReco')
)
process.MessageLogger = cms.Service("MessageLogger",
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO'),
        noLineBreaks = cms.untracked.bool(True)
    ),
    destinations = cms.untracked.vstring('cout')
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/relval/2008/4/6/RelVal-RelValTTbar-1207410101-HLT/0000/0AE8E8BE-1404-DD11-8A26-00304885AA12.root', 
        '/store/relval/2008/4/6/RelVal-RelValTTbar-1207410101-HLT/0000/0C53192B-1504-DD11-B99E-00304885AA28.root', 
        '/store/relval/2008/4/6/RelVal-RelValTTbar-1207410101-HLT/0000/125A556B-1304-DD11-801E-000423D95220.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)
process.poolALCARECOTkAlZMuMu = cms.OutputModule("PoolOutputModule",
    process.OutALCARECOTkAlZMuMu,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('ALCARECOTkAlZMuMu'),
        dataTier = cms.untracked.string('ALCARECO')
    ),
    fileName = cms.untracked.string('file:ALCARECOTkAlZMuMu.root')
)

process.poolALCARECOTkAlMuonIsolated = cms.OutputModule("PoolOutputModule",
    process.OutALCARECOTkAlMuonIsolated,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('ALCARECOTkAlMuonIsolated'),
        dataTier = cms.untracked.string('ALCARECO')
    ),
    fileName = cms.untracked.string('file:ALCARECOTkAlMuonIsolated.root')
)

process.poolALCARECOTkAlMinBias = cms.OutputModule("PoolOutputModule",
    process.OutALCARECOTkAlMinBias,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('ALCARECOTkAlMinBias'),
        dataTier = cms.untracked.string('ALCARECO')
    ),
    fileName = cms.untracked.string('file:ALCARECOTkAlMinBias.root')
)

process.poolALCARECOTkAlJpsiMuMu = cms.OutputModule("PoolOutputModule",
    process.OutALCARECOTkAlJpsiMuMu,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('ALCARECOTkAlJpsiMuMu'),
        dataTier = cms.untracked.string('ALCARECO')
    ),
    fileName = cms.untracked.string('file:ALCARECOTkAlJpsiMuMu.root')
)

process.poolALCARECOTkAlUpsilonMuMu = cms.OutputModule("PoolOutputModule",
    process.OutALCARECOTkAlUpsilonMuMu,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('ALCARECOTkAlUpsilonMuMu'),
        dataTier = cms.untracked.string('ALCARECO')
    ),
    fileName = cms.untracked.string('file:ALCARECOTkAlUpsilonMuMu.root')
)

process.poolALCARECOSiPixelLorentzAngle = cms.OutputModule("PoolOutputModule",
    process.OutALCARECOSiPixelLorentzAngle,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('ALCARECOSiPixelLorentzAngle'),
        dataTier = cms.untracked.string('ALCARECO')
    ),
    fileName = cms.untracked.string('file:ALCARECOSiPixelLorentzAngle.root')
)

process.poolALCARECOSiStripCalMinBias = cms.OutputModule("PoolOutputModule",
    process.OutALCARECOSiStripCalMinBias,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('ALCARECOSiStripCalMinBias'),
        dataTier = cms.untracked.string('ALCARECO')
    ),
    fileName = cms.untracked.string('file:ALCARECOSiStripCalMinBias.root')
)

process.poolALCARECOTkAlCosmics = cms.OutputModule("PoolOutputModule",
    process.OutALCARECOTkAlCosmics,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('ALCARECOTkAlCosmics'),
        dataTier = cms.untracked.string('ALCARECO')
    ),
    fileName = cms.untracked.string('file:ALCARECOTkAlCosmics.root')
)

process.poolALCARECOEcalCalPhiSym = cms.OutputModule("PoolOutputModule",
    process.OutALCARECOEcalCalPhiSym,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('ALCARECOEcalCalPhiSym'),
        dataTier = cms.untracked.string('ALCARECO')
    ),
    fileName = cms.untracked.string('file:ALCARECOEcalCalPhiSym.root')
)

process.poolALCARECOEcalCalElectron = cms.OutputModule("PoolOutputModule",
    process.OutALCARECOEcalCalElectron,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('ALCARECOEcalCalElectron'),
        dataTier = cms.untracked.string('ALCARECO')
    ),
    fileName = cms.untracked.string('file:ALCARECOEcalCalElectron.root')
)

process.poolALCARECOEcalCalPi0Calib = cms.OutputModule("PoolOutputModule",
    process.OutALCARECOEcalCalPi0Calib,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('ALCARECOEcalCalPi0Calib'),
        dataTier = cms.untracked.string('ALCARECO')
    ),
    fileName = cms.untracked.string('file:ALCARECOEcalCalPi0Calib.root')
)

process.poolALCARECOHcalCalDijets = cms.OutputModule("PoolOutputModule",
    process.OutALCARECOHcalCalDijets,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('ALCARECOHcalCalDijets'),
        dataTier = cms.untracked.string('ALCARECO')
    ),
    fileName = cms.untracked.string('file:ALCARECOHcalCalDijets.root')
)

process.poolALCARECOHcalCalGammaJet = cms.OutputModule("PoolOutputModule",
    process.OutALCARECOHcalCalGammaJet,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('ALCARECOHcalCalGammaJet'),
        dataTier = cms.untracked.string('ALCARECO')
    ),
    fileName = cms.untracked.string('file:ALCARECOHcalCalGammaJet.root')
)

process.poolALCARECOHcalCalIsoTrk = cms.OutputModule("PoolOutputModule",
    process.OutALCARECOHcalCalIsoTrk,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('ALCARECOHcalCalIsoTrk'),
        dataTier = cms.untracked.string('ALCARECO')
    ),
    fileName = cms.untracked.string('file:ALCARECOHcalCalIsoTrk.root')
)

process.poolALCARECOHcalCalIsoTrkNoHLT = cms.OutputModule("PoolOutputModule",
    process.OutALCARECOHcalCalIsoTrkNoHLT,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('ALCARECOHcalCalIsoTrkNoHLT'),
        dataTier = cms.untracked.string('ALCARECO')
    ),
    fileName = cms.untracked.string('file:ALCARECOHcalCalIsoTrkNoHLT.root')
)

process.poolALCARECOHcalCalMinBias = cms.OutputModule("PoolOutputModule",
    process.OutALCARECOHcalCalMinBias,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('ALCARECOHcalCalMinBias'),
        dataTier = cms.untracked.string('ALCARECO')
    ),
    fileName = cms.untracked.string('file:ALCARECOHcalCalMinBias.root')
)

process.poolALCARECOHcalCalHO = cms.OutputModule("PoolOutputModule",
    process.OutALCARECOHcalCalHO,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('ALCARECOHcalCalHO'),
        dataTier = cms.untracked.string('ALCARECO')
    ),
    fileName = cms.untracked.string('file:ALCARECOHcalCalHO.root')
)

process.poolALCARECOMuAlZMuMu = cms.OutputModule("PoolOutputModule",
    process.OutALCARECOMuAlZMuMu,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('ALCARECOMuAlZMuMu'),
        dataTier = cms.untracked.string('ALCARECO')
    ),
    fileName = cms.untracked.string('file:ALCARECOMuAlZMuMu.root')
)

process.poolALCARECOMuAlOverlaps = cms.OutputModule("PoolOutputModule",
    process.OutALCARECOMuAlOverlaps,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('ALCARECOMuAlOverlaps'),
        dataTier = cms.untracked.string('ALCARECO')
    ),
    fileName = cms.untracked.string('file:ALCARECOMuAlOverlaps.root')
)

process.poolALCARECORpcCalHLT = cms.OutputModule("PoolOutputModule",
    process.OutALCARECORpcCalHLT,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('ALCARECORpcCalHLT'),
        dataTier = cms.untracked.string('ALCARECO')
    ),
    fileName = cms.untracked.string('file:ALCARECORpcCalHLT.root')
)

process.e = cms.EndPath(process.poolALCARECOTkAlZMuMu*process.poolALCARECOTkAlMuonIsolated*process.poolALCARECOTkAlJpsiMuMu*process.poolALCARECOTkAlUpsilonMuMu*process.poolALCARECOTkAlMinBias*process.poolALCARECOSiPixelLorentzAngle*process.poolALCARECOSiStripCalMinBias*process.poolALCARECOEcalCalElectron*process.poolALCARECOEcalCalPhiSym*process.poolALCARECOEcalCalPi0Calib*process.poolALCARECOHcalCalMinBias*process.poolALCARECOHcalCalDijets*process.poolALCARECOHcalCalGammaJet*process.poolALCARECOHcalCalIsoTrkNoHLT*process.poolALCARECOHcalCalHO*process.poolALCARECOMuAlZMuMu*process.poolALCARECOMuAlOverlaps*process.poolALCARECORpcCalHLT)
process.schedule = cms.Schedule(process.pathALCARECOTkAlZMuMu,process.pathALCARECOTkAlMuonIsolated,process.pathALCARECOTkAlJpsiMuMu,process.pathALCARECOTkAlUpsilonMuMu,process.pathALCARECOTkAlMinBias,process.pathALCARECOSiPixelLorentzAngle,process.pathALCARECOSiStripCalMinBias,process.pathALCARECOEcalCalElectron,process.pathALCARECOEcalCalPhiSym,process.pathALCARECOEcalCalPi0Calib,process.pathALCARECOHcalCalMinBias,process.pathALCARECOHcalCalDijets,process.pathALCARECOHcalCalGammaJet,process.pathALCARECOHcalCalIsoTrkNoHLT,process.pathALCARECOHcalCalHO,process.pathALCARECOMuAlZMuMu,process.pathALCARECOMuAlOverlaps,process.pathALCARECORpcCalHLT,process.e)


