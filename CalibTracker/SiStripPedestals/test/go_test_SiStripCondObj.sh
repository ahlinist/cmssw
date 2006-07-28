#!/bin/sh

. ./compare_write_read.sh

eval `scramv1 runtime -sh`

export workdir=/tmp/${USER}/$$
echo -e "workdir $workdir\n"

[ -e ${workdir} ] && rm -rf ${workdir} 
mkdir ${workdir}

mapping=blobmappingfile_pednoise.xml
dbfile=${workdir}/dummy.db

echo	pool_build_object_relational_mapping -f ${mapping} -d CondFormatsSiStripObjects -c sqlite_file:${dbfile} -u me -p mypass  
pool_build_object_relational_mapping -f ${mapping} -d CondFormatsSiStripObjects -c sqlite_file:${dbfile} -u me -p mypass  

cat write_SiStripCondObj.cfg | sed -e "s#insert_dbfile#${workdir}/dummy.db#" -e "s#insert_dbcatalog#${workdir}/dummy.xml#" > ${workdir}/write_SiStripCondObj.cfg
cat read_SiStripCondObj.cfg | sed -e "s#insert_dbfile#${workdir}/dummy.db#" -e "s#insert_dbcatalog#${workdir}/dummy.xml#" > ${workdir}/read_SiStripCondObj.cfg


echo -e "\ncmsRun ${workdir}/write_SiStripCondObj.cfg > ${workdir}/out_write"
cmsRun ${workdir}/write_SiStripCondObj.cfg > ${workdir}/out_write

echo -e "\ncmsRun ${workdir}/read_SiStripCondObj.cfg > ${workdir}/out_read"
cmsRun ${workdir}/read_SiStripCondObj.cfg > ${workdir}/out_read

thisdir=`pwd`
cd ${workdir}
echo "------------"
compare_conn
echo "------------"
compare_ped
echo "------------"
compare_noise
echo "------------"
cd $thisdir
