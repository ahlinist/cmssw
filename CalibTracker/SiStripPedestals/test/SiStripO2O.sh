#!/bin/sh

function usage () {
    echo -e "\n[usage]\n SiStripO2O.sh [options]"
    echo -e " -IOV=<runNb>"
    echo -e " -ConfigDb=<user/passwd@path>"
    echo -e " -ConfigDbVersion=<Major.Minor>"
    echo -e " -ConfigDbPartition=<partitionName>"
    echo -e " -doPedNoiseTransfer"
    echo -e " -doFedCablingTransfer"

    echo -e "\nDefault values"
    echo -e " -IOV=$IOV"
    echo -e " -ConfigDb=${ConfigDb}"
    echo -e " -ConfigDbVersion=${ConfigDbVersion}"
    echo -e " -ConfigDbPartition=${ConfigDbPartition}"
    echo -e " -doPedNoiseTransfer"
#    echo -e " -doFedCablingTransfer"
    
    exit
}

#################
## MAIN
#################

IOV=1
doPedNoiseTansfer=0
doFedCablingTransfer=0
ConfigDb=cms_mtcc_sitracker/cms_mtcc@omds
ConfigDbVersion=8.14
ConfigDbPartition="MTCC_DEMO"

[ `echo $@ | grep -c "\-help"` = 1 ] && usage
[ `echo $@ | grep -c "\-doPedNoiseTransfer[ ]*"` = 1 ] && doPedNoiseTansfer=1 
[ `echo $@ | grep -c "\-doFedCablingTransfer[ ]*"` = 1 ] && doFedCablingTransfer=1 
[ `echo $@ | grep -c "\-ConfigDb="`  = 1 ] && ConfigDb=`echo $@ | awk -F"\-ConfigDb=" '{print $2}' | awk '{print $1}'`
[ `echo $@ | grep -c "\-ConfigDbVersion="`  = 1 ] && ConfigDbVersion=`echo $@ | awk -F"\-ConfigDbVersion=" '{print $2}' | awk '{print $1}'`
[ `echo $@ | grep -c "\-ConfigDbPartition="`  = 1 ] && ConfigDbPartition=`echo $@ | awk -F"\-ConfigDbPartition=" '{print $2}' | awk '{print $1}'`
[ `echo $@ | grep -c "\-IOV="`  = 1 ] && IOV=`echo $@ | awk -F"\-IOV=" '{print $2}' | awk '{print $1}'`

ConfigDbUser=`echo ${ConfigDb}| awk -F'/' '{print $1}'`
ConfigDbPasswd=`echo ${ConfigDb}| awk -F'/' '{print $2}' | awk -F'@' '{print $1}'`
ConfigDbPath=`echo ${ConfigDb}| awk -F'@' '{print $2}'`
ConfigDbMajorVersion=`echo ${ConfigDbVersion}| awk -F'.' '{print $1}'`
ConfigDbMinorVersion=`echo ${ConfigDbVersion}| awk -F'.' '{print $2}'`

test_area=/tmp/$USER/o2o

[ ! -e ${test_area} ] && mkdir ${test_area}

DBfile=${test_area}/dummy.db
DBcatalog=${test_area}/dummy.xml

o2otrans="dummy"
if [ "$doPedNoiseTansfer" == "1" ] && [ "$doFedCablingTransfer" == "1" ];
    then 
    o2otrans="#o2o_all"
else
    [ "$doPedNoiseTansfer" == "1" ]    && o2otrans="#o2o_pn"
    [ "$doFedCablingTransfer" == "1" ] && o2otrans="#o2o_c"
fi


cfg_file=${test_area}/SiStripO2O_IOV_${IOV}.cfg

export CORAL_AUTH_USER=me
export CORAL_AUTH_PASSWORD=me

eval `scramv1 runtime -sh`

cat template_SiStripO2O.cfg | sed -e "s#insert_DBfile#$DBfile#" -e "s#insert_DBcatalog#$DBcatalog#"  -e "s#insert_IOV#${IOV}#" \
-e "s#insert_ConfigDbUser#${ConfigDbUser}#" -e "s#insert_ConfigDbPasswd#${ConfigDbPasswd}#" -e "s#insert_ConfigDbPath#${ConfigDbPath}#"	-e "s#insert_ConfigDbPartition#${ConfigDbPartition}#" -e "s#insert_ConfigDbMajorVersion#${ConfigDbMajorVersion}#" -e "s#insert_ConfigDbMinorVersion#${ConfigDbMinorVersion}#" \
-e "s#insert_doPedNoiseTransfer#$doPedNoiseTansfer#" -e "s#insert_doFedCablingTransfer#$doFedCablingTransfer#" -e "s@${o2otrans}@@"> ${cfg_file}

rm -f $DBfile
rm -f $DBcatalog

if [ ! -e ${DBfile} ];
    then

    mapping=blobmappingfile_pednoise.xml
    echo pool_build_object_relational_mapping -f ${mapping} -d CondFormatsSiStripObjects -c sqlite_file:${DBfile} -u me -p mypass  
    pool_build_object_relational_mapping -f ${mapping} -d CondFormatsSiStripObjects -c sqlite_file:${DBfile} -u me -p mypass  
fi

echo "cmsRun ${cfg_file}"
cmsRun ${cfg_file} > ${test_area}/out_o2o_${IOV}


