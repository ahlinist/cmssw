#!/bin/tcsh
#####################
# Charged Higgs
#####################

setenv example chargedHiggs

if ( $extent == "short" ) then
  setenv masses "70 75 80 85 90"
endif
if ( $extent == "medium" ) then
  setenv masses "70 72.5 75 77.5 80 82.5 85 87.5 90"
endif
if ( $extent == "long" ) then
 setenv masses "70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90"
endif

setenv xx "$masses"

source runExample_1d.tcsh  "87.0"  "m_{H^{#pm}} (GeV)"



