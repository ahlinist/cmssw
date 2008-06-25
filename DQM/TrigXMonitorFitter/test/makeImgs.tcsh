#!/bin/tcsh


./cleanOldFiles.pl


cd ./L1
./../multi.pl ./../ps2img.sh --dpi=40 %.eps %.gif
cd ./..

cd ./L2
./../multi.pl ./../ps2img.sh --dpi=40 %.eps %.gif
cd ./..

cd ./L3
./../multi.pl ./../ps2img.sh --dpi=40 %.eps %.gif
cd ./..

cd ./Ln
./../multi.pl ./../ps2img.sh --dpi=40 %.eps %.gif
cd ./..

echo "finished making gifs"


# cd ./L1
# ./../multi.pl ./../ps2img.sh --dpi=40 %.eps %.png >& l1.out
# cd ./..

# cd ./L2
# ./../multi.pl ./../ps2img.sh --dpi=40 %.eps %.png >& l2.out
# cd ./..

# cd ./L3
# ./../multi.pl ./../ps2img.sh --dpi=40 %.eps %.png >& l3.out
# cd ./..

# cd ./Ln
# ./../multi.pl ./../ps2img.sh --dpi=40 %.eps %.png >& ln.out
# cd ./..

# echo "finished making pngs"
