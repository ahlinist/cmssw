#!/bin/csh -f

#--------------------------------------------------------------------------------
# kill all crab jobs for analysis channel given as function argument
# and delete all directories created by crab
#
# NOTE: this script needs to be passed one command-line parameter
#
#      (1) name of channel
#
#       e.g. './crabKill.csh AHtoMuTau'
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

set crabDirectories=(`ls | grep crabdir | grep $1`)
foreach crabDirectory (${crabDirectories})
    echo "crab -kill all -c ${crabDirectory}"
    crab -kill all -c ${crabDirectory}
end
foreach crabDirectory (${crabDirectories})
    echo "rm -r ${crabDirectory}"
    rm -r ${crabDirectory}
end
echo "rm *$1*"
rm *{$1}*
