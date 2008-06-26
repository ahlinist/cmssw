#! /bin/csh -f
export STAGE_SVCCLASS=cmscaf
export STAGER_TRACE=3
# write the path to your CMSSW area below
cd ..../CMSSW_2_0_9/src
# set the cms environment
cmsenv
# below takes you to the scatch area of the batch system
cd -         
# write the correct path to your cfg file below 
cmsRun ..../CMSSW_2_0_4/src/HcalProm/test/file0$1.cfg
# copy the output file ....root to wherever you want, if you would like to copy to castor, use rfcp instead of cp 
cp ....root /tmp/....
