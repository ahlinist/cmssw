import FWCore.ParameterSet.Config as cms

from Configuration.GenProduction.PythiaUESettings_cfi import *


source = cms.Source("ComphepSource",
                         pythiaPylistVerbosity = cms.untracked.int32(0),
                         filterEfficiency = cms.untracked.double(0.4614),
                         pythiaHepMCVerbosity = cms.untracked.bool(False),
                         fileNames = cms.untracked.vstring('file:Mixed.pev'),
                         process = cms.untracked.string('p1,p1 -> m,M'),
                         CompHEPFirstEvent = cms.int32(1),
                         crossSection = cms.untracked.double(0.5496),
                         getInputFromMCDB = cms.untracked.bool(False),
                         maxEventsToPrint = cms.untracked.int32(1),
                         MCDBArticleID = cms.int32(0),
                         ##########################################
                         # PYTHIA
                         #############################################
                         PythiaParameters = cms.PSet(
                         pythiaUESettingsBlock,
                         pythiaCMSDefaults = cms.vstring(
                        'MSEL=0         ! User defined processes/Full user control'),
                         # This is a vector of ParameterSet names to be read, in this order
                         # The first is general default pythia parameters, the second are own additions
                         parameterSets = cms.vstring('pythiaUESettings','pythiaCMSDefaults')
                      )
                         )










