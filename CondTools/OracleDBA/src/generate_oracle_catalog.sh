#! /bin/bash
#-------------------------------------------------------------------------
#Prerequesits: 
#   -CMS and LCG software are installed on the machine
#   -to be run inside a CMSSW project directory  
#   -there is authentication.xml in POOL_AUTH_PATH
#-------------------------------------------------------------------------
#environment variables
export SCRAM_ARCH=slc3_ia32_gcc323
eval `scramv1 runtime -sh`
THISDIR=`pwd`
POOL_AUTH_PATH=${THISDIR}
export POOL_AUTH_PATH
#global variables
CATALOGRECREATION_FLAG=0
XMLCATALOGFILE=cmsconddb_catalog_template.xml
XMLCATALOG="file:${XMLCATALOGFILE}"
#user defined variables
GENERAL_SCHEMA_NAME=CMS_COND_GENERAL
#GENERAL_SCHEMA_NAME=CMS_XIEZHEN_DEV

#-------------------------------------------------------------------------
# check ()
# check existence of authentication file
# Returns; 0 on success
# Exit 1 on failure
#-------------------------------------------------------------------------
check () {
  echo "${THISDIR}/authentication.xml"
  if [ ! -e "${THISDIR}/authentication.xml" ] 
  then
     echo "authentication.xml not found, cannot proceed."
     exit 1
  fi
  return 0
}
#-------------------------------------------------------------------------
# create_xmlcatalogtemplate ()
# register in the template xml catalog PFN for the specified detector
# Parameters: DETECTORNAME($1)
# Returns; 0 on success
#-------------------------------------------------------------------------
create_xmlcatalogtemplate () {
  local detector=$1
  echo "FCregisterPFN -u $XMLCATALOG -p oracle://host/CMS_COND_${detector} -t POOL_RDBMS"
  FCregisterPFN -u ${XMLCATALOG} -p oracle://host/CMS_COND_${detector} -t POOL_RDBMS
  return 0
}
#-------------------------------------------------------------------------
# setup_catalog ()
# setup the catalog entry for the given detector and oracle server 
# Parameters: ORACLESERVICENAME($1), DETECTORNAME($2)
# Returns; 0 on success
#-------------------------------------------------------------------------
setup_catalog () {
  local servicename=$1
  local detector=$2
  local mycat=relationalcatalog_oracle://${servicename}/${GENERAL_SCHEMA_NAME}
  echo "FCrenamePFN -u ${mycat} -p oracle://host/CMS_COND_ ${detector} -n oracle://${servicename}/CMS_COND_${detector}"
  FCrenamePFN -u ${mycat} -p oracle://host/CMS_COND_${detector} -n oracle://${servicename}/CMS_COND_${detector}
  return 0
}
    
#main
NUM_ARGS=1
if [ $# -ne ${NUM_ARGS} ]
then
  echo "Wrong number of arguments"
  echo "Usage: `basename $0` oracleservicename"
  exit 2
fi
ORACLESERVICENAME=$1
check
DETECTORS="
CSC 
DT
ECAL
HCAL
STRIP
PIXEL
PRESH
RPC
HLT
ALIGNMENT
" 
if [ "$CATALOGRECREATION_FLAG" -eq 1 ]
then
  echo "WARNING recreating ${XMLCATALOGFILE}"
  /bin/rm -f "${XMLCATALOGFILE}"
  for det in ${DETECTORS}
  do
    create_xmlcatalogtemplate ${det}
  done
fi
DESTCATALOG=relationalcatalog_oracle://${ORACLESERVICENAME}/${GENERAL_SCHEMA_NAME}
echo "FCpublish -u ${XMLCATALOG} -d $DESTCATALOG"
FCpublish -u ${XMLCATALOG} -d $DESTCATALOG
for det in ${DETECTORS}  
do
    setup_catalog ${ORACLESERVICENAME} ${det} 
done
exit 0
