#!/bin/tcsh
#####################
# Usage:  runExample_2d.tcsh ["doNotSubmit"]
# Uses environment variables:
#         passwd
#         example
#         xx
#         yy
#         a
#         b
#         c
#####################

cd $RELEASE_DIR/Quaero/tests/
setenv idsFile ids/ids_${exp}_$example$exampleSuffix.txt

# Submit Quaero requests

if ( "$1" != "doNotSubmit" ) then
  rm $idsFile >& /dev/null
  foreach y ( `echo $yy` )
    foreach x ( `echo $xx` )
      perl runTestJob.pl $passwd $example $x $y $a $b $c >> $idsFile
      sleep 5
    end
  end
endif

# Sleep until all Quaero results have been returned

./keepTabsOnQuaeroJobs.tcsh `cat $idsFile`
sleep 5
source setupCDF.tcsh

# Draw sensitivity plot

python drawSensitivity_2d.py $example `head -1 $idsFile` `wc $idsFile | awk '{print $1}'` \
	> logL/${exp}_${example}${exampleSuffix}_sensitivity.txt
root -b drawSensitivity_2d.C
mv model_tag.eps $RELEASE_DIR/Quaero/doc/quaero_${exp}/${exp}_${example}${exampleSuffix}_sensitivity_2d.eps

# Draw logL plot

python drawLogLcurve_2d.py $example `head -1 $idsFile` \
	`wc $idsFile | awk '{print $1}'` | grep -v 'failed' \
	> logL/${exp}_${example}${exampleSuffix}_logL.txt
root -b drawLogLcurve_2d.C
mv model_tag.eps $RELEASE_DIR/Quaero/doc/quaero_${exp}/${exp}_${example}${exampleSuffix}_logL_2d.eps

# Draw exclusion contour

root -b drawExclusionContour_2d.C
mv model_tag.eps $RELEASE_DIR/Quaero/doc/quaero_${exp}/${exp}_${example}${exampleSuffix}_exclusion_2d.eps

# Create clickable logL plot

if ( `printenv exclusionPlotTopToBottomToTop | wc -l` == 0 ) setenv exclusionPlotTopToBottomToTop "bottomToTop"

python createImageMap.py $exp ${example}${exampleSuffix} `echo $xx | wc -w` \
	`echo $yy | wc -w` `head -1 $idsFile` $exclusionPlotTopToBottomToTop
cp ${exp}_${example}${exampleSuffix}.html $RELEASE_DIR/Quaero/doc/devel/$exp/clickableExclusionPlots/
cp ${exp}_${example}${exampleSuffix}_logL_2d.jpg $RELEASE_DIR/Quaero/doc/devel/$exp/clickableExclusionPlots/
cp ${exp}_${example}${exampleSuffix}.html ~/Public/Quaero/quaero_development/doc/devel/$exp/clickableExclusionPlots/
cp ${exp}_${example}${exampleSuffix}_logL_2d.jpg ~/Public/Quaero/quaero_development/doc/devel/$exp/clickableExclusionPlots/

# Send email notification of completion

echo "$collider $exp ${example}${exampleSuffix} is done\n    http://mit.fnal.gov/~knuteson/Quaero/quaero_development/doc/devel/${exp}/clickableExclusionPlots/${exp}_${example}${exampleSuffix}.html" | /usr/lib/sendmail ${USER}@fnal.gov



