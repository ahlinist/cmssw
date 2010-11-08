#!/bin/bash

python aTGCRooStats.py --workspaceName=test --couplingType=ZZg --treeName=photonTree --phoEtVar=photonEt --phoEtMin=10 --phoEtMax=70 --nEtBins=6 --intLumi=15.0 --lumiErr=0.1 --h3Max=.4 --h4Max=.01 --inputData=/scratch/lgray/zgamma_workdir/Data_mm.root --inputMC=ZgZ_MC_merged.root --MCbackground --backgroundFile=ZJets_mm_15pb.root 