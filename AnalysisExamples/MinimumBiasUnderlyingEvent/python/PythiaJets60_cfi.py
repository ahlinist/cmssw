import FWCore.ParameterSet.Config as cms

# Disclaimer :
#
# This set of Pythia configuration cards has been copied
# and pasted from
#   RecoTracker/RoadSearchCloudMaker/test/sim_pythia.cfg
# prepared by Oliver Gutsche (FNAL)
#
# the only change is MSTP(128)=2 - this option takes decay
# products out of doc section; decay products point at parents
# in the main section (Julia Yarba, FNAL).
source = cms.Source("PythiaSource",
    maxEvents = cms.untracked.int32(10),
    # to printout pythia event record (call pylist)
    pythiaPylistVerbosity = cms.untracked.int32(0),
    tuneModel = cms.untracked.int32(100),
    # to printout HepMC::GenEvent record (HepMC::GenEvent::print())
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    maxEventsToPrint = cms.untracked.int32(0),
    PythiaParameters = cms.PSet(
        pythiaJets60 = cms.vstring('MSEL=1', 'CKIN(3)=60'),
        parameterSets = cms.vstring('pythiaJets60')
    )
)


