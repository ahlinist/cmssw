#!/bin/tcsh
#####################
# Scalar leptoquark
#####################

setenv example lq
setenv exampleSuffix "_0"

if ( $extent == "one" ) then
  setenv mlq "297"
endif
if ( $extent == "test" ) then
  setenv mlq "292 297 302"
endif
if ( $extent == "short" ) then
  setenv mlq "292 294.5 297 299.5 302"
endif
if ( $extent == "medium" ) then
  setenv mlq "292 294 296 298 300 302"
endif
if ( $extent == "long" ) then
  setenv mlq "`printRange 294 302`"
endif

setenv a "u"
# charge conjugate
setenv b "e-"
setenv c "s"

setenv xx "$mlq"
setenv y "0.3"

source runExample_1d.tcsh "300.0" "m_{LQ} (GeV)" $1




