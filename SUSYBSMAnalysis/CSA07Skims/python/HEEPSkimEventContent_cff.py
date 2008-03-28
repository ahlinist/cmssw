import FWCore.ParameterSet.Config as cms

#
# Currently no private output for the HEEP skim, maybe add something later
#
hEEPSkimEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring()
)
#
# keep only events triggered by High pt triggers
#
hEEPSignalHighEtEventSelection = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('hEEPSignalHighEt')
    )
)
#
# Keep events by High Et triggers plus std. single electron with pt cut
#
hEEPSignalMedEtEventSelection = cms.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('hEEPSignalMedEtHigh', 'hEEPSignalMedEtMedBarrel', 'hEEPSignalMedEtMedEndcap')
    )
)

