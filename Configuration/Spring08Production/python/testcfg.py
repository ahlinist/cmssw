#!/usr/bin/env python
cfgFile="Configuration/Spring08Production/data/Spring08_Gamma_Jets_PythiaFilterGammaGamma_GEN.cfg"
from FWCore.ParameterSet.Config import include
process = include(cfgFile)

