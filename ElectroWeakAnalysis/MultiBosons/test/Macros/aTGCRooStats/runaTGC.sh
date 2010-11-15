#!/bin/bash

python aTGCRooStats.py --workspaceName=ZZg_ee_15pb_limits --couplingType=ZZg --treeName=photonTree --phoEtVar=photonEt --phoEtMin=10 --phoEtMax=90 --nEtBins=8 --intLumi=11.0 --lumiErr=0.1 --h3Max=.4 --h4Max=.01 --inputData=Data_mm_35pb.root --inputMC=ZZg_mm_MC_merged_35pb.root --MCbackground --backgroundFile=ZJets_mm_35pb.root