import FWCore.ParameterSet.Config as cms

from Configuration.EventContent.EventContent_cff import *
from SUSYBSMAnalysis.CSA07Skims.lepSUSY_2Muon_0Elec_2Jets_MET_EventContent_cff import *
lepSUSY_2Muon_0Elec_2Jets_MET_OutputModule = cms.OutputModule("PoolOutputModule",
    AODSIMEventContent,
    lepSUSY_2Muon_0Elec_2Jets_MET_EventSelection,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('lepSUSY_2Muon_0Elec_2Jets_MET'),
        dataTier = cms.untracked.string('USER')
    ),
    fileName = cms.untracked.string('lepSUSY_2Muon_0Elec_2Jets_MET.root')
)


