import FWCore.ParameterSet.Config as cms

process = cms.Process("PrintTree")
process.load('Configuration/StandardSequences/Services_cff')
process.include( "FWCore/MessageLogger/data/MessageLogger.cfi" )
process.include( "SimGeneral/HepPDTESSource/data/pythiapdt.cfi" )
 

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound')
)


process.source = cms.Source("ComphepSource",
    pythiaPylistVerbosity = cms.untracked.int32(0),
    filterEfficiency = cms.untracked.double(0.4614),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    fileNames = cms.untracked.vstring('file:Mixed.pev'),
    process = cms.untracked.string('p1,p1 -> m,M'),
    CompHEPFirstEvent = cms.int32(1),
    crossSection = cms.untracked.double(0.5496),
    getInputFromMCDB = cms.untracked.bool(False),
    maxEventsToPrint = cms.untracked.int32(1),
    MCDBArticleID = cms.int32(0),
    PythiaParameters = cms.PSet(
        parameterSets = cms.vstring('pythiaUESettings','pythia'),
        pythiaUESettings = cms.vstring('MSTJ(11)=3     ! Choice of the fragmentation function', 
                'MSTJ(22)=2     ! Decay those unstable particles', 
                'PARJ(71)=10 .  ! for which ctau  10 mm', 
                'MSTP(2)=1      ! which order running alphaS', 
                'MSTP(33)=0     ! no K factors in hard cross sections', 
                'MSTP(51)=7     ! structure function chosen', 
                'MSTP(81)=1     ! multiple parton interactions 1 is Pythia default', 
                'MSTP(82)=4     ! Defines the multi-parton model', 
                'MSTU(21)=1     ! Check on possible errors during program execution', 
                'PARP(82)=1.9409   ! pt cutoff for multiparton interactions', 
                'PARP(89)=1960. ! sqrts for which PARP82 is set', 
                'PARP(83)=0.5   ! Multiple interactions: matter distrbn parameter', 
                'PARP(84)=0.4   ! Multiple interactions: matter distribution parameter', 
                'PARP(90)=0.16  ! Multiple interactions: rescaling power', 
                'PARP(67)=2.5    ! amount of initial-state radiation', 
                'PARP(85)=1.0  ! gluon prod. mechanism in MI', 
                'PARP(86)=1.0  ! gluon prod. mechanism in MI', 
                'PARP(62)=1.25   ! ', 
                'PARP(64)=0.2    ! ', 
                'MSTP(91)=1     !', 
                'PARP(91)=2.1   ! kt distribution', 
                'PARP(93)=15.0  ! ')
       
        ,pythia = cms.vstring('MSEL=0              !(D=2)')
    )
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





