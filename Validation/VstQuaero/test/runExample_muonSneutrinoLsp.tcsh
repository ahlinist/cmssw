#!/bin/tcsh
#####################
# Muon Sneutrino LSP
#####################

setenv example muonSneutrinoLsp

if ( $extent == "one" ) then
  setenv mChi "125"
endif
if ( $extent == "test" ) then
  setenv mChi "75 125 175"
endif
if ( $extent == "short" ) then
  setenv mChi "75 100 125 150 175"
endif
if ( $extent == "medium" ) then
  setenv mChi "75 100 125 150 175"
#  setenv mChi "60 80 100 120 140 160 180 190"
endif
if ( $extent == "long" ) then
  setenv mChi "75 100 125 150 175"
#  setenv mChi "60 70 80 90 100 110 120 130 140 150 160 170 180 190"
endif

setenv xx "$mChi"

source runExample_1d.tcsh  0  "m_{#chi^{#pm}} (GeV)"

