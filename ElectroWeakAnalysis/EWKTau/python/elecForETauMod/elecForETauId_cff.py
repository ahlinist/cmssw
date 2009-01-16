import FWCore.ParameterSet.Config as cms

from RecoEcal.EgammaClusterProducers.geometryForClustering_cff import *
from RecoEgamma.ElectronIdentification.likelihoodPdfsDB_cfi import *
from RecoEgamma.ElectronIdentification.likelihoodESetup_cfi import *
from RecoEgamma.ElectronIdentification.neuralNetElectronId_cfi import *

from RecoEgamma.ElectronIdentification.electronIdCutBasedExt_cfi        import eidCutBasedExt
from RecoEgamma.ElectronIdentification.electronIdCutBasedClassesExt_cfi import eidCutBasedClassesExt as eidPtdrExt ## PTDR is how people know it
from RecoEgamma.ElectronIdentification.electronIdNeuralNetExt_cfi       import eidNeuralNetExt
from RecoEgamma.ElectronIdentification.electronIdLikelihoodExt_cfi      import eidLikelihoodExt 
 

elecIdForETauCutBasedExtPAT = eidCutBasedExt.copy();  
elecIdForETauCutBasedExtPAT.src = cms.InputTag("allLayer0ElecForETau")
elecIdForETauCutBasedRobust = elecIdForETauCutBasedExtPAT.copy(); 
elecIdForETauCutBasedRobust.electronQuality = 'robust'
elecIdForETauCutBasedRobust.robustEleIDCuts = cms.PSet(
   barrel = cms.vdouble(0.015, 0.012, 0.02, 0.0025),
   endcap = cms.vdouble(0.018, 0.025, 0.02, 0.0040)
)
#barrel = cms.vdouble(0.015, 0.0092, 0.02, 0.0025),
#endcap = cms.vdouble(0.018, 0.025, 0.02, 0.0040)

elecIdForETauCutBasedLoose  = elecIdForETauCutBasedExtPAT.copy(); elecIdForETauCutBasedLoose.electronQuality  = 'loose'
elecIdForETauCutBasedTight  = elecIdForETauCutBasedExtPAT.copy(); elecIdForETauCutBasedTight.electronQuality  = 'tight'


patElecIdForETau = cms.Sequence(
                                 elecIdForETauCutBasedRobust * 
                                 elecIdForETauCutBasedLoose * 
                                 elecIdForETauCutBasedTight 
                               )
