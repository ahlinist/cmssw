#!/bin/csh -f

if($#argv < 5 || $#argv > 6) then
	echo " usage: checkForMissingFiles.csh <castor_path> <sample> <total number> <string before number> <string after number> [<date string>]"
	exit(1)
endif

set pth = $1
set search = $2
set total = $3
set pre = $4
set post = $5

rm -f numTmp numTmp2

if( $#argv == 5) then
	rfdir ${pth} | grep ${search} | grep -v "Sum" | awk '{print $9}' | awk -F${pre} '{print $2}' | sed 's/\(.*\)${post}/\1/' | awk '{printf "%02d\n",$1}'| sort -n > numTmp
else
	rfdir ${pth} | grep ${search} | grep -v "Sum" | egrep "$6" | awk '{print $9}' | awk -F${pre} '{print $2}' | sed 's/\(.*\)${post}/\1/' | awk '{printf "%02d\n",$1}' | sort -n > numTmp
endif

seq --format=%02g ${total} > numTmp2 

echo "Missing numbers:"
diff numTmp numTmp2 | grep ">"
echo "Duplicate numbers:"
diff numTmp numTmp2 | grep "<"


rm -f numTmp numTmp2
