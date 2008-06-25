#!/bin/tcsh -f
#
set i=100
set j=132

while ( $i <= $j ) 
	bsub -q cmscaf -R cmscaf CAF_prompt.csh $i
    @ i++
end	
