#!/bin/tcsh
######################################################################
#  Usage:    source ./Quaero/utilities/evening_install.tcsh |& tee log_evening_install.txt
#  Purpose:  Fully update the Quaero web
######################################################################


#setenv experiments "hera/h1 tev1/d0 tev2/cdf lep2/l3 lep2/aleph"
setenv experiments "tev2/cdf"

if ( ( "$1" == "full" ) && ( "$2" == "cdf" ) ) then
  foreach exp ( "tev2/cdf" )
    cd $RELEASE_DIR/Quaero/experiments/${exp}/check
    ./go
    cd -
  end
endif

setenv edition quaero_development
 
cd $RELEASE_DIR/;
gmake commit
#setenv allQuaeroCcPlatforms 1
source Quaero/utilities/install.tcsh $edition cc_clean
source Quaero/utilities/install.tcsh $edition web code

sleep 1m
source Quaero/utilities/install.tcsh $edition exp sys
foreach exp ( `echo $experiments` )
  source Quaero/utilities/install.tcsh $edition exp $exp
end
if ( "$1" == "full" ) then
  foreach exp ( `echo $experiments` )
    source Quaero/utilities/install.tcsh $edition exp $exp full
  end
  sleep 5m
  foreach exp ( `echo $experiments` )
    source Quaero/utilities/install.tcsh $edition exp $exp
  end
endif
sleep 10

unsetenv allQuaeroCcPlatforms
source Quaero/utilities/install.tcsh $edition web

