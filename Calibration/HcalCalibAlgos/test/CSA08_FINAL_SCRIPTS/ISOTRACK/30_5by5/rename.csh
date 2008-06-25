#!/bin/tcsh -f
#

set i=1
set j=54

while ( $i <= $j )
   mv isoCal1pb_$i.cfg isoCal1pb_5by5_$i.cfg	
   @ i++
end 
