#!/bin/sh

runNb=$1
DBfile=$2
DBcatalog=$3

output_file_name=/tmp/$USER/Display_PedNoise_RunNb_${runNb}.root 
cfg_file=/tmp/$USER/SiStripCondObjDisplay_${runNb}.cfg

echo ${cfg_file}

export CORAL_AUTH_USER=me
export CORAL_AUTH_PASSWORD=me


eval `scramv1 runtime -sh`

cat template_SiStripCondObjDisplay.cfg | sed -e "s#insert_DBfile#$DBfile#" -e "s#insert_DBcatalog#$DBcatalog#" -e "s#insert_output_filename#${output_file_name}#" -e "s#insert_runNb#${runNb}#" > ${cfg_file}

echo "cmsRun ${cfg_file}"
cmsRun ${cfg_file}
