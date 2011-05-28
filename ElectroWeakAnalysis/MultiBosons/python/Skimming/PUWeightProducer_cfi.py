import FWCore.ParameterSet.Config as cms
import os

EWAMBpath=os.environ['CMSSW_BASE']+'/src/ElectroWeakAnalysis/MultiBosons'

pileupweight = cms.EDProducer(
    'ConfigurablePUWeightProducer',
    src = cms.InputTag('addPileupInfo')
    )

