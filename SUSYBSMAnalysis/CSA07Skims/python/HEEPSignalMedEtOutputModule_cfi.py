import FWCore.ParameterSet.Config as cms

from SUSYBSMAnalysis.CSA07Skims.HEEPSkimEventContent_cff import *
from SUSYBSMAnalysis.CSA07Skims.AODSIMHEEPSkimEventContent_cff import *
from SUSYBSMAnalysis.CSA07Skims.RECOSIMHEEPSkimEventContent_cff import *
hEEPSignalMedEtOutputModuleAODSIM = cms.OutputModule("PoolOutputModule",
    AODSIMHEEPSkimEventContent,
    hEEPSignalMedEtEventSelection,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('hEEPSignalMedEtAODSIM'),
        dataTier = cms.untracked.string('USER')
    ),
    fileName = cms.untracked.string('HeepSignalMedEtAODSIM.root')
)

hEEPSignalMedEtOutputModuleRECOSIM = cms.OutputModule("PoolOutputModule",
    RECOSIMHEEPSkimEventContent,
    hEEPSignalMedEtEventSelection,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('hEEPSignalMedEtRECOSIM'),
        dataTier = cms.untracked.string('USER')
    ),
    fileName = cms.untracked.string('HeepSignalMedEtRECOSIM.root')
)


