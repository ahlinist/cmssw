#!/bin/tcsh
#####################
# Single Top
#####################

setenv example singleTop

if ( $extent == "one" ) then
  setenv kappaZs     "0.50"
  setenv kappaGammas "0.50"
endif
if ( $extent == "test" ) then
  setenv kappaZs     "0.25 0.75"
  setenv kappaGammas "0.25 0.75"
endif
if ( $extent == "short" ) then
  setenv kappaZs     "0.0 0.4 0.8"
  setenv kappaGammas "0.0 0.4 0.8"
endif
if ( $extent == "medium" ) then
  setenv kappaZs     "0.0 0.2 0.4 0.6"
  setenv kappaGammas "0.0 0.2 0.4 0.6"
endif
if ( $extent == "long" ) then
  setenv kappaZs     "0.0 0.15 0.3 0.45 0.6"
  setenv kappaGammas "0.0 0.15 0.3 0.45 0.6"
endif

setenv xx "$kappaGammas"
setenv yy "$kappaZs"

source runExample_2d.tcsh $1

