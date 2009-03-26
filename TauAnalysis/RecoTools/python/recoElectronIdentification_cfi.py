import FWCore.ParameterSet.Config as cms

from RecoEgamma.ElectronIdentification.electronIdCutBasedExt_cfi import *

elecIdCutBasedRobust = eidCutBasedExt.copy()
elecIdCutBasedRobust.src = cms.InputTag("pixelMatchGsfElectrons")
elecIdCutBasedRobust.electronQuality = 'robust'
elecIdCutBasedRobust.robustEleIDCuts = cms.PSet(
   barrel = cms.vdouble(0.015, 0.012, 0.02, 0.0025), # [0.015, 0.0092, 0.020, 0.0025]
   endcap = cms.vdouble(0.018, 0.025, 0.02, 0.0040)  # [0.018, 0.025, 0.020, 0.0040]
)

elecIdCutBasedLoose = eidCutBasedExt.copy()
elecIdCutBasedLoose.electronQuality = 'loose'

elecIdCutBasedTight = eidCutBasedExt.copy()
elecIdCutBasedTight.electronQuality = 'tight'

electronIdCutBased = cms.Sequence( elecIdCutBasedRobust
                                  *elecIdCutBasedLoose
                                  *elecIdCutBasedTight )
