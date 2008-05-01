#!/bin/tcsh
#####################
# Standard Model Higgs
#####################

setenv example smHiggs

if ( $extent == "one" ) then
  setenv mhs "80"
endif
if ( $extent == "test" ) then
  setenv mhs "70 90 110"
endif
if ( $extent == "short" ) then
  setenv mhs "60 70 80 90 100 110"
endif
if ( $extent == "medium" ) then
  setenv mhs "75 80 85 90 95 100 105 110 115"
endif
if ( $extent == "long" ) then
  setenv mhs "75 80 85 90 95 100 105 110 115"
endif

setenv xx "$mhs"

source runExample_1d.tcsh "114.0" "m_{h} (GeV)"




