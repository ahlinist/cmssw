import FWCore.ParameterSet.Config as cms

from ElectroWeakAnalysis.MultiBosons.Skimming.PUWeightProducer_cfi import *

from SimGeneral.MixingModule.mix_E7TeV_FlatDist10_2011EarlyData_inTimeOnly_cfi import mix as Spring11MC 

#weights for 2011 data with spring11 probdist
pileupweight.pu2011Spring11 = cms.PSet(
    dataNPUSrc = cms.FileInPath('ElectroWeakAnalysis/MultiBosons/data/pudist2011A.root'),
    mcNPUprobs = Spring11MC.input.nbPileupEvents.probValue
    )

#weights for 2010 data with spring11 probdist
pileupweight.pu2010Spring11 = cms.PSet(
    dataNPUSrc = cms.FileInPath('ElectroWeakAnalysis/MultiBosons/data/pudist2010B.root'),
    mcNPUprobs = Spring11MC.input.nbPileupEvents.probValue
    )
