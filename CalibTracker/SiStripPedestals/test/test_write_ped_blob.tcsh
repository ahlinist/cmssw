#!/bin/tcsh
rfcp /castor/user/g/gennai/RU0512863_000.root /tmp/
eval `scramv1 runtime -csh`
SealPluginRefresh
cmsRun --parameter-set test_write_ped_blob.cfg



