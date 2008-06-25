#!/bin/tcsh -f
#

set i=1
set j=54

while ( $i <= $j )
   mv isoCal50GeV_1pb_$i.cfg isoCal50GeV_1pb_3by3_$i.cfg	
   @ i++
end 
