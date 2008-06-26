#!/bin/tcsh -f
#
#specify how many cfg files you have
set i=1 
set j=54

while ( $i <= $j )
        bsub -q cmscaf -R cmscaf caf.csh $i
    @ i++
end
