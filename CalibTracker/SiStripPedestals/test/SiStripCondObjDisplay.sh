#!/bin/sh

if [ `echo $@ | grep -c "\-help"` = 1 ];
    then
    echo -e "\n[usage] SiStripCondObjDisplay.sh [options]"
    echo -e " -run=<runNb>"
    echo -e " -oracle  (otherwise specify sqlite dbfile and dbcatalog) "
    echo -e " -dbfile=<dbfile>"
    echo -e " -dbcatalog=<dbcatalog>"

    exit
fi

export TNS_ADMIN=/afs/cern.ch/project/oracle/admin
#export CORAL_AUTH_PATH=CalibTracker/SiStripPedestals/test
export CORAL_AUTH_PATH=/afs/cern.ch/cms/DB/conddb

scramv1 setup -i oracle <<EOF



/afs/cern.ch/project/oracle/admin

EOF

oracle_db=0
runNb=1
dbfile_=""
dbcatalog_=""
[ `echo $@ | grep -c "\-run="` = 1 ] && runNb=`echo $@ | awk -F"\-run=" '{print $2}' | awk '{print $1}'`
[ `echo $@ | grep -c "\-oracle"` = 1 ] && oracle_db=1 
[ `echo $@ | grep -c "\-dbfile="` = 1 ] && dbfile_=`echo $@ | awk -F"\-dbfile=" '{print $2}' | awk '{print $1}'`
[ `echo $@ | grep -c "\-dbcatalog="` = 1 ] && dbcatalog_=`echo $@ | awk -F"\-dbcatalog=" '{print $2}' | awk '{print $1}'`


if [ "${oracle_db}" == 1 ]; 
    then
    DBfile="oracle://devdb10/CMS_COND_STRIP"
    DBcatalog="relationalcatalog_oracle://devdb10/CMS_COND_GENERAL"
elif [ ${dbfile_} != "" ] && [ ${dbcatalog_} != "" ];
    then
    DBfile="sqlite_file:${dbfile_}"
    DBcatalog="file:${dbcatalog_}"
else
    echo -e "\nPlease, specify an option: -oracle or -dbfile=<dbfile> and -dbcatalog=<dbcatalog> "
    exit
fi


test_area=/tmp/$USER/Display
[ ! -e ${test_area} ] && mkdir ${test_area}

output_file_name=${test_area}/Display_PedNoise_RunNb_${runNb}.root 
ps_file_name=${test_area}/Display_PedNoise_RunNb_${runNb}.ps 
cfg_file=${test_area}/SiStripCondObjDisplay_RunNb_${runNb}.cfg

echo ${cfg_file}

eval `scramv1 runtime -sh`

cat template_SiStripCondObjDisplay.cfg | sed -e "s#insert_DBfile#$DBfile#" -e "s#insert_DBcatalog#$DBcatalog#" -e "s#insert_output_filename#${output_file_name}#" -e "s#insert_ps_filename#${ps_file_name}#" -e "s#insert_runNb#${runNb}#" > ${cfg_file}

echo "cmsRun ${cfg_file}"
cmsRun ${cfg_file} > ${test_area}/out_diplay_${runNb}

grep "SiStripCondObjDisplay::geometry_Vs_Cabling"  ${test_area}/out_diplay_${runNb}
echo
echo
echo "Empty Pedestal Histos" `grep "Empty Ped"  ${test_area}/out_diplay_${runNb} | wc -l`
echo "Empty Noise Histos" `grep "Empty Nois"  ${test_area}/out_diplay_${runNb} | wc -l`
echo "Empty BadStrip Histos" `grep "Empty Bad"  ${test_area}/out_diplay_${runNb} | wc -l`
echo "Pedestal Histos with all channel at zero" `grep "All channel at zero in Ped"  ${test_area}/out_diplay_${runNb} | wc -l`
echo "Noise Histos with all channel at zero" `grep "All channel at zero in Nois"  ${test_area}/out_diplay_${runNb} | wc -l`
echo "BadStrips Histos with all channel at zero" `grep "All channel at zero in Bad"  ${test_area}/out_diplay_${runNb} | wc -l`

echo -e "\nroot file and postscript file with histos can be found in  ${test_area}"
echo -e "to see .ps file do\ngv  ${ps_file_name}&"