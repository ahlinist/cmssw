#!/bin/sh

CORAL_AUTH_PATH=/afs/cern.ch/cms/DB/conddb

DBfile="sqlite_file:/tmp/$USER/testSiStripNoisesBuilder_blob.db"
echo "CondTools/OracleDBA/scripts/cmscond_bootstrap_detector.pl --offline_connect $DBfile --auth ${CORAL_AUTH_PATH}/authentication.xml STRIP "
cmscond_bootstrap_detector.pl --offline_connect $DBfile --auth ${CORAL_AUTH_PATH}/authentication.xml STRIP 

DBfile="sqlite_file:/tmp/$USER/testSiStripPedestalsBuilder_blob.db"
echo "cmscond_bootstrap_detector.pl --offline_connect $DBfile --auth ${CORAL_AUTH_PATH}/authentication.xml STRIP "
cmscond_bootstrap_detector.pl --offline_connect $DBfile --auth ${CORAL_AUTH_PATH}/authentication.xml STRIP 

DBfile="sqlite_file:/tmp/$USER/testSiStripPedestalsNoisesBuilder_blob.db"
echo "cmscond_bootstrap_detector.pl --offline_connect $DBfile --auth ${CORAL_AUTH_PATH}/authentication.xml STRIP "
cmscond_bootstrap_detector.pl --offline_connect $DBfile --auth ${CORAL_AUTH_PATH}/authentication.xml STRIP 


