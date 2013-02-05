#!/bin/bash

ln -s /afs/cern.ch/user/v/vlimant/public/reco/logger_cfi.py $CMSSW_BASE/src/logger_cfi.py

export PYTHONPATH=$PYTHONPATH:./:$CMSSW_BASE/src/.

scram build USER_CXXFLAGS="-DEDM_ML_DEBUG" -j 4 -r 

cmsDriver.py mc -s RAW2DIGI,RECO,VALIDATION --eventcontent RECO --conditions auto:startup -n 100 --no_exec --mc --customise RecoTracker/Loopers/Adapter.adapt,RecoTracker/Loopers/debug.debug --dbs "find file,file.parent where dataset= /RelValTTbar/CMSSW_5_3_0_*/GEN-SIM-RECO" --process LOOPER

cmsDriver.py data --conditions auto:com10 --scenario pp --process LOOPER --data -s RAW2DIGI,RECO --customise RecoTracker/Loopers/Adapter.adapt,RecoTracker/Loopers/debug.debug --magField AutoFromDBCurrent --no_exec --fileout file:step2.root -n 100 --filein /store/data/Run2011B/L1JetHPF/RAW/v1/000/179/828/34ED3E87-40FF-E011-BB81-BCAEC518FF52.root


##with 5.3/6.1 depending on the imput file

cmsDriver.py mc -s RECO:siStripMatchedRecHits+siStripMatchedRecHits+siPixelRecHits+iterTracking+offlinePrimaryVertices --eventcontent RECO --conditions auto:startup -n 100 --no_exec --mc --customise RecoTracker/Loopers/Adapter.adapt+looperTracks --process LOOPER --dbs "find file,file.parent where dataset= /RelValTTbar/CMSSW_6_1_0-START61_V8-v1/GEN-SIM-RECO"


cmsDriver.py data --conditions auto:com10 --scenario pp --process LOOPER --data -s RECO:siStripMatchedRecHits+siStripMatchedRecHits+siPixelRecHits+iterTracking+offlinePrimaryVertices --customise RecoTracker/Loopers/Adapter.adapt+looperTracks --no_exec  -n 100 --filein /store/relval/CMSSW_6_1_0-GR_R_61_V6_RelVal_jet2011B/Jet/RECO/v1/00000/DCA8E572-A64C-E211-829F-002618FDA259.root
