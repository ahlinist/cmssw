#!/bin/csh -f

if($#argv < 3 || $#argv > 4) then
	echo " usage: checkForMissingFiles.csh <castor_path> <sample> <total number> [<date string>]"
	exit(1)
endif

set pth = $1
set search = $2
set total = $3

rm -f numTmp numTmp2

if( $#argv == 3) then
	rfdir ${pth} | grep ${search} | awk '{print $9}' | awk -Fpart '{print $2}' | sed 's/\(.*\).root/\1/' | sort -n > numTmp
else
	rfdir ${pth} | grep ${search} | egrep "$4" | awk '{print $9}' | awk -Fpart '{print $2}' | sed 's/\(.*\).root/\1/' | sort -n > numTmp
endif

seq --format=%02g ${total} > numTmp2 
diff numTmp numTmp2 | grep ">"

rm -f numTmp numTmp2
