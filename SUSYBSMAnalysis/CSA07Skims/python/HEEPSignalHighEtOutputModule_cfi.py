import FWCore.ParameterSet.Config as cms

from SUSYBSMAnalysis.CSA07Skims.HEEPSkimEventContent_cff import *
from SUSYBSMAnalysis.CSA07Skims.AODSIMHEEPSkimEventContent_cff import *
hEEPSignalHighEtOutputModuleAODSIM = cms.OutputModule("PoolOutputModule",
    AODSIMHEEPSkimEventContent,
    hEEPSignalHighEtEventSelection,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('hEEPSignalHighEtAODSIM'),
        dataTier = cms.untracked.string('USER')
    ),
    fileName = cms.untracked.string('HeepSignalHighEtAODSIM.root')
)


