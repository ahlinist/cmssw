#!/bin/bash

echo $1
if [ $# -eq 0 ]; then
  echo "Usage:    checkoutTags.sh skim|analyzer"
  echo
  exit
fi

set -e

if [ -n "${SRT_CMSSW_VERSION_SCRAMRTDEL-x}" ]; then
#  echo "cmsenv not set"
#  exit
  echo "cmsenv not set, setting cmsenv"
  eval `scramv1 runtime -sh`
fi

# Tag list modification history

#CMSSW_5_2_5 Skim 28.5.2012/S.Lehti:
if [ "$1" == "skim" ]; then
  cvs co -r V01-04-17 RecoTauTag/RecoTau #equivalent to 04-14
  cvs co -r V01-04-03 RecoTauTag/Configuration
  cvs co -r V00-04-01 CondFormats/EgammaObjects
  addpkg PhysicsTools/PatAlgos
  cvs up -r 1.53      PhysicsTools/PatAlgos/python/tools/tauTools.py
  cvs up -r 1.12      PhysicsTools/PatAlgos/python/producersLayer1/tauProducer_cff.py
  cvs up -r 1.15      PhysicsTools/PatAlgos/python/recoLayer0/tauDiscriminators_cff.py
  cvs co -r V00-10-04 ElectroWeakAnalysis/TauTriggerEfficiency
fi


#CMSSW_5_2_5_patch1 Analyzer 28.5.2012/S.Lehti:
if [ "$1" == "analyzer" ]; then
  cvs co -r V00-10-04      ElectroWeakAnalysis/TauTriggerEfficiency
  cvs co -r V13-05-07-01   FastSimulation/Configuration                     
  cvs co -r V13-06-12      HLTrigger/Configuration                                 
  checkdeps -a
fi
