#!/bin/tcsh
#####################
# Excited electrons
#####################

setenv example excitedElectron

if ( $extent == "short" ) then
  setenv Lambdas "500 2000 4000"
  setenv mEstars "100 150 200"
endif
if ( $extent == "medium" ) then
  setenv Lambdas "500 1000 2000 3000 4000"
  setenv mEstars "100 133 166 200"
endif
if ( $extent == "long" ) then
  setenv Lambdas "500 1000 1500 2000 2500 3000 3500 4000"
  setenv mEstars "100 125 150 175 200"
endif

setenv xx "$mEstars"
setenv yy "$Lambdas"

source runExample_2d.tcsh

