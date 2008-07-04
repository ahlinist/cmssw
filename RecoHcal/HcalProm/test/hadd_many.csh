#!/bin/tcsh -f
#
set i=$1
set j=$2
 echo -n "hadd $3 "
 while ( $i <= $j )
    echo -n "rfio:/castor/cern.ch/user/e/efe/prompt/prompt_out_$i.root "
   @ i++
end
echo
