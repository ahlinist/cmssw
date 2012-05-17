#!/bin/bash

ln -s /afs/cern.ch/user/v/vlimant/public/reco/logger_cfi.py $CMSSW_BASE/src/logger_cfi.py

export PYTHONPATH=$PYTHONPATH:./:$CMSSW_BASE/src/.

scram build USER_CXXFLAGS="-DEDM_ML_DEBUG" -j 4 -r 

cmsDriver.py mc -s RAW2DIGI,RECO,VALIDATION --eventcontent RECO --conditions auto:startup -n 100 --no_exec --mc --customise RecoTracker/Loopers/Adapter.adapt,RecoTracker/Loopers/debug.debug --dbs "find file,file.parent where dataset= /RelValTTbar/CMSSW_5_3_0_*/GEN-SIM-RECO" --process LOOPER

cmsDriver.py data --conditions auto:com10 --scenario pp --process LOOPER --data -s RAW2DIGI,RECO --customise RecoTracker/Loopers/Adapter.adapt,RecoTracker/Loopers/debug.debug --magField AutoFromDBCurrent --no_exec --fileout file:step2.root -n 100 --filein /store/data/Run2011B/L1JetHPF/RAW/v1/000/179/828/34ED3E87-40FF-E011-BB81-BCAEC518FF52.root
