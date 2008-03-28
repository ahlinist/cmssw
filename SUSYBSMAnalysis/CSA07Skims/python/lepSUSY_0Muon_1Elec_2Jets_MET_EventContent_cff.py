import FWCore.ParameterSet.Config as cms

# Can insert block to customize what goes inside root file on top of AOD/RECO
lepSUSYEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring()
)
lepSUSY_0Muon_1Elec_2Jets_MET_EventSelection = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('lepSUSY_0Muon_1Elec_2Jets_MET')
    )
)

