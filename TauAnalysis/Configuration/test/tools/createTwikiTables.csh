#!/bin/csh -f

#
# parse DQM filter statistics tables and twikify
#

#if( $#argv != 2):
#	echo " createTwikiTables.csh <file_name> <sample_name>"
#	echo "   where <file_name> contains the output of the DQMDumpFilterStatisticsTables module"
#	echo "   and <sample_name> is specified in the DQMDumpFilterStatisticsTables configuration."
#	exit(1)
#endif

set file = $1
set sample = $2

echo "---+++" "\!${sample}"
echo "|*Cut*|*Passed*|*cumul. Efficiency*|*margin. Efficiency*|*indiv. Efficiency*|*excl. Rejection*|"
grep -A35 "process = ${sample}" ${file} | tail -28 | sed 's/ \{8,50\}\([0-9]\)/ |  \1/g' | sed 's/^/|/' | sed 's/$/|/' | sed 's/||/ or /'
