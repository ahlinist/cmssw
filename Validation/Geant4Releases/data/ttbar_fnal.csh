#!/bin/tcsh -f

setenv SCRAM_ARCH slc4_ia32_gcc345
source /uscmst1/prod/sw/cms/cshrc cmslpc
setenv PATH /usr/bin:$PATH

cd /uscms/home/yarba_j/work/SL4/CMSSW_1_7_3_g490p1/src
eval `scramv1 runtime -csh`

if ( ! -e /uscmst1b_scratch/lpc1/3DayLifetime/yarba_j/173g490p1 ) mkdir /uscmst1b_scratch/lpc1/3DayLifetime/yarba_j/173g490p1
cd /uscmst1b_scratch/lpc1/3DayLifetime/yarba_j/173g490p1

set rndm_source=135799753
set rndm_VtxSmeared=123456789
set rndm_g4SimHits=9876

@ rndm_source = (${rndm_source} + $1)
@ rndm_VtxSmeared = (${rndm_VtxSmeared} + $1 )
@ rndm_g4SimHits  = (${rndm_g4SimHits} + $1)

echo "rndm_source "     : ${rndm_source}
echo "rndm_VtxSmeared " : ${rndm_VtxSmeared}
echo "rndm_g4SimHits "  : ${rndm_g4SimHits}


cat > temp_ttbar_$1.cfg <<EOF

process Sim = {

   untracked PSet maxEvents = { untracked int32 input = 100 }
   
   service = MessageLogger
   {
      untracked vstring destinations = {"cout"}

      untracked vstring categories = { "FwkJob", "SimG4CoreApplication", "G4cout", "G4cerr" }

      untracked PSet cout = 
      {
         untracked bool noTimeStamps = true
	 untracked PSet default = { untracked int32 limit = 0 }  # kill all messages in the log
	 untracked PSet FwkJob  = { untracked int32 limit = -1 } # but FwkJob category - those unlimitted
	 untracked PSet SimG4CoreApplication = { untracked int32 limit = -1 }
	 untracked PSet G4cout = {  untracked int32 limit = -1 }
	 untracked PSet G4cerr = {  untracked int32 limit = -1 }	 
      }

      # uncomment if you want it...
      #
      # untracked vstring fwkJobReports = {"FrameworkJobReport.xml"}      
   }

   service = Timing {}

   service = SimpleMemoryCheck
   {
      untracked int32 ignoreTotal = 1 # default is one
      untracked bool oncePerEventMode = true # default is false, so it only reports increases
   }   
   
   service = RandomNumberGeneratorService {
           untracked uint32 sourceSeed = ${rndm_source}
           PSet moduleSeeds =
           {
                   untracked uint32 VtxSmeared = ${rndm_VtxSmeared}
                   untracked uint32 g4SimHits = ${rndm_g4SimHits}
           }
   }

   #
   # this module is standard features starting release 1_2_0_pre4
   # and should be used to store random numbers for modules into
   # edm::Event (but not for sources, as of today Nov.7, 2006)
   #
   # of course, you have to place in the path to execute;
   # however, it's the service that caches the state of the engines,
   # and the producer only writes it into the event, thus it's NOT
   # important where in the path you place it - the state of the
   # engines will always be stored as of the beginning of an event
   # 
   module rndmStore = RandomEngineStateProducer { }

   source = PythiaSource {
     untracked int32 pythiaPylistVerbosity = 0
     untracked bool pythiaHepMCVerbosity = false
     untracked int32 maxEventsToPrint = 0
     untracked bool pythiaVerbosity = false

     PSet PythiaParameters = {
       # This is a vector of ParameterSet names to be read, in this order
       vstring parameterSets =  {
         "pythiaUESettings",
         "processParameters"
       }

       # CMS default pythia parameters
       include "Configuration/Generator/data/PythiaUESettings.cfi"
   
       vstring processParameters = {
         'MSEL=0                  ! User defined processes',
         'MSUB(81) = 1            ! qqbar to QQbar',
         'MSUB(82) = 1            ! gg to QQbar',
         'MSTP(7) = 6             ! flavour = top',
         'PMAS(6,1) = 175.        ! top quark mass'
       }
     }
   }


   # event vertex smearing - applies only once (internal check)
   # Note : all internal generators will always do (0,0,0) vertex
   #
   include "IOMC/EventVertexGenerators/data/VtxSmearedGauss.cfi"

   # Geant4-based CMS Detector simulation
   #
   include "SimG4Core/Configuration/data/SimG4Core.cff"

   # Event, etc. output
   #

   # standard "prescription of what to keep in edm::Event upon output
   #
   include "Configuration/EventContent/data/EventContent.cff"
   
   # keeping random seeds must be added
   #
   block RndmStoreFEVT =
   {
      untracked vstring outputCommands = { "keep RandomEngineStates_*_*_*" }
   }
   replace FEVTSIMEventContent.outputCommands += RndmStoreFEVT.outputCommands

   module GEN-SIM = PoolOutputModule 
   { 
      using FEVTSIMEventContent
      untracked string fileName = "pyth_ttbar_detsim_${1}.root" 
#      untracked PSet dataset =
#      {
#         untracked string dataTier = "GEN-SIM"
#      }
   }
		
   # now the order of execution
   #   
   path p1 = { VtxSmeared, g4SimHits, rndmStore }
   endpath outpath = { GEN-SIM }
   schedule = { p1, outpath }

}

EOF

cmsRun  temp_ttbar_$1.cfg

###dccp pyth_ttbar_detsim_${1}.root /pnfs/cms/WAX/2/yarba_j/pyth_ttbar_detsim_${1}.root
