#!/bin/sh

export SCRAM_ARCH=slc4_ia32_gcc345
. /uscmst1/prod/sw/cms/bashrc cmslpc
export PATH=/usr/bin:$PATH

cd /uscms/home/yarba_j/work/SL4/CMSSW_1_6_0_pre7/src
eval `scramv1 runtime -sh`

cd /uscms/home/yarba_j/work/SL4/CMSSW_1_6_0_pre7/src/Validation/Geant4Releases/data

###if ( ! -e /uscms_scratch/3DayLifetime/yarba_j/ttbar-160pre7 ) mkdir /uscms_scratch/3DayLifetime/yarba_j/ttbar-160pre7
###cd /uscms_scratch/3DayLifetime/yarba_j/ttbar-160pre7

rndm_source=135799753
rndm_VtxSmeared=123456789
rndm_g4SimHits=9876

let "rndm_source+=$1"
let "rndm_VtxSmeared+=$1"
let "rndm_g4SimHits+=$1"

echo "rndm_source "     : ${rndm_source}
echo "rndm_VtxSmeared " : ${rndm_VtxSmeared}
echo "rndm_g4SimHits "  : ${rndm_g4SimHits}


cat > tmp1_ttbar_$1.cfg <<EOF

process Sim = {

   untracked PSet maxEvents = { untracked int32 input = 2 }
   
   service = MessageLogger
   {
      untracked vstring destinations = {"cout"}

      untracked vstring categories = { "FwkJob", "SimG4CoreApplication" }

      untracked PSet cout = 
      {
         untracked PSet default = { untracked int32 limit = 0 }  # kill all messages in the log
	 untracked PSet FwkJob  = { untracked int32 limit = -1 } # but FwkJob category - those unlimitted
	 untracked PSet SimG4CoreApplication = { untracked int32 limit = -1 } 
      }

      # uncomment if you want it...
      #
      # untracked vstring fwkJobReports = {"FrameworkJobReport.xml"}      
   }

#    untracked PSet options = {
#       include "FWCore/Framework/test/cmsExceptionsFatalOption.cff"
#       untracked bool wantSummary = true
#       untracked bool makeTriggerResults = true 
#    }

   service = Timing {}
   
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

   include "SimG4Core/Configuration/data/SimG4Core.cff"
   # Event, etc. output
   #
   module GEN-SIM = PoolOutputModule 
   { 
      untracked string fileName = "pythia_ttbar_detsim_${1}.root" 
      untracked PSet dataset =
      {
         untracked string dataTier = "GEN-SIM"
      }
   }
		
   # Geant4-based CMS Detector simulation
   #
   include "SimG4Core/Application/data/g4SimHits.cfi"

   # now the order of execution
   #   
   path p1 = { VtxSmeared, g4SimHits, rndmStore }
   ### endpath outpath = { GEN-SIM }

}

EOF

cmsRun  tmp1_ttbar_$1.cfg

###dccp pythia_ttbar_detsim_${1}.root /pnfs/cms/WAX/2/yarba_j/pythia_ttbar_detsim_${1}.root
