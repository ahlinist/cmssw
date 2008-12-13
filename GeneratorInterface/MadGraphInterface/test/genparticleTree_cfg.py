import FWCore.ParameterSet.Config as cms

process = cms.Process("PrintTree")
process.include( "FWCore/MessageLogger/data/MessageLogger.cfi" )
process.include( "SimGeneral/HepPDTESSource/data/pythiapdt.cfi" )
process.maxEvents = cms.untracked.PSet( 
  input = cms.untracked.int32(100) 
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:MGtest.root')
)

process.genParticles = cms.EDProducer("GenParticleProducer",
    saveBarCodes = cms.untracked.bool(True),
    src = cms.InputTag("source"),
    abortOnUnknownPDGCode = cms.untracked.bool(True)
)


process.printTree = cms.EDAnalyzer( "ParticleTreeDrawer",
  src =cms.InputTag( "genParticles" ),
#    printP4 = cms.untracked.bool( True ),
#    printPtEtaPhi = cms.untracked.bool( True ),
#    printStatus = cms.untracked.bool( True ),
  status = cms.untracked.vint32( 3 ),
  printIndex = cms.untracked.bool(True )
)
	
process.printDecay = cms.EDAnalyzer( "ParticleDecayDrawer",
  src = cms.InputTag( "genParticles" ),
#    untracked bool printP4 = true
#    untracked bool printPtEtaPhi = true
  status = cms.untracked.vint32( 3 )
)

process.printEventNumber = cms.OutputModule("AsciiOutputModule")

process.p = cms.Path( 
  process.genParticles *
  process.printTree * 
  process.printDecay 
)

process.o = cms.EndPath( 
  process.printEventNumber 
)

