#!/bin/bash

FILES=`dbs search --query="find file where dataset like $1" --url=http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_01/servlet/DBSServlet --noheader`  
SITE=`dbs search --query="find site where dataset like $1" --url=http://cmsdbsprod.cern.ch/cms_dbs_ph_analysis_01/servlet/DBSServlet --noheader`

for a in $FILES 
do
  lcg-cp --verbose -U srmv2 "srm://"$SITE":8443/srm/managerv2?SFN="$3$a  "file:$2/${a##*/}"
done

