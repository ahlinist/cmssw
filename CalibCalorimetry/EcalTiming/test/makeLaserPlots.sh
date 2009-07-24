#!/bin/bash

usage='Usage: -r <run number>'

args=`getopt r: -- "$@"`
if test $? != 0
     then
         echo $usage
         exit 1
fi

eval set -- "$args"
for i 
  do
  case "$i" in
      -r) shift; run_num=$2;shift;;
#      -at) shift; analy_type=$2;shift;;
  esac      
done

if [ "X"${run_num} == "X" ]
    then
    echo "INVALID RUN NUMBER! Please give a valid run number!"
    echo $usage
    exit 
fi

#if [ "X"${analy_type} == "X" ]
#    then
#    analy_type="Laser"
#    echo " using default analysis type of Laser"
#else
#    echo "using ${analy_type} type events  "
#fi

echo 'Making Laser Webpages for ' ${run_num} 


# specify directories here
my_cmssw_base='/afs/cern.ch/cms/CAF/CMSCOMM/COMM_ECAL/ccecal/CRAFT_devel_321/src'
work_dir=${my_cmssw_base}'/CalibCalorimetry/EcalTiming/test/SCRIPTS'

plots_dir=plots/Laser/$run_num;
mkdir $plots_dir

crab_dir=`\ls -rt1 ${work_dir}/Laser_${run_num} | grep "crab_" | tail -1 | awk '{print $NF}'`;

echo $crab_dir

#root_file=Laser_${run_num}_1.root
root_file=Laser_${run_num}.root
cp ${work_dir}/Laser_${run_num}/${crab_dir}/res/${root_file} ${plots_dir}

echo
echo 'Going to make the plots, by running in ROOT:'
echo
echo '.L '${my_cmssw_base}'/CalibCalorimetry/EcalTiming/test/SCRIPTS/plotLaser.C'
echo 'DrawLaserPlots("'${plots_dir}'/'${root_file}'",'${run_num}',kTRUE,"png","'${plots_dir}'",kFALSE)'
echo

#now I need to make a little python script to make my root plots

cat > ${plots_dir}/plot.py <<EOF

from ROOT import gROOT

#load my macro
gROOT.LoadMacro(  '${my_cmssw_base}/CalibCalorimetry/EcalTiming/test/plotLaser.C')

#get my cute class
from ROOT import DrawLaserPlots

DrawLaserPlots("${plots_dir}/${root_file}",${run_num},True,"png","${plots_dir}",False)


EOF


python ${plots_dir}/plot.py -b

rm ${plots_dir}/plot.py



cat > ${plots_dir}/index.html <<EOF


<HTML>

<HEAD><TITLE>ECAL Laser Analysis Run ${run_num}</TITLE></HEAD>
 
<BODY link="Red">
<FONT color="Black">
 
<Center>
<h2><FONT color="Blue"> ECAL Laser Analysis: </FONT></h2>
</Center>

<Center>
<h3> Run: <A href=http://cmsmon.cern.ch/cmsdb/servlet/RunSummary?RUN=${run_num}>${run_num}</A></h3>
</center>


Jump to:<br>
<FONT color="Blue"> 
<A href="#EB">ECAL Barrel</A><BR>
<A href="#EEM">ECAL Endcap Minus Side</A><BR>
<A href="#EEP">ECAL Endcap Plus Side</A><BR>
<A href="#ALL">ALL ECAL : EB+EE</A><BR>
</FONT>
<br>
<A href=http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/CalibAnalysis/MWGR/${run_num}>Link to Calibration Analysis</A>

<h3><A name="EB"><FONT color="Blue">ECAL Barrel</FONT></A><BR></h3>

<A HREF=http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_timeCHProfile_${run_num}.png> <img height="200" src="http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_timeCHProfile_${run_num}.png"> </A>
<A HREF=http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_timeTTProfile_${run_num}.png> <img height="200" src="http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_timeTTProfile_${run_num}.png"> </A>
<br>
<A HREF=http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_timeCHAllFEDsEta_${run_num}.png> <img height="200" src="http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_timeCHAllFEDsEta_${run_num}.png"> </A>
<A HREF=http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_timeTTAllFEDsEta_${run_num}.png> <img height="200" src="http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_timeTTAllFEDsEta_${run_num}.png"> </A>
<br>
<A HREF=http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_fullAmpProfileEB_${run_num}.png> <img height="200" src="http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_fullAmpProfileEB_${run_num}.png"> </A>
<br>

<h3><A name="EEM"><FONT color="Blue">ECAL Endcap Minus Side</FONT></A><BR></h3>

<A HREF=http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_EEMtimeCHProfile_${run_num}.png> <img height="200" src="http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_EEMtimeCHProfile_${run_num}.png"> </A>

<A HREF=http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_timeTTAllFEDsEtaEEM_${run_num}.png> <img height="200" src="http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_timeTTAllFEDsEtaEEM_${run_num}.png"> </A>
 <br>
<A HREF=http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_fullAmpProfileEEM_${run_num}.png> <img height="200" src="http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_fullAmpProfileEEM_${run_num}.png"> </A>

<A HREF=http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_EEMtimeTTProfile_${run_num}.png> <img height="200" src="http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_EEMtimeTTProfile_${run_num}.png"> </A>

<h3><A name="EEP"><FONT color="Blue">ECAL Endcap Plus Side</FONT></A><BR></h3>

<A HREF=http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_EEPtimeCHProfile_${run_num}.png> <img height="200" src="http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_EEPtimeCHProfile_${run_num}.png"> </A>

<A HREF=http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_timeTTAllFEDsEtaEEP_${run_num}.png> <img height="200" src="http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_timeTTAllFEDsEtaEEP_${run_num}.png"> </A>
 <br>
<A HREF=http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_fullAmpProfileEEP_${run_num}.png> <img height="200" src="http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_fullAmpProfileEEP_${run_num}.png"> </A>

<A HREF=http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_EEPtimeTTProfile_${run_num}.png> <img height="200" src="http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_EEPtimeTTProfile_${run_num}.png"> </A>


<br>

<h3><A name="ALL"><FONT color="Blue">ALL ECAL EB+EE</FONT></A><BR></h3>

<h4> Laser </h4>

<A HREF=http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_XtalsPerEvt_${run_num}.png> <img height="200" src="http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_XtalsPerEvt_${run_num}.png"> </A>

<A HREF=http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_laserShift_${run_num}.png> <img height="200" src="http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_laserShift_${run_num}.png"> </A>

<br>

<h4> Timing from Laser </h4>

<A HREF=http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_LM_timing_${run_num}.png> <img height="200" src="http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_LM_timing_${run_num}.png"> </A>

<A HREF=http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_LM_timingCorrected_${run_num}.png> <img height="200" src="http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_LM_timingCorrected_${run_num}.png"> </A>

<br>

<A HREF=http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_SM_timing_${run_num}.png> <img height="200" src="http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_SM_timing_${run_num}.png"> </A>

<A HREF=http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_SM_timingCorrected_${run_num}.png> <img height="200" src="http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_SM_timingCorrected_${run_num}.png"> </A>

<br>


<A HREF=http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_TTMeanWithRMS_All_FEDS_${run_num}.png> <img height="200" src="http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_TTMeanWithRMS_All_FEDS_${run_num}.png"> </A>

<A HREF=http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_TTMeanWithRMS_All_FEDS_Corrected_${run_num}.png> <img height="200" src="http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_TTMeanWithRMS_All_FEDS_Corrected_${run_num}.png"> </A>

<br>

<A HREF=http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_RelRMS_vs_AbsTime_${run_num}.png> <img height="200" src="http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_RelRMS_vs_AbsTime_${run_num}.png"> </A>

<A HREF=http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_Rel_TimingSigma_${run_num}.png> <img height="200" src="http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_Rel_TimingSigma_${run_num}.png"> </A>

<A HREF=http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_Inside_TT_timing_${run_num}.png> <img height="200" src="http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/LaserAnalysis_Inside_TT_timing_${run_num}.png"> </A>



<h4> ROOT File (download) </h4>
<A HREF=http://test-ecal-cosmics.web.cern.ch/test-ecal-cosmics/LaserAnalysis/MWGR/${run_num}/${root_file}> ${root_file} </A>

</FONT>
</BODY>
</HTML>

EOF

exit
