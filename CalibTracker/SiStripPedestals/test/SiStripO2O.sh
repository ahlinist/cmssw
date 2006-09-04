#!/bin/sh

function usage () {
    echo -e "\n[usage]\n SiStripO2O.sh [options]"
    echo -e " -IOV=<runNb>"
    echo -e " -ConfigDb=<user/passwd@path>"
    echo -e " -ConfigDbVersion=<Major.Minor>"
    echo -e " -ConfigDbPartition=<partitionName>"
    echo -e " -doPedNoiseTransfer"
    echo -e " -doFedCablingTransfer"
    echo -e " -oracle (otherwise use sqlite file) "
    echo -e " -first_upload (otherwise works in append mode) "

    echo -e "\nDefault values"
    echo -e " -IOV=$IOV"
    echo -e " -ConfigDb=${ConfigDb}"
    echo -e " -ConfigDbVersion=${ConfigDbVersion}"
    echo -e " -ConfigDbPartition=${ConfigDbPartition}"
#    echo -e " -doPedNoiseTransfer"
#    echo -e " -doFedCablingTransfer"
    
    exit
}


function settings (){

    export IOV=1
    export doPedNoiseTansfer=0
    export doFedCablingTransfer=0
    export ConfigDb=cms_mtcc_sitracker/cms_mtcc@omds
    export ConfigDbVersion=8.189
    export ConfigDbPartition="MTCC_DEMO"
    export append=1
    export oracle_db=0

    [ `echo $@ | grep -c "\-help"` = 1 ] && usage
    [ `echo $@ | grep -c "\-doPedNoiseTransfer"` = 1 ] && doPedNoiseTansfer=1 
    [ `echo $@ | grep -c "\-doFedCablingTransfer"` = 1 ] && doFedCablingTransfer=1 
    [ `echo $@ | grep -c "\-ConfigDb="`  = 1 ] && ConfigDb=`echo $@ | awk -F"\-ConfigDb=" '{print $2}' | awk '{print $1}'`
    [ `echo $@ | grep -c "\-ConfigDbVersion="`  = 1 ] && ConfigDbVersion=`echo $@ | awk -F"-ConfigDbVersion=" '{print $2}' | awk '{print $1}'`
    [ `echo $@ | grep -c "\-ConfigDbPartition="`  = 1 ] && ConfigDbPartition=`echo $@ | awk -F"-ConfigDbPartition=" '{print $2}' | awk '{print $1}'`
    [ `echo $@ | grep -c "\-IOV="`  = 1 ] && IOV=`echo $@ | awk -F"-IOV=" '{print $2}' | awk '{print $1}'`
    [ `echo $@ | grep -c "\-oracle"` = 1 ] && oracle_db=1 
    [ `echo $@ | grep -c "\-first_upload"` = 1 ] && append=0 
    
    ConfigDbUser=`echo ${ConfigDb}| awk -F'/' '{print $1}'`
    ConfigDbPasswd=`echo ${ConfigDb}| awk -F'/' '{print $2}' | awk -F'@' '{print $1}'`
    ConfigDbPath=`echo ${ConfigDb}| awk -F'@' '{print $2}'`
    ConfigDbMajorVersion=`echo ${ConfigDbVersion}| awk -F'.' '{print $1}'`
    ConfigDbMinorVersion=`echo ${ConfigDbVersion}| awk -F'.' '{print $2}'`
}

#################
## MAIN
#################

export TNS_ADMIN=/afs/cern.ch/project/oracle/admin
export CORAL_AUTH_PATH=/afs/cern.ch/cms/DB/conddb

scramv1 setup -i oracle <<EOF



/afs/cern.ch/project/oracle/admin

EOF


settings "$@"

test_area=/tmp/$USER/o2o

[ ! -e ${test_area} ] && mkdir ${test_area}

if [ "${oracle_db}" == 1 ]; 
    then
    DBfile="oracle://devdb10/CMS_COND_STRIP"
    DBcatalog="relationalcatalog_oracle://devdb10/CMS_COND_GENERAL"
else
    DBfile="sqlite_file:${test_area}/dummy_${IOV}.db"
    DBcatalog="file:${test_area}/dummy_${IOV}.xml"
fi


o2otrans="dummy"
if [ "$doPedNoiseTansfer" == "1" ] && [ "$doFedCablingTransfer" == "1" ];
    then 
    o2otrans="#o2o_all"
else
    [ "$doPedNoiseTansfer" == "1" ]    && o2otrans="#o2o_pednoi"
    [ "$doFedCablingTransfer" == "1" ] && o2otrans="#o2o_cabl"
fi


cfg_file=${test_area}/SiStripO2O_IOV_${IOV}.cfg


eval `scramv1 runtime -sh`

cat template_SiStripO2O.cfg | sed -e "s#insert_DBfile#$DBfile#" -e "s#insert_DBcatalog#$DBcatalog#"  -e "s#insert_IOV#${IOV}#" -e "s#insert_appendflag#${append}#g" -e "s@#appendMode_${append}@@g" \
-e "s#insert_ConfigDbUser#${ConfigDbUser}#" -e "s#insert_ConfigDbPasswd#${ConfigDbPasswd}#" -e "s#insert_ConfigDbPath#${ConfigDbPath}#"	-e "s#insert_ConfigDbPartition#${ConfigDbPartition}#" -e "s#insert_ConfigDbMajorVersion#${ConfigDbMajorVersion}#" -e "s#insert_ConfigDbMinorVersion#${ConfigDbMinorVersion}#" \
-e "s#insert_doPedNoiseTransfer#$doPedNoiseTansfer#" -e "s#insert_doFedCablingTransfer#$doFedCablingTransfer#" -e "s@${o2otrans}@@"> ${cfg_file}



if [ "${oracle_db}" == 0 ] && [ "${append}" == 0 ];
    then 
    rm -f ${test_area}/dummy_${IOV}.db
    rm -f ${test_area}/dummy_${IOV}.xml

    mapping=blobmappingfile_pednoise.xml
    echo pool_build_object_relational_mapping -f ${mapping} -d CondFormatsSiStripObjects -c ${DBfile} -u me -p mypass  
    pool_build_object_relational_mapping -f ${mapping} -d CondFormatsSiStripObjects -c ${DBfile} -u me -p mypass  
fi

echo "cmsRun ${cfg_file}"
cmsRun ${cfg_file} > ${test_area}/out_o2o_${IOV}


