import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.tools.sysProdConfigurator import *

RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    saveFileName = cms.untracked.string('')
)

randomEngineStateProducer = cms.EDProducer("RandomEngineStateProducer")

sysConfiguratorMuons = sysProdConfigurator(pyModuleName = __name__, pyNameSpace = locals())
prodSmearedMuons = sysConfiguratorMuons.createSequence(
    randomNumberService = RandomNumberGeneratorService,
    modulePrefix        = 'smearedMuons',
    moduleType          = 'SmearedMuonProducer',
    src                 = 'allLayer1Muons',
    energyScaleMean     = 1.0,
    energyScaleShift    = 0.025,
    energyScaleSmearing = 0.01,
    ptShift             = 0.0,
    ptSmearing          = 0.1,
    etaShift            = 0.025,
    etaSmearing         = 0.01,
    phiShift            = 0.025,
    phiSmearing         = 0.01
)

sysConfiguratorTaus = sysProdConfigurator(pyModuleName = __name__, pyNameSpace = locals())
prodSmearedTaus = sysConfiguratorTaus.createSequence(
    randomNumberService = RandomNumberGeneratorService,
    modulePrefix        = 'smearedTaus',
    moduleType          = 'SmearedTauProducer',
    src                 = 'allLayer1Taus',
    energyScaleMean     = 1.0,
    energyScaleShift    = 0.1,
    energyScaleSmearing = 0.025,
    ptShift             = 1.0,
    ptSmearing          = 0.5,
    etaShift            = 0.05,
    etaSmearing         = 0.025,
    phiShift            = 0.05,
    phiSmearing         = 0.025
)
