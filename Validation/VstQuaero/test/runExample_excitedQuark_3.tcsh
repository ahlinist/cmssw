#!/bin/tcsh
#####################
# Excited Quark _3
#####################

setenv exampleSuffix "_3"
source runExample_excitedQuark.tcsh

if ( "$1" != "doNotSubmit" ) then 
  setenv f 1
  setenv fPrime 1
  setenv fs 0
  setenv signalFileStorageString "-signalFileStorage(store)"
  setenv lowestLambda `echo $lambdas | awk '{print $(NF/2)}'`

if ( "$1" != "doNotSubmitInitial" ) then
rm $idsFile.storage >& /dev/null
setenv i 0
foreach m ( `echo $ms` )
  setenv i `expr $i + 1`
  setenv mQstar_u $m
  setenv mQstar_d `echo "print $m/2" | python`
  echo $f "$fPrime" $mQstar_u $mQstar_d $lowestLambda
  perl runTestJob.pl $passwd$signalFileStorageString $example $f "$fPrime" $mQstar_u $mQstar_d $lowestLambda $fs >> $idsFile.storage
end
./keepTabsOnQuaeroJobs.tcsh `cat $idsFile.storage`
sleep 5
endif

rm $idsFile >& /dev/null
foreach lambda ( `echo $lambdas` )
  setenv i 0
  foreach m ( `echo $ms` )
    setenv i `expr $i + 1`
    setenv signalFileStorageFactor `echo "print (1.*$lowestLambda/$lambda)**2" | python`
    setenv signalFileStorageId `cat $idsFile.storage | head -$i | tail -1`
    setenv signalFileStorageString "-signalFileStorage($signalFileStorageId,$signalFileStorageFactor)"

    setenv mQstar_u $m
    setenv mQstar_d `echo "print $m/2" | python`
    echo $f "$fPrime" $mQstar_u $mQstar_d $lambda
    perl runTestJob.pl $passwd$signalFileStorageString $example $f "$fPrime" $mQstar_u $mQstar_d $lambda $fs >> $idsFile
  end
end

endif

source runExample_2d.tcsh "doNotSubmit"
