import FWCore.ParameterSet.Config as cms


from TauAnalysis.RecoTools.tools.sysProdConfigurator import *

sysConfigurator =sysProdConfigurator(pyModuleName = __name__,
                                  pyNameSpace  = locals())

tauSystematicsSequence =sysConfigurator.createSequence(
                                    modulePrefix        = 'smearedTaus',
                                    moduleType          = 'smearedTauProducer',
                                    src                 = 'allLayer1Taus',
                                    energyScaleMean     = 1.0,
                                    energyScaleShift    = 0.1,
                                    energyScaleSmearing = 0.01,
                                    ptShift             = 1.0,
                                    ptSmearing          = 0.5,
                                    etaShift            = 0.05,
                                    etaSmearing         = 0.01,
                                    phiShift            = 0.05,
                                    phiSmearing         = 0.01
)

