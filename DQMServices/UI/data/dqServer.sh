#/bin/bash

while [ 1 ]; do
#python dqServer.py --url=http://cmssrv17.fnal.gov:8989/DBS116Oracle/servlet/DBSServlet
python dqServer.py --url=https://cmsdbsprod.cern.ch:8443/cms_dbs_prod_global_writer/servlet/DBSServlet
  sleep 10
done
