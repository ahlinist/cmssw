#!/bin/bash

python aTGCRooStats.py --workspaceName=WW_35pb_limits --couplingType=WWV --treeName=ww --obsVar=pt1 --obsMin=30 --obsMax=120 --nObsBins=9  --lumiErr=0.1 --par1Name=dkz --par1Max=1.5 --par2Name=lg --par2Max=1.5 --inputData=Data_ww.root --inputMC=WW_SignalExpectation.root --backgroundFile=bkg_ww_mc.root --par1GridMax=.75 --par2GridMax=.5 --nGridParBins=3