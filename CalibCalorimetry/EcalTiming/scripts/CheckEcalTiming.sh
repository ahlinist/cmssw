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
echo "      -bhp|--bh_plus        bh_plus         Is Direction of BeamHalo Plus; default is true"
echo "      -ebr|--eb_radius      eb_radius       Correct EB radius in Readout Timing; default is 1.4(m)"
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

from_file="False"
from_file_name="Emptyfile.root"

correct_ecal="False"
correct_bh="False"
bh_plus="True"

data_type="Laser"
eb_radius=1.4

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
				
      -ebr|--eb_radius)
				eb_radius=$2
				;;				
				
      -bhp|--bh_plus)
				bh_plus=$2
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
echo "Beam halo direction plus:                     $bh_plus"
echo "EB Radius:                                    $eb_radius m"


echo ""
echo ""

if [[ $extension == "root" ]]; then
  input_module="
# if getting data from a .root pool file
process.source = cms.Source('PoolSource',
      skipEvents = cms.untracked.uint32($first_event),
      fileNames = cms.untracked.vstring('file:$data_path'),
      debugFlag = cms.untracked.bool(True),
      debugVebosity = cms.untracked.uint32(10)
  )"
else
  input_module="
     # if getting data from a .root pool file
process.source = cms.Source('NewEventStreamFileReader',
      skipEvents = cms.untracked.uint32($first_event),
      fileNames = cms.untracked.vstring('file:$data_path'),
      debugFlag = cms.untracked.bool(True),
      debugVebosity = cms.untracked.uint32(10)
  )"
 
fi

if [[ $manyfiles == "1" ]]; then
    echo "doing many files"
    input_module="
process.source = cms.Source('PoolSource',
      skipEvents = cms.untracked.uint32($first_event),
      fileNames = cms.untracked.vstring(`/bin/cat $files_file`),
      debugFlag = cms.untracked.bool(True),
      debugVebosity = cms.untracked.uint32(10)
  )"
fi

path="
process.p = cms.Path(process.ecalEBunpacker*rocess.ecalDccDigis*process.uncalibHitMaker*process.timing)
"

if [[ $from_file == "True" ]]; then
    echo "using an input file using the empty source"
	input_module="
process.source = cms.Source('EmptySource')
	"
	path="
process.p = cms.Path(process.timing)
        "
fi

maxevnts=$(($last_event-$first_event))

cat > "$cfg_path$data_file".graph.$$.py <<EOF
import FWCore.ParameterSet.Config as cms

process = cms.Process("ECALTIMING")
process.load("EventFilter.EcalRawToDigiDev.EcalUnpackerMapping_cfi")

process.load("EventFilter.EcalRawToDigiDev.EcalUnpackerData_cfi")

process.load("Geometry.CaloEventSetup.CaloTopology_cfi")

process.load("Geometry.EcalCommonData.EcalOnly_cfi")

process.load("Geometry.CaloEventSetup.CaloGeometry_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.GlobalTag.globaltag = 'GR09_31X_V4P::All'

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32( $maxevnts )
        )

$input_module

process.uncalibHitMaker = cms.EDProducer("EcalUncalibRecHitProducer",
                                             EEdigiCollection = cms.InputTag("ecalDccDigis","eeDigiSkim"),
                                             betaEE = cms.double(1.37),
                                             alphaEE = cms.double(1.63),
                                             EBdigiCollection = cms.InputTag("ecalDccDigis","ebDigiSkim"),
                                             EEhitCollection = cms.string('EcalUncalibRecHitsEE'),
                                             AlphaBetaFilename = cms.untracked.string('NOFILE'),
                                             betaEB = cms.double(1.7),
                                             MinAmplEndcap = cms.double(14.0),
                                             MinAmplBarrel = cms.double(8.0),
                                             alphaEB = cms.double(1.2),
                                             UseDynamicPedestal = cms.bool(True),
                                             EBhitCollection = cms.string('EcalUncalibRecHitsEB'),
                                             algo = cms.string("EcalUncalibRecHitWorkerFixedAlphaBetaFit")
                                         )


process.ecalDccDigis = cms.EDFilter("EcalDccDigiSkimer",
                                        EEdigiCollectionOut = cms.string('eeDigiSkim'),
                                        EEdigiCollection = cms.InputTag("ecalEBunpacker","eeDigis"),
                                        EBdigiCollectionOut = cms.string('ebDigiSkim'),
                                        EBdigiCollection = cms.InputTag("ecalEBunpacker","ebDigis"),
                                        DigiUnpacker = cms.InputTag("ecalEBunpacker"),
                                        DigiType = cms.string('$data_type')
                                    )

process.timing = cms.EDFilter("EcalTimingAnalysis",
                                  rootfile = cms.untracked.string('Timing${data_type}_$data_file.$$.root'),
                                  CorrectBH = cms.untracked.bool($correct_bh),
                                  hitProducer = cms.string('uncalibHitMaker'),
                                  minNumEvt = cms.untracked.double($number),
                                  FromFileName = cms.untracked.string('$from_file_name'),
                                  TTPeakTime = cms.untracked.string('TTPeakPositionFile${data_type}_${data_file}.$$.txt'),
                                  SMAverages = cms.untracked.vdouble(5.0703, 5.2278, 5.2355, 5.1548, 5.1586,
                                                                             5.1912, 5.1576, 5.1625, 5.1269, 5.643,
                                                                             5.6891, 5.588, 5.5978, 5.6508, 5.6363,
                                                                             5.5972, 5.6784, 5.6108, 5.6866, 5.6523,
                                                                             5.6666, 5.7454, 5.729, 5.7751, 5.7546,
                                                                             5.7835, 5.7529, 5.5691, 5.6677, 5.5662,
                                                                             5.6308, 5.7097, 5.6773, 5.76, 5.8025,
                                                                             5.9171, 5.8771, 5.8926, 5.9011, 5.8447,
                                                                             5.8142, 5.8475, 5.7123, 5.6216, 5.6713,
                                                                             5.3747, 5.3564, 5.39, 5.8081, 5.8081,
                                                                             5.1818, 5.1125, 5.1334, 5.2581),
                                  hitProducerEE = cms.string('uncalibHitMaker'),
                                  amplThr = cms.untracked.double($threshold),
                                  SMCorrections = cms.untracked.vdouble(0.0, 0.0, 0.0, 0.0, 0.0,
                                                                                0.0, 0.0, 0.0, 0.0, 0.2411,
                                                                                0.2411, 0.2221, 0.2221, -0.1899, -0.1899,
                                                                                -0.1509, -0.1509, 0.0451, 0.0451, -0.1709,
                                                                                -0.1709, 0.2221, 0.2221, -0.1899, -0.1899,
                                                                                -0.1359, -0.1359, -0.1359, -0.1359, 0.2221,
                                                                                0.2221, -0.2099, -0.2099, 0.2531, 0.2531,
                                                                                -0.1359, -0.1359, -0.2099, -0.2099, 0.2411,
                                                                                0.2411, 0.2531, 0.2531, -0.1709, -0.1709,
                                                                                0.0, 0.0, 0.0, 0.0, 0.0,
                                                                                0.0, 0.0, 0.0, 0.0),
                                  BeamHaloPlus = cms.untracked.bool($bh_plus),
                                  hitCollectionEE = cms.string('EcalUncalibRecHitsEE'),
                                  ChPeakTime = cms.untracked.string('ChPeakTime${data_type}_${data_file}.$$.txt'),
                                  hitCollection = cms.string('EcalUncalibRecHitsEB'),
                                  digiProducer = cms.string('ecalEBunpacker'),
                                  CorrectEcalReadout = cms.untracked.bool($correct_ecal),
                                  FromFile = cms.untracked.bool($from_file),
                                  RunStart = cms.untracked.double($start_time),
                                  RunLength = cms.untracked.double($run_length),
                                  EBRadius = cms.untracked.double($eb_radius)
                              )

$path

EOF



echo "initializing cmssw..."
#. /nfshome0/cmssw/cmsset_default.sh
export FRONTIER_FORCERELOAD=long
export STAGER_TRACE=3
cd $cmssw_dir;
eval `scramv1 ru -sh`;
cd -;
echo "... running"
cmsRun "$cfg_path$data_file".graph.$$.py >& "$log_dir$data_file".$$.graph

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



