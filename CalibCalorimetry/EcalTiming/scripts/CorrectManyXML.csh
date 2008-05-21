#!/bin/tcsh -f

set fed = 601
set runnum = $1
set proc = $2
echo "starting with fed" $fed
while ( $fed < 655 )
    echo "working on fed" $fed
    ProduceRelTimeOffsetFileCMSSWNew sm_${fed}.xml ../data/SM_${fed}_TTPeakPositionFileLaser_Run_${runnum}.${proc}.txt sm_${fed}_${runnum}
    @ fed  = $fed + 1
end


#end of file
