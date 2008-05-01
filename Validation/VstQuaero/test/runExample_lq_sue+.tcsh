#!/bin/tcsh
#####################
# Leptoquarks
#

source runExample_lq.tcsh
setenv exampleSuffix "_sue+"

setenv a "u"
# charge conjugate
setenv b "e-"
setenv c "s"

source runExample_2d.tcsh $1



