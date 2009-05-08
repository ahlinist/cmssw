#/bin/bash

while [ 1 ]; do
  python dqServerFake.py
  #python dqServer.py --url=https://cmsdbsprod.cern.ch:8443/cms_dbs_prod_global_writer/servlet/DBSServlet
  sleep 10
done
