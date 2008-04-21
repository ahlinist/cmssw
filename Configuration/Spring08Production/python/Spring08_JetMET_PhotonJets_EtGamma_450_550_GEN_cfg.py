import FWCore.ParameterSet.Config as cms

process = cms.Process("Gen")
process.load("FWCore.MessageService.MessageLogger_cfi")

# control point for all seeds
process.load("Configuration.StandardSequences.SimulationRandomNumberGeneratorSeeds_cff")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

# PYTHIA input file
process.load("Configuration.Spring08Production.Spring08_JetMET_PhotonJets_EtGamma_450_550_cfi")

# Filter
process.load("GeneratorInterface.GenFilters.PythiaFilterGammaJetWithOutBg_cfi")

process.load("Configuration.EventContent.EventContent_cff")

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/Spring08Production/data/Spring08_JetMET_PhotonJets_EtGamma_450_550_GEN.cfg,v $'),
    annotation = cms.untracked.string('FastSim gamma+jet Etgamma=450-550 GeV for Spring08')
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
    fileName = cms.untracked.string('gamma_jet450_550.root')
)

process.p1 = cms.Path(process.gj_filter)
process.e = cms.EndPath(process.GEN)
process.schedule = cms.Schedule(process.p1,process.e)

process.gj_filter.MinPhotonPt = 450
process.gj_filter.MaxPhotonPt = 550
process.gj_filter.MaxEvents = 1000

