import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.tools.sysProdConfigurator import *
from TauAnalysis.RecoTools.randomNumberGenerator_cfi import *

sysConfiguratorJets = sysProdConfigurator(pyModuleName = __name__, pyNameSpace = locals())
prodSmearedJets = sysConfiguratorJets.createSequence(
    randomNumberService = RandomNumberGeneratorService,
    modulePrefix        = 'smearedJets',
    moduleType          = 'SmearedJetProducer',
    src                 = 'cleanPatJets',
    energyScaleMean     = 1.0,
    energyScaleShift    = 0.05,
    energyScaleSmearing = 0.05,
    ptShift             = 1.0,
    ptSmearing          = 0.5,
    etaShift            = 0.05,
    etaSmearing         = 0.025,
    phiShift            = 0.05,
    phiSmearing         = 0.025
)
