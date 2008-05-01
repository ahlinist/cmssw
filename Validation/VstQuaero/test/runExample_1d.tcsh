#!/bin/tcsh
################################
# runExample_1d.tcsh
################################
# $1 = previous limit (e.g., "114.0")
# $2 = label for x axis (e.g., "m_{h} (GeV)")

cd $RELEASE_DIR/Quaero/tests/
setenv idsFile ids/ids_${exp}_$example$exampleSuffix.txt

# Submit Quaero requests

if ( "$3" != "doNotSubmit" ) then
  rm $idsFile >& /dev/null
  foreach x ( `echo $xx` )
    perl runTestJob.pl $passwd $example $x $y $a $b $c >> $idsFile
  end
endif

# Wait until all Quaero results are returned

sleep 5
./keepTabsOnQuaeroJobs.tcsh `cat $idsFile`

# Draw logL plot

source setupCDF.tcsh
perl drawLogLcurve_1d.pl `head -1 $idsFile | awk '{print $1}'` \
	`wc $idsFile | awk '{print $1}'` $example "$1" "$2"
mv model_tag.eps $RELEASE_DIR/Quaero/doc/quaero_${exp}/${exp}_${example}_logL_1d.eps

# Create clickable logL plot

python createImageMap_1d.py $exp $example `echo $xx | wc | awk '{print $2}'` `head -1 $idsFile`
cp ${exp}_${example}.html $RELEASE_DIR/Quaero/doc/devel/$exp/clickableExclusionPlots/
cp ${exp}_${example}_logL_1d.jpg $RELEASE_DIR/Quaero/doc/devel/$exp/clickableExclusionPlots/
cp ${exp}_${example}.html ~/Public/Quaero/quaero_development/doc/devel/$exp/clickableExclusionPlots/
cp ${exp}_${example}_logL_1d.jpg ~/Public/Quaero/quaero_development/doc/devel/$exp/clickableExclusionPlots/

# Send email notification of completion

echo "$collider $exp $example is done \n  http://mit.fnal.gov/~knuteson/Quaero/quaero_development/doc/devel/$exp/clickableExclusionPlots/${exp}_${example}.html" | /usr/lib/sendmail ${USER}@fnal.gov



