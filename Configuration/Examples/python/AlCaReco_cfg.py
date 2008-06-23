# The following comments couldn't be translated into the new config version:

# last update: $Date: 2008/04/21 00:53:45 $ by $Author: rpw $

import FWCore.ParameterSet.Config as cms

process = cms.Process("P")
#service = SimpleMemoryCheck {
#        untracked int32 ignoreTotal = 1 # default is one
#      } 
process.load("Configuration.StandardSequences.Reconstruction_cff")

process.load("Configuration.StandardSequences.AlCaReco_cff")

process.load("Configuration.EventContent.AlCaRecoOutput_cff")

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/Examples/python/AlCaReco_cfg.py,v $'),
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
    fileNames = cms.untracked.vstring('/store/relval/2008/6/22/RelVal-RelValTTbar-1213987236-IDEAL_V2-2nd/0003/C4807780-C140-DD11-A03E-000423D94700.root',
	'/store/relval/2008/6/22/RelVal-RelValTTbar-1213987236-IDEAL_V2-2nd/0004/4C747DCB-C240-DD11-9FFD-001617C3B654.root',
	'/store/relval/2008/6/22/RelVal-RelValTTbar-1213987236-IDEAL_V2-2nd/0004/AE397234-CA40-DD11-9413-001617E30F48.root')
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

process.poolALCARECOTkAlCosmicsHLT = cms.OutputModule("PoolOutputModule",
    process.OutALCARECOTkAlCosmicsHLT,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('ALCARECOTkAlCosmicsHLT'),
        dataTier = cms.untracked.string('ALCARECO')
    ),
    fileName = cms.untracked.string('file:ALCARECOTkAlCosmicsHLT.root')
)

process.poolALCARECOTkAlCosmics0T = cms.OutputModule("PoolOutputModule",
    process.OutALCARECOTkAlCosmics0T,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('ALCARECOTkAlCosmics0T'),
        dataTier = cms.untracked.string('ALCARECO')
    ),
    fileName = cms.untracked.string('file:ALCARECOTkAlCosmics0T.root')
)

process.poolALCARECOTkAlCosmics0THLT = cms.OutputModule("PoolOutputModule",
    process.OutALCARECOTkAlCosmics0THLT,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('ALCARECOTkAlCosmics0THLT'),
        dataTier = cms.untracked.string('ALCARECO')
    ),
    fileName = cms.untracked.string('file:ALCARECOTkAlCosmics0THLT.root')
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

process.poolALCARECOMuAlZeroFieldGlobalCosmics = cms.OutputModule("PoolOutputModule",
    process.OutALCARECOMuAlZeroFieldGlobalCosmics,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('ALCARECOMuAlZeroFieldGlobalCosmics'),
        dataTier = cms.untracked.string('ALCARECO')
    ),
    fileName = cms.untracked.string('file:ALCARECOMuAlZeroFieldGlobalCosmics.root')
)

process.poolALCARECOMuCaliMinBias = cms.OutputModule("PoolOutputModule",
    process.OutALCARECOMuCaliMinBias,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('ALCARECOMuCaliMinBias'),
        dataTier = cms.untracked.string('ALCARECO')
    ),
    fileName = cms.untracked.string('file:ALCARECOMuCaliMinBias.root')
)

process.poolALCARECOMuAlCalIsolatedMu = cms.OutputModule("PoolOutputModule",
    process.OutALCARECOMuAlCalIsolatedMu,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('ALCARECOMuAlCalIsolatedMu'),
        dataTier = cms.untracked.string('ALCARECO')
    ),
    fileName = cms.untracked.string('file:ALCARECOMuAlCalIsolatedMu.root')
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

process.e = cms.EndPath(process.poolALCARECOTkAlZMuMu*process.poolALCARECOTkAlMuonIsolated*process.poolALCARECOTkAlJpsiMuMu*process.poolALCARECOTkAlUpsilonMuMu*process.poolALCARECOTkAlMinBias*process.poolALCARECOSiPixelLorentzAngle*process.poolALCARECOSiStripCalMinBias*process.poolALCARECOEcalCalElectron*process.poolALCARECOEcalCalPhiSym*process.poolALCARECOEcalCalPi0Calib*process.poolALCARECOHcalCalMinBias*process.poolALCARECOHcalCalDijets*process.poolALCARECOHcalCalGammaJet*process.poolALCARECOHcalCalIsoTrkNoHLT*process.poolALCARECOHcalCalHO*process.poolALCARECOMuCaliMinBias*process.poolALCARECOMuAlCalIsolatedMu*process.poolALCARECOMuAlOverlaps*process.poolALCARECORpcCalHLT)
process.schedule = cms.Schedule(process.pathALCARECOTkAlZMuMu,process.pathALCARECOTkAlMuonIsolated,process.pathALCARECOTkAlJpsiMuMu,process.pathALCARECOTkAlUpsilonMuMu,process.pathALCARECOTkAlMinBias,process.pathALCARECOSiPixelLorentzAngle,process.pathALCARECOSiStripCalMinBias,process.pathALCARECOEcalCalElectron,process.pathALCARECOEcalCalPhiSym,process.pathALCARECOEcalCalPi0Calib,process.pathALCARECOHcalCalMinBias,process.pathALCARECOHcalCalDijets,process.pathALCARECOHcalCalGammaJet,process.pathALCARECOHcalCalIsoTrkNoHLT,process.pathALCARECOHcalCalHO,process.poolALCARECOMuCaliMinBias,process.pathALCARECOMuAlCalIsolatedMu,process.pathALCARECOMuAlOverlaps,process.pathALCARECORpcCalHLT,process.e)


