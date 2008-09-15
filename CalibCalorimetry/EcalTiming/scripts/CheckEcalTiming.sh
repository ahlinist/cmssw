#!/bin/bash

#preferred_dir="/home/daq/"
preferred_dir=`pwd`
log_dir=$preferred_dir/log/
conf_dir=$preferred_dir/conf/
#cmssw_dir="/home/daq/DUMP/CMSSW_1_3_1/src"
cmssw_dir=`pwd`

# in case you wanted to force  execution inside the preferred_dir
#if [ "$PWD" != $preferred_dir ]; then
#
# echo ""
# echo "this script should be executed from $preferred_dir"
# echo "please do:"
# echo "            cd $preferred_dir"
# echo ""
## exit
#
#fi

mkdir -p  $preferred_dir/log/
mkdir -p  $preferred_dir/conf/
mkdir -p  $preferred_dir/data/

if [ ! -n "$1" ]

then

echo ""
echo "This script produces timing plots for all SM's and all TT's and each crystal based on laser data."
echo ""
echo "Options:"
echo ""
echo "      -p|--path_file        file_path       data file to be analyzed preceeded by path"
echo ""
echo "      -f|--first_ev         f_ev            first (as written to file) event that will be analyzed; default is 1"
echo "      -l|--last_ev          l_ev            last  (as written to file) event that will be analyzed; default is 9999"
echo "      -mfed|--mask_fed_id   mask_fed_id     list of FEDids to mask; default is no masking"
echo "      -meb|--mask_ieb_id    mask_ieb_id     list of sm barrel ids to mask; default is no masking"
echo "      -mcry|--mask_cry      mask_cry        list of channels (use hashedIndex) to mask; default is no masking"
echo "      -t|--threshold        threshold       ADC count threshold for laser event to be considered good; default is 200.0"
echo "      -n|--number           number          Minnimum number of events in crystal to be considered good"
echo "      -files|--files_file   files_file      File with list of Runs"
echo "      -st|--start_time      start_time      StartTime of run from Jan 1 1970 in s"
echo "      -rl|--run_length      run_length      Length of Run in hours"
echo "      -ff|--from_file       from_file       Read from an input file; default is false"
echo "      -ffn|--from_file_name from_file_name  Name of input file; default is empty"
echo "      -dt|--data_type       data_type       Data Type of interest; default is Laser"
echo "      -cet|--correct_ecal   correct_ecal    Correct For Ecal Readout Timing; default is false"
echo "      -cbh|--correct_bh     correct_bh      Correct For BeamHalo Readout Timing; default is false"
echo ""
echo "To specify multiple fed_id's/ieb_id's/cry's to mask use a comma-separated list in between double quotes, e.g., \"1,2,3\" "
exit

fi


data_path="/data/ecalod-22/daq-data/"
data_file="none"

cfg_path="$conf_dir"


mfed=-1
mieb="-1"
mcry=-1

threshold=200.0
number=5

start_time=1215192037
run_length=2

first_event=1
last_event=999999

from_file="false"
from_file_name="Emptyfile.root"

correct_ecal="false"
correct_bh="false"

data_type="Laser"

manyfiles="0"

  while [ $# -gt 0 ]; do    # while there are parameters available...
    case "$1" in

      -p|--path_file)
                data_path="$2"
                ;;


      -f|--first_ev)
                first_event="$2"
                ;;


      -l|--last_ev)
                last_event="$2"
                ;;


      -mfed|--mask_fed_id)
                mfed=$2
                ;;

      -meb|--mask_ieb_id)
                mieb=$2
                ;;

      -mcry|--mask_cry)
                mcry=$2
                ;;

      -n|--number)
                number=$2
                ;;

      -t|--threshold)
                threshold=$2
                ;;

      -files|--files_file)
                manyfiles="1"
                files_file=$2
                ;;
      
      -st|--start_time)
                start_time=$2
                ;;
	
      -rl|--run_length)
                run_length=$2
                ;;
				
	  -ff|--from_file)
                from_file=$2
                ;;
				
	  -ffn|--from_file_name)
                from_file_name=$2
                ;;
				
	  -dt|--data_type)
                data_type=$2
                ;;	
				
	  -cet|--correct_ecal)
				correct_ecal=$2
				;;
				
	  -cbh|--correct_bh)
				correct_bh=$2
				;;			
				

    esac
    shift       # Verifica la serie successiva di parametri.

done

data_file=${data_path##*/} 
extension=${data_file##*.}

echo ""
echo ""
echo "data to be analyzed:                          $data_file"
echo "or data to be analyzed:                       $files_file"
echo "first event analyzed will be:                 $first_event"
first_event=$(($first_event-1))

echo "last event analyzed will be:                  $last_event"
echo "supermodules to mask:                         ${mieb} (-1 => no masking)"
echo "feds to mask:                                 ${mfed} (-1 => no masking)"
echo "crys to mask:                                 ${mcry} (-1 => no masking)"

echo "amplitude threshold:                          $threshold"

echo "number:                                       $number"
echo "start time:                                   $start_time"
echo "run length:                                   $run_length (hours)"
echo "from_file:                                    $from_file"
echo "from_file_name:                               $from_file_name"
echo "data_type:                                    $data_type"
echo "correct for ecal readout:                     $correct_ecal"
echo "correct for beam halo:                        $correct_bh"


echo ""
echo ""

if [[ $extension == "root" ]]; then
  input_module="
# if getting data from a .root pool file
  source = PoolSource {
    untracked uint32 skipEvents = $first_event
      untracked vstring fileNames = { 'file:$data_path' }
    untracked bool   debugFlag     = true
   }"
else
  input_module="
     source = NewEventStreamFileReader{
       untracked uint32 skipEvents = $first_event
       untracked vstring fileNames = { 'file:$data_path' }
       untracked uint32 debugVebosity = 10
       untracked bool   debugFlag     = true
     }" 
fi

if [[ $manyfiles == "1" ]]; then
    echo "doing many files"
    input_module="
    #source = NewEventStreamFileReader{
	source = PoolSource{
       untracked uint32 skipEvents = $first_event
       untracked vstring fileNames = { 
	   `/bin/cat $files_file` 
	   }
       untracked uint32 debugVebosity = 10
       untracked bool   debugFlag     = true
    }"
fi

path="
path p = {ecalEBunpacker, ecalDccDigis, uncalibHitMaker, timing}
"

if [[ $from_file == "true" ]]; then
    echo "using an input file using the empty source"
	input_module="
	source = EmptySource{	
	}"
	path="
    path p = {timing}
    "
fi

maxevnts=$(($last_event-$first_event))

cat > "$cfg_path$data_file".graph.$$.cfg <<EOF


process ECALTIMING = { 

include "EventFilter/EcalRawToDigiDev/data/EcalUnpackerMapping.cfi"
include "EventFilter/EcalRawToDigiDev/data/EcalUnpackerData.cfi"  

include "Geometry/CaloEventSetup/data/CaloTopology.cfi"
include "Geometry/EcalCommonData/data/EcalOnly.cfi"
include "Geometry/CaloEventSetup/data/CaloGeometry.cff"

untracked PSet maxEvents = {untracked int32 input = $maxevnts }

$input_module


es_source src1 = EcalTrivialConditionRetriever{
     untracked vdouble amplWeights = { -0.333, -0.333, -0.333,
                                        0.000,  0.000,  1.000,
                                        0.000,  0.000,  0.000,  0.000 }
     untracked vdouble pedWeights  = {  0.333,  0.333,  0.333,
                                        0.000,  0.000,  0.000,
                                        0.000,  0.000,  0.000,  0.000 }
     untracked vdouble jittWeights = {  0.040,  0.040,  0.040,
                                        0.000,  1.320, -0.050,
                                       -0.500, -0.500, -0.400,  0.000 }
 }

      
module uncalibHitMaker =  EcalFixedAlphaBetaFitUncalibRecHitProducer{

       InputTag EBdigiCollection = ecalDccDigis:ebDigiSkim
       InputTag EEdigiCollection = ecalDccDigis:eeDigiSkim
       #InputTag EBdigiCollection = ecalEBunpacker:ebDigis
       #InputTag EEdigiCollection = ecalEBunpacker:eeDigis
       string EBhitCollection  = "EcalUncalibRecHitsEB"
       string EEhitCollection  = "EcalUncalibRecHitsEE"
       double alphaEB  = 1.2
   double betaEB   = 1.7
   double alphaEE  = 1.63
   double betaEE   = 1.37
   double MinAmplBarrel	= 8.
   double MinAmplEndcap	= 14.
   bool UseDynamicPedestal = true
   untracked string AlphaBetaFilename  = "NOFILE"

  }
  
 module ecalDccDigis = EcalDccDigiSkimer {
   InputTag DigiUnpacker     = ecalEBunpacker
   InputTag EBdigiCollection =  ecalEBunpacker:ebDigis
   InputTag EEdigiCollection =  ecalEBunpacker:eeDigis
   
   string EBdigiCollectionOut = 'ebDigiSkim'
   string EEdigiCollectionOut = 'eeDigiSkim'

   string DigiType = "$data_type"
}

 module timing = EcalTimingAnalysis {
   string digiProducer   = "ecalEBunpacker"
   string hitProducer   = "uncalibHitMaker"
   string hitProducerEE   = "uncalibHitMaker"
   string hitCollection = "EcalUncalibRecHitsEB"
   string hitCollectionEE = "EcalUncalibRecHitsEE"

   untracked string rootfile = "Timing${data_type}_$data_file.$$.root"
   untracked string TTPeakTime = "TTPeakPositionFile${data_type}_${data_file}.$$.txt"
   untracked string ChPeakTime = "ChPeakTime${data_type}_${data_file}.$$.txt"
   untracked double amplThr = $threshold
   untracked double minNumEvt = $number
   untracked double RunStart = $start_time
   untracked double RunLength = $run_length
   untracked bool FromFile = $from_file
   untracked string FromFileName = "$from_file_name"
   untracked bool CorrectEcalReadout = $correct_ecal
   untracked bool CorrectBH = $correct_bh
   #untracked vdouble SMAverages = {5.00, 5.00,  5.00,  5.00,  5.00,  5.00,  5.00,  5.00,  5.00,   5.2177, 5.2282,  5.1461, 5.1395, 4.8314, 4.7773, 4.8276, 4.8607, 4.9925,   5.0648, 4.837, 4.7639, 5.2952, 5.2985, 4.8695, 4.8308, 4.9181, 4.8526, 4.7157, 4.7297, 5.1266, 5.1656, 4.8872, 4.8274, 5.3140, 5.3209, 5.0342, 5.0402, 5.0712, 4.9686, 5.4509, 5.3868, 5.3950, 5.2342, 5.00, 5.00, 5.00,  5.00,  5.00,  5.00,  5.00,  5.00,  5.00,  5.00,  5.00}
   
  # untracked vdouble SMAverages = {5.00, 5.00,  5.00,  5.00,  5.00,  5.00,  5.00,  5.00,  5.00,   
   #                               6.2177, 6.2282, 6.1271, 6.1205, 5.4004, 5.3463, 5.4356, 5.4687, 5.7965,   
#				  5.8688, 5.4250, 5.3519, 6.2762, 6.2795, 5.4385, 5.3998, 5.5411, 5.4756, 
#				  5.3387, 5.3527, 6.1076, 6.1466, 5.4364, 5.3764, 6.3260, 6.3329, 5.6572, 
#				  5.6632, 5.6202, 5.5176, 6.4509, 6.3868, 6.4070, 6.2462, 5.00, 5.00, 
#				  5.00,  5.00,  5.00,  5.00,  5.00,  5.00,  5.00,  5.00,  5.00}
				  
   untracked vdouble SMAverages = {5.00, 5.00,  5.00,  5.00,  5.00,  5.00,  5.00,  5.00,  5.00,   
                                  5.3805, 5.4358, 5.377, 5.287, 4.96, 4.9758, 4.9893, 5.0638, 5.2273,   
				  5.242, 4.9926, 4.9928, 5.5974, 5.4966, 5.1288, 5.127, 5.2627, 5.1177, 
				  5.0262, 5.0702, 5.3998, 5.5244, 5.1255, 5.0881, 5.7494, 5.6323, 5.4287, 
				  5.4655, 5.3829, 5.3226, 5.7801, 5.7548, 5.8181, 5.6015, 5.00, 5.00, 
				  5.00,  5.00,  5.00,  5.00,  5.00,  5.00,  5.00,  5.00,  5.00}	
				  
     untracked vdouble SMCorrections = {0.00, 0.00,  0.00,  0.00,  0.00,  0.00,  0.00,  0.00,  0.00,   
                                  0.2411, 0.2411, 0.2221, 0.2221, -0.1899, -0.1899, -0.1509, -0.1509, 0.0451,   
				  0.0451, -0.1709,-0.1709, 0.2221, 0.2221, -0.1899, -0.1899, -0.1359, -0.1359, 
				  -0.1359, -0.1359, 0.2221, 0.2221, -0.2099, -0.2099, 0.2531, 0.2531, -0.1359, 
				  -0.1359, -0.2099, -0.2099, 0.2411, 0.2411, 0.2531, 0.2531, -0.1709, -0.1709, 
				  0.00,  0.00,  0.00,  0.00,  0.00,  0.00,  0.00,  0.00,  0.00}					  
  				  
   #
}

      

$path

}


EOF



echo "initializing cmssw..."
#. /nfshome0/cmssw/cmsset_default.sh
export FRONTIER_FORCERELOAD=long
export STAGER_TRACE=3
cd $cmssw_dir;
eval `scramv1 ru -sh`;
cd -;
echo "... running"
cmsRun "$cfg_path$data_file".graph.$$.cfg >& "$log_dir$data_file".$$.graph

echo ""
echo ""

mv Timing*.root log/
mv *Peak*txt data/
echo "File root with graphs was created:" 
ls -ltrFh $preferred_dir/log/Timing*.root | tail -1 | awk '{print $9}'

echo ""
echo ""
echo "Now you can look at the plots..."
echo ""
echo ""



