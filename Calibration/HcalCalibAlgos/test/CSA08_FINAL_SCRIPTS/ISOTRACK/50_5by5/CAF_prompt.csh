#! /bin/csh -f
setenv STAGE_SVCCLASS cmscaf
#export STAGE_SVCCLASS=cmscaf
#export STAGER_TRACE=3
cd /afs/cern.ch/user/e/efe/CMSSW_2_0_7/src
#cmsenv
eval `scramv1 ru -csh`
cd -
cmsRun /afs/cern.ch/user/e/efe/CMSSW_2_0_7/src/Calibration/HcalCalibAlgos/test/50_5by5/isoCal50GeV_1pb_$1.cfg
#rfcp input_to_l3.txt /castor/cern.ch/user/e/efe/VALIDATION_50_5by5_$1.txt
rfcp iso.root /castor/cern.ch/user/e/efe/VALIDATION_50_5by5_$1.root
