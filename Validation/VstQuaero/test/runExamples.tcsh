#!/bin/tcsh
####################################################################################################
#  Usage:    cd $RELEASE_DIR/Quaero/tests/
#            ./runExamples.tcsh  ["doNotSubmit"]
#  Purpose:  Run a Quaero parameter plot
####################################################################################################

#foreach myExample ( smHiggs smHiggs1 smHiggsGammaGamma mSugra doublyChargedHiggs doublyChargedHiggs_ee chargedHiggs singleTop singleTop_1d excitedElectron lq lq_0 lq_1 muonSneutrinoLsp excitedQuark excitedQuark_4x top_gR top_gRgL top_massXsec rsGraviton )
# excitedElectrons awaits a better MadEvent integration technique for handling particles of narrow width

setenv collider all
setenv exp all
setenv extent long
setenv passwd tpc-bgo-evb
#setenv passwd tpc-bgo-dis-ular-evb
setenv exampleSuffix ""
setenv y 0
setenv a 0
setenv b 0
setenv c 0
#foreach myExample ( singleTop mSugra doublyChargedHiggs smHiggsGammaGamma )
foreach myExample ( smHiggs1 )
  echo $exp $myExample
  nohup ./runExample_$myExample.tcsh $1 &
  sleep 1m
end

