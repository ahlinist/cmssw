import FWCore.ParameterSet.Config as cms

process = cms.Process("Gen")
process.load("FWCore.MessageService.MessageLogger_cfi")

# control point for all seeds
process.load("Configuration.StandardSequences.SimulationRandomNumberGeneratorSeeds_cff")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

# PYTHIA input file
process.load("Configuration.Spring08Production.Spring08_JetMET_PhotonJetsBkg_EtGamma_135_165_cfi")

# Filter
process.load("GeneratorInterface.GenFilters.PythiaFilterGammaJetWithBg_cfi")

process.load("Configuration.EventContent.EventContent_cff")

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/Spring08Production/data/Spring08_JetMET_PhotonJetsBkg_EtGamma_135_165_GEN.cfg,v $'),
    annotation = cms.untracked.string('FastSim gamma+jet+bg Etgamma=135-165 GeV for Spring08')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)
process.GEN = cms.OutputModule("PoolOutputModule",
    process.FEVTSIMEventContent,
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('p1')
    ),
    fileName = cms.untracked.string('gamma_jet_bg135_165.root')
)

process.p1 = cms.Path(process.gj_filter)
process.e = cms.EndPath(process.GEN)
process.schedule = cms.Schedule(process.p1,process.e)

process.gj_filter.MinPhotonPt = 135
process.gj_filter.MaxPhotonPt = 165
process.gj_filter.MaxEvents = 1000

