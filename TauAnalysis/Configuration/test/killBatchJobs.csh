#!/bin/csh -f

#--------------------------------------------------------------------------------
# kill all batch jobs of a certain type and channel
#
# NOTE: this script needs to be passed two command-line parameters
#
#      (1) job name
#      (2) channel name
#
#       e.g. './killBatchJobs.csh bgEstSkim ZtoMuTau'
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

set batchJobs=(`bjobs -w | grep $1 | grep $2`)
foreach batchJob (${batchJobs})
    echo ${batchJob}
    if (${batchJob} =~ [0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]) then
	echo "killing batchJob ${batchJob}"
	bkill ${batchJob}
    endif
end
