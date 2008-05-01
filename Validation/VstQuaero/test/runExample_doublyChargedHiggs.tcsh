#!/bin/tcsh
#####################
# Doubly Charged Higgs
#####################

setenv example doublyChargedHiggs

if ( $extent == "one" ) then
  setenv masses "95"
endif
if ( $extent == "test" ) then
  setenv masses "92 102"
endif
if ( $extent == "short" ) then
  setenv masses "90 95 100 105"
endif
if ( $extent == "medium" ) then
  setenv masses "90 92 94 96 98 100"
endif
if ( $extent == "long" ) then
  setenv masses "90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105"
endif

setenv xx "$masses"

source runExample_1d.tcsh "98.5" "m_{H^{#pm#pm}} (GeV)" $1




