import FWCore.ParameterSet.Config as cms

process = cms.Process("Gen")
process.load("FWCore.MessageService.MessageLogger_cfi")

# control point for all seeds
#
process.load("Configuration.StandardSequences.SimulationRandomNumberGeneratorSeeds_cff")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

# physics event generation
#  
process.load("Configuration.Spring08Production.Spring08_QCD_Pythia_cfi")

process.load("Configuration.EventContent.EventContent_cff")

process.maxEvents = cms.untracked.PSet(
    output = cms.untracked.int32(10)
)
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.2 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/Spring08Production/data/Spring08_QCD_PythiaFilterGammaGamma_GEN.cfg,v $'),
    annotation = cms.untracked.string('generation of QCD, CTEQ 6L1 used')
)
process.filter = cms.EDFilter("PythiaFilterGammaGamma",
    AcceptPrompts = cms.untracked.bool(False),
    PtSeedThr = cms.untracked.double(5.0),
    NTkConeSum = cms.untracked.int32(3),
    moduleLabel = cms.untracked.string('source'),
    EtaElThr = cms.untracked.double(2.8),
    EtaSeedThr = cms.untracked.double(2.8),
    dRNarrowCone = cms.untracked.double(0.02),
    EtaMaxCandidate = cms.untracked.double(3.0),
    dPhiSeedMax = cms.untracked.double(0.3),
    EtaGammaThr = cms.untracked.double(2.8),
    InvMassWide = cms.untracked.double(80.0),
    EtaTkThr = cms.untracked.double(2.2),
    PtElThr = cms.untracked.double(2.0),
    NTkConeMax = cms.untracked.int32(2),
    dEtaSeedMax = cms.untracked.double(0.12),
    PromptPtThreshold = cms.untracked.double(20.0),
    PtMinCandidate2 = cms.untracked.double(22.5),
    PtGammaThr = cms.untracked.double(0.0),
    PtMinCandidate1 = cms.untracked.double(37.5),
    dRSeedMax = cms.untracked.double(0.0),
    PtTkThr = cms.untracked.double(1.6),
    InvMassNarrow = cms.untracked.double(14000.0),
    dRTkMax = cms.untracked.double(0.2)
)

process.GEN = cms.OutputModule("PoolOutputModule",
    process.FEVTSIMEventContent,
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('p1')
    ),
    fileName = cms.untracked.string('PythiaQCDFilterGG.root')
)

process.p1 = cms.Path(process.filter)
process.outpath = cms.EndPath(process.GEN)
process.schedule = cms.Schedule(process.p1,process.outpath)


