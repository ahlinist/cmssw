#!/bin/csh -f

grep -r "Events total = " $1/res/CMSSW_*stdout | awk '{x+=$5;y+=$8} END {print x,y}'
