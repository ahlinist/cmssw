#!/bin/tcsh
#####################
# Single Top
#
setenv example singleTop
cd $RELEASE_DIR/Quaero/tests/
rm tmp_$example.txt >& /dev/null
setenv dir $RELEASE_DIR/Quaero/experiments/hera/h1/desy/signal/singleTop/
foreach r ( 1 2 3 4 5 )
  foreach k ( 0.05 0.10 0.15 0.20 0.25 0.30 0.35 0.40 0.45 0.50 0.225 0.275 )
    echo "print ($k/0.60)**2" > tmp.py; setenv f `python tmp.py`; rm tmp.py
    $RELEASE_DIR/bin/extractRandomSubsetOfHepevtEvents $r $f $dir/anotop_k0.60.hepevt $dir/anotop_k${k}.hepevt
    hepevt2stdhep $dir/anotop_k${k}.hepevt $dir/anotop_k${k}.stdhep
    perl Quaero/utilities/runTestJob.pl hera h1 $passwd singleTop $k 0 0 0 | tail -1 | awk '{print $2}' >> tmp_$example.txt
    rm $dir/anotop_k${k}.hepevt $dir/anotop_k${k}.stdhep
  end
  sleep 300
end
./keepTabsOnQuaeroJobs.tcsh `cat tmp_$example.txt`
source setupCDF.tcsh
perl drawLogLcurve_1d.pl `head -1 tmp_$example.txt | awk '{print $1}'` `wc tmp_$example.txt | awk '{print $1}'` 0.05 0.05 0.25 "#kappa_{tu#gamma}"
mv model_tag.eps $RELEASE_DIR/Quaero/doc/results_h1/h1_${example}_logL_1d.eps
echo "$example is done" > tmp_$$.txt; /usr/lib/sendmail knuteson@fnal.gov < tmp_$$.txt; rm tmp_$$.txt;
#####################


