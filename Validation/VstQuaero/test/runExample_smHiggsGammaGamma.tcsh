#!/bin/tcsh
#####################
# Fermiophobic Higgs
#####################

setenv example smHiggsGammaGamma

if ( $extent == "one" ) then
  setenv mhs "75"
  setenv brs "0.1"
endif
if ( $extent == "test" ) then
  setenv mhs "60 90"
  setenv brs "0.05 0.3"
endif
if ( $extent == "short" ) then
  setenv mhs "50 75 100"
  setenv brs "0.02 0.1 0.5"
endif
if ( $extent == "medium" ) then
  setenv mhs "50 70 90 110"
  setenv brs "0.01 0.03 0.10 0.30 1.00"
endif
if ( $extent == "long" ) then
  setenv mhs "50 60 70 80 90 100 110"
  setenv brs "0.01 0.02 0.05 0.10 0.20 0.50 1.00"
endif

setenv xx "$mhs"
setenv yy "$brs"

source runExample_2d.tcsh $1


