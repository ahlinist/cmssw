#!/bin/sh

if [ "$#" != "3" ]; then
    echo "usage: SiStripCondObjDisplay.sh runNb DBfile DBcatalog"
    exit
fi


runNb=$1
DBfile=$2
DBcatalog=$3

test_area=/tmp/$USER/Display
[ ! -e ${test_area} ] && mkdir ${test_area}

output_file_name=${test_area}/Display_PedNoise_RunNb_${runNb}.root 
ps_file_name=${test_area}/Display_PedNoise_RunNb_${runNb}.ps 
cfg_file=${test_area}/SiStripCondObjDisplay_RunNb_${runNb}.cfg

echo ${cfg_file}

export CORAL_AUTH_USER=me
export CORAL_AUTH_PASSWORD=me


eval `scramv1 runtime -sh`

cat template_SiStripCondObjDisplay.cfg | sed -e "s#insert_DBfile#$DBfile#" -e "s#insert_DBcatalog#$DBcatalog#" -e "s#insert_output_filename#${output_file_name}#" -e "s#insert_ps_filename#${ps_file_name}#" -e "s#insert_runNb#${runNb}#" > ${cfg_file}

echo "cmsRun ${cfg_file}"
cmsRun ${cfg_file} > ${test_area}/out_diplay_${runNb}

echo "Empty Pedestal Histos" `grep "Empty Ped"  ${test_area}/out_diplay_${runNb} | wc -l`
echo "Empty Noise Histos" `grep "Empty Nois"  ${test_area}/out_diplay_${runNb} | wc -l`
echo "Pedestal Histos with all channel at zero" `grep "All channel at zero in Ped"  ${test_area}/out_diplay_${runNb} | wc -l`
echo "Noise Histos with all channel at zero" `grep "All channel at zero in Nois"  ${test_area}/out_diplay_${runNb} | wc -l`
grep "SiStripCondObjDisplay::geometry_Vs_Cabling"  ${test_area}/out_diplay_${runNb}