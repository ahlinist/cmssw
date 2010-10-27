#!/bin/csh
if( $#argv != 2 ) then
	echo " usage: makeRecoSampleDefs.csh <castor_path> <starting number>"
	exit (1)
endif

rfdir $1 | awk -v path="$1" -v x="$2" '{printf "fileNamesZtoElecTau_Data2010B_part"x" = cms.untracked.vstring(\n\t'\''rfio:"path"/"$9"'\''\n)\n"; x+=1}'
