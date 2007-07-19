#!/bin/bash

eval `scramv1 ru -sh`

HOSTNAME=$(echo `/bin/hostname` | sed 's/\//\\\//g')
echo "The hostname is = $HOSTNAME"
# HOSTNAME=127.0.0.1

TEST_PATH=$(echo "${PWD}" | sed 's/\//\\\//g')
echo "The current directory is = $PWD"

MWC_LIB1="${LOCALRT}/lib/${SCRAM_ARCH}/libDQMCSCMonitorClient.so"
echo "Looking for the DQMCSCMonitorClient library... $MWC_LIB1"
if [ ! -f $MWC_LIB1 ]; then
    echo "Not Found! Will pick it up from the release area..."
    MWC_LIB1="/afs/cern.ch/cms/Releases/CMSSW/prerelease/${CMSSW_VERSION}/lib/${SCRAM_ARCH}/libDQMCSCMonitorClient.so"
else 
    echo "Found!"
fi

MWC_LIB=$(echo "$MWC_LIB1" | sed 's/\//\\\//g')
echo $MWC_LIB1

if [ -e profile.xml ]; then
    rm profile.xml
fi 
if [ -e monClient.xml ]; then
    rm CSCWebClient.xml
fi
if [ -e startMonitorClient ]; then
    rm startMonitorClient
fi

sed -e "s/.portn/1972/g" -e "s/.host/${HOSTNAME}/g" -e "s/.pwd/${TEST_PATH}/g" -e "s/.libpath/${MWC_LIB}/g" .profile.xml > profile.xml
sed -e "s/.portn/1972/g" -e "s/.host/${HOSTNAME}/g" -e "s/.pwd/${TEST_PATH}/g" -e "s/.libpath/${MWC_LIB}/g" .WebTest.xml > CSCWebClient.xml 
sed -e "s/.portn/1972/g" -e "s/.host/${HOSTNAME}/g" -e "s/.pwd/${TEST_PATH}/g" -e "s/.libpath/${MWC_LIB}/g" .startMonitorClient > startMonitorClient

chmod 751 profile.xml
chmod 751 CSCWebClient.xml
chmod 751 startMonitorClient



