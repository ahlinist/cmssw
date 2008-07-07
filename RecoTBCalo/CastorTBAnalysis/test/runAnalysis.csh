#!/bin/bash

if [  $# -ne 2 ]
then
   echo "-----------------------------------------------------------------"
   echo "   Usage: ./bc.sh runnumber nevents_to_analyse"
   echo "   where optional runnumber = 5 digit TB07-runnumber,"
   echo "   nevents_to_analyse is optional number of events to be analysed"
   echo "-----------------------------------------------------------------"
   #exit
fi

#if (( ${#LOCALRT} < 4 ))
#then
#    echo Please setup your runtime environment!
#    exit
#fi

ELIMDEF=1000000
if [ $# -eq 1 ]
then
   RUNNUMBER=$1
   EVENTLIMIT=$ELIMDEF
elif [ $# -eq 2 ]
then
   RUNNUMBER=$1
   EVENTLIMIT=$2
else
   RUNNUMBER=36086
   EVENTLIMIT=$ELIMDEF
fi

CURRENT=`pwd`

FILE=/castor/cern.ch/cms/testbeam/tbh2acdr/TB2008_CASTOR/HTB_0${RUNNUMBER}.root
#QADCCONFIG=/afs/cern.ch/user/k/katsas/scratch0/CMSSW_1_7_5/src/RecoTBCalo/CastorTBAnalysis/test/configQADCTDC.txt
QADCCONFIG=${CURRENT}/configQADCTDC.txt
OUTDIR=`pwd`

FILEPRE=${FILE%.root}
POOLFILE=${FILEPRE}_unpacked.root
POOLFILE=${POOLFILE##*/}
#rm -f ${POOLFILE}
ANAFILE=${FILEPRE}_bc.root
ANAFILE=${ANAFILE##*/}
#rm -f ${ANAFILE}
LOGFILE=${FILEPRE}.log
LOGFILE=${LOGFILE##*/}
rm -f ${LOGFILE}


### create the file
CFGFILE=${OUTDIR}/bc.cfg

echo " "
echo "RUNNUMBER:  $RUNNUMBER"
echo "EVENTLIMIT: $EVENTLIMIT"
echo "QADCCONFIG: $QADCCONFIG"
echo "FILE:       $FILE"
echo "OUTDIR:     $OUTDIR"
echo "POOLFILE:   $POOLFILE"
echo "ANAFILE:    $ANAFILE"
echo "LOGFILE:    $LOGFILE"
echo "CFGFILE:    $CFGFILE"
echo " "

cat > ${CFGFILE}<<EOF


process PROD  = {

	source = HcalTBSource { 
		untracked vstring fileNames = { 'rfio:$FILE' }
		#untracked int32 maxEvents = ${EVENTLIMIT}
                untracked vstring streams = { 'HCAL_Trigger','HCAL_DCC700','HCAL_QADCTDC','HCAL_SlowData' }
	}

        untracked PSet maxEvents = {untracked int32 input = ${EVENTLIMIT} }

	module tbunpack = HcalTBObjectUnpacker {
                untracked int32 HcalTriggerFED  = 1    #-1
                untracked int32 HcalSlowDataFED = 3    #-1
                untracked int32 HcalTDCFED = 8
                untracked int32 HcalQADCFED = 8
		untracked bool  IncludeUnmatchedHits = false  #'true' filteres out WCE ?
		untracked string ConfigurationFile =  '$QADCCONFIG'
        }


	module out = PoolOutputModule {	
		untracked string fileName = '$POOLFILE'
	}

        module analysis = CastorTBAnalyzer
        {
                PSet CastorTBAnalyzer =
                {
                        string OutputFileName = "$ANAFILE"
                }
        }

	#module dump = HcalTBObjectDump{}
	#path p = { tbunpack, dump }

        path p = { tbunpack, analysis }
	endpath ep = { out }

}

EOF


# run cmsRun
cmsRun ${CFGFILE} > ${LOGFILE}

