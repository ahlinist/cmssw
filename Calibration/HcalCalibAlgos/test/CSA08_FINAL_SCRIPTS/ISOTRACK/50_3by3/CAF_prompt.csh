#! /bin/csh -f
setenv STAGE_SVCCLASS cmscaf
#export STAGE_SVCCLASS=cmscaf
#export STAGER_TRACE=3
cd /afs/cern.ch/user/e/efe/CMSSW_2_0_7/src
#cmsenv
eval `scramv1 ru -csh`
cd -
cmsRun /afs/cern.ch/user/e/efe/CMSSW_2_0_7/src/Calibration/HcalCalibAlgos/test/50_3by3/isoCal50GeV_1pb_3by3_$1.cfg
#rfcp input_to_l3.txt /castor/cern.ch/user/e/efe/TEST_Without_Phi_Sym__50_3by3_$1.txt
rfcp iso.root /castor/cern.ch/user/e/efe/VALIDATION_50_3by3_$1.root
