#!/bin/tcsh
#####################
# Right-handed top
#####################

setenv example rightHandedTop

if ( $extent == "test" ) then
  setenv rightHandedCoupling "0.3"
endif
if ( $extent == "short" ) then
  setenv rightHandedCoupling "0.1 0.3 0.5"
endif
if ( $extent == "medium" ) then
  setenv rightHandedCoupling "0.1 0.2 0.3 0.4 0.5"
endif
if ( $extent == "long" ) then
  setenv rightHandedCoupling "0.001 0.01 0.1 1.0 10.0"
endif

setenv xx "$rightHandedCoupling"

source runExample_1d.tcsh  "0.5"  "g_{R}"



