#!/bin/tcsh
rfcp /castor/cern.ch/user/g/gennai/RU0512863_000.root /tmp/
eval `scramv1 runtime -csh`
source create_pool_build_object_relational_mapping.sh
SealPluginRefresh
cmsRun --parameter-set test_write_ped_blob.cfg



