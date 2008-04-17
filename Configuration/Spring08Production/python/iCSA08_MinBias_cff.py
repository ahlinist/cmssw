import FWCore.ParameterSet.Config as cms

def customise(process):

  #SA CSA08 enable storage of simtracks decay products
  #will probably be the default at some point
  process.g4SimHits.StackingAction.SavePrimaryDecayProductsAndConversions = True
  process.g4SimHits.TrackerSD.EnergyThresholdForPersistencyInGeV = 0.2
  
  #SA CSA08
  #turn off hcal zero suppression
  process.hcalDigis.hbhe.level = -10000
  process.hcalDigis.ho.level   = -10000
  process.hcalDigis.hf.level   = -10000
  
  return process
