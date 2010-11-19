#!/bin/csh -f

if( $#argv != 1) then
	echo " usage: getNumEventsFromCrabOutput.csh <crab path>"
	exit(1)
endif

grep -r "Events total = " $1/res/CMSSW_*stdout | awk '{x+=$5;y+=$8} END {print x,y}'
