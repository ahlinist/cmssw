#!/bin/bash

usage()
{
    echo "usage: Timing.sh <runNumber> "
    exit 1
}

[ -z "$1" ] && usage

RUNNUM=$1 && shift


cat > "cfg/timingLaser_${RUNNUM}.cfg" << EOF
process ProcessOne = {

##########################################
         source = PoolSource {
                   untracked vstring fileNames={'file:/data/h4tb.pool/h4b.000${RUNNUM}.A.0.0.root'}
                    untracked int32 maxEvents = 1200
                   untracked uint32 debugVebosity = 10
                   untracked bool   debugFlag     = false
                  }

#######################################################


 es_source = EcalTrivialConditionRetriever { }


  # unpack raw data from test beam
  module ecalEBunpacker = EcalDCCUnpackingModule{ }


# producer of rechits starting from with custom 3 parameters fit
#(alpfa and beta fixed)

 module uncalibHitMaker =  EcalFixedAlphaBetaFitUncalibRecHitProducer{

       InputTag EBdigiCollection = ecalEBunpacker:
       InputTag EEdigiCollection = ecalEBunpacker:
       string EBhitCollection  = "EcalUncalibRecHitsEB"
       string EEhitCollection  = ""
   untracked int32 nMaxPrintout = 0
 #  untracked string AlphaBetaFilename = "/home/ghezzi/CMSSW_0_7_2/src/RecoTBCalo/EcalTBTiming/laser_shape22722.txt"   
 untracked double alpha = 1.2
 untracked double beta = 1.7
  }



  # analysis module for the timing
  module timing = EcalTimingLaser {
   string digiProducer   = "ecalEBunpacker"
   string hitProducer   = "uncalibHitMaker"
   string hitCollection = "EcalUncalibRecHitsEB"

   untracked string rootfile = "TimingLaser_${RUNNUM}.root"
   untracked string TTPeakTime = "TTPeakPositionFileLaser_${RUNNUM}.txt"
   untracked string ChPeakTime = "ChPeakTimeLaser_${RUNNUM}.txt"
   untracked double amplThr = 200. 
   untracked double minNumEvt = 100. 
   untracked double SM = 22. 
}


  path p = { ecalEBunpacker, uncalibHitMaker, timing }

    service = MessageLogger{
      untracked vstring destinations = { "pippo.txt" }
      untracked     PSet cout = { untracked string threshold = "INFO"
                  untracked   bool noTimeStamps = true
                  untracked   bool noLineBreaks = true
                   }

    untracked   PSet pippo.txt = { untracked string threshold = "DEBUG"
        untracked   PSet default = { untracked int32 limit = 1000 }
                    untracked bool noTimeStamps = true
                    untracked bool noLineBreaks = true
                   }

     }


}


EOF

cmsRun "cfg/timingLaser_${RUNNUM}.cfg" 
