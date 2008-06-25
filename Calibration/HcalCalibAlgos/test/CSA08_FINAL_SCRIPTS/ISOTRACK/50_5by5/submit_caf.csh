#!/bin/tcsh -f
#
set i=40
set j=54

while ( $i <= $j ) 
	bsub -q cmscaf -R cmscaf CAF_prompt.csh $i
    @ i++
end	
