#!/bin/csh -f
if( $#argv < 3 || $#argv > 5) then
	echo " usage: makeRecoSampleDefs.csh <crab dir> <sample> <starting number> [<reco files per pat-tuple>]"
	echo "   default: one reco file per pat-tuple"
	exit (1)
endif

set step = 1
if( $#argv == 4 ) then
	@ step = $4
endif

grep 'FrameworkJobReport Status="Success"' $1/res/crab_fjr_*.xml | awk -F: '{print $1}' | xargs grep skimElecTau | grep srm | awk -F= -v sample="$2" -v x="$3" -v stp="${step}" 'BEGIN {y=x} { if((x-1)%stp == 0) {printf "fileNamesZtoElecTau_"sample"_part%02d = cms.untracked.vstring(\n",y;y+=1}; printf "\t'\''rfio:"$2"'\''"; if(x%stp == 0) {printf "\n)\n"} else {printf ",\n"}; x+=1 }'
echo ")"
