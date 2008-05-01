#!/bin/tcsh
#####################################################################################################
# Usage:    cd $RELEASE_DIR/;
#           checkDistributionsWithCuts <finalState>
#           ./Quaero_utils/utilities/createHbookFiles.tcsh <finalState> "<distribution>"
# Purpose:  Create hbook files for pythia tuning
# Output:   tuneHistos_<finalState>_<distribution>.hb
# Notes:    This script works on the last.kumac file produced by checkDistributionsWithCuts
#####################################################################################################

setenv finalState $1
setenv finalStatePawStyle `echo $finalState | sed -e 's/_.*//'`
setenv distribution $2
setenv distributionPawStyle `echo $distribution | sed -e 's/_/#-#/g' | sed -e 's/+/^+\\!/g'`

mv last.kumac tmp0.kumac

tail +`grep -n -i "* begin finalState $finalStatePawStyle" tmp0.kumac | awk '{print $1}' | sed -e 's/:\*//'` tmp0.kumac > tmp1.kumac
head -`grep -n -i "* end finalState $finalStatePawStyle" tmp1.kumac | awk '{print $1}' | sed -e 's/:\*//'` tmp1.kumac > tmp2.kumac

tail +`grep -n -i "* begin distribution $distributionPawStyle" tmp2.kumac | awk '{print $1}' | sed -e 's/:\*//'` tmp2.kumac > tmp3.kumac
head -`grep -n -i "* end distribution $distributionPawStyle" tmp3.kumac | awk '{print $1}' | sed -e 's/:\*//'` tmp3.kumac > tmp4.kumac

echo "\n\n\n\n filecase keep \n h/file 1 tuneHistos_${finalState}_${distribution}.hb ! N \n\n" > tmp5.kumac
cat tmp5.kumac tmp4.kumac > tmp6.kumac
echo "hrout 0\n" >> tmp6.kumac

rm ~/higz_windows.dat
paw < tmp6.kumac

mv tmp0.kumac last.kumac
rm tmp?.kumac

echo "tuneHistos_${finalState}_${distribution}.hb created" 

