#!/bin/csh -f

#--------------------------------------------------------------------------------
# delete all files stored in castor directory given as function argument
#
# NOTE: this script needs to be passed one command-line parameter
#
#      (1) name of castor directory
#
#       e.g. './clearCastorDirectory.csh /castor/cern.ch/user/v/veelken/bgEstSkim/ZtoMuTau'
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

set fileNames=(`nsls $1`)
foreach fileName (${fileNames})
    echo "deleting fileName $1/${fileName}"
    nsrm -f $1/${fileName}
end
