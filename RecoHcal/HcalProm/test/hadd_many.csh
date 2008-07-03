#!/bin/tcsh -f
#
set i=1
set j=$1
 echo -n "hadd $2 "
 while ( $i <= $j )
    echo -n "rfio:/castor/cern.ch/user/e/efe/mc_muons/prompt_out_$i.root "
   @ i++
end
echo
