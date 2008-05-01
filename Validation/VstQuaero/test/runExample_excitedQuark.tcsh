#!/bin/tcsh
#####################
# Excited Quark
#####################

setenv example excitedQuark

if ( $extent == "one" ) then
  setenv lambdas "100"
  setenv ms "200"
endif
if ( $extent == "test" ) then
  setenv lambdas "10 1000"
  setenv ms "100 300"
endif
if ( $extent == "short" ) then
  setenv lambdas "10 100 1000"
  setenv ms "100 200 300"
endif
if ( $extent == "medium" ) then
  setenv lambdas "10 30 100 300 1000"
  setenv ms "100 150 200 250 300"
endif
if ( $extent == "long" ) then
  setenv lambdas "10 100 250 500 1000"
  setenv ms "100 133 167 200 233 267 300"
endif

setenv idsFile ids/ids_${exp}_${example}${exampleSuffix}.txt

setenv xx "$ms"
setenv yy ""
foreach lambda ( $lambdas )
  setenv yy `echo "print 1./$lambda" | python`" $yy"
end

setenv exclusionPlotTopToBottomToTop "topToBottom"
