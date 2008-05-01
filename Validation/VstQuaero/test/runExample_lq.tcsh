#!/bin/tcsh
#####################
# Leptoquarks
#
setenv example lq

if ( $extent == "one" ) then
  setenv masses "250"
  setenv lambdas "0.1"
endif
if ( $extent == "test" ) then
  setenv masses "225 275"
  setenv lambdas "0.03 0.3"
endif
if ( $extent == "short" ) then
  setenv masses "200 250 300"
  setenv lambdas "0.01 0.1 1.0"
endif
if ( $extent == "medium" ) then
  setenv masses "200 225 250 275 300"
  setenv lambdas "0.001 0.01 0.1 1.0"
endif
if ( $extent == "long" ) then
  setenv masses "200 220 240 260 280 300"
  setenv lambdas "0.003 0.01 0.03 0.10 0.30 1.00 3.00"
endif
if ( $extent == "verylong" ) then
  setenv masses "200 210 220 230 240 250 260 270 280 290 300"
  setenv lambdas "0.001 0.002 0.005 0.01 0.02 0.05 0.10 0.20 0.50 1.00"
endif

setenv xx "$masses"
setenv yy "$lambdas"




