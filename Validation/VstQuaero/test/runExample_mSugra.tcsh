#!/bin/tcsh
#####################
# Minimal supergravity
#####################

setenv example mSugra

if ( $extent == "one" ) then
  setenv xx "250"
  setenv yy "125"
endif
if ( $extent == "test" ) then
  setenv xx "250 750"
  setenv yy "125 175"
endif
if ( $extent == "short" ) then
  setenv xx "0 500 1000"
  setenv yy "100 150 200"
endif
if ( $extent == "medium" ) then
  setenv xx "0 250 500 750 1000"
  setenv yy "100 125 150 200"
endif
if ( $extent == "long" ) then
  setenv xx "0 250 500 750 1000"
  setenv yy "100 120 130 140 150 175 200"
endif

source runExample_2d.tcsh $1

