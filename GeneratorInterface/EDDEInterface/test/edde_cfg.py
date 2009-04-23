import FWCore.ParameterSet.Config as cms

process = cms.Process("GEN")

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/Generator_cff')
process.load("GeneratorInterface.Pythia6Interface.pythiaDefault_cff")

process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.cerr.INFO.limit = -1

process.MessageLogger = cms.Service("MessageLogger",
                                    destinations = cms.untracked.vstring("cout"),
                                    cout = cms.untracked.PSet(default = cms.untracked.PSet(limit = cms.untracked.int32(0))
                                                              )
                                    )
#process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration/StandardSequences/Generator_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)
process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    generator = cms.PSet(
                         initialSeed = cms.untracked.uint32(98767),
                         engineName = cms.untracked.string('HepJamesRandom')
         )
)

process.source = cms.Source( "EmptySource")

process.generator = cms.EDProducer("EDDEProducer",
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    maxEventsToPrint = cms.untracked.int32(3),
    pythiaPylistVerbosity = cms.untracked.int32(2),

# for detail description of EDDE parameters see arXiv:0711.1794v2 [hep-ph] 15 Nov 2007
    EDDE = cms.PSet(
# pp centre mass energy in GeV (for LHC = 14000)
        CMEnergy = cms.double(14000.0),
#	 
# number of generating process:
# 400 p p -> p M p  
# 401 p p -> p QQbar p  (Q=b by default)                                             
# 402 p p -> p gg p 
# 403 p p -> p gamma gamma p
# 404 p p -> p {QQbar g*} p 
# 405 p p -> p {gg g*} p
# 406 p p -> p {X M Y} p  
# 407 p p -> p {X QQbar Y} p (Q=b by default)                                               
# 408 p p -> p {X gg Y} p 
# 409 p p -> p {X gamma gamma Y} p 
        ProcessNum = cms.int32(400),
#	
# for processes 401, 406: code of the central particle M, for example,
#  25 -- Higgs (RS1 scalar particles with changed branchings and total c.-s.)
#  50 -- Glueball 
        CodPart400 = cms.int32(25),
#
# for processes 401, 406: mass in GeV of the central particle M
        MassX = cms.double(120.0),
#
# transverse mass cut in GeV Mt=SQRT(Et**2+MQ**2) for the hard final partons in the c.m.s.
# for gluens and photons is equal to Et cut
       LowerEt = cms.double(25.0),
#	
# J^PC of the central particle(system), changing phi0 distributions of fwd protons
# 1(default) -> general heavy 0++ (Higgs, H*, R*,...)                           
# 2 -> (Jz=0 and +-2)
# 3 -> heavy conserved 0-+
# 4 -> conserved "glueball"
# 5 -> (Jz=+-2) ^
        JPCofSystem = cms.int32(1),
# for processes 406-409: pseudirapidity interval for "soft" radiation X and Y
# |eta_soft.rad.|<2*PseudoRapRange, Max. value of PseudoRapRange = 15
        PseudoRapRange = cms.double(5.0),
# for processes 401, 404, 407: mass in GeV of the final "hard" quark Q in the c.m.s. 
        MQinQQ = cms.double(4.8),
#	
# type of the central particle in RS1 model 
# 0(default) - SM Higgs, 
# 1 - RS1-H*, Higgs boson mass state in RS1 model
# 2 - RS1-Radion R*, Radion mass state in RS1 model
        NRSNum = cms.int32(0),
#
# for processes 401, 406 and NRSNum= 1 or 2: RS1 parameters (Phys.Rev.Lett. 83 (1999) 3370)
#
# mixing parameter (typical value -0.2->0.2)
        XIMixing = cms.double(0.16),
#	
# scale parameter GAM=246[GeV] / Lambda[GeV] (typical value of Lambda 1000->5000)	
        GAM = cms.double(0.246),
#	
# "bare" mass of Higgs in GeV before mixing (typical value 120->180)	
        MH = cms.double(150.0),
#
# "bare" mass of Radion in GeV before mixing (typical value ~100->130, ~200->300)	
        MR = cms.double(110.0)	
    ),
    PythiaParameters = cms.PSet(
	process.pythiaDefaultBlock,
        parameterSets = cms.vstring('pythiaDefault','pythia'),
        pythia = cms.vstring(
#...some PYTHIA definitions...
#            'MSTU(53) = 0            ! no smearing energy', 
#            'MSTU(54) = 3            ! massless jets',
            'MSTP(61) =1             ! Initial-state QCD and QED radiation',
            'MSTP(71) =1             ! Final-state QCD and QED radiation',
            'MSTP(81) =1             ! multiple interaction',
            'MSTP(111)=1             ! fragmentation and decay',
            'MSTP(122)=0             ! switch off X section print out',
#...Higgs decay definition...
            'MDME(210,1) =0           ! h0 -> d dbar',
            'MDME(211,1) =0           ! h0 -> u ubar',
            'MDME(212,1) =0           ! h0 -> s sbar',
            'MDME(213,1) =0           ! h0 -> c cbar',
            'MDME(214,1) =1           ! h0 -> b bbar', 
            'MDME(215,1) =0           ! h0 -> t tbar',  
            'MDME(216,1) =-1          ! h0 -> bprime bbar',
            'MDME(217,1) =-1          ! h0 -> tprime tbar',
            'MDME(218,1) =0           ! h0 -> e+e-',
            'MDME(219,1) =0           ! h0 -> mu+mu-', 
            'MDME(220,1) =0           ! h0 -> tau+tau-',
            'MDME(221,1) =-1          ! h0 -> tauprime+ tauprime-',
            'MDME(222,1) =0           ! h0 ->  gg', 
            'MDME(223,1) =0           ! h0-> gamma gamma',
            'MDME(224,1) =0           ! h0 -> gamma Z0', 
            'MDME(225,1) =0           ! h0 -> Z0 Z0',  
            'MDME(226,1) =0           ! h0 -> W+W-')
    )
)

process.output = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('edde_1.root'),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN'),
        filterName = cms.untracked.string('')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    )	
)

process.generation_step = cms.Path(process.generator)
process.out_step = cms.Path(process.output)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.out_step)
