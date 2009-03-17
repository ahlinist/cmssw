import FWCore.ParameterSet.Config as cms

process = cms.Process("PrintTree")
process.load('Configuration/StandardSequences/Services_cff')
process.include( "FWCore/MessageLogger/data/MessageLogger.cfi" )
process.include( "SimGeneral/HepPDTESSource/data/pythiapdt.cfi" )
process.load("Configuration.Generator.PythiaUESettings_cfi") 

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)

process.source = cms.Source("ComphepSource",
    pythiaPylistVerbosity = cms.untracked.int32(0),
    filterEfficiency = cms.untracked.double(1),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    fileNames = cms.untracked.vstring('file:/afs/fnal.gov/files/home/room1/jchen/HSCPSummer08/kktau/Mixedevts23958_kktau300_10TeV_trick.PEV'),
    process = cms.untracked.string('p1,p1 -> ~l1, ~L1'),
    CompHEPFirstEvent = cms.int32(1),
   ## comEnergy = cms.untracked.double(14000.0),                        
    crossSection = cms.untracked.double(0.01),
    getInputFromMCDB = cms.untracked.bool(False),
    maxEventsToPrint = cms.untracked.int32(1),
    MCDBArticleID = cms.int32(0),
        PythiaParameters = cms.PSet(
        process.pythiaUESettingsBlock,
        #process setting
        pythia = cms.vstring(
        'MSEL=0          ! User defined processes',        
        'MDCY(C2000015,1) = 0 !L2 does not decay'),
        
        parameterSets = cms.vstring('pythiaUESettings','pythia')
    )
)

  


process.genParticles = cms.EDProducer("GenParticleProducer",
    saveBarCodes = cms.untracked.bool(True),
    src = cms.InputTag("source"),
    abortOnUnknownPDGCode = cms.untracked.bool(True)
)

	
process.printDecay = cms.EDAnalyzer( "ParticleDecayDrawer",
  src = cms.InputTag( "genParticles" ),
#    untracked bool printP4 = true
#    untracked bool printPtEtaPhi = true
     status = cms.untracked.vint32( 3 )
)

process.printEventNumber = cms.OutputModule("AsciiOutputModule")

process.p = cms.Path( 
  process.genParticles +  process.printDecay 
)

process.o = cms.EndPath( 
  process.printEventNumber 
)





