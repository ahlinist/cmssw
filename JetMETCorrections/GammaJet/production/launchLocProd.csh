#!/bin/bash

#Producing fileList
#./makeListFile.sh /castor/cern.ch/user/m/meridian/Summer11Prod/VBF_HToGG_M-120_7TeV-pythia6-TuneProQ20-PU_S4_START42_V12/meridian/VBF_HToGG_M-120_7TeV-pythia6-TuneProQ20-PU_S4_START42_V12/VBF_HToGG_M-120_7TeV-pythia6-TuneProQ20-PU_S4_START42_V12/5ed30c00a24eeaae38a8382f2bdc3bd2 VBF_HToGG_M-120_TuneProQ20_7TeV-pythia6-paolo
#./makeListFile.sh /castor/cern.ch/user/m/meridian/Summer11Prod/VBF_HToGG_M-120_7TeV-pythia6-TuneD6T-PU_S4_START42_V12/meridian/VBF_HToGG_M-120_7TeV-pythia6-TuneD6T-PU_S4_START42_V12/VBF_HToGG_M-120_7TeV-pythia6-TuneD6T-PU_S4_START42_V12/3efa9c4cd6dd3a5bd43ef19c176fea94 VBF_HToGG_M-120_TuneD6T_7TeV-pythia6-paolo
#./makeListFile.sh /castor/cern.ch/user/m/meridian/Summer11Prod/VBF_HToGG_M-120_7TeV-pythia6-TuneP0-PU_S4_START42_V12/meridian/VBF_HToGG_M-120_7TeV-pythia6-TuneP0-PU_S4_START42_V12/VBF_HToGG_M-120_7TeV-pythia6-TuneP0-PU_S4_START42_V12/f6ea68763bcab9299c4706975cf3ad54 VBF_HToGG_M-120_TuneP0_7TeV-pythia6-paolo
#./makeListFile.sh /castor/cern.ch/user/m/meridian/Summer11Prod/VBF_HToGG_M-120_7TeV-pythia6-TuneProPT0-PU_S4_START42_V12/meridian/VBF_HToGG_M-120_7TeV-pythia6-TuneProPT0-PU_S4_START42_V12/VBF_HToGG_M-120_7TeV-pythia6-TuneProPT0-PU_S4_START42_V12/a347d897b1f18ffdebca978acabf2ef6 VBF_HToGG_M-120_TuneProPT0_7TeV-pythia6-paolo
#./makeListFile.sh /castor/cern.ch/user/m/meridian/Summer11Prod/VBF_HToGG_M-120_7TeV-pythia6-TuneZ2-PU_S4_START42_V12/meridian/VBF_HToGG_M-120_7TeV-pythia6-TuneZ2-PU_S4_START42_V12/VBF_HToGG_M-120_7TeV-pythia6-TuneZ2-PU_S4_START42_V12/c4ec5f6d976a7eb3dd89ed4ee8836747 VBF_HToGG_M-120_TuneZ2_7TeV-pythia6
#
#./makeListFileEos.sh /eos/cms/store/caf/user/meridian/VBF_HToGG_M-120_7TeV-powhegrenfactscaledown-pythia6-TuneZ2-PU_S4_START42_V12_LHE/VBF_HToGG_M-120_7TeV-powhegrenfactscaledown-pythia6-TuneZ2-PU_S4_START42_V12_LHE-AODSIM/9e35c528ca0023cbfbcd8aa40fd27c85 VBF_HToGG_M-120_7TeV-powhegrenfactscaledown-pythia6-TuneZ2-paolo
#./makeListFileEos.sh /eos/cms/store/caf/user/meridian/VBF_HToGG_M-120_7TeV-powhegrenfactscaleup-pythia6-TuneZ2-PU_S4_START42_V12_LHE/VBF_HToGG_M-120_7TeV-powhegrenfactscaleup-pythia6-TuneZ2-PU_S4_START42_V12_LHE-AODSIM/9e35c528ca0023cbfbcd8aa40fd27c85 VBF_HToGG_M-120_7TeV-powhegrenfactscaleup-pythia6-TuneZ2-paolo
./makeListFileEos.sh /eos/cms/store/caf/user/meridian/VBF_HToGG_M-120_7TeV-powheg-pythia6-TuneZ2-PU_S4_START42_V12_LHE/VBF_HToGG_M-120_7TeV-powheg-pythia6-TuneZ2-PU_S4_START42_V12_LHE-AODSIM/9e35c528ca0023cbfbcd8aa40fd27c85 VBF_HToGG_M-120_7TeV-powheg-pythia6-TuneZ2-paolo
#Launching fileList
#./runProduction_LOCAL.sh VBF_HToGG_M-120_TuneZ2_7TeV-pythia6-paolo.txt
#./runProduction_LOCAL.sh VBF_HToGG_M-120_TuneProPT0_7TeV-pythia6-paolo.txt
#./runProduction_LOCAL.sh VBF_HToGG_M-120_TuneP0_7TeV-pythia6-paolo.txt
#./runProduction_LOCAL.sh VBF_HToGG_M-120_TuneProQ20_7TeV-pythia6-paolo.txt
#./runProduction_LOCAL.sh VBF_HToGG_M-120_TuneD6T_7TeV-pythia6-paolo.txt
#./runProduction_LOCAL.sh VBF_HToGG_M-120_7TeV-powhegrenfactscaledown-pythia6-TuneZ2-paolo.txt
#./runProduction_LOCAL.sh VBF_HToGG_M-120_7TeV-powhegrenfactscaleup-pythia6-TuneZ2-paolo.txt
./runProduction_LOCAL.sh VBF_HToGG_M-120_7TeV-powheg-pythia6-TuneZ2-paolo.txt