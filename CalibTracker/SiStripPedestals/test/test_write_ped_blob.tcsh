#!/bin/tcsh

echo "... doing rfcp /castor/cern.ch/user/g/gennai/RU0512863_000.root /tmp/$USER/"
#rfcp /castor/cern.ch/user/g/gennai/RU0512863_000.root /tmp/$USER/
echo "... done"

eval `scramv1 runtime -csh`

setenv CORAL_AUTH_USER me
setenv CORAL_AUTH_PASSWORD me
setenv CORAL_AUTH_PATH /afs/cern.ch/cms/DB/conddb

setenv DBfile "/tmp/$USER/ped_geom_ideal_blob.db"
setenv DBcatalog "/tmp/$USER/ped_geom_ideal_blob.xml"

rm -f $DBfile $DBcatalog

echo "CondTools/OracleDBA/scripts/cmscond_bootstrap_detector.pl --offline_connect sqlite_file:$DBfile --auth ${CORAL_AUTH_PATH}/authentication.xml STRIP "
cmscond_bootstrap_detector.pl --offline_connect sqlite_file:$DBfile --auth ${CORAL_AUTH_PATH}/authentication.xml STRIP 
echo " " 
echo pool_insertFileToCatalog -u file:${DBcatalog} -t POOL_RDBMS sqlite_file:${DBfile}
pool_insertFileToCatalog -u file:${DBcatalog} -t POOL_RDBMS sqlite_file:${DBfile}

#SealPluginRefresh


cat test_write_ped_blob.cfg | sed  -e "s#insert_sqlite#${DBfile}#" -e "s#insert_catalog#${DBcatalog}#" -e "s#insert_inputfile#/tmp/$USER/RU0512863_000.root#" >! mytest_write_ped_blob.cfg

echo
echo "cmsRun mytest_write_ped_blob.cfg"
cmsRun mytest_write_ped_blob.cfg

