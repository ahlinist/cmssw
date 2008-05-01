#!/bin/tcsh
#####################
# Single Top
#####################

setenv example singleTop
setenv exampleSuffix "_1d"

if ( $extent == "one" ) then
  setenv kappaGammas "0.25"
endif
if ( $extent == "test" ) then
  setenv kappaGammas "0.0 0.5"
endif
if ( $extent == "short" ) then
  setenv kappaGammas "0.0 0.25 0.50"
endif
if ( $extent == "medium" ) then
  setenv kappaGammas "0.0 0.1 0.2 0.3 0.4 0.5"
endif
if ( $extent == "long" ) then
  setenv kappaGammas "0.0 0.1 0.2 0.3 0.4 0.5"
#  setenv kappaGammas "0.0 0.05 0.1 0.15 0.2 0.25 0.3 0.35 0.4 0.45 0.5"
endif

setenv xx "$kappaGammas"
setenv y "0.0"

if ( "$1" != "doNotSubmit" ) then 
  setenv signalFileStorageString "-signalFileStorage(store)"
  setenv largestKappaGamma `echo $kappaGammas | awk '{print $NF}'`
  setenv masterId `perl runTestJob.pl $passwd$signalFileStorageString $example $largestKappaGamma $y $a $b $c`
  ./keepTabsOnQuaeroJobs.tcsh $masterId
  sleep 5

  setenv idsFile ids/ids_${exp}_$example$exampleSuffix.txt
  rm $idsFile >& /dev/null
  foreach kappaGamma ( `echo $kappaGammas` )
    setenv signalFileStorageFactor `echo "print ($kappaGamma/$largestKappaGamma)**2" | python`
    setenv signalFileStorageId $masterId
    setenv signalFileStorageString "-signalFileStorage($signalFileStorageId,$signalFileStorageFactor)"
    perl runTestJob.pl $passwd$signalFileStorageString $example $kappaGamma $y $a $b $c >> $idsFile
  end
endif

source runExample_1d.tcsh "0.25" "#kappa_{tu#gamma}" "doNotSubmit"

