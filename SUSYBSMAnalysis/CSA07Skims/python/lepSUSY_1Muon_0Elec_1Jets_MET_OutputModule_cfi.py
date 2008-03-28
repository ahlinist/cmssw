import FWCore.ParameterSet.Config as cms

from Configuration.EventContent.EventContent_cff import *
from SUSYBSMAnalysis.CSA07Skims.lepSUSY_1Muon_0Elec_1Jets_MET_EventContent_cff import *
lepSUSY_1Muon_0Elec_1Jets_MET_OutputModule = cms.OutputModule("PoolOutputModule",
    lepSUSY_1Muon_0Elec_1Jets_MET_EventSelection,
    AODSIMEventContent,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('lepSUSY_1Muon_0Elec_1Jets_MET'),
        dataTier = cms.untracked.string('USER')
    ),
    fileName = cms.untracked.string('lepSUSY_1Muon_0Elec_1Jets_MET.root')
)


