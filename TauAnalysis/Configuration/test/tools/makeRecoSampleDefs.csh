#!/bin/csh -f
if( $#argv < 3 || $#argv > 5) then
	echo " usage: makeRecoSampleDefs.csh <castor path> <sample name> <starting number> [<reco files per pat-tuple>]"
	echo "   default: one reco file per pat-tuple"
	exit (1)
endif

set step = 1
if( $#argv == 4 ) then
	@ step = $4
endif

rfdir $1 | awk -v path="$1" -v sample="$2" -v x="$3" -v stp="${step}" 'BEGIN {y=x} { if(x%stp == 0) {printf ")\nfileNamesZtoElecTau_"sample"_part%02d = cms.untracked.vstring(\n\t'\''rfio:"path"/"$9"'\'',\n",y; x+=1;y+=1} else { printf "\t'\''rfio:"path"/"$9"'\''\n";x+=1}}'

