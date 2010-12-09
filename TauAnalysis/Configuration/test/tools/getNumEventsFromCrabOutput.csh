#!/bin/csh -f

if( $#argv != 1) then
	echo " usage: getNumEventsFromCrabOutput.csh <crab path>"
	exit(1)
endif

grep -r "Events total = " $1/res/CMSSW_*stdout | awk '{x+=$5;y+=$8} END {printf "processed: %d passed: %d eff: %f\n",x,y,y/x}'
